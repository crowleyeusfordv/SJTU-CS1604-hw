#include <iostream>
#include <string>
#include <sstream>
using namespace std;
bool isLeapYear(int year);
int LeapYearCounter(int y1,int y2);
void cinpro(string date,int& year,int& month,int& day);
int arradd(int arr[],int start,int end);


//Return the day difference between two years.
int main()
{
    string date1;
    cin >> date1;
    int year1 = 0;
    int month1 = 0;
    int day1 = 0;
    cinpro(date1,year1,month1,day1);
    string date2;
    cin >> date2;
    int year2 = 0;
    int month2 = 0;
    int day2 = 0;
    cinpro(date2,year2,month2,day2);


    int y1day = 0;
    int y2day = 0;
    int monthday[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int leapmonthday[] = {31,29,31,30,31,30,31,31,30,31,30,31};
    int result;
    if (year1 == year2) {
        if (isLeapYear(year2)) {
            result = day2 + arradd(leapmonthday,0,month2 - 2) - day1 - arradd(leapmonthday,0,month1 - 2);}
        else {
            result = day2 + arradd(monthday,0,month2 - 2) - day1 - arradd(monthday,0,month1 - 2);}
        cout << result;
        return 0;
    }

    if (isLeapYear(year1)) {
        y1day = leapmonthday[month1 - 1] - day1 + arradd(leapmonthday,month1,11);}
    else {
        y1day = monthday[month1 - 1] - day1 + arradd(monthday,month1,11); }

    if (isLeapYear(year2)) {
        y2day = day2 + arradd(leapmonthday,0,month2 - 2);}
    else {
        y2day = day2 + arradd(monthday,0,month2 - 2);}


    // The days between y1 and y2.(not including y1 and y2)
    int numLeap = LeapYearCounter(year1 + 1,year2 - 1);//=0
    int mday = 366 * numLeap + 365 * (year2 - year1 - 1 - numLeap);//-365,error:ÎŞÆ½Äê


    //cin << "y1day = " << y1day;
    //cin << "y2day = " << y2day;
    result = y1day + mday + y2day;
    cout << result;
    return 0;
}


//add from array[i] to array [j]
int arradd(int arr[],int start,int stop)
{
    int sum = 0;
    for (int i = start;i <= stop;i++) {
        sum += arr[i];
    }
    return sum;
}
//analyze the input and get year,month,day assigned.
void cinpro(string date,int& year,int& month,int& day)
{
    istringstream iss(date);
    string year_str,month_str,day_str;
    getline(iss, year_str, '/'); // use "/" as separator
    getline(iss, month_str, '/');
    getline(iss, day_str, '/');
    year = stoi(year_str); // convert string to int
    month = stoi(month_str);
    day = stoi(day_str);

}

//Count the number of leap years between y1 and y2.
int LeapYearCounter(int y1,int y2)
{
    int num = 0;
    int m = y2 - y1;
    for (int i = 0;i <= m;i++){
        if (isLeapYear(y1+i))
            num += 1;
    }
    return num;
}


//Determine whether a given year is a leap year.
bool isLeapYear(int year)
{
    if (year % 4 != 0)
        return false;
    else if (year % 4 == 0 && year % 100 != 0)
        return true;
    else if (year % 100 ==0 && year %400 != 0)
        return false;
    else
        return true;
}
