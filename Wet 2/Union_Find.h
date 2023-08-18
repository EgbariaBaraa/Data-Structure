//
//  Union_Find.h
//  wet #2
//
//  Created by Baraa Egbaria on 03/06/2022.
//

#ifndef Union_Find_h
#define Union_Find_h

#include "CompanyData.h"

class DataCard;

class Company{
public:
    int companyID;
    companyData* data;
    Company* father;
    DataCard* card;
    double rank;

    Company(int companyID)
    {
        companyID = companyID;
        data = new companyData(companyID);
        father = nullptr;
        card = nullptr;
        rank = 0;
    }
    ~Company() {
        delete this->data;
    };
};

class DataCard
{
public:
    int groupId;
    int groupSize;
    Company* head;
    companyData* acquirer;


    DataCard();
    DataCard(int group_id, Company* head) : groupId(group_id), groupSize(1), head(head), acquirer(head->data) {};
    ~DataCard() = default;
};

class UF_groups{

public:
    int size;
    DataCard** groups_data_arr;
    Company** companies_arr;

    UF_groups(int k) : size(k+1)
    {
        groups_data_arr = new DataCard*[k + 1];
        companies_arr = new Company*[k + 1];

        groups_data_arr[0] = nullptr;
        companies_arr[0] = nullptr;
        for (int i = 1; i < k + 1; i++)
        {
            auto* company = new Company(i);
            auto* data_card = new DataCard(i, company);
            groups_data_arr[i] = data_card;
            company->card = data_card;
            companies_arr[i] = company;
        }
    }

    ~UF_groups()
    {
        for (int i = 1; i < size; i++)
        {
            delete companies_arr[i];
            delete groups_data_arr[i];
        }

        delete[] companies_arr;
        delete[] groups_data_arr;
    }

    DataCard* Find(int cmpID) // return group node
    {
        if(cmpID > size - 1)
            return nullptr;
        Company* root = companies_arr[cmpID];
        double sumR = 0, toSubtract=0 ;
        while(root->father != nullptr)
        {
            sumR += root->rank;
            root = root->father;
        }
        if (root != nullptr && companies_arr[cmpID]->father != nullptr )
        {
            Company* node = companies_arr[cmpID];
            Company* temp = companies_arr[cmpID];
            while (temp->father != root)
            {
                double old_rank = temp->rank;
                temp->rank = sumR - toSubtract;
                toSubtract += old_rank;
                temp = temp->father;
                node->father = root;
                node = temp;
            }
        }
        return root->card->head->card;
    }

    DataCard* Union(int Acquirer, int Target, double Factor)
    {
        if(Acquirer > size || Target > size)
            return nullptr;
        DataCard* AcquirerCard = Find(Acquirer);
        DataCard* TargetCard = Find(Target);
        Company* AcquirerHead = AcquirerCard->head;
        Company* TargetHead = TargetCard->head;

        if( AcquirerHead->card->groupSize < TargetHead->card->groupSize )
        {
            // update nodes ranks in the tree
            AcquirerHead->rank += Factor*(TargetCard->acquirer->value + returnSumOfValues(TargetCard->acquirer->companyID)) - TargetHead->rank;

            // merge heads of the groups ands update fields
            AcquirerHead->father = TargetHead;
            TargetCard->groupSize += AcquirerCard->groupSize;
            TargetCard->acquirer = AcquirerCard->acquirer;

            // update the card for the Target's head;
            AcquirerHead->card = TargetHead->card;
            AcquirerHead->card->acquirer = TargetCard->acquirer;

            return TargetCard;

        }
        else
        {
            // update nodes ranks in the tree
            AcquirerHead->rank += Factor*(TargetCard->acquirer->value + returnSumOfValues(TargetCard->acquirer->companyID));
            TargetHead->rank -= AcquirerHead->rank;

            // merge heads of the groups ands update fields
            TargetHead->father = AcquirerHead;
            AcquirerCard->groupSize += TargetCard->groupSize;
            TargetCard->acquirer = AcquirerCard->acquirer;

            // update the card for the Target's head;
            TargetHead->card = AcquirerHead->card;
            TargetHead->card->acquirer = AcquirerHead->card->acquirer;

            return AcquirerCard;

        }
    }

    double returnSumOfValues(int companyID)
    {
        double total_sum = 0;
        Company* node = companies_arr[companyID];
        while (node != nullptr )
        {
            total_sum += node->rank;
            node = node->father;
        }
        return total_sum;
    }
};

#endif /* Union_Find_h */