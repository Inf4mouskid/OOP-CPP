#include "stdafx.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

string ReverseWord(string);
void PalindromeIdentifier(string, string);

int main()
{
	//Variables
	string word1, word2 = "";
	char Restart;

	//Greeting the user
	cout << "This is an app to check to see if a word is a palindrome!\n\n";

	do
	{
		cout << "Enter a word: ";
		cin >> word1;

		//Reverse the word
		word2 = ReverseWord(word1);

		//Checks if there is a Palindrome
		PalindromeIdentifier(word1, word2);

		//Asks for restart
		cout << "Would you like to restart? (Y/N)\n";
		cin >> Restart;
		word2 = "";
	} while (Restart == 'Y' || Restart == 'y');

	//end
	return 0;
}

string ReverseWord(string Word1)
{
	string Word2;

	//Cycle through the word and 
	//store each value in a string.
	for (int i = Word1.size() - 1; i >= 0; --i)
		Word2 += Word1[i];
	return Word2;
}

void PalindromeIdentifier(string Word1, string Word2)
{
	//identifies the Palinedrome
	if (Word2 == Word1)
		cout << "You have a Palindrome!\n";
	else
		cout << "Not a Palindrome.\n";
}
