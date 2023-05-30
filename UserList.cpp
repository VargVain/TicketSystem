#include "UserList.hpp"

UserList::UserList() {

}

UserList::~UserList() {

}

void UserList::add_user() {
    User user;
    int cp;
    std::string key, arg;
    bool fail = false;
    while (std::cin >> key >> arg) {
        if (key == "-c") {
            if (!loggedIn(arg) && !list.empty()) fail = true;
            else cp = list.find(arg)->second.privilege;
        }
        else if (key == "-u") {
            if (found(arg)) fail = true;
            user.username = arg;
        }
        else if (key == "-p") {
            user.password = arg;
        }
        else if (key == "-n") {
            user.name = arg;
        }
        else if (key == "-m") {
            user.mailAddr = arg;
        }
        else if (key == "-g") {
            user.privilege = atoi(arg.c_str());
        }
        if (std::cin.get() == '\n') break;
    }
    if (list.empty()) {
        cp = 11;
        user.privilege = 10;
    }
    if (cp <= user.privilege) fail = true;
    if (fail) {
        std::cout << -1;
        return;
    }
    list[user.username] = user;
    std::cout << 0;
}

void UserList::login() {
    std::string key, arg;
    mString username, password;
    bool fail = false;
    while (std::cin >> key >> arg) {
        if (key == "-u") {
            if (!found(arg)) fail = true;
            if (loggedIn(arg)) fail = true;
            username = arg;
        }
        if (key == "-p") {
            password = arg;
        }
        if (std::cin.get() == '\n') break;
    }
    if (list.find(username)->second.password != password) fail = true;
    if (fail) {
        std::cout << -1;
        return;
    }
    loginList.insert(username);
    std::cout << 0;
}

void UserList::logout() {
    std::string key, arg;
    std::cin >> key >> arg;
    if (!loggedIn(arg)) {
        std::cout << -1;
        return;
    }
    loginList.erase(arg);
    std::cout << 0;
}

void UserList::query_profile() {
    User user;
    int cp;
    mString cu;
    std::string key, arg;
    bool fail = false;
    while (std::cin >> key >> arg) {
        if (key == "-c") {
            if (!loggedIn(arg)) fail = true;
            else {
                cp = list.find(arg)->second.privilege;
                cu = list.find(arg)->second.username;
            }
        }
        if (key == "-u") {
            if (!found(arg)) fail = true;
            else {
                user = list.find(arg)->second;
            }
        }
        if (std::cin.get() == '\n') break;
    }
    if (cp <= user.privilege && cu != user.username) fail = true;
    if (fail) {
        std::cout << -1;
        return;
    }
    print(user);
}

void UserList::modify_profile() {
    User user;
    int cp;
    mString cu;
    std::string key, arg;
    mString password, name, mailAddr;
    int privilege = 0;
    bool p = false, n = false, m = false, g = false, fail = false;
    while (std::cin >> key >> arg) {
        if (key == "-c") {
            if (!loggedIn(arg)) fail = true;
            else {
                cp = list.find(arg)->second.privilege;
                cu = list.find(arg)->second.username;
            }
        }
        if (key == "-u") {
            if (!found(arg)) fail = true;
            else {
                user = list.find(arg)->second;
            }
        }
        if (key == "-p") {
            password = arg;
            p = true;
        }
        if (key == "-n") {
            name = arg;
            n = true;
        }
        if (key == "-m") {
            mailAddr = arg;
            m = true;
        }
        if (key == "-g") {
            privilege = atoi(arg.c_str());
            g = true;
        }
        if (std::cin.get() == '\n') break;
    }
    if ((cp <= user.privilege && cu != user.username) || cp <= privilege) fail = true;
    if (fail) {
        std::cout << -1;
        return;
    }
    if (p) user.password = password;
    if (n) user.name = name;
    if (m) user.mailAddr = mailAddr;
    if (g) user.privilege = privilege;
    mString username = user.username;
    list.erase(list.find(username));
    list[username] = user;
    print(user);
}

void UserList::exit() {
    loginList.clear();
    std::cout << "bye";
}

bool UserList::found(mString username) {
    return list.find(username) != list.end();
}

bool UserList::loggedIn(mString username) {
    return loginList.find(username) != loginList.end();
}

void UserList::print(User user) {
    std::cout << user.username << " " << user.name << " " << user.mailAddr << " " << user.privilege;
}
