//
//  library1.cpp
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#include "library1.hpp"
#include "EmployeeManager.hpp"

void* Init()
{
    EmployeeManager* new_EM;
    try {
        new_EM = new EmployeeManager();
    } catch(bad_alloc &e) {
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


StatusType AddCompany(void *DS, int CompanyID, int Value)
{
    if(DS == nullptr || CompanyID <= 0 || Value <= 0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->addCompany(CompanyID, Value);
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade)
{
    if(DS == nullptr || CompanyID <= 0 || EmployeeID <=0 || Salary <= 0 || Grade < 0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->AddEmployee(EmployeeID, CompanyID, Salary, Grade);
}



StatusType RemoveEmployee(void *DS, int EmployeeID)
{
    if(DS == nullptr || EmployeeID <=0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->RemoveEmployee(EmployeeID);
}


StatusType RemoveCompany(void *DS, int CompanyID)
{
    if(DS == nullptr || CompanyID <=0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->RemoveCompany(CompanyID);
}

StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees)
{
    if(DS == nullptr || !Value || !NumEmployees || CompanyID <=0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->GetCompanyInfo(CompanyID, Value, NumEmployees);
}

StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade)
{
    if(DS == nullptr || !EmployerID || !Salary || !Grade || EmployeeID <=0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->GetEmployeeInfo(EmployeeID, EmployerID, Salary, Grade);
}


StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease)
{
    if(DS == nullptr || CompanyID <=0 || ValueIncrease <= 0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->IncreaseCompanyValue(CompanyID, ValueIncrease);

}

StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade)
{
    if(DS == nullptr || EmployeeID <=0 || SalaryIncrease <= 0 )
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->PromoteEmployee(EmployeeID, SalaryIncrease, BumpGrade);
}

StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID)
{
    if(DS == nullptr || EmployeeID <=0 || NewCompanyID <= 0 )
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->HireEmployee(EmployeeID, NewCompanyID);
}

StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor)
{
    if(DS == nullptr || TargetID == AcquirerID || AcquirerID <=0 || TargetID <= 0 || Factor < 1.00)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->AcquireCompany(AcquirerID, TargetID, Factor);
}

StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID)
{
    if(DS == nullptr || CompanyID==0 || EmployeeID == nullptr)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->GetHighestEarner(CompanyID, EmployeeID);
}

StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees)
{
    if(DS == nullptr || Employees == nullptr || NumOfEmployees == nullptr || CompanyID == 0)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->GetAllEmployeesBySalary(CompanyID, Employees, NumOfEmployees);
}

StatusType GetHighestEarnerInEachCompany(void* DS, int NumOfCompanies, int **Employees)
{
    if (DS == nullptr || NumOfCompanies <= 0 || !Employees)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->GetHighestEarnerInEachCompany(NumOfCompanies, Employees);
}

StatusType  GetNumEmployeesMatching(void* DS ,int CompanyID, int MinEmployeeID, int MaxEmployeeId,
                                    int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees)
{
    if (DS==nullptr || TotalNumOfEmployees==nullptr || NumOfEmployees==nullptr)
        return INVALID_INPUT;
    if ( CompanyID==0 || MinEmployeeID<0 || MaxEmployeeId<0 || MinSalary<0 || MinGrade<0 || MinEmployeeID>MaxEmployeeId)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->GetNumEmployeesMatching(CompanyID, MinEmployeeID, MaxEmployeeId,
                                                           MinSalary, MinGrade, TotalNumOfEmployees, NumOfEmployees);

}
