//
// Created by Shadi Diab on 30/12/2021.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include "List.h"
//#include "Player.h"


template <class T>
class HashTable{
public:

    int size;
    int used;
    list<T,int>** arr;
    explicit HashTable(int size = 10);
    ~HashTable();
    bool insert(int key,T* Data);
    bool remove(int key);
    bool Contains(int key);
    T* find(int key);
    void printTheHash();

    void mergeHash(HashTable<T> *old_hashTable);
    void copy_T(T* e, int index, list<T, int> **arr);
    int get_index(int index);
    void increase_arr();
    void decrease_arr();
    void copy_arr(int old_size, list<T,int>** new_arr);

};

template <class T>
HashTable<T>::HashTable(int size):size(size),used(0){
    arr = new list<T,int>*[size];
    for (int i = 0; i < size ; ++i) {
        arr[i] = nullptr;
    }
}

template <class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < size; ++i) {
        if(arr[i] != nullptr){
            delete arr[i];
            arr[i]= nullptr;
        }
    }
    delete[] arr;
}

template <class T>
bool HashTable<T>::insert(int key,T *Data) {
    int index = get_index(key);
    if( arr[index] != nullptr && arr[index]->find(key) != nullptr){
        return false;
    }
    if(arr[index]== nullptr){
        list<T,int>* new_list=new list<T,int>();
        arr[index]=new_list;
    }
    int* tmpkey=new int(key);
    arr[index]->addFirst(*Data,*tmpkey);
    used++;
    if(used>=size){
        increase_arr();
    }
    return true;
}

template <class T>
bool HashTable<T>::remove(int key) {
    int index=get_index(key);
    if (arr[index] == nullptr){
        return false;
    }
    if (arr[index]->find(key)== nullptr){
        return false;
    }
    auto ptr = arr[index]->find(key);
    arr[index]->remove(ptr);
    if (arr[index]->getFirst() == nullptr){
        delete arr[index];
        arr[index] = nullptr;
    }
    used--;
    if(used < (1/10)*size && size > 10){
        decrease_arr();
    }
    return true;
}

template<class T>
bool HashTable<T>::Contains(int key) {
    return find(key) != nullptr;
}

template <class T>
T* HashTable<T>::find(int key) {
    int new_index = get_index(key);
    if(arr[new_index]->find(key) == nullptr)
    {
        return nullptr;
    }
    return (arr[new_index]->findnodeBykey(key));
}

template<class T>
void HashTable<T>::mergeHash(HashTable<T> *old_hashTable)
{
    if (old_hashTable == nullptr)
    {
        return;
    }
    for (int i = 0; i < old_hashTable->size; ++i)
    {
        while (old_hashTable->arr[i] != nullptr && old_hashTable->arr[i]->head != nullptr)
        {
            this->insert( (*old_hashTable->arr[i]->head->key)
                    ,old_hashTable->arr[i]->head->info );

            old_hashTable->arr[i]->deletefirst();
            (old_hashTable->used)--;
        }
    }
}

template <class T>
int HashTable<T>::get_index(int index) {
    return index % (size);
}

template <class T>
void HashTable<T>::increase_arr() {
    int old_size=size;
    size = 10*size;
    list<T,int>** new_arr = new  list<T,int>*[size];
    for (int i = 0; i <size ; ++i) {
        new_arr [i] = nullptr;
    }
    copy_arr(old_size,new_arr);
    delete[] this->arr;
    this->arr = new_arr;
}

template <typename T>
void HashTable<T>::decrease_arr() {
    int old_size = size;
    size = size/2;
    list<T,int>** new_arr = new  list<T,int>*[size];
    for (int i = 0; i < size ; ++i) {
        new_arr[i] = nullptr;
    }
    copy_arr(old_size, new_arr);
    delete[] this->arr;
    this->arr = new_arr;
}

template <typename T>
void HashTable<T>::copy_T(T *e , int index, list<T, int> **arr) {
    int new_index= get_index(index);
    if( arr[new_index] == nullptr){
        list<T,int>* new_list = new list<T,int>();
        arr[new_index] = new_list;
    }
    int* temp_index = new int(index);
    arr[new_index]->addFirst(*e,*(temp_index));
}

template <typename T>
void HashTable<T>::copy_arr(int old_size, list<T,int>** new_arr) {
    for (int i = 0; i < old_size ; ++i) {
        if(arr[i] == nullptr){
            new_arr[i] = nullptr;
            continue;
        }
        ListNode<T,int>* ptr=arr[i]->getFirst();
        while ( ptr != nullptr){
            int key = *(ptr->key);
            T* e = arr[i]->pop(ptr);
            //arr[i]->remove(ptr);
            copy_T(e, key, new_arr);
            delete e;
            ptr=arr[i]->getFirst();
        }
        delete arr[i];
    }
}

template<class T>
void HashTable<T>::printTheHash() {
    for (int i = 0; i < this->size; ++i)
    {
        if(this->arr[i] != nullptr)
        {
            ListNode<T,int>* tmp = this->arr[i]->getFirst();
            while (tmp != nullptr)
            {
                std::cout << "the key is : " << (*tmp->key) << std::endl;
                tmp = tmp->next;
            }
        }

    }

}


#endif //WET2_HASHTABLE_H
