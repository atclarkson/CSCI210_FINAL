/*  Program name: tournamentMain
*		Quick What's Up:  This is the final project for CSCI210.
*   Author: Adam Clarkson
*   Date last updated: 04/26/2019
*	Purpose: Create a Database for a business idea that we've been working on all semester
*/

// TODO this is just a starting point from a previous project.  TODO - Everything.

#include <iostream>
#include <string>
#include <iomanip>
#include "sqlite3.h"

using namespace std;


void customerbyregion(sqlite3 *db);
void salesperquarter(sqlite3 *db);
int main()
{
	int choice;
	int rc;

	sqlite3 *mydb;
	// Open the Database
	rc = sqlite3_open_v2("SaleCo.db", &mydb, SQLITE_OPEN_READWRITE, "win32");

	cout << "Reports Available: " << endl;
	cout << "1. Customers by Region" << endl;
	cout << "2. Sales per Quarter" << endl;
	cout << "Enter Choice: ";
	cin >> choice;
	while (!cin)
	{
		cout << endl;
		cout << "That is not a valid choice:" << endl;
		cout << endl;
		cin.clear();
		cin.ignore();
		cout << "Reports Available: " << endl;
		cout << "1. Customers by Region" << endl;
		cout << "2. Sales per Quarter" << endl;
		cout << "Enter Choice: ";
		cin >> choice;
	}
	if (choice == 1)
	{
		customerbyregion(mydb);
	}
	else if (choice == 2)
	{
		salesperquarter(mydb);
	}
	else
	{
		cout << "I don't understand the choice the program will now exit" << endl;
		return 0;
	}
	return 0;
}


void customerbyregion(sqlite3 *db)
{
	/*Need to provide the select statement to get the region names from the database.*/
	string query = "SELECT REG_NAME FROM DWREGION;";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string query2;
	string region;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		cout << "Please choose a region:" << endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt *pRes2;
		cout << left;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			cout << i << ". " << sqlite3_column_text(pRes, 0);
			cout << endl;
			i++;
		}
		do
		{
			cin >> choice;
			if (!cin || choice < 1 || choice > i)
				cout << "That is not a valid choice! Try Again!" << endl;
			if (!cin)
			{
				cin.clear();
				cin.ignore();
			}
		} while (!cin || choice < 1 || choice > i);

		sqlite3_reset(pRes);
		for (int j = 0; j < choice; j++)
			sqlite3_step(pRes);
		region = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		sqlite3_finalize(pRes);
		//need to provide the query to select the customers with the chosen region from the database
		query2 = "SELECT CUS_FNAME || ' ' || CUS_LNAME AS Name FROM DWCUSTOMER WHERE REG_ID = '" + to_string(choice) + "'";

		if (sqlite3_prepare_v2(db, query2.c_str(), -1, &pRes2, NULL) != SQLITE_OK)
		{
			m_strLastError = sqlite3_errmsg(db);
			sqlite3_finalize(pRes2);
			cout << "There was an error: " << m_strLastError << endl;
			return;
		}
		else
		{
			columnCount = sqlite3_column_count(pRes2);
			cout << left;
			for (int i = 0; i < columnCount; i++)
			{
				cout << "|" << setw(20) << sqlite3_column_name(pRes2, i);
			}
			cout << "|" << endl;
			while (sqlite3_step(pRes2) == SQLITE_ROW)
			{
				for (int i = 0; i < columnCount; i++)
				{
					if (sqlite3_column_type(pRes2, i) != SQLITE_NULL) //need to bring up to students
						cout << "|" << setw(20) << sqlite3_column_text(pRes2, i);
					else
						cout << "|" << setw(20) << " ";
				}
				cout << "|" << endl;
			}
			sqlite3_finalize(pRes2);
		}

	}
}

void salesperquarter(sqlite3 * db)
{
	/*Need to provide the select statement to get quarter and year from DB.*/
	string query = "SELECT t.TM_QTR || ' - ' || TM_YEAR AS \"Quarter\", sum(d.SALE_PRICE) AS 'Quarter Total' FROM DWDAYSALESFACT d INNER JOIN DWTIME t ON d.TM_ID = t.TM_ID GROUP BY d.TM_ID;";
	sqlite3_stmt* pRes;
	string m_strLastError;



	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes);
		columnCount = sqlite3_column_count(pRes);
		cout << left;
		for (int i = 0; i < columnCount; i++)
		{
			cout << "|" << setw(20) << sqlite3_column_name(pRes, i);
		}
		cout << "|" << endl;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			for (int i = 0; i < columnCount; i++)
			{
				if (sqlite3_column_type(pRes, i) != SQLITE_NULL) //need to bring up to students
					cout << "|" << setw(20) << sqlite3_column_text(pRes, i);
				else
					cout << "|" << setw(20) << " ";
			}
			cout << "|" << endl;
		}
		sqlite3_finalize(pRes);
	}



}

//Need to create the salesperquarter function.  The quarters should be displayed by quarter - year from the DW time database.