// PasswordChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

//Prototypes
string BoolMessage(bool Flag);

int main()
{
	bool IsPasswordcorrect = false;
	string Password, refPassword;
	bool Upper = false;
	bool Lower = false;
	bool Digit = false;
	bool Symbol = false;

	cout << "Enter a Password: ";
	cin >> refPassword;

	while (refPassword.length() < 8)
	{
		cout << "Sorry, but your password must be longer than 8 characters.\n";
		cout << "Enter a Password: ";
		cin >> refPassword;
	}

	for (int count = 0; count < refPassword.size(); count++)
	{
		if (islower(refPassword[count]))
			Lower = true;

		if (isupper(refPassword[count]))
			Upper = true;

		if (isdigit(refPassword[count]))
			Digit = true;
		
		if (ispunct(refPassword[count]))
			Symbol = true;
	}

	cout << "What is your password? ";
	cin >> Password;

	while (IsPasswordcorrect = false)
	{
		if (Password != refPassword)
		{

		}
		else
		{
			cout << "Login Successful!";
			IsPasswordcorrect = true;
		}
	}


    return 0;
}



string BoolMessage(bool Flag)
{
	if (Flag)
	{
		cout <<
	}
	else
	{

	}
}