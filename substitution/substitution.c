#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int validate_params(int argc, string argv[]); //function for validation parametrs of main function
string encrypt_text(string key, string text);
int get_number_char(char letter); //function for searching position of a letter in alphabet after a or A from 0

int main(int argc, string argv[])
{
    int validation = validate_params(argc, argv);
    if (validation == 1)
    {
        return 1;
    }
    string input_text = get_string("plaintext: ");
    string key = argv[1];
    printf("ciphertext: %s\n", encrypt_text(key, input_text));
    return 0;
}

int validate_params(int argc, string argv[])
{
    if (argc == 1) //check variant with only one argument
    {
        printf("You shoud input key for start\n");
        return 1;
    }
    else if (argc > 2 || strlen(argv[1]) != 26) // check variant with problem with lenght key or more than 1 key
    {
        printf("The key must be one string and contain 26 characters.\n");
        return 1;
    }
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if ((argv[1][i] < 'a' || argv[1][i] > 'z') && (argv[1][i] < 'A' || argv[1][i] > 'Z')) // if the key content not only chars
        {
            printf("The key should content only characters.\n");
            return 1;
        }
        for (int j = 0; j < n; j++) //check unique chars in the key
        {
            if (argv[1][j] == argv[1][i] && i != j)
            {
                printf("The each character shoul be unique.\n");
                return 1;
            }
        }
    }
    return 0;
}

string encrypt_text(string key, string text)
{
    string cipher_text = text;
    for (int i = 0, n = strlen(text); i < n; i++)
    {

        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) //filter for working with only chars
        {
            int number_char = get_number_char(text[i]);
            if (isupper(cipher_text[i]) == isupper(key[number_char])) //if type of the letter from text is equal type of the letter from key
            {
                cipher_text[i] = key[number_char];
            }
            else if (isupper(cipher_text[i]) != 0) // if first condition false, we need to make type letter key like the letter from text
            {
                cipher_text[i] = toupper(key[number_char]);
            }
            else if (isupper(cipher_text[i]) == 0)
            {
                cipher_text[i] = (char) tolower((int) key[number_char]);
            }
        }
        else
        {
            cipher_text[i] = text[i]; // just take a symbol, if isn't letter
        }
    }
    return cipher_text;
}


int get_number_char(char letter)
{
    if (letter >= 'a' && letter <= 'z') //for lower chars
    {
        return (int) letter - (int) 'a';
    }
    else if (letter >= 'A' && letter <= 'Z') //for upper chars
    {
        return (int) letter - (int) 'A';
    }
    else
    {
        return 0;
    }
}


