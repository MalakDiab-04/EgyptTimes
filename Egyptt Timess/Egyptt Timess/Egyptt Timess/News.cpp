#include "News.h"
#include "Admin.h"
#include <list>
#include<iostream>
#include<chrono>
#include<ctime>

using namespace std;

unordered_map<int, News> News::news;
unordered_map<string, int> News::newsTitle;
unordered_map<string, int> News::newsDescription;
list <pair <Date, int>> News::latest;
list <pair <float, int>> News::trending;
unordered_map<string, list<int>>News::catMap;

News::News() {
}

News::News(int i, string tit, string des, Date dat, float rat, int rc, string cat, stack<string> comm, int sp) {
	id = i;
	title = tit;
	description = des;
	date.day = dat.day;
	date.month = dat.month;
	date.year = dat.year;
	rating = rat;
	rCount = rc;
	category = cat;
	comments = comm;
	spam = sp;
}

string News::sTolower(string s) {
	for (int i = 0; i < s.size(); i++) {
		s[i] = tolower(s[i]);
	}
	return s;
}

void News::hide(int id){
	if (News::news.at(id).rating < 2.0) {
		Admin::removeNews(id);
	}
}

void News::setTodayDate() {
	auto now = chrono::system_clock::now();  
	time_t time_now = chrono::system_clock::to_time_t(now); //converts the current time to a time_t object.
	/*struct tm* local_now = localtime(&time_now);*/  //converts the time_t object to a tm structure representing the local time.
	struct tm local_now;
	localtime_s(&local_now, &time_now);
	//The fields of the Date object are set using the corresponding fields from the tm structure.
	date.day = local_now.tm_mday;
	date.month = local_now.tm_mon + 1; // tm_mon is 0-based
	date.year = local_now.tm_year + 1900; // tm_year is years since 1900




//struct tm is a structure C++ defined in the <ctime> header.
//struct tm is commonly used with functions like localtime() to convert between time_t (a numeric representation of time) and a human-readable calendar time.
}

bool News::isValidDate(const Date& datee)
{
	if (datee.year < 0) return false;
	if (datee.month < 1 || datee.month > 12) return false;
	if (datee.day < 1 || datee.day > daysInMonth(datee.month, datee.year)) return false;
	return true;
}

int News::daysInMonth(int month, int year)
{
	switch (month) {
	case 2:
		return (isLeapYear(year)) ? 29 : 28;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	default:
		return 31;
	}
}

bool News::isLeapYear(int year) {
	return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}


