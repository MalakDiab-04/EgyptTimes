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
void System::inputAdmins() {
	ifstream adfile;
	adfile.open("Admin.txt");
	string name, pass;
	while (!adfile.eof()) {
		adfile >> name >> pass;
		Admin::admins[name] = pass;
	}
	adfile.close();
}
void System::inputUsers() {
	ifstream usfile;
	usfile.open("User.txt");
	string line;
	string field;
	vector<string> fields;
	string name, pass, book, spam, rate;
	while (getline(usfile, line)) {
		stringstream ss(line);
		fields.clear();
		while (getline(ss, field, '_')) {
			fields.push_back(field);
		}
		name = fields[0];
		pass = fields[1];
		stringstream sc(fields[2]);
		unordered_set<int> bookmarks;
		while (getline(sc, book, ',')) {
			bookmarks.insert(stoi(book));
		}
		stringstream sp(fields[3]);
		unordered_set<int> spams;
		while (getline(sp, spam, ',')) {
			spams.insert(stoi(spam));
		}
		stringstream sr(fields[4]);
		unordered_set<int> rates;
		while (getline(sr, rate, ',')) {
			rates.insert(stoi(rate));
		}
		User u(name, pass, bookmarks, spams, rates);
		User::users.insert(make_pair(name, u));
	}
	usfile.close();
}
void System::inputNews() {
	ifstream nfile;
	nfile.open("News.txt");
	string line;
	string field;
	vector<string> fields;
	int id, spam, rc;
	string title, description, category, comment, datee;
	Date date;
	float rating;
	while (getline(nfile, line)) {
		stringstream ss(line);
		fields.clear();
		while (getline(ss, field, '_')) {
			fields.push_back(field);
		}
		id = stoi(fields[0]);
		title = fields[1];
		description = fields[2];
		datee = fields[3];
		istringstream dat(datee);
		dat >> date.day >> date.month >> date.year;
		rating = stof(fields[4]);
		rc = stoi(fields[5]);
		category = fields[6];
		stringstream sc(fields[7]);
		string com;
		stack<string> comments;
		while (getline(sc, com, '-')) {
			comments.push(com);
		}
		spam = stoi(fields[8]);
		News n(id, title, description, date, rating, rc, category, comments, spam);
		Admin::insertInDs(n);
		News::catMap[News::sTolower(category)].push_back(id);
	}
	News::latest.sort();
	nfile.close();
}

void System::InputFiles() {
	inputAdmins();
	inputUsers();
	inputNews();
}

void System::outputNews() {
	ofstream out;
	out.open("News.txt");
	for (auto it = News::news.begin(); it != News::news.end(); it++) {
		out << it->first << "_";
		out << it->second.title << "_" << it->second.description << "_";
		out << it->second.date.day << " " << it->second.date.month << " " << it->second.date.year << "_";
		out << it->second.rating << "_" << it->second.rCount << "_" << it->second.category << "_";
		if (!it->second.comments.empty()) {
			stack<string>tempCom;
			for (int i = 0; i <= it->second.comments.size(); i++) {
				tempCom.push(it->second.comments.top());
				it->second.comments.pop();
			}
			for (int i = 0; i <= tempCom.size(); i++) {
				out << tempCom.top() << "-";
				tempCom.pop();
			}
			out.seekp(-1, ios_base::end);
		}
		out << "_" << it->second.spam << "\n";
	}
	out.close();
}
void System::outputUsers() {
	ofstream uout;
	uout.open("User.txt");
	for (auto it = User::users.begin(); it != User::users.end(); it++) {
		uout << it->first << "_";
		uout << it->second.password << "_";
		if (!it->second.bookmarks.empty()) {
			for (auto iter = it->second.bookmarks.begin(); iter != it->second.bookmarks.end(); iter++) {
				uout << *iter << ",";
			}
			uout.seekp(-1, ios_base::end);
		}
		uout << "_";
		if (!it->second.spams.empty()) {
			for (auto iter = it->second.spams.begin(); iter != it->second.spams.end(); iter++) {
				uout << *iter << ",";
			}
			uout.seekp(-1, ios_base::end);
		}
		uout << "_";
		if (!it->second.rates.empty()) {
			for (auto iter = it->second.rates.begin(); iter != it->second.rates.end(); iter++) {
				uout << *iter << ",";
			}
			uout.seekp(-1, ios_base::end);
		}
		else {
			uout << "0";
		}
		uout << "\n";
	}
	uout.close();
}

void System::OutputFiles() {
	outputNews();
	outputUsers();
}