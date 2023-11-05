""" will compute approximate grade level needed to comprehend given text"""
# I basically translated my C code from a couple weeks ago


def main():
    userin = input("Text: ")
    letter = letters(userin)
    word = words(userin)
    sentence = sentences(userin)
    L = (letter / word) * 100
    S = (sentence / word) * 100
    unrounded_grade = 0.0588 * L - 0.296 * S - 15.8
    grade = round(unrounded_grade)
    if grade > 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


def letters(paragraph):
    letter_count = 0
    for i in range(len(paragraph)):
        # Got isalpha method from stack overflow
        if paragraph[i].isalpha():
            letter_count += 1
        else:
            letter_count += 0
    return letter_count


def words(paragraph):
    word_count = 0
    for i in range(len(paragraph)):
        if paragraph[i] == " ":
            word_count += 1
        else:
            word_count += 0
    return word_count + 1


def sentences(paragraph):
    sentence_count = 0
    for i in range(len(paragraph)):
        if paragraph[i] in [".", "?", "!"]:
            sentence_count += 1
        else:
            sentence_count += 0
    return sentence_count


main()
