#ifndef TICKET_SYSTEM_MASTER_MDATATYPE_HPP
#define TICKET_SYSTEM_MASTER_MDATATYPE_HPP

#include <string>
#include <iostream>
#include <cstring>

struct mString {
    char _str[65];

    mString() = default;

    mString(const std::string &temp) {
        strcpy(_str, temp.c_str());
    }

    mString(char *t) {
        strcpy(_str, t);
    }

    friend bool operator<(const mString &a, const mString &b) {
        return strcmp(a._str, b._str) < 0;
    }

    friend bool operator>(const mString &a, const mString &b) {
        return strcmp(a._str, b._str) > 0;
    }

    friend bool operator==(const mString &a, const mString &b) {
        return strcmp(a._str, b._str) == 0;
    }

    friend bool operator!=(const mString &a, const mString &b) {
        return strcmp(a._str, b._str) != 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const mString& s) {
        os << s._str;
        return os;
    }
};

int nextDay(int &day);

int nextDays(int &day, int len);

int nextDays_n(int day, int len);

int passDays(int endDate, int startDate);

std::string printDay(int day);

std::string printTime(int time);

int addTime(int &time, int len);

int hash(mString s);

static mString TrainID[200000], Place[200000];

#endif //TICKET_SYSTEM_MASTER_MDATATYPE_HPP
