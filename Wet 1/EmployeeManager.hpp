//
//  EmployeeManager.hpp
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#ifndef EmployeeManager_hpp
#define EmployeeManager_hpp

#include <stdio.h>
#include "typedefs.h"
#include "AVL_Tree.h"
#include "EmployeeKey.h"
#include "EmployeeData.h"
#include "CompanyData.h"
#include "intObject.h"
#include "library1.hpp"


class EmployeeManager
{
    public:
    // All the Employees in the Program
    IDs_Tree* all_employees_ID;
    
    // All the employees in the Program sorted by salary
    Salaries_Tree* all_employees_salary;
    
    // non-empty companies in the Program
    Non_Empty_Companies_Tree* non_empty_companies;
    
    // companies in the Program
    Companies_Tree* companies;
        
    // constructor and destructor
    EmployeeManager();
    ~EmployeeManager();
    
    // functions.
    StatusType addCompany(int ID, int Val);
    StatusType AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade);
    StatusType RemoveEmployee(int EmployeeID);
    StatusType RemoveCompany(int CompanyID);
    StatusType GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees);
    StatusType GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade);
    StatusType IncreaseCompanyValue(int CompanyID, int ValueIncrease);
    StatusType PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade);
    StatusType HireEmployee(int EmployeeID, int NewCompanyID);
    StatusType AcquireCompany(int AcquirerID, int TargetID, double Factor);
    StatusType GetHighestEarner(int CompanyID, int *EmployeeID);
    StatusType GetAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees);
    StatusType GetHighestEarnerInEachCompany(int NumOfCompanies, int **Employees);
    StatusType GetNumEmployeesMatching(int CompanyID, int MinEmployeeID, int MaxEmployeeId,
    int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees);
};

#endif /* EmployeeManager_hpp */
