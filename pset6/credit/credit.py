card_number = input("Number: ")
lenght = len(card_number)
result = 0


for i in range(1, lenght + 1):
    """Check sum of a card"""
    if i % 2 == 1:
        result += int(card_number[-i])
    else:
        tmp = int(card_number[-i]) * 2
        if tmp >= 10:
            result += tmp - 9
        else:
            result += tmp


"""Choose type of card:"""
if result % 10 != 0:
    print("INVALID")
elif lenght == 15 and card_number[:2] in ['34', '37']:  # Conditions for AMEX
    print("AMEX")
elif lenght == 16 and int(card_number[:2]) in range(51, 56):  # Conditions for MasterCard
    print("MASTERCARD")
elif (lenght == 13 or lenght == 16) and card_number[:1] == '4':  # Conditions for VISA
    print("VISA")
else:
    print("INVALID")
