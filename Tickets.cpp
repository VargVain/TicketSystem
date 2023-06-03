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
            else {
                train.trainID = hash(arg);
                TrainID[train.trainID] = arg;
            }
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
                    train.stations[++num] = hash(token);
                    Place[train.stations[num]] = token;
                    token = "";
                }
                else {
                    token += arg[pos];
                }
                ++pos;
            }
            train.stations[++num] = hash(token);
            Place[train.stations[num]] = token;
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
            train.type = arg[0];
        }
        char c = std::cin.get();
        if (c == '\n' || c == '\r') break;
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
    for (int k = train.saleDate[0], trainNo = 1; k <= train.saleDate[1]; nextDay(k), ++trainNo) {
        mQuerySeat[getNo(train.trainID, trainNo)].set(train.stationNum, train.seatNum);
    }
    trainList[train.trainID] = train;
    std::cout << 0;
}

void Tickets::delete_train() {
    std::string key, arg;
    std::cin >> key >> arg;
    if (!found(arg) || trainList.find(hash(arg))->second.released) {
        std::cout << -1;
        return;
    }
    trainList.erase(hash(arg));
    std::cout << 0;
}

bool Tickets::found(mString trainID) {
    return trainList.find(hash(trainID)) != trainList.end();
}

void Tickets::release_train() {
    std::string key, arg;
    std::cin >> key >> arg;
    if (!found(arg) || trainList.find(hash(arg))->second.released) {
        std::cout << -1;
        return;
    }
    Train train = trainList.find(hash(arg))->second;
    train.released = true;
    trainList[train.trainID] = train;
    int num = train.stationNum, st = train.saleDate[0], ed = train.saleDate[1];
    for (int i = 1; i <= num - 1; ++i) {
        for (int j = i + 1; j <= num; ++j) {
            for (int k = st, trainNo = 1; k <= ed; nextDay(k), ++trainNo) {
                Query1 query1 = {train.stations[i], train.stations[j], nextDays_n(k, train.levDayPass[i])};
                Query2 query2 = {train.trainID, train.stations[i], train.stations[j], nextDays_n(k, train.levDayPass[i])};

                Info info = {train.trainID, trainNo, train.stations[i], train.stations[j], i, j,
                             nextDays_n(k, train.levDayPass[i]), train.leavingTimes[i],
                             nextDays_n(k, train.arrDayPass[j]), train.arrivingTimes[j],
                             0, train.totalPrice[j] - train.totalPrice[i]};
                info.totalTime = passTime(info.arrDate, info.arrTime, info.levDate, info.levTime);
                mQueryTicket.insert(std::make_pair(query1, info));
                mBuyTicket[query2] = info;
            }
            Transfer transfer = {train.stations[j], train.trainID};
            mQueryTransfer.insert(std::make_pair(train.stations[i], transfer));
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
                train = trainList.find(hash(arg))->second;
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
        char c = std::cin.get();
        if (c == '\n' || c == '\r') break;
    }
    if ((date < train.saleDate[0]) || (date > train.saleDate[1])) fail = true;
    if (fail) {
        std::cout << -1;
        return;
    }
    std::cout << TrainID[train.trainID] << " " << train.type;
    int num = train.stationNum, trainNo = passDays(date, train.saleDate[0]);
    for (int i = 1; i <= num; ++i) {
        std::cout << '\n';
        std::string date1, time1, date2, time2;
        if (i == 1) {
            date1 = "xx-xx";
            time1 = "xx:xx";
            date2 = printDay(date);
            time2 = printTime(train.startTime);
            std::cout << Place[train.stations[i]] << " " << date1 << " " << time1 << " -> ";
            std::cout << date2 << " " << time2 << " " << train.totalPrice[i] << " "
                      << mQuerySeat[getNo(train.trainID, trainNo)].seat[i];
        }
        else if (i == num) {
            date1 = printDay(nextDays_n(date, train.arrDayPass[i]));
            time1 = printTime(train.arrivingTimes[i]);
            date2 = "xx-xx";
            time2 = "xx:xx";
            std::cout << Place[train.stations[i]] << " " << date1 << " " << time1 << " -> ";
            std::cout << date2 << " " << time2 << " " << train.totalPrice[i] << " "
                      << "x";
        }
        else {
            date1 = printDay(nextDays_n(date, train.arrDayPass[i]));
            time1 = printTime(train.arrivingTimes[i]);
            date2 = printDay(nextDays_n(date, train.levDayPass[i]));
            time2 = printTime(train.leavingTimes[i]);
            std::cout << Place[train.stations[i]] << " " << date1 << " " << time1 << " -> ";
            std::cout << date2 << " " << time2 << " " << train.totalPrice[i] << " "
                      << mQuerySeat[getNo(train.trainID, trainNo)].seat[i];
        }
    }
}

void Tickets::query_ticket() {
    std::string key, arg;
    Query1 query1;
    bool cost = false;
    while (std::cin >> key >> arg) {
        if (key == "-s") {
            query1.start = hash(arg);
        }
        else if (key == "-t") {
            query1.end = hash(arg);
        }
        else if (key == "-d") {
            std::string token;
            token += arg[0];
            token += arg[1];
            token += arg[3];
            token += arg[4];
            query1.date = atoi(token.c_str());
        }
        else if (key == "-p") {
            if (arg == "cost") cost = true;
        }
        char c = std::cin.get();
        if (c == '\n' || c == '\r') break;
    }
    auto range = mQueryTicket.equal_range(query1);
    if (cost) {
        std::set<Info, CmpCost> ticketSet;
        for (auto it = range.first; it != range.second; ++it) {
            ticketSet.insert(it->second);
        }
        std::cout << ticketSet.size();
        for (auto it = ticketSet.begin(); it != ticketSet.end(); ++it) {
            std::cout << '\n' << TrainID[it->trainID] << " " << Place[query1.start] << " " << printDay(query1.date) << " "
                      << printTime(it->levTime) << " -> " << Place[query1.end] << " " << printDay(it->arrDate)
                      << " " << printTime(it->arrTime) << " " << it->totalCost << " "
                      << mQuerySeat[getNo(it->trainID, it->trainNo)].min(it->st, it->ed);
        }
    }
    else {
        std::set<Info, CmpTime> ticketSet;
        for (auto it = range.first; it != range.second; ++it) {
            ticketSet.insert(it->second);
        }
        std::cout << ticketSet.size();
        for (auto it = ticketSet.begin(); it != ticketSet.end(); ++it) {
            std::cout << '\n' << TrainID[it->trainID] << " " << Place[query1.start] << " " << printDay(query1.date) << " "
                      << printTime(it->levTime) << " -> " << Place[query1.end] << " " << printDay(it->arrDate)
                      << " " << printTime(it->arrTime) << " " << it->totalCost << " "
                      << mQuerySeat[getNo(it->trainID, it->trainNo)].min(it->st, it->ed);
        }
    }
}

void Tickets::buy_ticket(UserList &users) {
    std::string key, arg;
    mString username;
    int date, number, trainID, start, end;
    bool fail = false, queue = false;
    while (std::cin >> key >> arg) {
        if (key == "-u") {
            if (!users.loggedIn(arg)) {
                fail = true;
            }
            else username = arg;
        }
        else if (key == "-i") {
            if (trainList.find(hash(arg)) == trainList.end() || !trainList.find(hash(arg))->second.released) {
                fail = true;
            }
            else trainID = hash(arg);
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
            start = hash(arg);
        }
        else if (key == "-t") {
            end = hash(arg);
        }
        else if (key == "-q") {
            if (arg == "true") queue = true;
        }
        char c = std::cin.get();
        if (c == '\n' || c == '\r') break;
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
    Query2 query2 = {trainID, start, end, date};
    if (mBuyTicket.find(query2) == mBuyTicket.end()) {
        std::cout << -1;
        return;
    }
    Info info = mBuyTicket.find(query2)->second;

    int checkMin = mQuerySeat[getNo(info.trainID, info.trainNo)].min(info.st, info.ed);
    if (number <= checkMin) {
        mQuerySeat[getNo(info.trainID, info.trainNo)].buy(info.st, info.ed, number);
        std::cout << number * info.totalCost;
        OrderInfo orderInfo = {0, info.trainID, info.trainNo, info.start, info.end, info.st, info.ed, info.levDate, info.levTime,
                               info.arrDate, info.arrTime, info.totalCost, number};
        mQueryOrder.insert(std::make_pair(username, static_cast<int>(vQueryOrder.size())));
        vQueryOrder.push_back(orderInfo);
    }
    else {
        if (queue) {
            std::cout << "queue";
            PendingInfo pendingInfo = {username, start, end, info.st, info.ed, number, static_cast<int>(vQueryOrder.size())};
            mQueryPending.insert(std::make_pair(getNo(info.trainID, info.trainNo), pendingInfo)); //真的吗？插入在最右边？
            OrderInfo orderInfo = {1, info.trainID, info.trainNo, info.start, info.end, info.st, info.ed, info.levDate, info.levTime,
                                   info.arrDate, info.arrTime, info.totalCost, number};
            mQueryOrder.insert(std::make_pair(username, static_cast<int>(vQueryOrder.size())));
            vQueryOrder.push_back(orderInfo);
        }
        else {
            std::cout << -1;
        }
    }
}

void Tickets::query_order(UserList &users) {
    std::string key, arg;
    mString username;
    std::cin >> key >> arg;
    if (!users.loggedIn(arg)) {
        std::cout << -1;
        return;
    }
    username = arg;
    auto range = mQueryOrder.equal_range(username);
    int count = mQueryOrder.count(username);
    std::cout << count;
    if (count == 0) return;
    auto it = range.second;
    while (true) {
        --it;
        vQueryOrder[it->second].print();
        if (it == range.first) break;
    }
}

void Tickets::refund_ticket(UserList &users) {
    std::string key, arg;
    mString username;
    int n = 1;
    bool fail = false;
    while (std::cin >> key >> arg) {
        if (key == "-u") {
            if (!users.loggedIn(arg)) {
                fail = true;
            }
            else username = arg;
        }
        if (key == "-n") {
            n = atoi(arg.c_str());
        }
        char c = std::cin.get();
        if (c == '\n' || c == '\r') break;
    }
    if (fail || mQueryOrder.count(username) < n) {
        std::cout << -1;
        return;
    }
    auto range = mQueryOrder.equal_range(username);
    auto it = range.second;
    for (int i = 1; i <= n; ++i) {it--;}
    if (vQueryOrder[it->second].status == 2) {
        std::cout << -1;
        return;
    }
    else if (vQueryOrder[it->second].status == 0) {
        vQueryOrder[it->second].status = 2;
        OrderInfo orderInfo = vQueryOrder[it->second];
        mQuerySeat[getNo(orderInfo.trainID, orderInfo.trainNo)].buy(orderInfo.st, orderInfo.ed, 0 - orderInfo.number);
        auto rangePending = mQueryPending.equal_range(getNo(orderInfo.trainID, orderInfo.trainNo));
        auto itPending = rangePending.first;
        if (mQueryPending.count(getNo(orderInfo.trainID, orderInfo.trainNo))) {
            for (; itPending != rangePending.second; ++itPending) {
                if (itPending->second.number <= mQuerySeat[getNo(orderInfo.trainID, orderInfo.trainNo)].min(itPending->second.st, itPending->second.ed)) {
                    mQuerySeat[getNo(orderInfo.trainID, orderInfo.trainNo)].buy(itPending->second.st, itPending->second.ed, itPending->second.number);//更新座位数据
                    vQueryOrder[itPending->second.order].status = 0;//更新订单状态
                    auto itErasePending = itPending++;
                    mQueryPending.erase(itErasePending);//移出候补队列
                    if (itPending == rangePending.second) break;
                    itPending--;
                }
            }
        }
        std::cout << "0";
        return;
    }
    else if (vQueryOrder[it->second].status == 1) {
        vQueryOrder[it->second].status = 2;
        OrderInfo orderInfo = vQueryOrder[it->second];
        PendingInfo pendingInfo = {username, orderInfo.start, orderInfo.end, orderInfo.st, orderInfo.ed, orderInfo.number, it->second};
        auto rangePending = mQueryPending.equal_range(getNo(orderInfo.trainID, orderInfo.trainNo));
        for (auto itPending = rangePending.first; itPending != rangePending.second; ++itPending) {
            if (itPending->second == pendingInfo) {
                mQueryPending.erase(itPending);
                std::cout << "0";
                return;
            }
        }
    }
    std::cout << "-1";
}

void Tickets::query_transfer() {
    std::string key, arg;
    Query2 query2;
    int query_start, query_end, date;
    Info minInfo1, minInfo2;
    bool cost = false, found = false;
    while (std::cin >> key >> arg) {
        if (key == "-s") {
            query_start = hash(arg);
        }
        else if (key == "-t") {
            query_end = hash(arg);
        }
        else if (key == "-d") {
            std::string token;
            token += arg[0];
            token += arg[1];
            token += arg[3];
            token += arg[4];
            date = atoi(token.c_str());
        }
        else if (key == "-p") {
            if (arg == "cost") cost = true;
        }
        char c = std::cin.get();
        if (c == '\n' || c == '\r') break;
    }
    auto rangeTransfer = mQueryTransfer.equal_range(query_start);
    for (auto itTransfer = rangeTransfer.first; itTransfer != rangeTransfer.second; ++itTransfer) {
        query2 = {itTransfer->second.trainID, query_start, itTransfer->second.to, date};
        if (mBuyTicket.find(query2) != mBuyTicket.end()) {
            Info info1 = mBuyTicket.find(query2)->second;
            for (int date2 = info1.arrDate; date2 <= 903; nextDay(date2)) {
                Query1 query1 = {info1.end, query_end, date2};
                if (mQueryTicket.find(query1) != mQueryTicket.end()) {
                    auto range = mQueryTicket.equal_range(query1);
                    for (auto it = range.first; it != range.second; ++it) {
                        Info info2 = it->second;
                        if (date2 == info1.arrDate && info2.levTime < info1.arrTime) continue;
                        if (info1.trainID == info2.trainID) continue;
                        if (!found) {
                            minInfo1 = info1;
                            minInfo2 = info2;
                            found = true;
                            continue;
                        }
                        if (cost) {
                            if (CmpCostTransfer(info1, info2, minInfo1, minInfo2)) {
                                minInfo1 = info1;
                                minInfo2 = info2;
                            }
                        }
                        else {
                            if (CmpTimeTransfer(info1, info2, minInfo1, minInfo2)) {
                                minInfo1 = info1;
                                minInfo2 = info2;
                            }
                        }
                    }
                }
            }
        }
    }
    if (!found) std::cout << "0";
    else {
        std::cout << TrainID[minInfo1.trainID] << " " << Place[minInfo1.start] << " " << printDay(date) << " "
                  << printTime(minInfo1.levTime) << " -> " << Place[minInfo1.end] << " " << printDay(minInfo1.arrDate)
                  << " " << printTime(minInfo1.arrTime) << " " << minInfo1.totalCost << " "
                  << mQuerySeat[getNo(minInfo1.trainID, minInfo1.trainNo)].min(minInfo1.st, minInfo1.ed);
        std::cout << '\n' << TrainID[minInfo2.trainID] << " " << Place[minInfo2.start] << " " << printDay(minInfo2.levDate) << " "
                  << printTime(minInfo2.levTime) << " -> " << Place[minInfo2.end] << " " << printDay(minInfo2.arrDate)
                  << " " << printTime(minInfo2.arrTime) << " " << minInfo2.totalCost << " "
                  << mQuerySeat[getNo(minInfo2.trainID, minInfo2.trainNo)].min(minInfo2.st, minInfo2.ed);
    }
}

void Tickets::test() {
    for (auto it : ooo) {
        it.print();
    }
}
