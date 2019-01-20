#include <iostream>
#include <Windows.h>

#include "Menu.h"

std::string Menu::selectFile(int& select)
{
	OPENFILENAME ofn;
	char fileBuffer[260];
	HANDLE handle;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = fileBuffer;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileBuffer);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == true)
		return ofn.lpstrFile;
	else
	{
		std::cout << "\nYou have to select file." << std::endl;
		select = 0;
		return "";
	}
}

void Menu::displayMenu()
{
	using namespace std;

	int select = 0;

	do
	{
		cout << "#---------------Menu---------------#" << endl;
		cout << "# 1) Select File to process.       #" << endl;
		cout << "# 2) Quit.                         #" << endl;
		cout << "# Press the option.                #" << endl;
		cout << "#----------------------------------#" << endl;

		cin >> select;

		switch (select)
		{
			case 1:
				selectFile(select);
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
bool Menu::isRunning()
{
	displayMenu();

	return m_isRunning; 
}
