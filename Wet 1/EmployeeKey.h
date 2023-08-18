//
//  EmployeeKey.h
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#ifndef EmployeeKey_h
#define EmployeeKey_h

class EmployeeKey {

public:
    int employeeID;
    int salary;

    EmployeeKey() : employeeID(0), salary(0) {};
    EmployeeKey(int ID, int salary) : employeeID(ID), salary(salary) {};

    //**** operators ****//
    bool operator>(EmployeeKey key1)
    {
        if(this->salary > key1.salary)
            return true;
        else if(this->salary < key1.salary)
            return false;
        else
        {
            if(this->employeeID < key1.employeeID)
                return true;
            return false;
        }
    }

    bool operator>=(EmployeeKey key1)
    {
        return key1==(*this) || (*this)>key1 ;
    }

    bool operator<(EmployeeKey key1)
    {
        return !((*this)>=key1) ;
    }

    bool operator==(EmployeeKey key1)
    {
        return this->salary == key1.salary && this->employeeID == key1.employeeID;
    }

    bool operator!=(EmployeeKey key1)
    {
        return !(*this==key1);
    }

    void operator=(EmployeeKey key1)
    {
        this->employeeID = key1.employeeID;
        salary = key1.salary;
    }
};

#endif /* EmployeeKey_h */
