#include <iostream>
#include <string>
#include "Admin.h"
#include "News.h"
#include "User.h"
#include <fstream>
#include <stack>
#include <sstream>
#include "System.h"
using namespace std;

bool isValidUser(string name, string pass) {
	return  (User::users.find(name) != User::users.end() && User::users.at(name).password == pass); // Return true if username exist in the users map
}
bool isValidAdmin(string name, string pass) {
	return (Admin::admins.find(name) != Admin::admins.end() && Admin::admins.at(name) == pass); // Return true if username exist in the admins map
}

void isAdmin() {
	string name, pass;
	bool correct = false;
	do {
		cout << "Enter your username: ";
		cin >> name;
		cout << "Enter your password: ";
		cin >> pass;
		if (isValidUser(name, pass)) {
			correct = true;
			User::users.at(name).menu();
		}
		else if (isValidAdmin(name, pass)) {
			correct = true;
			Admin a;
			a.menu();
		}
		else {
			cout << "Incorrect username or password." << endl;
		}
	} while (!correct);

}

bool signup() { //emmy
	string name, pass;
	bool unique = false;
	do {
		cout << "Enter your username: ";
		cin >> name;
		cout << "Enter your password: ";
		cin >> pass;
		if (User::users.find(name) != User::users.end())
			cout << "Invalid username."<<endl;
		else {
			unique = true;
			User u(name, pass);
			User::users[name] = u;
			return true;
		}
	} while (!unique);
}

void start() {
	int resp;
	cout << "1)LOG IN" << endl << "2)SIGN UP" << endl;
	cin >> resp;

	if (resp == 1) {
		isAdmin();
	}
	else if (resp == 2) {
		if (signup()) {
			cout << "You're successfully signed up. Now please log in:" << endl;
			cout << "LOG IN" << endl;
			isAdmin();
		}
	}
}

int main() {

	System::InputFiles();
	start();
	System::OutputFiles();

	return 0;
}
