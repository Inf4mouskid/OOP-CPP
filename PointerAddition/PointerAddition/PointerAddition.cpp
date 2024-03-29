// PointerAddition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>

using namespace std;

//Prototypes
void pointerChange(int *);

int main()
{
	int *ptr1 = nullptr;
	int *ptr2 = nullptr;
	int num1, num2;

	//Gets numbers from the user
	cout << "Enter a number: ";
	cin >> num1;
	cout << "Enter another number: ";
	cin >> num2;

	//Stores the numbers in memory
	ptr1 = &num1;
	ptr2 = &num2;

	//Display memory addresses
	cout << "Memory address 1: " << ptr1 << endl;
	cout << "Memory address 2: " << ptr2 << endl;

	//Output 1
	cout << *ptr1 + *ptr2 << endl;

	//change pointer value
	pointerChange(ptr1);

	//Output 2
	cout << *ptr1 + *ptr2 << endl;

    return 0;
}

void pointerChange(int * ptr)
{
	//Pointer value is now 100x
	*ptr = *ptr * 100;
}

