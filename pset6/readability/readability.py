def count_letters(text):
    """count letter in text"""
    count = 0
    for i in text:
        if i.upper() in "QWERTYUIOPASDFGHJKLZXCVBNM":
            count += 1

    return count


def count_words(text):
    """count words in text"""
    return text.count(" ") + 1


def count_sentences(text):
    """count sentences in text"""
    return text.count(".") + text.count("!") + text.count("?")


text = input("Text: ")
letters = count_letters(text)
words = count_words(text)
sentences = count_sentences(text)
l = letters * 100 / words  # the average number of letters per 100 words in the text
s = sentences * 100 / words  # the average number of sentences per 100 words in the text
index = 0.0588 * l - 0.296 * s - 15.8  # formula for Coleman-Liau index

grade = round(index)


if grade < 1:
    print("Before Grade 1")
elif grade <= 16:
    print(f"Grade {grade}")
else:
    print("Grade 16+")