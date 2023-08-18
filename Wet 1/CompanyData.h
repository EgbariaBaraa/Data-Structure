//
//  CompanyData.h
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#ifndef CompanyData_h
#define CompanyData_h
#include "AVL_Tree.h"
#include "intObject.h"

class EmployeeData;
class EmployeeKey;

class companyData {
    
public:
    int companyID, value, size;
    AVL_Tree<EmployeeKey, EmployeeData*>* employees_salaries;
    AVL_Tree<intObject, EmployeeData*>* employees_ids;
    
    companyData() : companyID(0), value(0), size(0), employees_salaries(nullptr), employees_ids(nullptr) {};
    companyData(int ID, int Val) : companyID(ID), value(Val), size(0)
    {
        employees_salaries = new AVL_Tree<EmployeeKey, EmployeeData*>();
        employees_ids = new AVL_Tree<intObject, EmployeeData*>();
    };
    companyData(const companyData &data) = delete;
    ~companyData()
    {
        delete employees_ids;
        delete employees_salaries;
    }
    
    void employeeInsert(EmployeeKey &key, EmployeeData* &ids_data, EmployeeData* &salaries_data)
    {
        intObject ID(key.employeeID);
        employees_ids->insert(ID, ids_data);
        employees_salaries->insert(key, salaries_data);
        size++;
    }
    
    void employeeRemove(EmployeeKey &key)
    {
        intObject ID(key.employeeID);
        employees_salaries->remove(key);
        employees_ids->remove(ID);
        size--;
    }
    
};

#endif /* CompanyData_h */
