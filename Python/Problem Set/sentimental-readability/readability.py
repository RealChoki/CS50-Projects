from cs50 import get_string

# TODO
# Get Text
text = get_string("Text: ")

letterCount = 0
# Count letters
for letters in text:
    if letters.isalpha():
        letterCount += 1


# Count Words
wordCount = len(text.split())


sentenceCount = 0
# Count sentences
for characters in text:
    if characters == "." or characters == "!" or characters == "?":
        sentenceCount += 1


# Get average
L = letterCount / wordCount * 100
S = sentenceCount / wordCount * 100

# Calculate Grade
grade = round(0.0588 * L - 0.296 * S - 15.8)

# Print out correct grade
if grade > 15:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print("Grade", grade)
