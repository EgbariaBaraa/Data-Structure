//
//  typedefs.h
//  233218_WET#1
//
//  Created by Baraa Egbaria on 25/04/2022.
//

#ifndef typedefs_h
#define typedefs_h
#include "AVL_Tree.h"

class EmployeeData;
class EmployeeKey;
class companyData;
class intObject;

typedef AVL_Tree<EmployeeKey, EmployeeData> Salaries_Tree;
typedef AVL_Tree<intObject, EmployeeData> IDs_Tree;
typedef AVL_Tree<intObject, companyData*> Non_Empty_Companies_Tree;
typedef AVL_Tree<intObject, companyData*> Companies_Tree;

#endif /* typedefs_h */
