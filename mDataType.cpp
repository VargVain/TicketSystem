#include "mDataType.hpp"

int nextDay(int &day) {
    if (day == 630) day = 701;
    else if (day == 731) day = 801;
    else ++day;
    return day;
}

int nextDay_n(int day) {
    if (day == 630) day = 701;
    else if (day == 731) day = 801;
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
        nextDay_n(day);
    }
    return day;
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
