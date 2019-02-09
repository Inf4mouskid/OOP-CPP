#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

int main()
{
	string Visual = "++++++++++", Constrain1, Constrain2;
	int Increment = 0, Decrement = 10;

	for (Increment; Increment < 11; Increment++)
	{
		Constrain1 = Visual.substr(0, Increment);
		Constrain2 = Visual.substr(0, Decrement);
		Constrain1;
	}
	system("Pause");
    return 0;
}

