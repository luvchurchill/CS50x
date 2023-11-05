"""will print a pyramid of blocks of a height specified by user"""


# Python is a breeze
def main():
    height = int(get_input())
    for i in range(1, height + 1):
        offset = height - i
        print(" " * offset, end="")
        print("#" * i)


# No training wheels for me
def get_input():
    while True:
        print("Height: ", end="")
        try:
            user_in = input()
            if int(user_in) > 0 and int(user_in) < 9:
                return user_in
        except ValueError:
            return get_input()


main()
