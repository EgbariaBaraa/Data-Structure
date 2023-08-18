//
//  CompanyData.h
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#ifndef CompanyData_h
#define CompanyData_h
#include "RankTree.h"
#include "HashTable.h"

class EmployeeData;
class EmployeeKey;

class companyData {
    
public:
    int companyID, value;
    long long int size;
    long long int unpaid_employees_num;
    long long int grades_sum_for_unpaid;
    RankTree<EmployeeKey, EmployeeData>* paid_employees;
    HashTable<EmployeeData>* all_employees;
    
    companyData() : companyID(0), value(0), size(0), unpaid_employees_num(0), grades_sum_for_unpaid(0), paid_employees(nullptr), all_employees(nullptr) {};
    
    companyData(int ID) : companyID(ID), value(ID), size(0), unpaid_employees_num(0), grades_sum_for_unpaid(0)
    {
        paid_employees = new  RankTree<EmployeeKey, EmployeeData>();
        all_employees = new HashTable<EmployeeData>();
    };
    
    companyData(const companyData &data) = delete;
    
    ~companyData()
    {
        delete paid_employees;
        delete all_employees;
    }
    
    void InsertEmployee(EmployeeData& to_add)
    {
        all_employees->insert(to_add.employeeID, &to_add);
        if(to_add.salary == 0)
        {
            grades_sum_for_unpaid += to_add.grade;
            unpaid_employees_num += 1;
        }
        if(to_add.salary > 0)
        {
            EmployeeKey key(to_add.employeeID, to_add.salary);
            paid_employees->insert(key, to_add, to_add.grade);
        }
        size++;
    }
    
    void RemoveEmployee(EmployeeData& to_remove)
    {
        all_employees->remove(to_remove.employeeID);
        if(to_remove.salary == 0)
        {
            grades_sum_for_unpaid -= to_remove.grade;
            unpaid_employees_num -= 1;
        }
        if(to_remove.salary > 0)
        {
            EmployeeKey key(to_remove.employeeID, to_remove.salary);
            paid_employees->remove(key);
        }
        size--;
    }
    
    bool find(int EmployeeID)
    {
        return all_employees->find(EmployeeID) != nullptr;
    }
};

#endif /* CompanyData_h */
