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
    
    EmployeeData() : companyID(0), employeeID(0), salary(0), grade(0), company(nullptr) {};
    
    EmployeeData(int cmpID, int empID, int grade, int salary, companyData* company) : companyID(cmpID), employeeID(empID), salary(salary), grade(grade), company(company) {};
    
    EmployeeData(const EmployeeData &Data)  = default;
    EmployeeData& operator=(const EmployeeData &Data) = default;

};

#endif /* EmployeeData_h */
