/*  Program name: tournamentMain
*	Quick What's Up:  This is the final project for CSCI210.
*   Date last updated: 04/26/2019
*	Purpose: Create a Database for a business idea that we've been working on all semester
*/


#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include "sqlite3.h"
#include "prompt.h"
#include <chrono>
#include <ctime>

using namespace std;

// function prototypes
// search and return
void tournamentsByLake(sqlite3 *db);
void registerAngler(sqlite3 *db);

// Add
void addAngler(sqlite3 *db);
void addTournament(sqlite3 *db);
void addResult(sqlite3 *db);
void addLocation(sqlite3 *db);

// Delete
void removeAngler(sqlite3 *db);
void removeTournament(sqlite3 *db);
void removeResult(sqlite3 *db);
void removeLocation(sqlite3 *db);

// Menus
bool mainMenu(sqlite3 *db);
void registrationMenu(sqlite3 *db);
void weighinMenu(sqlite3 *db);
void resultsMenu(sqlite3 *db);
void settingsMenu(sqlite3 *db);

// Sub Menus
string locidSubMenu(sqlite3 * db);
string tournidSubMenu(sqlite3 * db, string loc_id);
string angleridSubMenu(sqlite3 * db);
string anglersByTournament(sqlite3 *db, string tourn_id);

/**
 * Main Functions
 * @return
 */
int main() {
	int rc;
	sqlite3 *mydb;
	// Open the Database
	rc = sqlite3_open_v2("tournament.db", &mydb, SQLITE_OPEN_READWRITE, "win32");
	bool isWorking = true;
	// Display and run the main menu portion of the program
	while (isWorking){
		isWorking = mainMenu(mydb);
	}

	return 0;
} // End main() -------------------------------------------

// Menu Functions
/**
 * Render Main Menu to screen and allow user to select what they want to do
 * @param  db the database
 * @return    true or false on whether or not to continue the program
 */
bool mainMenu(sqlite3 *db) {
	int choice;
	cout << "\nPro Tournament Manager: " << endl;
	cout << "1. Registration" << endl;
	cout << "2. Weigh-In" << endl;
	cout << "3. Results" << endl;
	cout << "4. Settings" << endl;
	cout << "0. Exit" << endl;
	cout << "Enter Choice: ";
	cin >> choice;
	while (!cin) {
		cout << endl;
		cout << "That is not a valid choice:" << endl;
		cout << endl;
		cin.clear();
		cin.ignore();
		cout << "\nPro Tournament Manager: " << endl;
		cout << "1. Registration" << endl;
		cout << "2. Weigh-In" << endl;
		cout << "3. Results" << endl;
		cout << "4. Settings" << endl;
		cout << "0. Exit" << endl;
		cout << "Enter Choice: ";
		cin >> choice;
	}
	if (choice == 1) {
		registrationMenu(db);
		return true;
	}
	else if (choice == 2) {
		weighinMenu(db);
		return true;
	}
	else if (choice == 3) {
		resultsMenu(db);
		return true;
	}
	else if (choice == 4) {
		settingsMenu(db);
		return true;
	}
	else if (choice == 0) {
		cout << "Thank you.  Now Exiting Program." << endl;
		return false;
	}
	else {
		cout << "I don't understand the choice the program will now exit" << endl;
		return true;
	}
	return false;
}
/**
 * registration sub Menu
 * @param db the database
 */
void registrationMenu(sqlite3 *db){
	int choice;
	cout << "\nPro Tournament Manager" << endl;
	cout << "Registration" << endl;
	cout << "1. Add Angler" << endl;
	cout << "2. Delete Angler" << endl;
	cout << "3. Register Anglers for Tournament" << endl;
  cout << "0. Go Back to Main" << endl;
	cout << "Enter Choice: ";
	cin >> choice;
	while (!cin) {
		cout << endl;
		cout << "That is not a valid choice:" << endl;
		cout << endl;
		cin.clear();
		cin.ignore();
		cout << "\nPro Tournament Manager" << endl;
		cout << "Registration" << endl;
		cout << "1. Add Angler" << endl;
		cout << "2. Delete Angler" << endl;
		cout << "3. Register Anglers for Tournament" << endl;
	  cout << "0. Go Back to Main" << endl;
		cout << "Enter Choice: ";
		cin >> choice;
		cin >> choice;
	}
	if (choice == 1) {
		addAngler(db);
	}
	else if (choice == 2) {
		removeAngler(db);
	}
	else if (choice == 3) {
		registerAngler(db);
	}
	else if (choice == 0) {
		mainMenu(db);
	}
	else {
		cout << "I don't understand the choice the program will now exit" << endl;
	}
}
/**
 * Weigh in procedure User selects the angler and adds their results
 * @param db
 */
void weighinMenu(sqlite3 *db){
	string loc_id = locidSubMenu(db);
	string tourn_id = tournidSubMenu(db, loc_id);
	string angler_id = anglersByTournament(db, tourn_id);
	cin.clear();
	cin.ignore();
	string weigh_weight = promptForString("Weight in Pounds (12.36): ");
	string weigh_numfish = promptForString("Total Fish: ");
	string weigh_shortfish = promptForString("Short Fish: ");
	string weigh_deadfish = promptForString("Dead Fish: ");
	string weigh_minlate = promptForString("Minutes Late: ");
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	string weigh_timestamp = ctime(&timenow);
	// Generate string for query  Use Ternary operators to put in NULL if empty string.
	string query = "INSERT INTO weighin (angler_id, tourn_id, weigh_timestamp, weigh_weight, weigh_numfish, weigh_shortfish, weigh_deadfish, weigh_minlate)	VALUES ("+ ((angler_id != "") ? ("'" + angler_id + "'") : "NULL") + ","+ ((tourn_id != "") ? ("'" + tourn_id + "'") : "NULL") + ","+ ((weigh_timestamp != "") ? ("'" + weigh_timestamp + "'") : "NULL") + ","+ ((weigh_weight != "") ? ("'" + weigh_weight + "'") : "NULL") + ","+ ((weigh_numfish != "") ? ("'" + weigh_numfish + "'") : "NULL") + ","+ ((weigh_shortfish != "") ? ("'" + weigh_shortfish + "'") : "NULL") + ","+ ((weigh_deadfish != "") ? ("'" + weigh_deadfish + "'") : "NULL") + ","+ ((weigh_minlate != "") ? ("'" + weigh_minlate + "'") : "NULL") + ");";
	cout << query << endl;
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
		cout << "| Record Added" << endl;

		sqlite3_finalize(pRes);
	}
}
/**
 * allow user to select a tournament and display the results
 * @param db db
 */
void resultsMenu(sqlite3 *db){
	string query = "SELECT loc_name FROM location;";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string query2;
	string location;
	string query3;
	string tournament;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{  // Handle an error from the previous sql query
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		cout << "\nPlease choose a location:" << endl;
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
		location = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		sqlite3_finalize(pRes);


		query2 = "SELECT tourn_name, tourn_id FROM tournament WHERE loc_id = '" + to_string(choice) + "'";
		if (sqlite3_prepare_v2(db, query2.c_str(), -1, &pRes, NULL) != SQLITE_OK)
		{
			m_strLastError = sqlite3_errmsg(db);
			sqlite3_finalize(pRes);
			cout << "There was an error: " << m_strLastError << endl;
			return;
		}
		else
		{
			cout << "\nPlease choose a tournament:" << endl;
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
			tournament = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
			sqlite3_finalize(pRes);
		query3 = "SELECT angler.angler_fname || ' ' || angler.angler_lname AS 'Name', weighin.weigh_weight AS 'Total Weight' FROM weighin INNER JOIN angler ON angler.angler_id = weighin.angler_id WHERE tourn_id = '" + tournament + "' ORDER BY weighin.weigh_weight DESC;";

		if (sqlite3_prepare_v2(db, query3.c_str(), -1, &pRes2, NULL) != SQLITE_OK)
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
}
/**
 * Display setting menu
 * @param db db
 */
void settingsMenu(sqlite3 *db){
	int choice;
	cout << "\nPro Tournament Manager" << endl;
	cout << "Settings" << endl;
	cout << "1. Add Location" << endl;
	cout << "2. Add Tournament" << endl;
	//cout << "3. Modify a tournament" << endl;
  cout << "0. Go Back to Main" << endl;
	cout << "Enter Choice: ";
	cin >> choice;
	while (!cin) {
		cout << endl;
		cout << "That is not a valid choice:" << endl;
		cout << endl;
		cin.clear();
		cin.ignore();
		cout << "\nPro Tournament Manager" << endl;
		cout << "Settings" << endl;
		cout << "1. Add Location" << endl;
		cout << "2. Add Tournament" << endl;
		//cout << "3. Modify a tournament" << endl;
	  cout << "0. Go Back to Main" << endl;
		cout << "Enter Choice: ";
		cin >> choice;
		cin >> choice;
	}
	if (choice == 1) {
		addLocation(db);
	}
	else if (choice == 2) {
		addTournament(db);
	}
	else if (choice == 3) {
		// TODO modifyTournament(db);
	}
	else if (choice == 0) {
		mainMenu(db);
	}
	else {
		cout << "I don't understand the choice the program will now exit" << endl;
	}
}

/**
 * User selects tournament and an angler to register to.
 * @param db [db]
 */
void registerAngler(sqlite3 *db){
	string loc_id = locidSubMenu(db);
	string tourn_id = tournidSubMenu(db, loc_id);
	string angler_id = angleridSubMenu(db);

	string query2 = "INSERT INTO registration (angler_id, tourn_id) VALUES ("+ ((angler_id != "") ? ("'" + angler_id + "'") : "NULL") + ","+ ((tourn_id != "") ? ("'" + tourn_id + "'") : "NULL")  + ");";

	sqlite3_stmt* pRes2;
	string m_strLastError;
	if (sqlite3_prepare_v2(db, query2.c_str(), -1, &pRes2, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes2);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes2);
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
				if (sqlite3_column_type(pRes2, i) != SQLITE_NULL)
					cout << "|" << setw(20) << sqlite3_column_text(pRes2, i);
				else
					cout << "|" << setw(20) << " ";
			}
			cout << "|" << endl;
		}
		sqlite3_finalize(pRes2);
	}
}

// Add
/**
 * User can add an angler to the Database
 * @param db db
 */
void addAngler(sqlite3 *db){
	// Prompt for the angler information
	cin.clear();
	cin.ignore();
	string fname = promptForString("First Name: ");
	string lname = promptForString("Last Name: ");
	string mname = promptForString("Middle Name: ");
	string add1 = promptForString("Address: ");
	string add2 = promptForString("Address2: ");
	string city = promptForString("City : ");
	string state = promptForString("State : ");
	string zip = promptForString("Zip : ");
	string phone1 = promptForString("Phone 1 : ");
	string phone2 = promptForString("Phone 2 : ");
	// Generate string for query  Use Ternary operators to put in NULL if empty string.
	string query = "INSERT INTO angler (angler_fname, angler_lname, angler_mname, angler_address1, angler_address2, angler_city, angler_state, angler_zip, angler_phone1, angler_phone2)	VALUES ("+ ((fname != "") ? ("'" + fname + "'") : "NULL") + ","+ ((lname != "") ? ("'" + lname + "'") : "NULL") + ","+ ((mname != "") ? ("'" + mname + "'") : "NULL") + ","+ ((add1 != "") ? ("'" + add1 + "'") : "NULL") + ","+ ((add2 != "") ? ("'" + add2 + "'") : "NULL") + ","+ ((city != "") ? ("'" + city + "'") : "NULL") + ","+ ((state != "") ? ("'" + state + "'") : "NULL") + ","+ ((state != "") ? ("'" + state + "'") : "NULL") + ","+ ((phone1 != "") ? ("'" + phone1 + "'") : "NULL") + ","+ ((phone2 != "") ? ("'" + phone2 + "'") : "NULL") + ");";
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
		cout << "| Record Added" << endl;

		sqlite3_finalize(pRes);
	}


}
/**
 * User can add a tournament to the Database
 * @param db
 */
void addTournament(sqlite3 *db){
	// Determine the location loc_id
	string query = "SELECT loc_name, loc_id FROM location;";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string loc_id;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{  // Handle an error from the previous sql query
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		cout << "\nPlease choose a location:" << endl;
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
		loc_id = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		sqlite3_finalize(pRes);
	}
	// Prompt for the rest of the tournament information
	cin.clear();
	cin.ignore();
	string tourn_name = promptForString("Tournament Name: ");
	string tourn_date = promptForDate("Date: ");
	string tourn_penlate = to_string(promptForDouble("Late Penalty: "));
	string tourn_penshort = to_string(promptForDouble("Short Fish Penalty: "));
	string tourn_pendead = to_string(promptForDouble("Dead Fish Penalty: "));

  string query2 = "INSERT INTO tournament (loc_id, tourn_name, tourn_date, tourn_penlate, tourn_penshort, tourn_pendead)	VALUES ("+ ((loc_id != "") ? ("'" + loc_id + "'") : "NULL") + ","+ ((tourn_name != "") ? ("'" + tourn_name + "'") : "NULL") + ","+ ((tourn_date != "") ? ("'" + tourn_date + "'") : "NULL") + ","+ ((tourn_penlate != "") ? ("'" + tourn_penlate + "'") : "NULL") + ","+ ((tourn_penshort != "") ? ("'" + tourn_penshort + "'") : "NULL") + ","+ ((tourn_pendead != "") ? ("'" + tourn_pendead + "'") : "NULL") + ");";

	sqlite3_stmt* pRes2;



	if (sqlite3_prepare_v2(db, query2.c_str(), -1, &pRes2, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes2);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes2);
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
	mainMenu(db);
}
/**
 * User can add a location to the Database
 * @param db
 */
void addLocation(sqlite3 *db){
	// Prompt for the location information
	cin.clear();
	cin.ignore();
	string loc_name = promptForString("Location Name: ");
	string loc_waterbody = promptForString("Waterbody: ");
	string loc_ramp = promptForString("Ramp Access: ");
	string loc_desc = promptForString("Description: ");

	// Generate string for query  Use Ternary operators to put in NULL if empty string.
	string query = "INSERT INTO location (loc_name, loc_waterbody, loc_ramp, loc_desc)	VALUES ("+ ((loc_name != "") ? ("'" + loc_name + "'") : "NULL") + ","+ ((loc_waterbody != "") ? ("'" + loc_waterbody + "'") : "NULL") + ","+ ((loc_ramp != "") ? ("'" + loc_ramp + "'") : "NULL") + ","+ ((loc_desc != "") ? ("'" + loc_desc + "'") : "NULL") + ");";
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


// Delete
/**
 * Allows user to select and angler to remove
 * @param db
 */
void removeAngler(sqlite3 *db){
	string angler_id = angleridSubMenu(db);

	string query2 = "DELETE FROM angler WHERE angler_id = " + angler_id + ";";
	cout << "query2: " << query2 << endl;
	sqlite3_stmt* pRes2;
	string m_strLastError;
	if (sqlite3_prepare_v2(db, query2.c_str(), -1, &pRes2, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes2);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes2);
		columnCount = sqlite3_column_count(pRes2);
		cout << left;
		cout << "| Record Deleted" << endl;
		cout << "Pre finaalize" << endl;
		sqlite3_finalize(pRes2);
		cout << "post finaalize" << endl;
	}
}
/**
 * allows user to select a tournament to remove
 * @param db
 */
void removeTournament(sqlite3 *db){
	string loc_id = locidSubMenu(db);
	string tourn_id = tournidSubMenu(db, loc_id);

	string query2 = "DELETE FROM tournament WHERE tourn_id = " + tourn_id + ";";

	sqlite3_stmt* pRes2;
	string m_strLastError;
	if (sqlite3_prepare_v2(db, query2.c_str(), -1, &pRes2, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes2);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes2);
		columnCount = sqlite3_column_count(pRes2);
		cout << left;
		cout << "| Record Deleted" << endl;

		sqlite3_finalize(pRes2);
	}
}
/**
 * Allows user to select a location to remove
 * @param db
 */
void removeLocation(sqlite3 *db){
	string loc_id = locidSubMenu(db);

	string query2 = "DELETE FROM location WHERE loc_id = " + loc_id + ";";

	sqlite3_stmt* pRes2;
	string m_strLastError;
	if (sqlite3_prepare_v2(db, query2.c_str(), -1, &pRes2, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes2);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		int columnCount = sqlite3_column_count(pRes2);
		columnCount = sqlite3_column_count(pRes2);
		cout << left;
		cout << "| Record Deleted" << endl;

		sqlite3_finalize(pRes2);
	}
}

// Submenus
/**
 * Used inside other query functions.
 * allows user to select location
 * @param  db
 * @return    loc_id
 */
string locidSubMenu(sqlite3 * db) {
	// Determine the location loc_id
	string query = "SELECT loc_name, loc_id FROM location;";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string loc_id;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{  // Handle an error from the previous sql query
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
	}
	else
	{
		cout << "\nPlease choose a location:" << endl;
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
		loc_id = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		sqlite3_finalize(pRes);
	}
	return loc_id;
}
/**
 * Used inside other query functions.
 * allows user to select a tournamentMain
 * @param  db
 * @param  loc_id location Id
 * @return        tourn_id
 */
string tournidSubMenu(sqlite3 * db, string loc_id) {
	// Determine the location loc_id
	string query = "SELECT tourn_name, tourn_id, tourn_date FROM tournament WHERE loc_id = " + loc_id + ";";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string tourn_id;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{  // Handle an error from the previous sql query
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
	}
	else
	{
		cout << "\nPlease choose a Tournament:" << endl;
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
		tourn_id = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		sqlite3_finalize(pRes);
	}
	return tourn_id;
}
/**
 * allows user to select an angler from a master list of all angler_state
 * @param  db
 * @return    angler_id
 */
string angleridSubMenu(sqlite3 * db) {
	string query = "SELECT angler_fname || ' ' || angler_lname AS Name, angler_id FROM angler;";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string angler_id;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{  // Handle an error from the previous sql query
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
	}
	else
	{
		cout << "\nPlease select an angler: " << endl;
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
		angler_id = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		sqlite3_finalize(pRes);
	}
	return angler_id;
}
/**
 * Allows user to select an angler based on being registerd for a tournamentMain
 * @param  db
 * @param  tourn_id
 * @return          angler_id
 */
string anglersByTournament(sqlite3 *db, string tourn_id){
	string query = "SELECT angler_fname || ' ' || angler_lname AS Name, angler.angler_id FROM angler INNER JOIN registration ON angler.angler_id = registration.angler_id WHERE registration.tourn_id = " + tourn_id + " ;";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string angler_id;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{  // Handle an error from the previous sql query
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
	}
	else
	{
		cout << "\nPlease select an angler: " << endl;
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
		angler_id = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		sqlite3_finalize(pRes);
	}
	return angler_id;
}
