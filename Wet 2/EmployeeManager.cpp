//
//  EmployeeManager.cpp
//  wet #2
//
//  Created by Baraa Egbaria on 03/06/2022.
//

#include "EmployeeManager.hpp"
#include "typedefs.h"

template<class Key>
static void mergeKeys(Key* Arr, long long int start, long long int mid, long long int end);
static void mergeEmployeeSalary(EmployeeData* Arr, long long int start, long long int mid, long long int end);
static void RemoveEmployeeFromSystem(EmployeeManager* DS, EmployeeData& to_remove);
static void insertEmployeeToSystem(EmployeeManager* DS, EmployeeData& to_add);
static void BumpAvgSumInTree(EmployeesTree* cmpEmployeesTree, int lowerSalary, int higherSalary, long long int* grades_sum, long long int* employees_num);

EmployeeManager::EmployeeManager(int companies_num)
{
    this->All_Employees = new EmployeesHash();
    this->Paid_Employees = new EmployeesTree();
    this->companies = new CompaniesUF(companies_num);
    this->companies_num = companies_num;
    grades_sum_for_unpaid = 0;
    unpaid_employees_num = 0;
}

EmployeeManager::~EmployeeManager()
{
    delete Paid_Employees;
    delete All_Employees;
    delete companies;
}


StatusType EmployeeManager::AddEmployee(int EmployeeID, int CompanyID, int Grade)
{
    companyData* cmp = companies->Find(CompanyID)->acquirer;
    if(All_Employees->Contains(EmployeeID) == true)
        return FAILURE;
    try
    {
        EmployeeData to_add(CompanyID, EmployeeID, Grade, 0, cmp);
        cmp->InsertEmployee(to_add);
        insertEmployeeToSystem(this, to_add);
    } catch(std::bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}



StatusType EmployeeManager::RemoveEmployee(int EmployeeID)
{
    EmployeeData* to_remove_temp = All_Employees->find(EmployeeID);
    if(to_remove_temp == nullptr)
        return FAILURE;

    companyData* cmp = to_remove_temp->company;
    EmployeeData to_remove(*to_remove_temp);
    try {
        cmp->RemoveEmployee(to_remove);
        RemoveEmployeeFromSystem(this, to_remove);
    } catch (std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    } catch(...)
    {
        return FAILURE;
    }

    return SUCCESS;
}

StatusType EmployeeManager::AcquireCompany(int AcquirerID, int TargetID, double Factor) // NOLINT(readability-make-member-function-const)
{
    if(companies->Find(AcquirerID) == companies->Find(TargetID))
        return INVALID_INPUT;

    companyData* acquirer = companies->Find(AcquirerID)->acquirer;
    companyData* target = companies->Find(TargetID)->acquirer;

    long long int acquirer_size, target_size, new_size, i=0;
    acquirer_size = acquirer->paid_employees->getSize();
    target_size = target->paid_employees->getSize();
    new_size = acquirer_size + target_size;

    acquirer->size += target->size;
    acquirer->grades_sum_for_unpaid += target->grades_sum_for_unpaid;
    acquirer->unpaid_employees_num += target->unpaid_employees_num;

    for (i = 0; i < target->all_employees->size; ++i)
    {
        while (target->all_employees->arr[i] != nullptr && target->all_employees->arr[i]->head != nullptr)
        {
            EmployeeData to_add = *target->all_employees->arr[i]->head->info;
            to_add.company = acquirer;
            to_add.companyID = acquirer->companyID;
            acquirer->all_employees->insert(to_add.employeeID, &to_add);
            EmployeeData* employee = All_Employees->find(to_add.employeeID);
            employee->company = acquirer;
            employee->companyID = acquirer->companyID;
            target->all_employees->arr[i]->deletefirst();
            (target->all_employees->used)--;
        }
    }

    EmployeeData* arr_sal_data = nullptr;
    EmployeeKey* arr_sal_key = nullptr;
    int* grades_arr = nullptr;

    try {
        arr_sal_data = new EmployeeData[new_size];
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    try {
        arr_sal_key = new EmployeeKey[new_size];
    } catch (std::bad_alloc& e) {

        delete[] arr_sal_data;
        return ALLOCATION_ERROR;
    }
    try {
        grades_arr = new int [new_size];
    } catch (std::bad_alloc& e) {

        delete[] arr_sal_data;
        delete[] arr_sal_key;
        return ALLOCATION_ERROR;
    }

    i=0;
    acquirer->paid_employees->TreeKey_to_array(acquirer->paid_employees->getRoot(), arr_sal_key, &i);
    target->paid_employees->TreeKey_to_array(target->paid_employees->getRoot(), arr_sal_key, &i);
    mergeKeys<EmployeeKey>(arr_sal_key, 0, acquirer_size, new_size);

    i=0;
    acquirer->paid_employees->TreeData_to_array(acquirer->paid_employees->getRoot(), arr_sal_data, &i);
    target->paid_employees->TreeData_to_array(target->paid_employees->getRoot(), arr_sal_data, &i);
    mergeEmployeeSalary(arr_sal_data, 0, acquirer_size, new_size);


    // O(new_size)
    for(i=0; i< new_size; i++)
    {
        int employeeID =  arr_sal_data[i].employeeID;
        arr_sal_data[i].company = acquirer;
        arr_sal_data[i].companyID = acquirer->companyID;
        grades_arr[i] = arr_sal_data[i].grade;
        All_Employees->find(employeeID)->company = acquirer;
        All_Employees->find(employeeID)->companyID = acquirer->companyID;
    }

    try
    {
        EmployeesTree* new_tree = new EmployeesTree();
        EmployeesTree* target_tree = new EmployeesTree();
        delete target->paid_employees;
        target->paid_employees = target_tree;
        i=0;
        new_tree->arrayToTree(arr_sal_key, arr_sal_data, grades_arr ,0, new_size-1, &i);
        new_tree->updateRanksForTree(new_tree->getRoot());

        delete acquirer->paid_employees;
        acquirer->paid_employees = new_tree;
        delete [] arr_sal_key;
        delete [] arr_sal_data;
        delete [] grades_arr;
    } catch(...)
    {
        throw ALLOCATION_ERROR; // NOLINT(misc-throw-by-value-catch-by-reference)
    }
    companies->Union(AcquirerID, TargetID, Factor);
    return SUCCESS;
}

StatusType EmployeeManager::EmployeeSalaryIncrease(int EmployeeID, int SalaryIncrease)
{
    EmployeeData* to_remove_temp = All_Employees->find(EmployeeID);
    if(to_remove_temp == nullptr)
        return FAILURE;

    companyData* cmp = to_remove_temp->company;
    EmployeeData to_remove(*to_remove_temp);
    EmployeeData to_add(to_remove.companyID, to_remove.employeeID, to_remove.grade, (to_remove.salary)+SalaryIncrease, cmp);
    try
    {
        cmp->RemoveEmployee(to_remove);
        RemoveEmployeeFromSystem(this, to_remove);
        cmp->InsertEmployee(to_add);
        insertEmployeeToSystem(this, to_add);

    } catch (std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType EmployeeManager::PromoteEmployee(int EmployeeID, int BumpGrade)
{
    EmployeeData* to_remove_temp = All_Employees->find(EmployeeID);
    if(to_remove_temp == nullptr)
        return FAILURE;

    if(BumpGrade <=0 )
        return SUCCESS;

    companyData* cmp = to_remove_temp->company;
    EmployeeData to_remove(*to_remove_temp);
    EmployeeData to_add(to_remove.companyID, to_remove.employeeID, to_remove.grade + BumpGrade, to_remove.salary, cmp);
    try
    {
        cmp->RemoveEmployee(to_remove);
        RemoveEmployeeFromSystem(this, to_remove);
        cmp->InsertEmployee(to_add);
        insertEmployeeToSystem(this, to_add);

    } catch (std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType EmployeeManager::SumOfBumpGradeBetweenTopWorkersByGroup(int CompanyID, int m)
{

    if(CompanyID == 0)
    {
        if(Paid_Employees->getSize() < m )
            return FAILURE;
        auto root = Paid_Employees->getRoot();
        long long int grades_sum = root->grades_sum;
        long long int result = Paid_Employees->sumGradesForRank(root, Paid_Employees->getSize()-m+1, &grades_sum);
        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", result);
        return SUCCESS;
    }
    else if(CompanyID > 0)
    {
        companyData* cmp = companies->Find(CompanyID)->head->card->acquirer;
        if(cmp->paid_employees->getSize() < m )
            return FAILURE;
        auto root = cmp->paid_employees->getRoot();
        long long int grades_sum = root->grades_sum;
        long long int result = cmp->paid_employees->sumGradesForRank(root, cmp->paid_employees->getSize()-m+1, &grades_sum);
        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", result);
        return SUCCESS;
    }
    return SUCCESS;
}

StatusType EmployeeManager::AverageBumpGradeBetweenSalaryByGroup(int CompanyID, int lowerSalary, int higherSalary)
{
    double avg = 0;
    if(CompanyID == 0)
    {
        // if we need only employees from the tree
        if(lowerSalary > 0 && higherSalary > 0)
        {
            // if tree is empty
            if (Paid_Employees->getSize() == 0)
                return FAILURE;
            // if there's no employees in the range
            if( *(Paid_Employees->getMin()->key) > higherSalary)
                return FAILURE;

            long long int grades_sum_in_tree = 0;
            long long int employees_num = 0;
            BumpAvgSumInTree(Paid_Employees, lowerSalary, higherSalary, &grades_sum_in_tree, &employees_num);
            avg = grades_sum_in_tree / ((double) employees_num);
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);

        }
            // if we need only employees from the hash
        else if(lowerSalary == 0 && higherSalary == 0)
        {
            // Software has no Employees is empty
            if(unpaid_employees_num == 0)
                return FAILURE;
            else
            {
                avg =  grades_sum_for_unpaid / ((double) unpaid_employees_num);
                printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);
                return SUCCESS;
            }
        }
            // if we need employees from the hash and the tree --> ( lowerSalary == 0 && higherSalary > 0)
        else
        {
            // if all employees in the tree are bigger;
            if( *(Paid_Employees->getMin()->key) > higherSalary)
            {
                if(unpaid_employees_num == 0)
                    return FAILURE;
                else
                {
                    avg =  grades_sum_for_unpaid / ((double) unpaid_employees_num);
                    printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);
                    return SUCCESS;
                }
            }

            long long int grades_sum_in_tree = 0;
            long long int employees_num = 0;
            BumpAvgSumInTree(Paid_Employees, lowerSalary, higherSalary, &grades_sum_in_tree, &employees_num);
            avg = (grades_sum_in_tree+grades_sum_for_unpaid) / ((double) employees_num+unpaid_employees_num);
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);
        }
    }
    if(CompanyID > 0)
    {
        companyData* cmp = companies->Find(CompanyID)->acquirer;
        EmployeesTree* cmpEmployeesTree = cmp->paid_employees;

        // if we need only employees from the tree
        if(lowerSalary > 0 && higherSalary > 0)
        {
            // if tree is empty
            if (cmpEmployeesTree->getSize() == 0)
                return FAILURE;
            // if there's no employees in the range
            if( *(cmpEmployeesTree->getMin()->key) > higherSalary)
                return FAILURE;

            long long int grades_sum_in_tree = 0;
            long long int employees_num = 0;
            BumpAvgSumInTree(cmpEmployeesTree, lowerSalary, higherSalary, &grades_sum_in_tree, &employees_num);
            avg = grades_sum_in_tree / ((double) employees_num);
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);

        }
            // if we need only employees from the hash
        else if(lowerSalary == 0 && higherSalary == 0)
        {
            // Software has no Employees is empty
            if(cmp->unpaid_employees_num == 0)
                return FAILURE;
            else
            {
                avg =  cmp->grades_sum_for_unpaid / ((double) cmp->unpaid_employees_num);
                printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);
                return SUCCESS;
            }
        }
            // if we need employees from the hash and the tree --> ( lowerSalary == 0 && higherSalary > 0)
        else
        {
            // if all employees in the tree are bigger;
            if( *(cmpEmployeesTree->getMin()->key) > higherSalary)
            {
                if(cmp->unpaid_employees_num == 0)
                    return FAILURE;
                else
                {
                    avg =  cmp->grades_sum_for_unpaid / ((double) cmp->unpaid_employees_num);
                    printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);
                    return SUCCESS;
                }
            }

            long long int grades_sum_in_tree = 0;
            long long int employees_num = 0;
            BumpAvgSumInTree(cmpEmployeesTree, lowerSalary, higherSalary, &grades_sum_in_tree, &employees_num);
            avg = (grades_sum_in_tree+(cmp->grades_sum_for_unpaid)) / ((double) employees_num+(cmp->unpaid_employees_num));
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);
        }
    }
    return SUCCESS;
}


StatusType EmployeeManager::CompanyValue(int CompanyID) // NOLINT(readability-make-member-function-const)
{
    double rank = companies->returnSumOfValues(CompanyID);
    rank += companies->companies_arr[CompanyID]->data->value;
    printf("CompanyValue: %.1f\n", rank);
    return SUCCESS;
}


template<class Key>
static void mergeKeys(Key* Arr, long long int start, long long int mid, long long int end)
{
    Key temp[(end - start )];
    long long int i = start, j = mid, k = 0;
    while(i < mid && j < end) {
        if(Arr[i] < Arr[j])
        {
            temp[k] = Arr[i];
            k += 1; i += 1;
        }
        else {
            temp[k] = Arr[j];
            k += 1; j += 1;
        }
    }
    while(i < mid) {
        temp[k] = Arr[i];
        k += 1; i += 1;
    }
    while(j < end) {
        temp[k] = Arr[j];
        k += 1; j += 1;
    }
    for(i = start; i < end; i += 1) {
        Arr[i] = temp[i - start];
    }
}

static void mergeEmployeeSalary(EmployeeData* Arr, long long int start, long long int mid, long long int end)
{
    EmployeeData temp[(end - start )];
    long long int i = start, j = mid, k = 0;
    while(i < mid && j < end)
    {
        if(Arr[i].salary < Arr[j].salary || ( Arr[i].salary == Arr[j].salary && Arr[i].employeeID <= Arr[j].employeeID))
        {
            temp[k] = Arr[i];
            k += 1; i += 1;
        }
        else
        {
            temp[k] = Arr[j];
            k += 1; j += 1;
        }
    }
    while(i < mid) {
        temp[k] = Arr[i];
        k += 1; i += 1;
    }
    while(j < end) {
        temp[k] = Arr[j];
        k += 1; j += 1;
    }
    for(i = start; i < end; i += 1) {
        Arr[i] = temp[i - start];
    }
}


static void RemoveEmployeeFromSystem(EmployeeManager* DS, EmployeeData& to_remove)
{
    DS->All_Employees->remove(to_remove.employeeID);
    if(to_remove.salary > 0)
    {
        EmployeeKey empKey(to_remove.employeeID, to_remove.salary);
        DS->Paid_Employees->remove(empKey);
    }
    if(to_remove.salary == 0)
    {
        DS->grades_sum_for_unpaid -= to_remove.grade;
        DS->unpaid_employees_num -= 1;
    }

}

static void insertEmployeeToSystem(EmployeeManager* DS, EmployeeData& to_add)
{
    DS->All_Employees->insert(to_add.employeeID, &to_add);
    if(to_add.salary > 0)
    {
        EmployeeKey empKey(to_add.employeeID, to_add.salary);
        DS->Paid_Employees->insert(empKey, to_add, to_add.grade);
    }
    if(to_add.salary == 0) {
        DS->grades_sum_for_unpaid += to_add.grade;
        DS->unpaid_employees_num += 1;
    }
}


static void BumpAvgSumInTree(EmployeesTree* cmpEmployeesTree, int lowerSalary, int higherSalary, long long int* grades_sum, long long int* employees_num)
{
    GNode<EmployeeKey, EmployeeData>* smaller_salary = cmpEmployeesTree->getRoot(), *higher_salary = cmpEmployeesTree->getRoot();

    getClosestKey(cmpEmployeesTree->getRoot(), smaller_salary, lowerSalary, false);
    getClosestKey(cmpEmployeesTree->getRoot(), higher_salary, higherSalary, true);

    // get ranks of the nodes
    long long int rank_of_smaller = cmpEmployeesTree->rank(*(smaller_salary->key));
    long long int rank_of_higher = cmpEmployeesTree->rank(*(higher_salary->key));

    // get the total sum of the nodes that are bigger
    long long int temp_total_sum = cmpEmployeesTree->getRoot()->grades_sum;
    long long int sum_bigger_than_lower = cmpEmployeesTree->sumGradesForRank(cmpEmployeesTree->getRoot(), rank_of_smaller, &temp_total_sum);
    temp_total_sum = cmpEmployeesTree->getRoot()->grades_sum;
    long long int sum_bigger_than_higher = cmpEmployeesTree->sumGradesForRank(cmpEmployeesTree->getRoot(), rank_of_higher, &temp_total_sum);

    *grades_sum = sum_bigger_than_lower - sum_bigger_than_higher + higher_salary->grade;
    *employees_num =  rank_of_higher - rank_of_smaller + 1;
}
