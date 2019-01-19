#include <string>
#include <iostream>
#include <Windows.h>

#include "Menu.h"


void Menu::displayMenu()
{
	using namespace std;

	int select = 0;

	do
	{
		cout << "-----Menu-----" << endl;
		cout << "1) Select File to process." << endl;
		cout << "2) Quit." << endl;
		cout << "Press the option." << endl;

		cin >> select;

		switch (select)
		{
			case 1:

				break;

			case 2:
				m_isRunning = false;
				break;

			default:
				cout << "Bad option!" << endl;
				break;
		}
	} while (select < 1 || select > 2);
} 