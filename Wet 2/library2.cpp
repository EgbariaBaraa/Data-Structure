//
//  library1.cpp
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#include "library2.h"
#include "EmployeeManager.hpp"
#include <new>

void* Init(int k)
{
    if (k<=0)
        return nullptr;
    EmployeeManager* new_EM;
    try {
        new_EM = new EmployeeManager(k);
    } catch(std::bad_alloc &e) {
        return nullptr;
    }
    return new_EM;
}

void Quit(void** DS)
{
    if(DS == nullptr)
        return;
    delete (EmployeeManager *)(*DS);
    *DS = NULL;
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Grade)
{
    if(DS == nullptr || CompanyID <= 0 || CompanyID > (((EmployeeManager*)DS)->companies_num) || EmployeeID <=0 || Grade < 0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->AddEmployee(EmployeeID, CompanyID, Grade);
}

StatusType RemoveEmployee(void *DS, int EmployeeID)
{
    if(DS == nullptr || EmployeeID <=0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->RemoveEmployee(EmployeeID);
}

StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor)
{
    if(DS == nullptr)
        return INVALID_INPUT;
    int size = (((EmployeeManager*)DS)->companies_num);
    if(AcquirerID <=0 || AcquirerID > size || TargetID <= 0 || TargetID > size  || Factor <= 0.0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->AcquireCompany(AcquirerID, TargetID, Factor);
}

StatusType EmployeeSalaryIncrease(void *DS, int EmployeeID, int SalaryIncrease)
{
    if(DS == nullptr || EmployeeID <=0 || SalaryIncrease <=0 )
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->EmployeeSalaryIncrease(EmployeeID, SalaryIncrease);
}

StatusType PromoteEmployee(void *DS, int EmployeeID, int BumpGrade)
{
    if(DS == nullptr || EmployeeID <=0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->PromoteEmployee(EmployeeID, BumpGrade);
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m)
{
    if(DS == nullptr || companyID < 0 || companyID > (((EmployeeManager*)DS)->companies_num) || m <=0 )
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->SumOfBumpGradeBetweenTopWorkersByGroup(companyID, m);
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary)
{
    if(DS == nullptr)
        return INVALID_INPUT;
    if(companyID < 0 || companyID > (((EmployeeManager*)DS)->companies_num) || higherSalary < 0 || lowerSalary < 0 || lowerSalary > higherSalary)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->AverageBumpGradeBetweenSalaryByGroup(companyID, lowerSalary, higherSalary);
}

StatusType CompanyValue(void *DS, int companyID)
{
    if(DS == nullptr || companyID <= 0 || companyID > (((EmployeeManager*)DS)->companies_num) )
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->CompanyValue(companyID);
}

