#ifndef PROMPT_H
#define PROMPT_H

#include <iostream>
#include <string>


// function prototypes
std::string promptForString(std::string prompt);
int promptForInt(std::string prompt);
int promptForIntBetween(int from, int to, std::string prompt);
double promptForDouble(std::string prompt);
int promptForPositiveInt(std::string prompt);
std::string promptForDate(std::string prompt);

#endif /* !PROMPT_H */
