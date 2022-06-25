//
// Created by sento on 29/06/2018.
//

#include "Date.h"
#include <ctime>
using namespace std;

Date::Date() {
    char mese[3]={'\0'};
    int i=0;
    time_t timer;
    time(&timer);       //ritorna la data attuale come un oggetto time_t e la setta anche su timer
    //ctime(&timer) ;     //ritorna la data attuale come fosse una stringa del tipo Wed Feb 13 16:06:10 2013

    for(;i<24;i++)
    {
        if(i<7 && i>3) mese[i-4] = ctime(&timer)[i];

        else if(i<10 && i>7) _day= _day*10+(ctime(&timer)[i]-'0');

        else if(i>19) _year= _year*10+(ctime(&timer)[i]-'0');
    }

    switch (mese[0]) {
        case 'J':
            if(mese[1]=='a') _month = 01;
            else if(mese[2]=='n') _month=06;
            else _month=07;
            break;
        case 'F':
            _month = 02;
            break;
        case 'M':
            if(mese[2]=='c') _month=03;
            else _month=05;
            break;
        case 'A':
            if(mese[2]=='r') _month=04;
            else _month=8;
            break;
        case 'S':
            _month = 9;
            break;
        case 'O':
            _month = 10;
            break;
        case 'N':
            _month = 11;
            break;
        case 'D':
            _month = 12;
            break;

        default:break;
    }

}

Date::Date(int d, int m, int y) {
    if(!(d<32 && d>0 && m>0 && m<13 && y>0)){
        cerr << "Invalid date."<<endl;

    }
    switch(m)
    {
        case 11:
        case 4:
        case 6:
        case 9:
            if(d>30) {
                cerr << "Invalid date." << endl;

            }
            break;
        case 2:
            if (d > 29) {
                cerr << "Invalid date." << endl;

            }
            break;
        default:break;
    }
    _day=d;
    _month=m;
    _year=y;
}

Date::Date(const Date &to_copy) {
    _day=to_copy.day();
    _month=to_copy._month;
    _year=to_copy._year;
}

int Date::year() const {
    return _year;
}

int Date::month() const {
    return _month;
}

int Date::day() const {
    return _day;
}

int Date::yearsFrom(Date d) const {
    Date Now;
    return Now.year()-d.year();
}

std::string Date::str() const { // return the date as a string in the form dd/mm/yyyy
    string output;
    char anno[5]={'\0'},mese[3]={'\0'}, giorno[3]={'\0'};
    itoa(year(),anno,10);
    itoa(month(),mese,10);
    itoa(day(),giorno,10);
    if(day()<10) {
        giorno[1]=giorno[0];
        giorno[0]='0'; }
    if(month()<10) {
        mese[1]=mese[0];
        mese[0]='0'; }
    output=giorno;
    output=output+'/';
    output=output+mese;
    output=output+'/';
    output=output+anno;
    return output ;
}

bool Date::operator<(const Date &d) {
    bool minore= false;      //0=falso
    if(_year<d._year) minore=1;
    else if(_year==d._year && _month<d._month) minore=1;
    else if(_year==d._year && _month==d._month && _day<d._day) minore=1;
    return minore;
}

bool Date::operator==(const Date &d) {

    return (_day==d._day && _month==d._month && _year==d._year) ;
}

void Date::operator=(const Date &d) {
    _year=d._year;
    _month=d._month;
    _day=d._day;
}

bool Date::isDate(std::string &riga) {      //si trasformano i formati dd/mm/yyyy dd-mm-yyyy ecc
    int d;
    if (riga.size() == 10 && (d=atoi(riga.c_str()))!=0 && ispunct(riga[2]) && ispunct(riga[5])) {
        int m=-1,y=-1;
        m=(riga[3]-'0')*10+(riga[4]-'0');
        y=(riga[6]-'0')*1000+(riga[7]-'0')*100+(riga[8]-'0')*10+(riga[9]-'0');
        return _isValid(d,m,y);
    }
    return false;
}

bool Date::_isValid(int d, int m, int y)  {
    if(!(d<32 && d>0 && m>0 && m<13 && y>0)){
        cerr << "Invalid date."<<endl;
        return false;
    }
    switch(m)
    {
        case 11:
        case 4:
        case 6:
        case 9:
            if(d>30) {
                cerr << "Invalid date." << endl;
                return false;
            }
            break;
        case 2:
            if (d > 29) {
                cerr << "Invalid date." << endl;
                return false;
            }
            break;
        default:break;
    }
    _day=d;
    _month=m;
    _year=y;
    Date oggi;
    Date verifica(d,m,y);
    return !(oggi < verifica);      //Controllo di dubbia utilità ma finchè è su un compleanno...
}

int Date::yearsFrom(std::string data) {
    isDate(data);
    Date Now;
    return Now.year()-_year;
}

Date Date::convert(const std::string &riga) {
    int d;
    if (riga.size() == 10 && (d = atoi(riga.c_str())) != 0 && ispunct(riga[2]) && ispunct(riga[5])) {
        int m = -1, y = -1;
        m = (riga[3] - '0') * 10 + (riga[4] - '0');
        y = (riga[6] - '0') * 1000 + (riga[7] - '0') * 100 + (riga[8] - '0') * 10 + (riga[9] - '0');
        return Date(d,m,y);
    }
}
