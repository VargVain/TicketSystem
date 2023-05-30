#ifndef TICKET_SYSTEM_MASTER_USERLIST_HPP
#define TICKET_SYSTEM_MASTER_USERLIST_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "mDataType.hpp"

struct User {
    mString username, password, name, mailAddr;
    int privilege;
};

class UserList {
public:
    UserList();

    ~UserList();

    void add_user();

    void login();

    void logout();

    void query_profile();

    void modify_profile();

    void exit();

private:

    bool found(mString username);

    bool loggedIn(mString username);

    void print(User user);

    std::map<mString, User> list;

    std::set<mString> loginList;
};

#endif //TICKET_SYSTEM_MASTER_USERLIST_HPP
