
#include <iostream>
#include <string>
#include "prompt.h"

using namespace std;

string promptForString(string prompt)
{
	// prompt the user with the provided prompt
	cout << prompt;
	string str;
	std::getline(std::cin, str);
	return str;
}

int promptForInt(string prompt)
{
	// prompt the user based on the provided prompt
	cout << prompt;
	int value;
	while (true) {
		if (std::cin >> value) {
			cin.ignore();
			return value;		// If valid integer then return the int
		}
		else {
			std::cout << "Please enter a valid integer" << std::endl;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	}
	return 0;
}

int promptForIntBetween(int from, int to, std::string prompt) {
	// prompt the user based on the provided prompt
	cout << prompt;
	int value;
	while (true) {
		if (std::cin >> value) {
			if (value >= from && value <= to) {
				cin.ignore();
				return value;		// If valid integer then return the int
			}
			else {
				std::cout << "Please enter a positive integer" << std::endl;
				std::cin.clear();
				std::cin.ignore(1000, '\n');
			}

		}
		else {
			std::cout << "Please enter a valid integer" << std::endl;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	}
	return 0;
}

double promptForDouble(string prompt) {
	// prompt the user based on the provided prompt
	cout << prompt;
	double value;
	while (true) {
		if (std::cin >> value) {
			cin.ignore();
			return value;		// If valid integer then return the int
		}
		else {
			std::cout << "Please enter a valid number" << std::endl;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	}
	return 0.0;
}
int promptForPositiveInt(string prompt)
{
	// prompt the user based on the provided prompt
	cout << prompt;
	int value;
	while (true) {
		if (std::cin >> value) {
			if (value >= 0) {
				cin.ignore();
				return value;		// If valid integer then return the int
			}
			else {
				std::cout << "Please enter a positive integer" << std::endl;
				std::cin.clear();
				std::cin.ignore(1000, '\n');
			}

		}
		else {
			std::cout << "Please enter a valid integer" << std::endl;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	}
	return 0;
}


std::string promptForDate(std::string prompt) {
	int month = promptForIntBetween(1, 12, "Month (1-12): ");
	int day = promptForIntBetween(1, 31, "Day (1-31): ");
	int year = promptForInt("Year (ex. 2014): ");
	std::string retStr = to_string(year) + "-" + ((month > 9) ? to_string(month) : "0" + to_string(month)) + "-" + ((day > 9) ? to_string(day) : "0" + to_string(day));
	return retStr;
}
