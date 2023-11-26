import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from time import time, ctime
import http.client, urllib.parse, json

from helpers import apology, login_required, lookup, usd


# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    holdings = db.execute(
        "SELECT * FROM purchases WHERE user_id = ?", session["user_id"]
    )
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    wealth = cash[0]["cash"]
    for holding in holdings:
        wealth += float(lookup(holding["stock"])["price"]) * int(holding["shares"])
    # Add current value to the list of dictionaries "holdings"
    for dict in holdings:
        dict["current_value"] = lookup(dict["stock"])["price"]

        # added a small news feed for final flourish
        # Took most of the code from https://www.marketaux.com/documentation
    conn = http.client.HTTPSConnection("api.marketaux.com")

    params = urllib.parse.urlencode(
        {
            "api_token": "Gn1ZRaRxS5TwnLw1OmCvlTj6BXMDay1SRBD0IGPh",
            "symbols": "AAPL,TSLA",
            "limit": 1,
            "countries": "us",
            "language": "en",
        }
    )

    conn.request("GET", "/v1/news/all?{}".format(params))

    res = conn.getresponse()
    data = res.read()
    jsond = json.loads(data.decode("utf-8"))

    final = jsond["data"][0]["description"]

    return render_template(
        "index.html", holding=holdings, cash=cash, wealth=wealth, news=final
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        stock = request.form.get("symbol")
        try:
            shares = float(request.form.get("shares"))
        except ValueError:
            return apology("stop playing with me", 400)
        if lookup(stock) == None:
            return apology("not a real stock", 400)
        price = float(lookup(stock)["price"])
        if int(shares) < 1:
            return apology("Choose a stock and shares", 400)
        remaining_cash = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )[0]["cash"]
        if price * shares > remaining_cash:
            return apology("not enough money", 400)
        if not shares.is_integer():
            return apology("choose a normal number of shares", 400)

        db.execute(
            "INSERT INTO purchases (user_id, price, stock, timestamp, shares) VALUES(?, ?, ?, ?, ?)",
            session["user_id"],
            (price * shares),
            stock,
            time(),
            shares,
        )
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            (remaining_cash - (price * shares)),
            session["user_id"],
        )
        db.execute(
            "INSERT INTO history (user_id, transaction_type, stock, price, shares, date) VALUES(?, ?, ?, ?, ?, ?)",
            session["user_id"],
            "buy",
            stock,
            price,
            shares,
            ctime(),
        )

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        "SELECT * FROM history WHERE user_id = ?", session["user_id"]
    )
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        price = lookup(symbol)
        if price == None:
            return apology("not a thing", 400)
        price["price"] = float(price["price"])
        return render_template("quoted.html", price=price)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("must provide username", 400)
        # Ensure password exists and is confirmed
        elif not password or password != confirmation:
            return apology("something's up with your password", 400)
        elif len(db.execute("SELECT * FROM users WHERE username = ?", username)) > 0:
            return apology("someone has that username already", 400)
        else:
            db.execute(
                "INSERT INTO users (username, hash, cash) VALUES(?, ?, ?)",
                username,
                generate_password_hash(password),
                10000,
            )
            return render_template("login.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        stocks = db.execute(
            "SELECT stock FROM purchases WHERE user_id = ?", session["user_id"]
        )
        return render_template("sell.html", stocks=stocks)
    else:
        stock = request.form.get("symbol")
        try:
            shares = float(request.form.get("shares"))
        except ValueError:
            return apology("stop fooling around", 400)
        stocks_owned = db.execute(
            "SELECT stock, shares FROM purchases WHERE user_id = ?", session["user_id"]
        )
        for dict in stocks_owned:
            if dict["stock"] == stock:
                if int(dict["shares"]) < shares:
                    return apology("not enough shares", 400)

        if not shares.is_integer():
            return apology("choose a normal number of shares", 400)

        if shares < 1:
            return apology("enough check50, enough!", 400)

        if stock not in [stonk for dict in stocks_owned for stonk in dict.values()]:
            return apology("You don't own that", 403)

        else:
            sell_price = float(lookup(stock)["price"]) * int(shares)
            cash = db.execute(
                "SELECT cash FROM users WHERE id = ?", session["user_id"]
            )[0]["cash"]
            existing_shares = db.execute(
                "SELECT shares FROM purchases WHERE user_id = ? AND stock = ?",
                session["user_id"],
                stock,
            )[0]["shares"]
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?",
                cash + sell_price,
                session["user_id"],
            )
            db.execute(
                "UPDATE purchases SET shares = ? WHERE user_id = ? AND stock = ?",
                int(existing_shares) - shares,
                session["user_id"],
                stock,
            )
            db.execute(
                "INSERT INTO history (user_id, transaction_type, stock, price, shares, date) VALUES(?, ?, ?, ?, ?, ?)",
                session["user_id"],
                "sell",
                stock,
                sell_price / shares,
                shares,
                ctime(),
            )
            return redirect("/")
