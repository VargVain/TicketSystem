#ifndef TICKET_SYSTEM_MASTER_TICKETS_HPP
#define TICKET_SYSTEM_MASTER_TICKETS_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "mDataType.hpp"
#include "UserList.hpp"
#include "Query.hpp"

struct Train {
    int trainID, stations[128];
    char type;
    int stationNum, seatNum, prices[128], startTime, travelTimes[128], stopoverTimes[128], saleDate[2];
    int arrivingTimes[128], leavingTimes[128], arrDayPass[128], levDayPass[128], totalTime[128], totalPrice[128];
    int released = 0;
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

    void query_order(UserList &users);

    void refund_ticket(UserList &users);

private:

    bool found(mString trainID);

    std::map<int, Train> trainList;
};

#endif //TICKET_SYSTEM_MASTER_TICKETS_HPP
