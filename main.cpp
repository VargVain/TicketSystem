#include <iostream>
#include <string>
#include "UserList.hpp"
#include "Tickets.hpp"
#pragma execution_character_set("gbk");

int main() {
    //freopen("in.in", "r", stdin);
    //freopen("out.out", "w", stdout);
    std::string time, opt;
    UserList users;
    Tickets tickets;
    while (std::cin >> time >> opt) {
        std::cout << time << " ";
        if (opt == "add_user") {
            users.add_user();
        }
        else if (opt == "login") {
            users.login();
        }
        else if (opt == "logout") {
            users.logout();
        }
        else if (opt == "query_profile") {
            users.query_profile();
        }
        else if (opt == "modify_profile") {
            users.modify_profile();
        }
        else if (opt == "add_train") {
            tickets.add_train();
        }
        else if (opt == "delete_train") {
            tickets.delete_train();
        }
        else if (opt == "release_train") {
            tickets.release_train();
        }
        else if (opt == "query_train") {
            tickets.query_train();
        }
        else if (opt == "query_ticket") {
            tickets.query_ticket();
        }
        else if (opt == "query_transfer") {
            //TODO
        }
        else if (opt == "buy_ticket") {
            tickets.buy_ticket(users);
        }
        else if (opt == "query_order") {
            //TODO
        }
        else if (opt == "refund_ticket") {
            //TODO
        }
        else if (opt == "clean") {
            //TODO
        }
        else if (opt == "exit") {
            users.exit();
            std::cout << '\n';
            break;
        }
        std::cout << '\n';
    }
    return 0;
}
