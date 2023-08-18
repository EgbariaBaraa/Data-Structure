//
//  EmployeeData.h
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#ifndef EmployeeData_h
#define EmployeeData_h

class companyData;

class EmployeeData {
    
public:
    int companyID;
    int employeeID;
    int salary;
    int grade;
    companyData* company;
    class EmployeeData* employee;
    
    EmployeeData() : companyID(0), employeeID(0), salary(0), grade(0), company(nullptr), employee(nullptr) {};
    EmployeeData(int cmpID, int empID, int salary, int grade, companyData* company) :  companyID(cmpID), employeeID(empID), salary(salary), grade(grade), company(company), employee(nullptr) {};
    
    EmployeeData(const EmployeeData &Data)
    {
        companyID = Data.companyID;
        employeeID = Data.employeeID;
        salary = Data.salary;
        grade = Data.grade;
        company = Data.company;
        employee = Data.employee;
        
    }
    
    void operator=(const EmployeeData &Data)
    {
        companyID = Data.companyID;
        employeeID = Data.employeeID;
        salary = Data.salary;
        grade = Data.grade;
        company = Data.company;
        employee = Data.employee;
    }
};

#endif /* EmployeeData_h */
