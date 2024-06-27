#include "Admin.h"
#include "News.h"
#include "User.h"
#include <string>
#include <iostream>

using namespace std;

unordered_map<string, string> Admin::admins;

Admin::Admin()
{
}

bool isValidNewsId(int id) {
	return News::news.find(id) != News::news.end(); // Return true if ID exists in the news map
}

bool Admin::uniqueTitle(string userTitle) {
	return (News::newsTitle.find(userTitle) != News::newsTitle.end()); // Return true if title exists in the news map
}

void Admin::insertInDs(News newn)
{
	News::news.insert(make_pair(newn.id, newn));
	News::latest.push_back(make_pair(newn.date, newn.id));
	News::trending.push_back(make_pair(newn.rating, newn.id));
	News::newsTitle.insert(make_pair(News::sTolower(newn.title), newn.id));
	News::newsDescription.insert(make_pair(News::sTolower(newn.description), newn.id));
}

void Admin::addNews(bool added,string cat) {

	News newn;
	char k;
	cout << "Enter news id: "<<endl;
	cin >> newn.id;
	while (isValidNewsId(newn.id)) {
		cout << "News with ID " << newn.id << " already exists. Enter a valid news id: ";
		cin >> newn.id;
	}
	cout << "Enter news title"<<endl;
	while (true) {  
		getline(cin >> ws, newn.title);
		if (!uniqueTitle(newn.title))
			break;
		else
			cout << "Title already exists. Please re-enter a valid title" << endl;
	}

	cout << "enter news desription" << endl;
	getline(cin>>ws, newn.description);


	cout << "\nIs it today's news? (y/n) "<<endl;
	cin >> k;
	switch (k) {
	case'y':
	case'Y':
		newn.setTodayDate();
		break;
	default:
		cout << "Enter date (day month year): ";
		cin >> newn.date.day >> newn.date.month >> newn.date.year;
		while (!newn.isValidDate(newn.date)) {
			cout << "Invalid date. Please enter again: "; 
			cin >> newn.date.day >> newn.date.month >> newn.date.year;
		}
		break;
	}
	if (!added) {
		cout << "What's the news' category?" << endl;
		cin >> newn.category;
		News::catMap[newn.category].push_back(newn.id);
	}
	else {
		News::catMap[cat].push_back(newn.id);
	}
	newn.rating = 0;
	newn.rCount = 0;
	newn.spam = 0;
	insertInDs(newn);
	News::latest.sort();
	cout << "News with ID " << newn.id << " added successfully.\n";
}

void Admin::updateTitle(int id,string newTitle) {
	News::news.at(id).title = newTitle;
	News::newsTitle.erase(News::news.at(id).title);
	News::newsTitle[newTitle] = id;
	cout << "Title updated successfully" << endl;
}

void Admin::updateDescription(int id,string newDescription) {
	News::news.at(id).description = newDescription;
	News::newsDescription.erase(News::news.at(id).description);
	News::newsDescription[newDescription] = id;
	cout << "Description updated successfully" << endl;
}

void Admin::updateCategory(int id, string newCategory) {
	auto it = News::catMap.find(News::news.at(id).category);
	if (it != News::catMap.end()) {
		it->second.remove(id);
		if (it->second.empty()) {        // If the list is empty, remove the category from the map
			News::catMap.erase(it);
		}
	}
	News::news.at(id).category = newCategory;
	News::catMap[newCategory].push_back(id);
	cout << "Category updated successfully" << endl;
}

void Admin::update(int id) {
	while (!isValidNewsId(id)) {
		cout << "News with ID " << id << " does not exist. Enter a valid news id: ";
		cin >> id;
	}

	int choice;
	cout << "which item do you want to update" << endl;
	cout << "1)Title" << endl << "2)Description" << endl << "3)Category" << endl;
	cin >> choice;

	switch (choice) {
	case 1: {
		string newTitle;
		cout << "Enter new title: ";
		getline(cin >> ws, newTitle);
		updateTitle(id, newTitle);
		break;
	}

	case 2: {
		string newDesc;
		cout << "Enter new description: ";
		getline(cin >> ws, newDesc);
		updateDescription(id, newDesc);
		break;
	}

	case 3: {
		string newCategory;
		cout << "Enter new category: ";
		getline(cin >> ws, newCategory);
		updateCategory(id, newCategory);
		break;
	}
	}
}

void Admin::remove(int id) {
	
	while (!isValidNewsId(id)) {
		cout << "News with ID " << id << " does not exist. Enter a valid news id: ";
		cin >> id;
	}
	removeNews(id);
	cout << "News with " << id << " erased successfully" << endl;
}

void Admin::removeNews(int id) {
	for (auto it = News::latest.begin();it != News::latest.end();it++) {
		if (it->second == id) {
			News::latest.erase(it);
			break;
		}
	}
	News::trending.remove(make_pair(News::news.at(id).rating, id));
	News::newsTitle.erase(News::news.at(id).title);
	News::newsDescription.erase(News::news.at(id).description);

	auto it = News::catMap.find(News::news.at(id).category);
	if (it != News::catMap.end()) {
		it->second.remove(id);

		if (it->second.empty()) {     // If the list is empty, remove the category from the map
			News::catMap.erase(it);
		}
	}
	for (auto it = User::users.begin(); it != User::users.end(); it++) {
		if (it->second.bookmarks.find(id) != it->second.bookmarks.end())
			it->second.bookmarks.erase(id);
		if (it->second.spams.find(id) != it->second.spams.end())
			it->second.spams.erase(id);
		if (it->second.rates.find(id) != it->second.rates.end())
			it->second.rates.erase(id);
	}

	News::news.erase(id);
}

bool Admin::checkCategory(string nCat)
{
	return(News::catMap.find(nCat) != News::catMap.end());
}

void Admin::addCategory(string nCat){
	if (checkCategory(nCat)) {
		cout << "Category already exists." << endl;
	}               
	else {
		bool added = true;
		addNews(added,nCat);
	}
}

float Admin::avgCatRate(string nCat){
	float sum=0;
	for (auto it = News::catMap[nCat].begin(); it != News::catMap[nCat].end(); it++) {
		sum += News::news.at(*it).rating;
	}
	return sum / News::catMap[nCat].size();
}

void Admin::menu() {
	News n;
	int resp = 10;
	while (resp != 0) {
		cout << "1)Add" << endl << "2)Update" << endl << "3)Remove" << endl << "4)Add Category and Assign News" << endl << "5)Display average rate for a Category" << endl << "0)DONE" << endl;
		cin >> resp;
		switch (resp) {
		case 1: {
			addNews(false, " ");
			break;
		}
		case 2: {
			cout << "Enter id of the news: ";
			cin >> n.id;
			update(n.id);
			break;
		}
		case 3: {
			cout << "Enter id of the news: ";
			cin >> n.id;
			remove(n.id);
			break;
		}
		case 4: {
			string cat;
			cout << "Enter new category: ";
			getline(cin >> ws, cat);
			addCategory(News::sTolower(cat));
			break;
		}
		case 5: {
			string cat;
			bool flag;
			cout << "Enter category: ";
			do {
				flag = true;
				getline(cin >> ws, cat);
				if (!checkCategory(News::sTolower(cat))) {
					flag = false;
					cout << "Invalid Category. Try again." << endl;
				}
			} while (!flag);
			cout << avgCatRate(News::sTolower(cat)) << endl;
			break;
		}
		case 0:
			break;
		default: {
			cout << "Invalid choice. Try again." << endl;
			break;
		}
			   cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

//float Admin::avgRate(string name){
//	auto it = News::newsTitle.find(name);
//	if (it == News::newsTitle.end()) {
//		cout << "Title not found." << endl;
//		return -1;
//	}
//	return News::news.at(it->second).rating;
//}