//
// Created by sento on 29/06/2018.
//

#ifndef PROJECT_DATE_H
#define PROJECT_DATE_H

#include <iostream>
#include <cctype>


class Date {
public:
    // Default Constructor , set the date to the actual date
    Date();
    // Constructor , pass the date in the form of dd,mm,yyyy  // Note that the program crashes if the date is not valid...
    Date(int d, int m, int y);
    // Copy Constructor
    Date(const Date & to_copy);
    // Getters
    int year() const;
    int month() const;
    int day() const;
    static Date convert(const std::string &riga);     //non ne controlla la validità
    // return the year from the date d, if d it not passed , it returns the year from now.
    // Usefull to compute the age of a person , where this is the person’s birth date
    int yearsFrom(Date d = Date()) const;
    int yearsFrom(std::string data);        //Non è const perchè richiama isDate
    // return the date as a string in the form dd/mm/yyyy  // Useful to print on the screen
    std::string str() const;
    // overload of the << operator for the Data class.  // the friend keyword is mandatory to define the overload
    bool isDate(std::string & riga) ;
    bool operator < (const Date& d);
    bool operator==(const Date & d);
    void operator=(const Date & d);

private:
    bool _isValid(int d, int m, int y);
    int _year=0;
    int _month=0;
    int _day=0;
};



#endif //PROJECT_DATE_H
