#pragma once

#include <string>

class Menu
{
private:
	bool m_isRunning = true;

	std::string selectFile(int& select);
	void displayMenu();

public:
	bool isRunning();

};