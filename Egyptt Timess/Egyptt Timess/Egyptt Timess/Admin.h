#pragma once
#include <string>
#include "News.h"
#include <unordered_map>
using namespace std;
class Admin {

public:
	static unordered_map<string, string> admins;

	Admin();
	Admin(string, string);
	
	static void removeNews(int id);
	static void insertInDs(News n);

	bool uniqueTitle(string userTitle);
	void addNews(bool ,string);
	void updateTitle(int id, string newTitle);
	void updateDescription(int id,string newDescription);
	void updateCategory(int id, string newCategory);
	void update(int id);
	void remove(int id);
	bool checkCategory(string);
	void addCategory(string);
//	float avgRate(string);
	float avgCatRate(string);
	void menu();
};

