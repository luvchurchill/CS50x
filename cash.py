""" Will return number of coins needed to make change"""
from cs50 import get_float


def main():
    # Ensure only a positive number is entered
    while True:
        cents = get_float("Change owed: ") * 100
        if cents > 0:
            break
    coins = 0
    quarters = cents // 25
    coins += quarters
    cents -= quarters * 25
    dimes = cents // 10
    coins += dimes
    cents -= dimes * 10
    nickles = cents // 5
    coins += nickles
    cents -= nickles * 5
    pennies = cents
    coins += pennies
    cents -= pennies
    final_coins = int(coins)
    print(final_coins)


main()
