from cs50 import get_float

# TODO
# Get user input
while True:
    change = get_float("Change owed: ")
    if change > 0:
        break

# Convert change into cents
cents = int(change * 100)


count = 0
coins = [25, 10, 5, 1]

# Count how many coins used
for coin in coins:
    count += cents // coin
    cents %= coin


print(count)
