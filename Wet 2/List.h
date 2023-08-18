//
// Created by Shadi Diab on 30/12/2021.
//

#ifndef WET2_LIST_H
#define WET2_LIST_H

template <class Data, class Key>
class ListNode{
public:
    Data* info;
    Key* key;
    ListNode* next;
    ListNode* prev;

    ListNode(Data& data= nullptr,Key& key= nullptr):key(&key),next(nullptr),prev(nullptr)
    {
        this->info = new Data(data);
    }
};



template <class Data, class Key>
class list{

public:

    ListNode<Data,Key>* head;
    ListNode<Data,Key>* tail;
    bool deletefirst();
    bool deletelast();

    list();
    ~list();
    bool addFirst(Data& info,Key& key);
    bool remove(ListNode<Data,Key>* node);
    Data* pop(ListNode<Data, Key> *node);
    Data* findnodeBykey(Key key);
    ListNode<Data,Key>* find(Key key);
    ListNode<Data,Key>* getFirst();
};

template<class Data, class Key>
list<Data, Key>::list():head(nullptr),tail(nullptr) {}

template<class Data, class Key>
list<Data, Key>::~list() {
    ListNode<Data,Key>* iterator=head;
    while (iterator!= nullptr){
        ListNode<Data,Key>* toDelete=iterator;
        iterator=iterator->next;
        delete toDelete->key;
        delete toDelete->info;
        delete toDelete;
    }
}

template <class Data, class Key>
bool list<Data,Key>::addFirst(Data& info, Key& key)
{
    ListNode<Data,Key>* newNode = new ListNode<Data,Key>(info,key);
    if(this->head== nullptr){
        head=newNode;
        return true;
    }
    newNode->next = this->head;
    this->head->prev = newNode;
    this->head = newNode;
    return true;

}

template <class Data, class Key>
bool list<Data,Key>::remove(ListNode<Data, Key> *node) {
    if(node == head){
        return deletefirst();
    }
    if(node == tail){
        return deletelast();
    }
    node->prev->next = node->next;
    if (node->next != nullptr){
        node->next->prev = node->prev;
    }
    if(node == tail){
        tail = node->prev;
        if (tail != nullptr){
            tail->next = nullptr;
        }
    }
    delete node->key;
    delete node->info;
    delete node;
    return true;
}

template <class Data, class Key>
Data* list<Data,Key>::findnodeBykey(Key key){
    return find(key)->info;
}

template<class Data, class Key>
ListNode<Data, Key> *list<Data, Key>::find(Key key) {
    ListNode<Data,Key>* iterator = getFirst();
    while (iterator != nullptr){
        if(*(iterator->key) == key){
            return iterator;
        }
        iterator = iterator->next;
    }
    return nullptr;
}

template <class Data, class Key>
Data* list<Data,Key>::pop(ListNode<Data, Key> *node){
    Data* tmp_info = node->info;
   // (*tmp_info) = (*node->info);

    if(node == head)
    {
        if(head == tail)
        {
            tail = nullptr;
        }
        head=node->next;
        if(head != nullptr){
            head->prev = nullptr;
        }
        delete node->key;
        //delete node->info;
        delete node;
        return tmp_info;
    }
    else{
        node->prev->next=node->next;
        if (node->next!= nullptr){
            node->next->prev=node->prev;
        }
        if(node == tail){
            tail = node->prev;
            if (tail!= nullptr){
                tail->next= nullptr;
            }
        }
        delete node->key;
       // delete node->info;
        delete node;
        return tmp_info;
    }
}

template<class Data, class Key>
ListNode<Data, Key> *list<Data, Key>::getFirst(){
    if(!this){
        return nullptr;
    }
    return head;
}

template<class Data, class Key>
bool list<Data, Key>::deletefirst() {
    if(head == nullptr){
        return false;
    }
    if(head == tail){
        delete head->key;
        delete head->info;
        delete head;
        head= nullptr;
        tail= nullptr;
        return true;
    }
    ListNode<Data,Key>* newHead=head->next;
    delete head->key;
    delete head->info;
    delete head;
    head=newHead;
    return true;
}

template<class Data, class Key>
bool list<Data, Key>::deletelast() {
    if(head == nullptr){
        return false;
    }
    if(head==tail){
        delete head->key;
        delete head->info;
        delete head;
        head= nullptr;
        tail= nullptr;
        return true;
    }
    ListNode<Data,Key>* newTail=tail->prev;
    delete tail->info;
    delete tail->key;
    delete tail;
    tail=newTail;
    return true;
}

#endif //WET2_LIST_H
