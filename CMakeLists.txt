cmake_minimum_required(VERSION 3.0)
project(Ticket_System_master)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ticket_System_master
        main.cpp
        mDataType.cpp
        UserList.cpp
        Tickets.cpp
        Query.cpp)
