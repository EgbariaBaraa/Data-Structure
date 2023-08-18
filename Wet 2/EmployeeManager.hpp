//
//  EmployeeManager.hpp
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#ifndef EmployeeManager_hpp
#define EmployeeManager_hpp

#include <stdio.h>

#include "RankTree.h"
#include "EmployeeKey.h"
#include "EmployeeData.h"
#include "CompanyData.h"
#include "library2.h"
#include "typedefs.h"


class EmployeeManager
{
    public:
    
    // Rank Tree of All the employees in the Program sorted by their salary
    EmployeesTree* Paid_Employees;
    
    // non-empty companies in the Program
    EmployeesHash* All_Employees;
    
    // companies in the Program
    CompaniesUF* companies;
    
    int companies_num;
    long long int grades_sum_for_unpaid;
    long long int unpaid_employees_num;
        
    // constructor and destructor
    EmployeeManager() = delete;
    EmployeeManager(int companies_num);
    ~EmployeeManager();
    
    // functions.
    StatusType AddEmployee(int EmployeeID, int CompanyID, int Grade);
    StatusType RemoveEmployee(int EmployeeID);
    StatusType AcquireCompany(int AcquirerID, int TargetID, double Factor);
    StatusType EmployeeSalaryIncrease(int EmployeeID, int SalaryIncrease);
    StatusType PromoteEmployee(int EmployeeID, int BumpGrade);
    StatusType SumOfBumpGradeBetweenTopWorkersByGroup(int CompanyID, int m);
    StatusType AverageBumpGradeBetweenSalaryByGroup(int CompanyID, int lowerSalary, int higherSalary);
    StatusType CompanyValue(int CompanyID);
};

#endif /* EmployeeManager_hpp */
