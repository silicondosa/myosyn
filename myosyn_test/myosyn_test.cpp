// myosyn_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <myosyn.h>

using namespace std;
int main()
{
	std::cout << "Hello World!\n";
	myosyn m[] = { myosyn(3, RING_OF_FIRE), myosyn(2, RING_OF_FIRE)};
	
	cout<<"Press any key to wind up motor"<<endl;
	getchar();
	m[0].windUp();
	cout << "mtr 2\n"; getchar();
	m[1].windUp();

	cout<<"Press any key to wind down motor"<<endl;
	getchar();
	m[0].windDown();
	cout << "mtr2\n"; getchar();
	m[1].windDown();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
