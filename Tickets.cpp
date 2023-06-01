#include "Tickets.hpp"

Tickets::Tickets() {

}

Tickets::~Tickets() {

}

void Tickets::add_train() {
    Train train;
    std::string key, arg;
    bool fail = false;
    while (std::cin >> key >> arg) {
        if (key == "-i") {
            if (found(arg)) fail = true;
            train.trainID = arg;
        }
        else if (key == "-n") {
            train.stationNum = atoi(arg.c_str());
        }
        else if (key == "-m") {
            train.seatNum = atoi(arg.c_str());
        }
        else if (key == "-s") {
            int pos = 0, num = 0;
            std::string token;
            while(pos < arg.length()) {
                if (arg[pos] == '|'){
                    train.stations[++num] = token;
                    token = "";
                }
                else {
                    token += arg[pos];
                }
                ++pos;
            }
            train.stations[++num] = token;
        }
        else if (key == "-p") {
            int pos = 0, num = 0;
            std::string token;
            while(pos < arg.length()) {
                if (arg[pos] == '|'){
                    train.prices[++num] = atoi(token.c_str());
                    token = "";
                }
                else {
                    token += arg[pos];
                }
                ++pos;
            }
            train.prices[++num] = atoi(token.c_str());
        }
        else if (key == "-x") {
            std::string token;
            token += arg[0];
            token += arg[1];
            token += arg[3];
            token += arg[4];
            train.startTime = atoi(token.c_str());
        }
        else if (key == "-t") {
            int pos = 0, num = 0;
            std::string token;
            while(pos < arg.length()) {
                if (arg[pos] == '|'){
                    train.travelTimes[++num] = atoi(token.c_str());
                    token = "";
                }
                else {
                    token += arg[pos];
                }
                ++pos;
            }
            train.travelTimes[++num] = atoi(token.c_str());
        }
        else if (key == "-o") {
            if (arg != "_") {
                int pos = 0, num = 0;
                std::string token;
                while(pos < arg.length()) {
                    if (arg[pos] == '|'){
                        train.stopoverTimes[++num] = atoi(token.c_str());
                        token = "";
                    }
                    else {
                        token += arg[pos];
                    }
                    ++pos;
                }
                train.stopoverTimes[++num] = atoi(token.c_str());
            }
        }
        else if (key == "-d") {
            std::string token;
            token += arg[0];
            token += arg[1];
            token += arg[3];
            token += arg[4];
            train.saleDate[0] = atoi(token.c_str());
            token = "";
            token += arg[6];
            token += arg[7];
            token += arg[9];
            token += arg[10];
            train.saleDate[1] = atoi(token.c_str());
        }
        else if (key == "-y") {
            train.type = arg;
        }
        if (std::cin.get() == '\n') break;
    }
    if (fail) {
        std::cout << -1;
        return;
    }
    int day = 0, num = train.stationNum, time = train.startTime, totalTime = 0, totalPrice = 0;
    for (int i = 1; i <= num; ++i) {
        int dayPass;
        if (i == 1) {
            train.leavingTimes[1] = time;
            train.levDayPass[1] = 0;
            train.totalTime[1] = 0;
            train.totalPrice[1] = 0;
        }
        else if (i == num) {
            dayPass = addTime(time, train.travelTimes[i - 1]);
            nextDays(day, dayPass);
            train.arrDayPass[i] = day;
            train.arrivingTimes[i] = time;
            totalTime += train.travelTimes[i - 1];
            train.totalTime[i] = totalTime;
            totalPrice += train.prices[i - 1];
            train.totalPrice[i] = totalPrice;
        }
        else {
            dayPass = addTime(time, train.travelTimes[i - 1]);
            nextDays(day, dayPass);
            train.arrDayPass[i] = day;
            train.arrivingTimes[i] = time;
            dayPass = addTime(time, train.stopoverTimes[i - 1]);
            nextDays(day, dayPass);
            train.leavingTimes[i] = time;
            train.levDayPass[i] = day;
            totalTime += train.travelTimes[i - 1] + train.stopoverTimes[i - 1];
            train.totalTime[i] = totalTime;
            totalPrice += train.prices[i - 1];
            train.totalPrice[i] = totalPrice;
        }
    }
    for (int i = 1; i <= num; ++i) {
        for (int k = train.saleDate[0]; k <= train.saleDate[1]; nextDay(k)) {
            Stop stop = {train.trainID, i, nextDays_n(k, train.levDayPass[i])};
            seatList[stop] = train.seatNum;
        }
    }
    trainList[train.trainID] = train;
    std::cout << 0;
}

void Tickets::delete_train() {
    std::string key, arg;
    std::cin >> key >> arg;
    if (!found(arg) || trainList.find(arg)->second.released) {
        std::cout << -1;
        return;
    }
    trainList.erase(arg);
    std::cout << 0;
}

bool Tickets::found(mString trainID) {
    return trainList.find(trainID) != trainList.end();
}

void Tickets::release_train() {
    std::string key, arg;
    std::cin >> key >> arg;
    if (!found(arg) || trainList.find(arg)->second.released) {
        std::cout << -1;
        return;
    }
    Train train = trainList.find(arg)->second;
    train.released = true;
    trainList[train.trainID] = train;
    int num = train.stationNum, seats = train.seatNum, st = train.saleDate[0], ed = train.saleDate[1];
    mString trainID = train.trainID;
    for (int i = 1; i <= num; ++i) {
        for (int j = i + 1; j <= num; ++j) {
            for (int k = st; k <= ed; nextDay(k)) {
                Trip trip = {train.stations[i], train.stations[j], nextDays_n(k, train.levDayPass[i])};
                Query query = {trainID, train.arrDayPass[j] - train.levDayPass[i],
                               train.arrivingTimes[j], train.leavingTimes[i],
                               i, j, train.totalTime[j] - train.totalTime[i] - train.stopoverTimes[i - 1],
                               train.totalPrice[j] - train.totalPrice[i]};
                tripList.insert(std::make_pair(trip, query));
            }
        }
    }
    std::cout << 0;
}

void Tickets::query_train() {
    std::string key, arg;
    Train train;
    int date;
    bool fail = false;
    while (std::cin >> key >> arg) {
        if (key == "-i") {
            if (!found(arg)) fail = true;
            else {
                train = trainList.find(arg)->second;
            }
        }
        else if (key == "-d") {
            std::string token;
            token += arg[0];
            token += arg[1];
            token += arg[3];
            token += arg[4];
            date = atoi(token.c_str());
        }
        if (std::cin.get() == '\n') break;
    }
    if ((date < train.saleDate[0]) || (date > train.saleDate[1])) fail = true;
    if (fail) {
        std::cout << -1;
        return;
    }
    std::cout << train.trainID << " " << train.type;
    int num = train.stationNum;
    for (int i = 1; i <= num; ++i) {
        std::cout << '\n';
        std::string date1, time1, date2, time2;
        if (i == 1) {
            date1 = "xx-xx";
            time1 = "xx:xx";
            date2 = printDay(date);
            time2 = printTime(train.startTime);
        }
        else if (i == num) {
            date1 = printDay(date + train.arrDayPass[i]);
            time1 = printTime(train.arrivingTimes[i]);
            date2 = "xx-xx";
            time2 = "xx:xx";
        }
        else {
            date1 = printDay(date + train.arrDayPass[i]);
            time1 = printTime(train.arrivingTimes[i]);
            date2 = printDay(date + train.levDayPass[i]);
            time2 = printTime(train.leavingTimes[i]);
        }
        std::cout << train.stations[i] << " " << date1 << " " << time1 << " -> ";
        std::cout << date2 << " " << time2 << " " << train.totalPrice[i] << " " << "SEAT";
    }
}

void Tickets::query_ticket() {
    std::string key, arg;
    Trip trip;
    int date;
    bool fail = false, cost = false;
    while (std::cin >> key >> arg) {
        if (key == "-s") {
            trip.start = arg;
        }
        else if (key == "-t") {
            trip.end = arg;
        }
        else if (key == "-d") {
            std::string token;
            token += arg[0];
            token += arg[1];
            token += arg[3];
            token += arg[4];
            trip.date = atoi(token.c_str());
        }
        else if (key == "-p") {
            if (arg == "cost") cost = true;
        }
        if (std::cin.get() == '\n') break;
    }
    auto range = tripList.equal_range(trip);
    if (cost) {
        std::set<Query, CmpCost> ticketSet;
        for (auto it = range.first; it != range.second; ++it) {
            ticketSet.insert(it->second);
        }
        std::cout << ticketSet.size();
        for (auto it = ticketSet.begin(); it != ticketSet.end(); ++it) {
            Stop stop = {it->trainID, it->st, trip.date};
            std::cout << '\n' << it->trainID << " " << trip.start << " " << printDay(trip.date) << " "
                      << printTime(it->levTime) << " -> " << trip.end << " " << printDay(nextDays(trip.date, it->dayPass))
                      << " " << printTime(it->arrTime) << " " << it->cost << " " << seatList.find(stop)->second;
        }
    }
    else {
        std::set<Query, CmpTime> ticketSet;
        for (auto it = range.first; it != range.second; ++it) {
            ticketSet.insert(it->second);
        }
        std::cout << ticketSet.size();
        for (auto it = ticketSet.begin(); it != ticketSet.end(); ++it) {
            Stop stop = {it->trainID, it->st, trip.date};
            std::cout << '\n' << it->trainID << " " << trip.start << " " << printDay(trip.date) << " "
                      << printTime(it->levTime) << " -> " << trip.end << " " << printDay(nextDays(trip.date, it->dayPass))
                      << " " << printTime(it->arrTime) << " " << it->cost << " " << seatList.find(stop)->second;
        }
    }
}

void Tickets::buy_ticket(UserList &users) {
    std::string key, arg;
    mString username, trainID, start, end;
    int date, number;
    bool fail = false, queue = false;
    while (std::cin >> key >> arg) {
        if (key == "-u") {
            if (!users.loggedIn(arg)) fail = true;
            else username = arg;
        }
        else if (key == "-i") {
            if (trainList.find(arg) == trainList.end() || !trainList.find(arg)->second.released) fail = true;
            else trainID = arg;
        }
        else if (key == "-d") {
            std::string token;
            token += arg[0];
            token += arg[1];
            token += arg[3];
            token += arg[4];
            date = atoi(token.c_str());
        }
        else if (key == "-n") {
            number = atoi(arg.c_str());
        }
        else if (key == "-f") {
            start = arg;
        }
        else if (key == "-t") {
            end = arg;
        }
        else if (key == "-q") {
            if (arg == "true") queue = true;
        }
        if (std::cin.get() == '\n') break;
    }
    if (fail) {
        std::cout << -1;
        return;
    }
    Train train = trainList.find(trainID)->second;
    if (number > train.seatNum) {
        std::cout << -1;
        return;
    }
    Trip trip = {start, end, date};
    auto range = tripList.equal_range(trip);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second.trainID == trainID) {
            int st = it->second.st, ed = it->second.ed;
            for (int i = st; i != ed; ++i) {
                Stop stop = {trainID, i, nextDays(date, train.levDayPass[i] - train.levDayPass[st])};
                int seatLeft = seatList.find(stop)->second;
                if (number > seatLeft) fail = true;
            }
            else {
                for (int i = st; i != ed; ++i) {
                    Stop stop = {trainID, i, nextDays(date, train.levDayPass[i] - train.levDayPass[st])};
                    int seatLeft = seatList.find(stop)->second;
                    seatList.erase(stop);
                    seatList[stop] = seatLeft - number;
                }
            }
            break;
        }
    }
    std::cout << -1;
}
