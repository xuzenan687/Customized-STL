//
// Created by hp on 2025/6/14.
//
#include <iostream>
#include <string>
#ifndef C__CODE_EMPLOYEE_H
#define C__CODE_EMPLOYEE_H


class Employee {
public:
    Employee(int id, std::string name, int gender, long phone, int salary, std::string job);
    Employee();
    ~Employee();
    void printemp();
private:
    int id;
    std::string name;
    int gender;
    long phone;
    int salary;
    std::string job;
};


#endif //C__CODE_EMPLOYEE_H
