#include "User.h"
#include <iostream>

using namespace std;

unordered_map<string, User> User::users;

User::User() {

}
User::User(string name, string pass) {
	username = name;
	password = pass;
}
User::User(string name, string pass, unordered_set<int> bookmark) {
	username = name;
	password = pass;
	bookmarks = bookmark;
}
User::User(string name, string pass, unordered_set<int> bookmark, unordered_set<int> spam) {
	username = name;
	password = pass;
	bookmarks = bookmark;
	spams = spam;
}
User::User(string name, string pass, unordered_set<int> bookmark, unordered_set<int> spam, unordered_set<int> rate) {
	username = name;
	password = pass;
	bookmarks = bookmark;
	spams = spam;
	rates = rate;
}

void User::rate(float rate, int id) {
	if (!rates.empty() && *rates.begin() == 0)
		rates.erase(0);
	float sum;
	sum = News::news.at(id).rating * News::news.at(id).rCount + rate;
	News::news.at(id).rating = sum / ++(News::news.at(id).rCount);
	for (auto it = News::trending.begin();it != News::trending.end();it++) {
		if (it->second == id) {
			it->first = News::news.at(id).rating;
		}
	}
	rates.insert(id);
}

bool User::checkIfRated(int id){
	return(rates.find(id) != rates.end());
}

void User::spam(int nid){
	spams.insert(nid);
	News::news[nid].spam++;
	cout << "News reported as spam successfully." << endl;
}

bool User::checkIfSpammed(int id)
{
	return(spams.find(id) != spams.end());
}

void User::viewBookmarks(){
	if (bookmarks.empty()) {
		cout << "No bookmarks yet" << endl;
	}
	else {
		for (auto it = bookmarks.begin(); it != bookmarks.end(); ++it) {
			displayy(*it);
		}
	
	}
}

void User::viewComments(int id) {
	if (!News::news.at(id).comments.empty()) {
		cout << "Comments: " << endl;
		stack<string> tempS = News::news.at(id).comments;
		while (!tempS.empty()) {
			cout << tempS.top() << endl;
			tempS.pop();
		}
	}
	else {
		cout << "No comments yet." << endl;
	}
}

bool User::displayy( int id)
{
	system("CLS");
	if (checkIfSpammed(id)) {
		return false; 
	}
	cout << "\t\t" << News::news.at(id).title << endl;
	cout << News::news.at(id).description << endl;
	cout << News::news.at(id).date.day << "/" << News::news.at(id).date.month << "/" << News::news.at(id).date.year << endl;
	cout << News::news.at(id).spam << " spams" << endl;
	if (News::news.at(id).rating != 0) {
		cout << "Rating: " << News::news.at(id).rating << endl;
	}
	else {
		cout << "No rating yet" << endl;
	}
	viewComments(id);
	cout << "---------------------------------" << endl;
	menuSingle(id);
	cout << "=================================" << endl;
	return true;
}

void User::latest() {
	int count = 0;
	bool anyDisplayed = false;
	for (auto it = News::latest.begin(); it != News::latest.end() && count < 10; it++) {
		
		if (displayy(it->second)) {
			count++;
			anyDisplayed = true;
		}
	}
	if (!anyDisplayed) {
		cout << "No output because all latest news are spammed." << endl;
	}
}

void User::trending() {
	int count = 0;
	News::trending.sort();
	News::trending.reverse();
	bool anyDisplayed = false;
	for (auto it = News::trending.begin(); it != News::trending.end() && count < 10; it++) {

		if (displayy(it->second)) {
			count++;
			anyDisplayed = true;
		}
	}
	if (!anyDisplayed) {
		cout << "No output because all latest news are spammed." << endl;
	}
}

void User::searchTitle(string tit) {
	tit = News::sTolower(tit);
	auto iter = News::newsTitle.find(tit);
	if (iter != News::newsTitle.end()) {
		if (!displayy(iter->second)) {
			cout << "This news is spammed." << endl;
		}
	}
	else {
		cout << "Title '" << tit << "' not found." << endl;
	}
}

void User::searchDate(Date date) {
		bool found = false;
		for (auto it = News::latest.begin(); it != News::latest.end(); it++) {
			if (it->first.operator=(date)) {
				found = true;
				if (!displayy(it->second))
					cout << "This news is spammed." << endl;
			}
			else if (!it->first.operator<(date))
				break;
		}
		if (!found)
			cout << "No news found at this date." << endl;
	
}

void User::searchKeyword(string desc) {
	bool found = false;
	desc = News::sTolower(desc);
	for (auto it = News::newsDescription.begin(); it != News::newsDescription.end(); it++) {
		if (it->first.find(desc) != string::npos) {        // npos :a constant representing the maximum possible value for the size of a string, typically indicating that the substring was not found.
			found = true;
			if (!displayy(it->second))
				cout << "This news is spammed." << endl;
		}
	}
	if (!found)
		cout << "News with such keyword not found" << endl;
}

void User::category(string cat) {
	cat = News::sTolower(cat);
	auto it = News::catMap.find(cat);
	if (it != News::catMap.end()) {
		for (auto iter = it->second.begin(); iter != it->second.end(); iter++) {
			if (!displayy(*iter))
				cout << "This news is spammed." << endl;
		}
	}
	else
		cout << "No news of this category." << endl;
}

void User::comment(int id) {
	string com;
	cout << "Comment: " << endl;
	getline(cin >> ws, com);
	News::news.at(id).comments.push(com);
}

bool User::checkBookmark(int id){
	return(bookmarks.find(id) != bookmarks.end());
}

void User::bookmark(int id) {
	bookmarks.insert(id);
}

void User::menuSingle(int id) {
	char choice = 'a';
	float r;
	string com;

	while (choice != '0') {
		cout << "1)Rate news" << endl << "2)Comment on news" << endl << "3)Report news as a spam" << endl << "4)Add to bookmarks" << endl;
		cout << "0) Next" << endl << "M) Main Menu." << endl;
		cin >> choice;
		switch (choice)
		{
		case '1': {
			if (!checkIfRated(id)) {
				bool correct;
				cout << "Enter rating: ";
				do {
					correct = true;
					cin >> r;
					if (r < 1 || r>5) {
						cout << "Invalid rating. Please enter a rating between 1 and 5" << endl;
						correct = false;
					}
				} while (!correct);
				rate(r, id);
				News::hide(id);
			}
			else {
				cout << "You already rated this piece of news before" << endl;
			}
			break;
		}

		case '2': {
			comment(id);
			break;
		}

		case '3': {
			spam(id);
			break;
		}
		case '4': {
			if (!checkBookmark(id)) {
				bookmark(id);
				cout << "Successfully added to your bookmarks list" << endl;
			}
			else
				cout << "You already bookmarked this piece of news before." << endl;
			break;
		}
		case '0': {
			break;
		}
		case 'M':
		case 'm': {
			menu();
			return;
		}
		default: {
			cout << "Invalid choice. Try again." << endl;
		}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (choice == '3' || choice == 'm')
			break;
	}
}

void User::menu() {
	int choice = 10;
	float r;
	string com;
	News n;
	cout << "-----------------------------------------------------" << endl;
	while (choice != 0) {
		cout << "1)Display latest news" << endl << "2)Display trending news" << endl << "3)View your bookmark" << endl << "4)Search by title" <<endl<< "5)Search by keyword" << endl<<"6)Search by date" << endl<<"7)Search by category" << endl<< "0)DONE" << endl;
		cin >> choice;

		switch (choice)
		{
		case 1: {
			latest();
			break;
		}

		case 2: {
			trending();
			break;
		}

		case 3: {
			viewBookmarks();
			break;
		}

		case 4: {
			string tit;
			cout << "Enter title: ";
			getline(cin >> ws, tit);
			searchTitle(tit);
			break;
		}

		case 5: {
			string key;
			cout << "Enter keyword: ";
			cin >> key;
			searchKeyword(key);
			break;
		}
		case 6: {
			Date datee;
			cout << "Enter date (day month year): ";
			cin >> datee.day >> datee.month >> datee.year;
			searchDate(datee);
			break;
		}
		case 7: {
			string cat;
			cout << "Enter category: ";
			getline(cin >> ws, cat);
			category(cat);
			break;
		}
		case 0: {
			System::OutputFiles();
			exit(0);
		}
		default: {
			cout << "Invalid choice. Try again." << endl;
		}
		 cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}