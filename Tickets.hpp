#ifndef TICKET_SYSTEM_MASTER_TICKETS_HPP
#define TICKET_SYSTEM_MASTER_TICKETS_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "mDataType.hpp"
#include "UserList.hpp"

struct Train {
    mString trainID, stations[128], type;
    int stationNum, seatNum, prices[128], startTime, travelTimes[128], stopoverTimes[128], saleDate[2];
    int arrivingTimes[128], leavingTimes[128], arrDayPass[128], levDayPass[128], totalTime[128], totalPrice[128];
    bool released = false;
};

struct Trip {
    mString start, end;
    int date;
    friend bool operator<(const Trip &lhs, const Trip &rhs) {
        if (lhs.date < rhs.date) return true;
        if (lhs.date == rhs.date && lhs.start < rhs.start) return true;
        if (lhs.start == rhs.start && lhs.end < rhs.end) return true;
        return false;
    }
};

struct Query {
    mString trainID;
    int dayPass, arrTime, levTime, st, ed, time, cost;
};

class CmpTime {
public:
    bool operator()(const Query &lhs, const Query &rhs) {
        return lhs.time < rhs.time || ((lhs.time == rhs.time) && (lhs.trainID < rhs.trainID));
    }
};

class CmpCost {
public:
    bool operator()(const Query &lhs, const Query &rhs) {
        return lhs.cost < rhs.cost || ((lhs.cost == rhs.cost) && (lhs.trainID < rhs.trainID));
    }
};

struct Stop {
    mString trainID;
    int st, date;
    friend bool operator<(const Stop &lhs, const Stop &rhs) {
        if (lhs.date < rhs.date) return true;
        if (lhs.date == rhs.date && lhs.st < rhs.st) return true;
        if (lhs.st == rhs.st && lhs.trainID < rhs.trainID) return true;
        if (lhs.trainID == rhs.trainID) return true;
        return false;
    }
};

class Tickets {
public:
    Tickets();

    ~Tickets();

    void add_train();

    void delete_train();

    void release_train();

    void query_train();

    void query_ticket();

    void buy_ticket(UserList &users);

private:

    bool found(mString trainID);

    std::map<mString, Train> trainList;

    std::map<Stop, int> seatList; //线段树优化

    std::multimap<Trip, Query> tripList;
};

#endif //TICKET_SYSTEM_MASTER_TICKETS_HPP
