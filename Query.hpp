#ifndef TICKET_SYSTEM_MASTER_QUERY_HPP
#define TICKET_SYSTEM_MASTER_QUERY_HPP

#include <iostream>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include "mDataType.hpp"

struct Info {
    int trainID, trainNo, start, end, st, ed, levDate, levTime, arrDate, arrTime, totalTime, totalCost;
};

struct Query1 {
    int start, end, date;
    friend bool operator<(const Query1 &lhs, const Query1 &rhs) {
        if (lhs.start < rhs.start) return true;
        if (lhs.start == rhs.start && lhs.end < rhs.end) return true;
        if (lhs.end == rhs.end && lhs.date < rhs.date) return true;
        return false;
    }
};

struct Query2 {
    int trainID, start, end, date;
    friend bool operator<(const Query2 &lhs, const Query2 &rhs) {
        if (lhs.start < rhs.start) return true;
        if (lhs.start == rhs.start && lhs.end < rhs.end) return true;
        if (lhs.end == rhs.end && lhs.date < rhs.date) return true;
        if (lhs.date == rhs.date && lhs.trainID < rhs.trainID) return true;
        return false;
    }
};

class CmpTime {
public:
    bool operator()(const Info &lhs, const Info &rhs) {
        return lhs.totalTime < rhs.totalTime || ((lhs.totalTime == rhs.totalTime) && (lhs.trainID < rhs.trainID));
    }
};

class CmpCost {
public:
    bool operator()(const Info &lhs, const Info &rhs) {
        return lhs.totalCost < rhs.totalCost || ((lhs.totalCost == rhs.totalCost) && (lhs.trainID < rhs.trainID));
    }
};

struct QuerySeat { //线段树优化？感觉没啥必要
    int seat[128];
    void set(int stationNum, int n) {
        for (int i = 1; i <= stationNum; ++i) {
            seat[i] = n;
        }
    }
    void buy(int start, int end, int n) {
        for (int i = start; i < end; ++i) {
            seat[i] -= n;
        }
    }
    int min(int start, int end) {
        int min = seat[start];
        for (int i = start; i < end; ++i) {
            min = seat[i] < min ? seat[i] : min;
        }
        return min;
    }
};

struct PendingInfo {
    mString username;
    int start, end, st, ed, number, order;
    friend bool operator==(const PendingInfo &lhs, const PendingInfo &rhs) {
        return lhs.order == rhs.order;
    }
};

struct OrderInfo {
    int status; //0-success 1-pending 2-refunded
    int trainID, trainNo, start, end, st, ed, levDate, levTime, arrDate, arrTime, price, number;

    void print() {
        std::cout << '\n';
        if (status == 0) std::cout << "[success] ";
        else if (status == 1) std::cout << "[pending] ";
        else std::cout << "[refunded] ";
        std::cout << TrainID[trainID] << " " << Place[start] << " " << printDay(levDate) << " " << printTime(levTime)
                  << " -> " << Place[end] << " " << printDay(arrDate) << " " << printTime(arrTime) << " "
                  << price << " " << number;
    }
};

static std::multimap<Query1, Info> mQueryTicket;

static std::map<Query2, Info> mBuyTicket;

static std::multimap<int, PendingInfo> mQueryPending;

static std::multimap<mString, int> mQueryOrder;

static std::vector<OrderInfo> vQueryOrder;

static QuerySeat mQuerySeat[200000];

#endif //TICKET_SYSTEM_MASTER_QUERY_HPP
