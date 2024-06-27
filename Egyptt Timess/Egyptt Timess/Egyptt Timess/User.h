#pragma once
#include <string>
#include <stack>
#include "News.h"
#include "System.h"
#include <unordered_map>
#include<unordered_set>
 
using namespace std;
class User {
public:
	static unordered_map<string, User> users;


	string username;
	string password;
	unordered_set<int> bookmarks;
	unordered_set<int> spams;
	unordered_set<int> rates;

	User();
	User(string, string);
	User(string, string, unordered_set<int>);
	User(string, string, unordered_set<int>, unordered_set<int>);
	User(string, string, unordered_set<int>, unordered_set<int>, unordered_set<int> spams);
	void rate(float rate, int id);
	bool checkIfRated(int id);
	void comment(int id);
	void viewComments(int id);
	void spam(int nid);
	bool checkIfSpammed(int id);
	bool checkBookmark(int id);
	void bookmark(int id);
	void viewBookmarks();
	bool displayy(int id);
	void latest();
    void trending();
	void searchTitle(string);
	void searchDate(Date);
	void searchKeyword(string);
	void category(string);
	void menuSingle(int);
	void menu();
};