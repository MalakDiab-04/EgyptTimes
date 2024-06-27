#pragma once
#include <string>
#include <stack>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>
using namespace std;
struct Date {
	int day;
	int month;
	int year;

	bool operator<(const Date& other) const {
		if (year != other.year)
			return year > other.year;
		if (month != other.month)
			return month > other.month;
		return day > other.day;
	}
	bool operator=(const Date& other) const {
		return (year == other.year && month == other.month && day == other.day);
	}
};
class News {
public:

	static unordered_map<int, News> news;
	static unordered_map<string, int> newsTitle;
	static unordered_map<string, int> newsDescription;
	static list <pair <Date, int>> latest;
	static list <pair <float, int>> trending;
	static unordered_map<string, list<int>> catMap;

	int id;
	string title;
	string description;
	Date date;
	float rating;
	int rCount;
	string category;
	stack<string> comments;
	int spam;

	News();
	News(int, string, string, Date, float, int, string, stack<string>, int);
	static string sTolower(string);
	static void hide(int id);
	void setTodayDate();
	bool isValidDate(const Date& date);
	int daysInMonth(int month, int year);
	bool isLeapYear(int year);

};