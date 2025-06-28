//
// Created by hp on 2025/6/14.
//

#include "employee.h"
Employee::Employee(int id, std::string name, int gender, long phone, int salary, std::string job) {
    this->id = id;
    this->name = name;
    this->gender = gender;
    this->phone = phone;
    this->salary = salary;
    this->job = job;
}
Employee::Employee() {
    std::cout << "Employee" << std::endl;
}
Employee::~Employee() {
    std::cout << "~Employee" << std::endl;
}
void Employee::printemp(){
//    int id;
//    std::string name;
//    int gender;
//    int phone;
//    int salary;
//    std::string job;
    std::cout << "员工编号：" << id << std::endl;
    std::cout << "员工姓名：" << name <<  std::endl;
    std::cout << "员工性别：" << gender << std::endl;
    std::cout << "员工电话：" << phone << std::endl;
    std::cout << "员工薪资：" << salary << std::endl;
    std::cout << "员工职位：" << job << std::endl;
}
