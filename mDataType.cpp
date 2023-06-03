#include "mDataType.hpp"

int nextDay(int &day) {
    if (day == 630) day = 701;
    else if (day == 731) day = 801;
    else if (day == 831) day = 901;
    else ++day;
    return day;
}

int nextDays(int &day, int len) {
    for (int i = 0; i < len; ++i) {
        nextDay(day);
    }
    return day;
}

int nextDays_n(int day, int len) {
    for (int i = 0; i < len; ++i) {
        nextDay(day);
    }
    return day;
}

int passDays(int endDate, int startDate) {
    if (endDate < startDate) return -1;
    int passDays = 1;
    while (endDate != startDate) {
        nextDay(startDate);
        ++passDays;
    }
    return passDays;
}

std::string printDay(int day) {
    std::string a = std::to_string(day / 100), b = std::to_string(day % 100);
    if (a.length() == 1) a = "0" + a;
    if (b.length() == 1) b = "0" + b;
    return a + "-" + b;
}

std::string printTime(int time) {
    std::string a = std::to_string(time / 100), b = std::to_string(time % 100);
    if (a.length() == 1) a = "0" + a;
    if (b.length() == 1) b = "0" + b;
    return a + ":" + b;
}

int addTime(int &time, int len) {
    int m = time % 100 + len;
    int h = time / 100;
    int mm = m % 60;
    int hh = (h + m / 60) % 24;
    time = hh * 100 + mm;
    return (h + m / 60) / 24;
}

int passTime(int dayEnd, int timeEnd, int dayStart, int timeStart) {
    int d = passDays(dayEnd, dayStart);
    int m = (timeEnd / 100 - timeStart / 100) * 60 + (timeEnd % 100 - timeStart % 100);
    return d * 1440 + m;
}
/*
int hash(mString s) {
unsigned long long h = 0;
for (int i = 0; i < strlen(s._str); ++i) {
    h = (h * 237 + s._str[i] + 199799) % 199799;
}
return (int)h;
}
*/

int hash(mString s) {
    if (mNameList.find(s) == mNameList.end()) {
        ++pos;
        mNameList[s] = pos;
        return pos;
    }
    else {
        return mNameList.find(s)->second;
    }
}

int getNo(int trainID, int trainNo) {
    return trainID * 100 + trainNo;
}
