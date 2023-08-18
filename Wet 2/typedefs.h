//
//  typedefs.h
//  wet #2
//
//  Created by Baraa Egbaria on 03/06/2022.
//

#ifndef typedefs_h
#define typedefs_h

#include "RankTree.h"
#include "Union_Find.h"
#include "EmployeeKey.h"
#include "EmployeeData.h"
#include "CompanyData.h"

typedef RankTree<EmployeeKey, EmployeeData> EmployeesTree;
typedef HashTable<EmployeeData> EmployeesHash;
typedef UF_groups CompaniesUF;

#endif /* typedefs_h */
