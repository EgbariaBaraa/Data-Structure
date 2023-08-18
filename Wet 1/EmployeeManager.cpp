//
//  EmployeeManager.cpp
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#include "EmployeeManager.hpp"
#define VALUE 10
#define IDS_ARRAY 0
#define SALARIES_ARRAY 1


/***************************************************************************/
// ****** Auxiliary Functions ******//
/***************************************************************************/

// *** Insert & Remove Auxiliaries *** //
static void removeEmployeeFromSoftware(EmployeeManager* DS, EmployeeData* to_delete);
static void addEmployeeToSoftware(EmployeeManager* DS, EmployeeData* to_add);

// *** AcquireCompany Auxiliaries *** //
static StatusType AcquirerAlloctaeArrays(EmployeeData*** arr_id_data, EmployeeData*** arr_sal_data, intObject** arr_id_key, EmployeeKey** arr_sal_key, companyData* acquirer, companyData* target, int acquirer_size, int target_size);
static void AcquirerFillArrays(EmployeeData** arr_id_data, EmployeeData** arr_sal_data, intObject* arr_id_key, EmployeeKey* arr_sal_key, companyData* acquirer, companyData* target, int acquirer_size, int target_size);
template<class KeyArray, class DaraArray>
static void changeArraysToTrees(KeyArray* arr_key, DaraArray** arr_data, companyData* acquirer, int size, bool id_or_sal);
static void freeAllAllocations(EmployeeData** arr_id_data, EmployeeData** arr_sal_data, intObject* arr_id_key, EmployeeKey* arr_sal_key);
static void updateAcquirerFields(EmployeeManager* DS, companyData* aquirer, companyData* target, double Factor, intObject targetId);

template<class Key>
static void mergeKeys(Key* Arr, Key* Arr2, int start, int mid, int end);
static void mergeEmployeeId(EmployeeData** Arr, EmployeeData** Arr2, int start, int mid, int end);
static void mergeEmployeeSalary(EmployeeData** Arr, EmployeeData** Arr2, int start, int mid, int end);

// ****** InOrder Traversals Used ******//
template<class Key>
static void addInOrderSearch(EmployeeData** arr, Key* arr2, GNode<Key, EmployeeData*>* node, int *i, int *size);
static void GetIdInOrderSearch(int** arr, GNode<intObject, companyData*>* node, int *i, int *size);
template<class D>
static void invertedInOrderSearch(int** arr, GNode<EmployeeKey,D> *node, int *i ,int size);

// the next two are overloaded;
static void oneSidedInOrderSearch(GNode<intObject, EmployeeData*>* root, int MinEmployeeID, int MaxEmployeeID, int minSalary, int minGrade, int* NumOfEmployees, int* TotalNumOfEmployees);
static void oneSidedInOrderSearch(GNode<intObject, EmployeeData>* root, int MinEmployeeID, int MaxEmployeeID, int minSalary, int minGrade, int* NumOfEmployees, int* TotalNumOfEmployees);




/***************************************************************************/
// ****** EmployeeManager Member Functions ******//
/***************************************************************************/

EmployeeManager::EmployeeManager()
{
    this->all_employees_ID = new AVL_Tree<intObject, EmployeeData>();
    this->all_employees_salary = new AVL_Tree<EmployeeKey, EmployeeData>();
    this->companies = new AVL_Tree<intObject, companyData*>();
    this->non_empty_companies = new AVL_Tree<intObject, companyData*>();
}

EmployeeManager::~EmployeeManager()
{
    while(companies->getSize() != 0)
    {
        companyData* to_delete = *companies->getRoot()->data;
        intObject key(to_delete->companyID);
        companies->remove(key);
        delete to_delete;
    }
    delete this->companies;
    delete this->non_empty_companies;
    delete this->all_employees_salary;
    delete this->all_employees_ID;
}

StatusType EmployeeManager::addCompany(int ID, int Val)
{
    intObject cmpID(ID);
    if(companies->find(cmpID) != nullptr)
        return FAILURE;
    try {
        companyData* new_company = new companyData(ID, Val);
        companies->insert(cmpID, new_company);
    } catch(bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType EmployeeManager::AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade)
{
    intObject cmpID(CompanyID), empID(EmployeeID);
    if(companies->find(cmpID) == nullptr)
        return FAILURE;
    if(this->all_employees_ID->find(empID) != nullptr)
        return FAILURE;
    companyData* company = *(companies->find(cmpID)->data);
    EmployeeData newEmployee(company->companyID, EmployeeID, Salary, Grade, company);
    try {
        addEmployeeToSoftware(this, &newEmployee);
        if(non_empty_companies->find(cmpID) == nullptr)
            non_empty_companies->insert(cmpID, *companies->find(cmpID)->data);
    } catch(bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType EmployeeManager::RemoveEmployee(int EmployeeID)
{
    intObject empID(EmployeeID);
    if(all_employees_ID->find(empID) == nullptr)
        return FAILURE;
    EmployeeData* to_delete = all_employees_ID->find(empID)->data;
    intObject cmpID(to_delete->companyID);
    try
    {
        if(to_delete->company->size == 1)
            non_empty_companies->remove(cmpID);
        removeEmployeeFromSoftware(this, to_delete);
    } catch(bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    } catch(...)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType EmployeeManager::RemoveCompany(int CompanyID)
{
    intObject cmpID(CompanyID);

    if(companies->find(cmpID) == nullptr)
        return FAILURE;
    companyData* to_delete = *(companies->find(cmpID)->data);
    if(to_delete->employees_ids->getSize() > 0 )
        return FAILURE;
    try
    {
        companies->remove(cmpID);
        delete to_delete;
    } catch(bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    } catch(...)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType EmployeeManager::GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees)
{
    intObject cmpID(CompanyID);
    if(companies->find(cmpID) == nullptr)
        return FAILURE;
    companyData* company = *(companies->find(cmpID)->data);
    *Value = company->value;
    *NumEmployees = company->employees_ids->getSize();
    return SUCCESS;
}


StatusType EmployeeManager::GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade)
{
    intObject empID(EmployeeID);
    if(all_employees_ID->find(empID) == nullptr)
        return FAILURE;
    EmployeeData* data = all_employees_ID->find(empID)->data;
    *EmployerID = data->companyID;
    *Salary = data->salary;
    *Grade = data->grade;
    return SUCCESS;
}

StatusType EmployeeManager::IncreaseCompanyValue(int CompanyID, int ValueIncrease)
{
    intObject cmpID(CompanyID);
    if(companies->find(cmpID) == nullptr)
        return FAILURE;
    companyData* company = *(companies->find(cmpID)->data);
    company->value += ValueIncrease;
    return SUCCESS;
}

StatusType EmployeeManager::PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade)
{
    intObject empID(EmployeeID);
    if(all_employees_ID->find(empID) == nullptr)
        return FAILURE;
    EmployeeData* to_delete = all_employees_ID->find(empID)->data;
    EmployeeData to_add(to_delete->companyID, EmployeeID, (to_delete->salary)+SalaryIncrease, (to_delete->grade)+(BumpGrade>0), to_delete->company);
    try
    {
        removeEmployeeFromSoftware(this, to_delete);
        addEmployeeToSoftware(this, &to_add);
    } catch(bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    } catch(...)
    {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType EmployeeManager::HireEmployee(int EmployeeID, int NewCompanyID)
{
    intObject empId(EmployeeID), new_cmpId(NewCompanyID);
    if(all_employees_ID->find(empId) == nullptr || companies->find(new_cmpId) == nullptr)
        return FAILURE;
    companyData* new_company = *companies->find(new_cmpId)->data;
    if(new_company->employees_ids->find(empId) != nullptr)
        return FAILURE;
    EmployeeData* to_delete = all_employees_ID->find(empId)->data;
    companyData* old_company = to_delete->company;
    intObject old_cmpId(old_company->companyID);
    EmployeeData to_add(NewCompanyID, EmployeeID, (to_delete->salary),(to_delete->grade), new_company);
    try
    {
        removeEmployeeFromSoftware(this, to_delete);
        if(old_company->size == 0)
            non_empty_companies->remove(old_cmpId);
        addEmployeeToSoftware(this, &to_add);
        if(non_empty_companies->find(new_cmpId) == nullptr)
            non_empty_companies->insert(new_cmpId, new_company);
    } catch(bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


// not finished
StatusType EmployeeManager::AcquireCompany(int AcquirerID, int TargetID, double Factor)
{
    intObject acquirerId(AcquirerID), targetId(TargetID);
    if(companies->find(targetId) == nullptr || companies->find(acquirerId) == nullptr)
        return FAILURE;
    companyData* acquirer = *companies->find(acquirerId)->data;
    companyData* target = *companies->find(targetId)->data;
    if(acquirer->value < VALUE*target->value)
        return FAILURE;

    int acquirer_size, target_size,i=0;
    acquirer_size = acquirer->employees_ids->getSize();
    target_size = target->employees_ids->getSize();

    if(target_size == 0)
    {
        updateAcquirerFields(this, acquirer, target, Factor, targetId);
        return SUCCESS;
    }

    EmployeeData** arr_id_data = nullptr;
    intObject* arr_id_key = nullptr;
    EmployeeData** arr_sal_data = nullptr;
    EmployeeKey* arr_sal_key = nullptr;
    
    if(AcquirerAlloctaeArrays(&arr_id_data, &arr_sal_data, &arr_id_key, &arr_sal_key, acquirer, target, acquirer_size, target_size) == ALLOCATION_ERROR)
        return ALLOCATION_ERROR;
    
    AcquirerFillArrays(arr_id_data, arr_sal_data, arr_id_key, arr_sal_key, acquirer, target, acquirer_size, target_size);


    for(i=0; i<(acquirer_size+target_size); i++)
    {
        arr_id_data[i]->company = acquirer;
        arr_id_data[i]->employee->company = acquirer;
        arr_id_data[i]->companyID = acquirer->companyID;
        arr_sal_data[i]->company = acquirer;
        arr_sal_data[i]->employee->company = acquirer;
        arr_sal_data[i]->companyID = acquirer->companyID;
    }

    try
    {
        // allocate new Trees
        AVL_Tree<intObject, EmployeeData*>* id_Tree = new AVL_Tree<intObject, EmployeeData*>();
        AVL_Tree<EmployeeKey, EmployeeData*>* sal_Tree = new AVL_Tree<EmployeeKey, EmployeeData*>();
        
        // delete old ones
        delete acquirer->employees_ids;
        delete acquirer->employees_salaries;
        
        // replace old Trees with new ones
        acquirer->employees_ids = id_Tree;
        acquirer->employees_salaries = sal_Tree;
        
        changeArraysToTrees<intObject, EmployeeData>(arr_id_key, arr_id_data, acquirer, acquirer_size+target_size-1, IDS_ARRAY);
        changeArraysToTrees<EmployeeKey, EmployeeData>(arr_sal_key, arr_sal_data, acquirer, acquirer_size+target_size-1, SALARIES_ARRAY);
        
        freeAllAllocations(arr_id_data, arr_sal_data,  arr_id_key, arr_sal_key);
        non_empty_companies->remove(targetId);
        if (non_empty_companies->find(acquirerId) == nullptr)
            non_empty_companies->insert(acquirerId,acquirer);
        updateAcquirerFields(this, acquirer, target, Factor, targetId);
    } catch (bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    }
    catch(...)
    {
        return FAILURE;
    }

    return SUCCESS;
}


StatusType EmployeeManager::GetHighestEarner(int CompanyID, int *EmployeeID)
{
    intObject cmpID(CompanyID);
    if(CompanyID > 0)
    {
        if(companies->find(cmpID) == nullptr)
            return FAILURE;
        companyData* company = *(companies->find(cmpID)->data);
        if(company->size == 0)
            return FAILURE;
        *EmployeeID = (*(company->employees_salaries->getMax()->data))->employeeID;
    }
    else
    {
        if(all_employees_salary->getSize() == 0)
            return FAILURE;
        *EmployeeID = all_employees_salary->getMax()->data->employeeID;
    }
    return SUCCESS;
}


StatusType EmployeeManager::GetAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees)
{
    int i = 0;
    if (CompanyID < 0)
    {
        if(all_employees_salary->getSize() == 0)
            return FAILURE;

        (*Employees) = (int*)malloc((sizeof(int))*all_employees_salary->getSize());
        if ((*Employees)==nullptr)
            return ALLOCATION_ERROR;


        invertedInOrderSearch(Employees, all_employees_salary->getRoot(), &i, all_employees_salary->getSize());
        (*NumOfEmployees) = this->all_employees_ID->getSize();

        return SUCCESS;
    }

    if (CompanyID > 0)
    {
        intObject cmp_id(CompanyID);
        GNode<intObject, companyData*> *company = companies->find(cmp_id);
        if(company == nullptr || (*company->data)->size == 0)
            return FAILURE;

        (*Employees) = (int*) malloc((sizeof(int))*(*company->data)->employees_salaries->getSize());
        if ((*Employees) == nullptr)
            return ALLOCATION_ERROR;

        invertedInOrderSearch(Employees, (*company->data)->employees_salaries->getRoot(), &i, (*company->data)->employees_salaries->getSize());
        (*NumOfEmployees) = (*company->data)->employees_salaries->getSize();
    }
    return SUCCESS;
}


StatusType EmployeeManager::GetHighestEarnerInEachCompany(int NumOfCompanies, int **Employees)
{
    if(non_empty_companies->getSize() < NumOfCompanies)
    {
        return FAILURE;
    }

    (*Employees) = (int*) malloc((sizeof(int))*NumOfCompanies);
    if ((*Employees)==nullptr)
    {
        return ALLOCATION_ERROR;
    }

    int i = 0;
    int size = NumOfCompanies;
    GetIdInOrderSearch(Employees , non_empty_companies->getRoot(), &i , &size);
    return SUCCESS;
}


StatusType EmployeeManager::GetNumEmployeesMatching(int CompanyID, int MinEmployeeID, int MaxEmployeeID, int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees)
{
    intObject cmpID(CompanyID);
    if(CompanyID > 0)
    {
        if(companies->find(cmpID) == nullptr)
            return FAILURE;
        *TotalNumOfEmployees = *NumOfEmployees = 0;
        companyData* company = *(companies->find(cmpID)->data);
        if(company->employees_ids->getSize() == 0)
            return FAILURE;
        oneSidedInOrderSearch(company->employees_ids->getRoot(), MinEmployeeID, MaxEmployeeID, MinSalary, MinGrade, NumOfEmployees, TotalNumOfEmployees);
    }
    if(CompanyID < 0)
    {
        if(all_employees_ID->getSize() == 0)
            return FAILURE;
        *TotalNumOfEmployees = *NumOfEmployees = 0;
        oneSidedInOrderSearch(all_employees_ID->getRoot(), MinEmployeeID, MaxEmployeeID, MinSalary, MinGrade, NumOfEmployees, TotalNumOfEmployees);
    }
    return SUCCESS;
}


/***************************************************************************/
// ****** Auxiliary Functions Implementation ******//
/***************************************************************************/

static void removeEmployeeFromSoftware(EmployeeManager* DS, EmployeeData* to_delete)
{
    intObject id(to_delete->employeeID);
    EmployeeKey key(to_delete->employeeID, to_delete->salary);
    to_delete->company->employeeRemove(key);
    DS->all_employees_ID->remove(id);
    DS->all_employees_salary->remove(key);
}

static void addEmployeeToSoftware(EmployeeManager* DS, EmployeeData* to_add)
{
    intObject cmpID(to_add->companyID), empID(to_add->employeeID);
    EmployeeKey empKey(to_add->employeeID, to_add->salary);
    companyData* company = to_add->company;
    DS->all_employees_ID->insert(empID, *to_add);
    DS->all_employees_salary->insert(empKey, *to_add);
    DS->all_employees_ID->find(empID)->data->employee = DS->all_employees_ID->find(empID)->data;
    DS->all_employees_salary->find(empKey)->data->employee = DS->all_employees_salary->find(empKey)->data;

    company->employeeInsert(empKey, DS->all_employees_ID->find(empID)->data, DS->all_employees_salary->find(empKey)->data);

}

static void AcquirerFillArrays(EmployeeData** arr_id_data, EmployeeData** arr_sal_data, intObject* arr_id_key, EmployeeKey* arr_sal_key, companyData* acquirer, companyData* target, int acquirer_size, int target_size)
{
    int i=0;
    acquirer->employees_ids->TreeKey_to_array(acquirer->employees_ids->getRoot(), arr_id_key, &i);
    target->employees_ids->TreeKey_to_array(target->employees_ids->getRoot(), arr_id_key, &i);
    mergeKeys<intObject>(arr_id_key, arr_id_key+acquirer_size, 0, acquirer_size, acquirer_size+target_size); // sorted data

    i=0;
    acquirer->employees_salaries->TreeKey_to_array(acquirer->employees_salaries->getRoot(), arr_sal_key, &i);
    target->employees_salaries->TreeKey_to_array(target->employees_salaries->getRoot(), arr_sal_key, &i);
    mergeKeys<EmployeeKey>(arr_sal_key, arr_sal_key+acquirer_size, 0, acquirer_size, acquirer_size+target_size); // sorted data
    i=0;
    acquirer->employees_ids->TreeData_to_array(acquirer->employees_ids->getRoot(), arr_id_data, &i);
    target->employees_ids->TreeData_to_array(target->employees_ids->getRoot(), arr_id_data, &i);
    mergeEmployeeId(arr_id_data, arr_id_data+acquirer_size, 0, acquirer_size, acquirer_size+target_size); // sorted data

    i=0;
    acquirer->employees_salaries->TreeData_to_array(acquirer->employees_salaries->getRoot(), arr_sal_data, &i);
    target->employees_salaries->TreeData_to_array(target->employees_salaries->getRoot(), arr_sal_data, &i);
    mergeEmployeeSalary(arr_sal_data, arr_sal_data+acquirer_size, 0, acquirer_size, acquirer_size+target_size); // sorted data
}

template<class KeyArray, class DaraArray>
static void changeArraysToTrees(KeyArray* arr_key, DaraArray** arr_data, companyData* acquirer, int size, bool id_or_sal)
{
    int i=0;
    if(id_or_sal == IDS_ARRAY)
        acquirer->employees_ids->arrayToTree((intObject*)arr_key, arr_data, 0, size, &i);
    if(id_or_sal == SALARIES_ARRAY)
        acquirer->employees_salaries->arrayToTree((EmployeeKey*)arr_key, arr_data, 0, size, &i);
}

static StatusType AcquirerAlloctaeArrays(EmployeeData*** arr_id_data, EmployeeData*** arr_sal_data, intObject** arr_id_key, EmployeeKey** arr_sal_key, companyData* acquirer, companyData* target, int acquirer_size, int target_size)
{
    *arr_id_data = (EmployeeData**) malloc(sizeof(EmployeeData*)*(target_size+acquirer_size));
    if(*arr_id_data == nullptr)
    {
        return ALLOCATION_ERROR;
    }

    *arr_id_key = (intObject*) malloc(sizeof(intObject)*(target_size+acquirer_size));
    if (*arr_id_key == nullptr) {
        free(*arr_id_data);
        return ALLOCATION_ERROR;
    }

    *arr_sal_data = (EmployeeData**) malloc(sizeof(EmployeeData*)*(target_size+acquirer_size));
    if(*arr_sal_data == nullptr)
    {
        free(*arr_id_key);
        free(*arr_id_data);
        return ALLOCATION_ERROR;
    }

    *arr_sal_key = (EmployeeKey*) malloc(sizeof(EmployeeKey)*(target_size+acquirer_size));
    if (*arr_sal_key == nullptr) {
        free(*arr_sal_data);
        free(*arr_id_key);
        free(*arr_id_data);
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

static void freeAllAllocations(EmployeeData** arr_id_data, EmployeeData** arr_sal_data, intObject* arr_id_key, EmployeeKey* arr_sal_key)
{
    free(arr_id_data);
    free(arr_id_key);
    free(arr_sal_data);
    free(arr_sal_key);
}

static void updateAcquirerFields(EmployeeManager* DS, companyData* aquirer, companyData* target, double Factor, intObject targetId)
{
    aquirer->value += target->value;
    aquirer->value *= Factor;
    DS->companies->remove(targetId);
    aquirer->size = aquirer->employees_ids->getSize();
    delete target;
}


template<class Key>
static void mergeKeys(Key* Arr, Key* Arr2, int start, int mid, int end)
{
    Key temp[(end - start )];
    int i = start, j = mid, k = 0;
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

static void mergeEmployeeId(EmployeeData** Arr, EmployeeData** Arr2, int start, int mid, int end)
{
    EmployeeData* temp[(end - start )];
    int i = start, j = mid, k = 0;
    while(i < mid && j < end) {
        if(Arr[i]->employeeID < Arr[j]->employeeID)
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

static void mergeEmployeeSalary(EmployeeData** Arr, EmployeeData** Arr2, int start, int mid, int end)
{
    EmployeeData* temp[(end - start )];
    int i = start, j = mid, k = 0;
    while(i < mid && j < end) {
        if(Arr[i]->salary < Arr[j]->salary || ( Arr[i]->salary == Arr[j]->salary && Arr[i]->employeeID > Arr[j]->employeeID))
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

template<typename Key>
static void addInOrderSearch(EmployeeData** arr, Key* arr2, GNode<Key, EmployeeData*>* node, int *i, int *size)
{

    if (node == nullptr)
        return;

    if (*size>0)
        addInOrderSearch(arr, arr2, node->left_son, i, size);
    if (*size>0){
        (arr)[(*i)] = (*node->data)->employee;
        (arr2)[(*i)] = *(node->key);
        (*i)++;
        (*size)--;
    }
    if (*size>0)
        addInOrderSearch(arr, arr2, node->right_son, i, size);

}

template<class D>
static void invertedInOrderSearch(int** arr, GNode<EmployeeKey,D> *node, int *i ,int size)
{
    if(node == nullptr)
        return;

    invertedInOrderSearch(arr,node->right_son, i, size);

    if((*i) < size) {
        (*arr)[(*i)] = node->key->employeeID;
        (*i)++;
    }

    invertedInOrderSearch(arr, node->left_son, i, size);
}

static void GetIdInOrderSearch(int** arr, GNode<intObject, companyData*>* node, int *i, int *size)
{
    if(node == nullptr)
        return;

    if(*size>0)
        GetIdInOrderSearch(arr,node->left_son, i, size);
    if(*size>0){
        (*arr)[(*i)] = (*node->data)->employees_salaries->getMax()->key->employeeID;
        (*i)++;
        (*size)--;
    }
    if (*size>0)
        GetIdInOrderSearch(arr, node->right_son, i, size);

}

static void oneSidedInOrderSearch(GNode<intObject, EmployeeData*>* root, int MinEmployeeID, int MaxEmployeeID, int minSalary, int minGrade, int* NumOfEmployees, int* TotalNumOfEmployees)
{
    if(root == nullptr)
        return;

    if( ((*root->data)->employeeID >= MinEmployeeID) && ((*root->data)->employeeID <= MaxEmployeeID) )
    {
        *TotalNumOfEmployees += 1;
        *NumOfEmployees += ((*root->data)->salary)>=minSalary && ((*root->data)->grade>=minGrade);
    }
    
    if((*root->data)->employeeID >= MinEmployeeID)
    {
        oneSidedInOrderSearch(root->left_son, MinEmployeeID, MaxEmployeeID, minSalary, minGrade, NumOfEmployees, TotalNumOfEmployees);
    }

    if((*root->data)->employeeID <= MaxEmployeeID)
    {
        oneSidedInOrderSearch(root->right_son, MinEmployeeID, MaxEmployeeID, minSalary, minGrade, NumOfEmployees, TotalNumOfEmployees);
    }
}


static void oneSidedInOrderSearch(GNode<intObject, EmployeeData>* root, int MinEmployeeID, int MaxEmployeeID, int minSalary, int minGrade, int* NumOfEmployees, int* TotalNumOfEmployees)
{
    if(root == nullptr)
        return;

    if((root->data)->employeeID >= MinEmployeeID)
    {
        oneSidedInOrderSearch(root->left_son, MinEmployeeID, MaxEmployeeID, minSalary, minGrade, NumOfEmployees, TotalNumOfEmployees);
    }

    if( (root->data->employeeID >= MinEmployeeID) && (root->data->employeeID <= MaxEmployeeID) )
    {
        *TotalNumOfEmployees += 1;
        *NumOfEmployees += (root->data->salary)>=minSalary && (root->data->grade>=minGrade);
    }

    if((root->data)->employeeID <= MaxEmployeeID)
    {
        oneSidedInOrderSearch(root->right_son, MinEmployeeID, MaxEmployeeID, minSalary, minGrade, NumOfEmployees, TotalNumOfEmployees);
    }
}
