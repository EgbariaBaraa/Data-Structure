//
// Created by Shadi Diab on 27/11/2021.
//

#ifndef WET1_AVL_TREE_H
#define WET1_AVL_TREE_H

#include <stdlib.h>
#include "iostream"
#include "exceptions.h"

using namespace std;


template <class Key , class Data>
class GNode
{
private:
public:

    int height;
    Key *key;
    Data *data;
    GNode *left_son;
    GNode *right_son;
    GNode *father;


    GNode(): height(0) ,key(nullptr), data(nullptr), left_son(nullptr), right_son(nullptr), father(nullptr)
    {}

    GNode( Key &new_key, Data &new_data, GNode *father = nullptr)
            : height(0), left_son(nullptr), right_son(nullptr), father(father)
    {

        this->key = new Key(new_key);

        this->data = new Data(new_data);

    }

    ~GNode()
    {
        left_son = nullptr;
        right_son = nullptr;
        father = nullptr;
    }

    bool isLeaf()
    {
        if(right_son == nullptr && left_son == nullptr)
        {
            return true;
        }
        return false;
    }

    void updateHeight()
    {
        int right_height = (right_son == nullptr ? -1 : this->right_son->height);
        int left_height = (left_son == nullptr ? -1 : left_son->height);

        this->height = (right_height > left_height ? right_height : left_height) + 1;
    }

    int getHeight(GNode *node)
    {
        if(node == nullptr)
            return -1;

        node->updateHeight();
        return node->height;
    }

    int balanceFactor()
    {
        return ((getHeight(left_son)) - (getHeight(right_son)));
    }

    void swap(GNode* to_swap)
    {

        auto tmp= to_swap->key;
        auto tmpData= to_swap->data;

        (to_swap->data) = (this->data);
        (to_swap->key) = (this->key);


        (this->data) = (tmpData);
        (this->key) = (tmp);
    }

};


template<class Key , class Data>
class AVL_Tree
{
    typedef GNode<Key,Data> Node;
private:
    int size;
    Node *root;
    Node *max;


public:

    AVL_Tree()
    {
        size = 0;
        root = nullptr;
        max= nullptr;
    }

    class Delete
    {
    public:
        void operator()( Node *node)
        {
            delete node->key;
            delete node->data;
            delete node;
        }
    };

    ~AVL_Tree()
    {
        treeClear();
    }
    Node* getRoot();
    Node* getMax();
    int getSize();
    void insert(Key &new_key , Data &new_data);
    void remove(Key &key);
    Node* find(Key &key);
    void rotateRightRight(Node *node);
    void rotateLeftLeft(Node *node);
    void rotateRightLeft(Node *node);
    void rotateLeftRight(Node *node);
    void fixTheTree(Node *node);
    void treeClear();
    void printTree();

    template <class Func>
    void inOrderSearch( Node *node, Func function);
    template <class Func>
    void preOrderSearch( Node *node, Func func);
    template <class Func>
    void postOrderSearch( Node *node, Func function);

    void TreeKey_to_array(GNode<Key,Data>* root, Key arr_key[], int* cnt);
    void TreeData_to_array(GNode<Key,Data>* root, Data arr_Data[], int* cnt);
    GNode<Key,Data>* arrayToTree(Key arr_key[],Data arr_data[], int start, int end,int* cnt);


    class Print
    {
    public:
        void operator()( Node *node)
        {
            std::cout << "the key is"<< (*node->key)<<std::endl;
        }
    };

    class Update
    {
    public:
        void operator()( Node *node)
        {
            node->updateHeight();
        }

    };



};


template<class Key,class Data>
void AVL_Tree<Key,Data>::TreeKey_to_array(GNode<Key,Data> *node,Key arr_key[],int *cnt)
{
    if(node == nullptr)
        return;

    TreeKey_to_array(node->left_son,arr_key,cnt);
    arr_key[(*cnt)] = (*node->key);
    (*cnt)++;
    TreeKey_to_array(node->right_son,arr_key,cnt);
}

template<class Key,class Data>
void AVL_Tree<Key,Data>::TreeData_to_array(GNode<Key,Data>* node,Data arr_Data[],int* cnt)
{
    if(node == nullptr)
        return;

    TreeData_to_array(node->left_son,arr_Data,cnt);
    arr_Data[(*cnt)] = (*node->data);
    (*cnt)++;
    TreeData_to_array(node->right_son,arr_Data,cnt);
}

template<class Key,class Data>
GNode<Key,Data>* AVL_Tree<Key,Data>::arrayToTree(Key arr_key[],Data arr_data[], int start, int end,int* cnt)
{
    /* Base Case */
    if (start > end) {
        return nullptr;
    }

    int mid = (start + end) / 2;
    auto newNode = new GNode<Key,Data>(arr_key[mid],arr_data[mid]);

    if (*cnt == 0)
    {
        this->size = end + 1;
        this->root = newNode;
    }
    if (*cnt == end)
    {
        this->max = newNode;
    }

    (*cnt) = (*cnt) + 1;

    newNode->left_son = arrayToTree(arr_key,arr_data, start, mid-1,cnt);
    if (newNode->left_son)
        newNode->left_son->father = newNode ;
    newNode->right_son = arrayToTree(arr_key,arr_data, mid+1, end,cnt);
    if (newNode->right_son)
        newNode->right_son->father = newNode ;

    return newNode;
}


template<class Key,class Data>
void AVL_Tree<Key,Data>::treeClear()
{
    if (this->root != nullptr)
    {
        remove(*this->root->key);
        treeClear();
    }
    max = nullptr;

}

template<class Key,class Data>
void AVL_Tree<Key,Data>::printTree()
{
    preOrderSearch(this->root , Print() );
}

template<class Key,class Data>
GNode<Key,Data> *AVL_Tree<Key,Data>::getRoot()
{
    return this->root;
}

template<class Key,class Data>
GNode<Key,Data> *AVL_Tree<Key,Data>::getMax()
{
    return this->max;
}

template<class Key,class Data>
int AVL_Tree<Key,Data>::getSize()
{
    return this->size;
}

template<class Key,class Data>
GNode<Key,Data> *AVL_Tree<Key,Data>::find(Key &key)
{

    Node *itr = this->root;
    while (itr != nullptr  )
    {
        if ((*itr->key) != key)
        {
            if (*(itr->key) > key)
                itr = itr->left_son;
            else
                itr = itr->right_son;
        } else
        {
            return itr;
        }

    }
    if(itr == nullptr)
    {
        return nullptr;
    }
    return itr;
}

template<class Key,class Data>
template<class Func>
void AVL_Tree<Key,Data>::inOrderSearch( Node *node, Func function)
{
    if (node == nullptr)
        return;

    inOrderSearch(node->left_son,function);
    function(node);
    inOrderSearch(node->right_son,function);
}

template<class Key,class Data>
template<class Func>
void AVL_Tree<Key,Data>::postOrderSearch( Node *node, Func function)
{
    if (node == nullptr)
        return;
    postOrderSearch(node->left_son,function);
    postOrderSearch(node->right_son,function);
    function(node);
}

template<class Key,class Data>
template<class Func>
void AVL_Tree<Key,Data>::preOrderSearch( Node *node, Func func)
{
    if (node == nullptr)
        return;
    func(node);
    preOrderSearch(node->left_son,func);
    preOrderSearch(node->right_son,func);
}

template<class Key,class Data>
void AVL_Tree<Key,Data>::insert( Key &new_key, Data &new_data)
{
    Node *itr = this->root;
    Node *father = nullptr;

    while (itr != nullptr)
    {
        if(new_key == (*itr->key))
        {
            throw AlreadyExist(); //return exception that there is already a node with this key;
        }
        father = itr;

        if(new_key > *(itr->key))
        {
            itr = itr->right_son;
        }
        else
        {
            itr = itr->left_son;
        }
    }

    Node *new_node = new GNode<Key,Data>(new_key, new_data, father);

    if (father == nullptr)
    {
        this->root = new_node;
    } else if(new_key > *(father->key))
    {
        father->right_son = new_node;
    }
    else
    {
        father->left_son = new_node;
    }

    if (new_node->left_son)
    {
        std::cout << new_node->left_son->key << std::endl;
    }

    fixTheTree(new_node); //we start fixing the Tree from the temp father by updating the height and calculating the balance factor and rotate right and left
    if (max == nullptr )
    {
        max =new_node;
    }
    else if(new_key > (*max->key))
    {
        max = new_node;
    }
    size = size + 1;


}

template<class Key,class Data>
void AVL_Tree<Key,Data>::remove(Key &key)
{
    Node *to_delete = find(key);
    if( to_delete == nullptr)
    {
        throw DoNotExist(); // return exception that no node with this key
    }

    Node *temp_father = to_delete->father;
    //if to delete have two sons ...
    if(to_delete->right_son  && to_delete->left_son)
    {
        Node *itr=to_delete->right_son;
        while(itr->left_son != nullptr)
        {
            itr=itr->left_son;
        }

        to_delete->swap(itr);
        to_delete=itr;

    }
    //deleting the line between to delete and his son after swapping the data ...
    if (to_delete->right_son)
    {
        to_delete->swap(to_delete->right_son);
        to_delete = to_delete->right_son;
        to_delete->father->right_son = nullptr;

    }
    else if (to_delete->left_son)
    {
        to_delete->swap(to_delete->left_son);
        to_delete = to_delete->left_son;
        to_delete->father->left_son = nullptr;

    }

    temp_father = to_delete->father;
    to_delete->father= nullptr;

    //if to delete is a leaf Or we swapped to delete with a leaf So we delete the line between them ...
    if(temp_father)
    {
        if (temp_father->right_son && temp_father->right_son->key == to_delete->key)
        {
            temp_father->right_son = nullptr;
        }

        if (temp_father->left_son && *(temp_father->left_son->key) == *(to_delete->key))
        {
            temp_father->left_son = nullptr;
        }
    }
        //to delete is root ...
    else
    {
        root = nullptr;
        max = nullptr;
    }

    if (temp_father && (*to_delete->key) == (*max->key))
    {
        max = temp_father;
    }

    delete to_delete->key;
    delete to_delete->data;
    delete to_delete;
    size = size - 1;


    fixTheTree(temp_father);

    return;

}

template<class Key , class Data>
void AVL_Tree<Key,Data>::rotateLeftLeft(Node *node)
{
    Node *temp_root = node->left_son;

    node->left_son = temp_root->right_son;
    if(node->left_son)
    {
        node->left_son->father = node;
    }
    // set the original node as left son of the temp/new root
    temp_root->father = node->father;
    node->father = temp_root;
    temp_root->right_son = node;

    // set the original root father as the new root father

    if(temp_root->father)
    {
        if(temp_root->father->right_son == node){
            temp_root->father->right_son = temp_root;
        } else
        {
            temp_root->father->left_son = temp_root;
        }
    } else
    {
        this->root = temp_root;
    }
    node->updateHeight();
    temp_root->updateHeight();

    if(temp_root->father)
    {
        temp_root->updateHeight();
    }

}

template<class Key , class Data>
void AVL_Tree<Key,Data>::rotateRightRight(Node *node)
{
    Node *temp_root = node->right_son;

    // set original node/root sons... (set its right son)
    node->right_son = temp_root->left_son;
    if(node->right_son)
    {
        node->right_son->father = node;
    }

    // set the original node as left son of the temp/new root
    temp_root->father = node->father;
    node->father = temp_root;
    temp_root->left_son = node;

    // set the original root father as the new root father

    if(temp_root->father)
    {
        if(temp_root->father->left_son == node){
            temp_root->father->left_son = temp_root;
        } else
        {
            temp_root->father->right_son = temp_root;
        }
    } else
    {
        this->root = temp_root;
    }

    node->updateHeight();
    temp_root->updateHeight();

    if(temp_root->father)
    {
        temp_root->updateHeight();
    }
    //std::cout<< "  start rr "<<std::endl;
}

template<class Key , class Data>
void AVL_Tree<Key,Data>::rotateRightLeft(Node *node)
{
    rotateLeftLeft(node->right_son);
    return rotateRightRight(node);
}

template<class Key , class Data>
void AVL_Tree<Key,Data>::rotateLeftRight(Node *node)
{
    rotateRightRight(node->left_son);
    return rotateLeftLeft(node);
}

template<class Key , class Data>
void AVL_Tree<Key, Data>::fixTheTree(Node *node)
{
    postOrderSearch(this->root , Update());
    Node* itr= node;
    while (itr != nullptr)
    {
        if (itr->balanceFactor() > 1)
        {
            if (itr->left_son->balanceFactor() >= 0)
            {
                rotateLeftLeft(itr); //LL
            }
            else
            {
                rotateLeftRight(itr);//LR
            }
        }
        else if (itr->balanceFactor() < -1)
        {
            if (itr->right_son->balanceFactor() == 1)
            {
                rotateRightLeft(itr); //RL
            } else
            {
                rotateRightRight(itr); //RR
            }
        }
        itr->updateHeight();
        itr = itr->father;
    }
}


#endif //WET1_AVL_TREE_H
