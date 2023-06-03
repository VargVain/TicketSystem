#include "Query.hpp"

bool CmpCostTransfer(const Info &lhs1, const Info &lhs2, const Info &rhs1, const Info &rhs2) {
    if (lhs1.totalCost + lhs2.totalCost < rhs1.totalCost + rhs2.totalCost) return true;
    else if (lhs1.totalCost + lhs2.totalCost > rhs1.totalCost + rhs2.totalCost) return false;
    else {
        if (passTime(lhs2.arrDate, lhs2.arrTime, lhs1.levDate, lhs1.levTime) < passTime(rhs2.arrDate, rhs2.arrTime, rhs1.levDate, rhs1.levTime)) return true;
        else if (passTime(lhs2.arrDate, lhs2.arrTime, lhs1.levDate, lhs1.levTime) > passTime(rhs2.arrDate, rhs2.arrTime, rhs1.levDate, rhs1.levTime)) return false;
        else {
            if (TrainID[lhs1.trainID] < TrainID[rhs1.trainID]) return true;
            else if (TrainID[lhs1.trainID] > TrainID[rhs1.trainID]) return false;
            if (TrainID[lhs2.trainID] < TrainID[rhs2.trainID]) return true;
        }
    }
    return false;
}

bool CmpTimeTransfer(const Info &lhs1, const Info &lhs2, const Info &rhs1, const Info &rhs2) {
    if (passTime(lhs2.arrDate, lhs2.arrTime, lhs1.levDate, lhs1.levTime) < passTime(rhs2.arrDate, rhs2.arrTime, rhs1.levDate, rhs1.levTime)) return true;
    else if (passTime(lhs2.arrDate, lhs2.arrTime, lhs1.levDate, lhs1.levTime) > passTime(rhs2.arrDate, rhs2.arrTime, rhs1.levDate, rhs1.levTime)) return false;
    else {
        if (lhs1.totalCost + lhs2.totalCost < rhs1.totalCost + rhs2.totalCost) return true;
        else if (lhs1.totalCost + lhs2.totalCost > rhs1.totalCost + rhs2.totalCost) return false;
        else {
            if (TrainID[lhs1.trainID] < TrainID[rhs1.trainID]) return true;
            else if (TrainID[lhs1.trainID] > TrainID[rhs1.trainID]) return false;
            if (TrainID[lhs2.trainID] < TrainID[rhs2.trainID]) return true;
        }
    }
    return false;
}
