#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long card_number = get_long("Number: ");
    long i = card_number;
    int result = 0;
    int lenght = 0;
    int last_numbers;
    while (i > 0)
    {   
        lenght ++; // Count lenght of card_number
        if (lenght % 2 == 0) // The condition for each second digits
        {
            if ((i % 10) * 2 > 9) // Check the condition for 2 * digits, when result more then 1 digits
            {
                result = result + (i % 10) * 2 - 10 + 1;
            }
            else
            {
                result = result + (i % 10) * 2;
            }
        }
        else
        {
            result = result + (i % 10); // The condition for odd numbers of card_number
        }
        if (i < 100 && i >= 10) // I save first 2 digits for a definition which card_number
        {
            last_numbers = i;
        }
        i = (i - i % 10) / 10; // To decrease card_number for next digit
    }
    if (result % 10 == 0)
    {
        if (lenght == 15 && (last_numbers == 34 || last_numbers == 37))
        {
            printf("AMEX\n");
        }
        else if (lenght == 16 && last_numbers >= 51 && last_numbers <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if ((lenght == 13 || lenght == 16) && last_numbers >= 40 && last_numbers < 50)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
