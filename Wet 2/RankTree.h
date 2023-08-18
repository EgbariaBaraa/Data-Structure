//
//  RankTree.h
//  wet #2
//
//  Created by Baraa Egbaria on 03/06/2022.
//

#ifndef WET2_RANKTREE_H
#define WET2_RANKTREE_H


#include <cstdlib>
#include "iostream"
#include "exceptions.h"

template <class Key , class Data>
class GNode
{
private:
public:

    int height;
    long long int sons_num;
    long long int grades_sum;
    int grade;
    Key *key;
    Data *data;
    GNode *left_son;
    GNode *right_son;
    GNode *father;

    GNode(): height(0), sons_num(1), grades_sum(0), grade(0), key(nullptr), data(nullptr), left_son(nullptr), right_son(nullptr), father(nullptr)
    {}

    GNode(Key &new_key, Data &new_data, int grade, GNode *father = nullptr)
            : height(0), sons_num(1), grades_sum(grade), grade(grade), left_son(nullptr), right_son(nullptr), father(father)
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
        int temp_grade = to_swap->grade;

        (to_swap->data) = (this->data);
        (to_swap->key) = (this->key);
        to_swap->grade = this->grade;

        (this->data) = (tmpData);
        (this->key) = (tmp);
        this->grade = temp_grade;
    }

    long long int sonsNum(GNode* node)
    {
        return node == NULL ? 0 : node->sons_num;
    }

    long long int gradesSum(GNode* node)
    {
        return node == NULL ? 0 : node->grades_sum;
    }
};


template<class Key , class Data>
class RankTree
{
    typedef GNode<Key,Data> Node;
private:
    int size;
    Node *root;
    Node *max;

public:

    RankTree()
    {
        size = 0;
        root = nullptr;
        max= nullptr;
    }

    class Delete
    {
    public:
        void operator()(Node *node)
        {
            delete node->key;
            delete node->data;
            delete node;
        }
    };

    ~RankTree()
    {
        treeClear();
    }

    // getters
    Node* getRoot();
    Node* getMax();
    Node* getMin();
    int getSize();

    // find, remove, insert;
    void insert(Key &new_key, Data &new_data, int grade);
    void remove(Key &key);
    Node* find(Key &key);

    // rank and additional data methods;
    Node* selectBySonsNum(Node *node , long long int rank);
    long long int sumGradesForRank(Node *node, long long int rank, long long int* sum);
    void updateRanksForTree(Node *node);
    long long int rank(Key &key);

    // rotations
    void rotateRightRight(Node *node);
    void rotateLeftLeft(Node *node);
    void rotateRightLeft(Node *node);
    void rotateLeftRight(Node *node);
    void fixTheTree(Node *node);

    // auxiliaries
    void treeClear();
    void printTree();

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

    // traversals
    template <class Func>
    void inOrderSearch( Node *node, Func function);
    template <class Func>
    void preOrderSearch( Node *node, Func func);
    template <class Func>
    void postOrderSearch( Node *node, Func function);

    // tree to array and array to tree;
    void TreeGrades_to_array(GNode<Key,Data>* node, int grades_arr[], long long int* cnt);
    void TreeKey_to_array(GNode<Key,Data> *node,Key arr_key[], long long int *cnt);
    void TreeData_to_array(GNode<Key,Data>* node,Data arr_Data[], long long int* cnt);
    GNode<Key,Data>* arrayToTree(Key arr_key[],Data arr_data[], int grades_arr[], long long int start, long long int end, long long int* cnt);



};

// non-member functions
template<class Key, class Data>
void getClosestKey(GNode<Key, Data> *node, GNode<Key, Data>* &result, int &key, bool bigger)
{
    if(node == nullptr)
        return;

    if(bigger)
    {
        if (*(node->key) < key || *(node->key) == key)
        {
            result = node;
            getClosestKey(node->right_son, result, key, true);
        }
        if(*(node->key) > key)
        {
            getClosestKey(node->left_son, result, key, true);
        }
    }
    else
    {
        if (*(node->key) < key)
        {
            getClosestKey(node->right_son, result, key, false);
        }
        if(*(node->key) > key || *(node->key) == key)
        {
            result = node;
            getClosestKey(node->left_son, result, key, false);
        }
    }
}


// member functions / methods :

template<class Key,class Data>
GNode<Key,Data> *RankTree<Key,Data>::getRoot()
{
    return this->root;
}

template<class Key,class Data>
GNode<Key,Data> *RankTree<Key,Data>::getMax()
{
    return this->max;
}

template<class Key,class Data>
GNode<Key,Data> *RankTree<Key,Data>::getMin()
{
    Node* itr = this->root;
    while(itr->left_son != nullptr)
        itr = itr->left_son;
    return itr;
}

template<class Key,class Data>
int RankTree<Key,Data>::getSize()
{
    return this->size;
}


template<class Key, class Data>
GNode<Key,Data> *RankTree<Key, Data>::selectBySonsNum(Node *node , long long int rank)
{
    if(node == nullptr)
        return nullptr;
    if(node->sonsNum(node->left_son) == rank - 1)
        return node;
    else if(node->sonsNum(node->left_son) > rank - 1)
        return selectBySonsNum(node->left_son, rank);
    else
        return selectBySonsNum(node->right_son , rank - (node->sonsNum(node->left_son)) - 1);
}


template<class Key, class Data>
long long int RankTree<Key, Data>::sumGradesForRank(Node *node, long long int rank, long long int* sum)
{
    if(node == nullptr)
        return 0;
    if(node->sonsNum(node->left_son) == rank - 1)
        return *sum - node->gradesSum(node->left_son);
    else if(node->sonsNum(node->left_son) > rank - 1)
        return sumGradesForRank(node->left_son, rank, sum);
    else
    {
        *sum = *sum - node->gradesSum(node->left_son) - node->grade;
        return sumGradesForRank(node->right_son , rank - (node->sonsNum(node->left_son)) - 1, sum);
    }
}

template<class Key,class Data>
void RankTree<Key,Data>::updateRanksForTree(Node *node)
{
    if (node == nullptr)
        return;
    updateRanksForTree(node->left_son);
    updateRanksForTree(node->right_son);
    node->grades_sum = node->gradesSum(node->left_son) + node->gradesSum(node->right_son) + node->grade;
    node->sons_num = node->sonsNum(node->left_son) + node->sonsNum(node->right_son) + 1;
    node->updateHeight();
}

template<class Key, class Data>
long long int RankTree<Key, Data>::rank(Key &key)
{
    if (find(key) == nullptr)
        return 0;

    long long int r = 0;
    Node* temp = this->root;

    while ((*temp->key) != key)
    {
        if ((*temp->key) < key)
        {
            r += temp->sonsNum(temp->left_son) +1;
            temp = temp->right_son;
        } else
        {
            temp = temp->left_son;
        }
    }
    return r += temp->sonsNum(temp->left_son) + 1 ;
}


template<class Key , class Data>
void RankTree<Key,Data>::rotateLeftLeft(Node *node)
{
    Node *temp_root = node->left_son;

    node->left_son = temp_root->right_son;
    if(node->left_son)
    {
        node->left_son->father = node;
    }

    node->sons_num = node->sonsNum(node->left_son) + node->sonsNum(node->right_son) + 1;
    node->grades_sum = node->gradesSum(node->left_son) + node->gradesSum(node->right_son) + node->grade;


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
    temp_root->sons_num = temp_root->sonsNum(temp_root->left_son) + temp_root->sonsNum(temp_root->right_son) + 1;
    temp_root->grades_sum = temp_root->gradesSum(temp_root->left_son) + temp_root->gradesSum(temp_root->right_son) + temp_root->grade;

}

template<class Key , class Data>
void RankTree<Key,Data>::rotateRightRight(Node *node)
{
    Node *temp_root = node->right_son;

    // set original node/root sons... (set its right son)
    node->right_son = temp_root->left_son;
    if(node->right_son)
    {
        node->right_son->father = node;
    }

    node->sons_num = node->sonsNum(node->left_son) + node->sonsNum(node->right_son) + 1;
    node->grades_sum = node->gradesSum(node->left_son) + node->gradesSum(node->right_son) + node->grade;

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
    temp_root->sons_num = temp_root->sonsNum(temp_root->left_son) + temp_root->sonsNum(temp_root->right_son) + 1;
    temp_root->grades_sum = temp_root->gradesSum(temp_root->left_son) + temp_root->gradesSum(temp_root->right_son) + temp_root->grade;
}

template<class Key , class Data>
void RankTree<Key,Data>::rotateRightLeft(Node *node)
{
    rotateLeftLeft(node->right_son);
    return rotateRightRight(node);
}

template<class Key , class Data>
void RankTree<Key,Data>::rotateLeftRight(Node *node)
{
    rotateRightRight(node->left_son);
    return rotateLeftLeft(node);
}

template<class Key , class Data>
void RankTree<Key, Data>::fixTheTree(Node *node)
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
                rotateLeftRight(itr); //LR
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


template<class Key,class Data>
void RankTree<Key,Data>::treeClear()
{
    if (this->root != nullptr)
    {
        remove(*this->root->key);
        treeClear();
    }
    max = nullptr;

}

template<class Key,class Data>
void RankTree<Key,Data>::printTree()
{
    preOrderSearch(this->root , Print());
}


template<class Key,class Data>
template<class Func>
void RankTree<Key,Data>::inOrderSearch(Node *node, Func function)
{
    if (node == nullptr)
        return;
    inOrderSearch(node->left_son,function);
    function(node);
    inOrderSearch(node->right_son,function);
}

template<class Key,class Data>
template<class Func>
void RankTree<Key,Data>::postOrderSearch( Node *node, Func function)
{
    if (node == nullptr)
        return;
    postOrderSearch(node->left_son,function);
    postOrderSearch(node->right_son,function);
    function(node);
}

template<class Key,class Data>
template<class Func>
void RankTree<Key,Data>::preOrderSearch( Node *node, Func func)
{
    if (node == nullptr)
        return;
    func(node);
    preOrderSearch(node->left_son,func);
    preOrderSearch(node->right_son,func);
}



template<class Key,class Data>
void RankTree<Key,Data>::insert(Key &new_key, Data &new_data, int grade)
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

    Node *new_node = new GNode<Key,Data>(new_key, new_data, grade, father);

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

    Node *itr2 = father;
    while (itr2 != nullptr)
    {
        itr2->sons_num = itr2->sonsNum(itr2->left_son) + itr2->sonsNum(itr2->right_son) + 1;
        itr2->grades_sum = itr2->gradesSum(itr2->left_son) + itr2->gradesSum(itr2->right_son) + itr2->grade;
        itr2 = itr2->father;
    }

    //we start fixing the Tree from the temp father by updating the height and calculating the balance factor and rotate right and left
    fixTheTree(new_node);
    if (max == nullptr || new_key > (*max->key) )
    {
        max = new_node;
    }
    size = size + 1;
}

template<class Key,class Data>
void RankTree<Key,Data>::remove( Key &key)
{
    Node *to_delete = find(key);
    if(to_delete == nullptr)
    {
        // return exception that no node with this key
        throw DoNotExist();
    }

    Node *temp_father = to_delete->father;
    // if to delete have two sons ...
    if(to_delete->right_son  && to_delete->left_son)
    {
        Node *itr = to_delete->right_son;
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

    Node *itr = temp_father;
    while (itr != nullptr)
    {
        itr->sons_num = itr->sonsNum(itr->left_son) + itr->sonsNum(itr->right_son) + 1;
        itr->grades_sum = itr->gradesSum(itr->left_son) + itr->gradesSum(itr->right_son) + itr->grade;
        itr = itr->father;
    }

    delete to_delete->key;
    delete to_delete->data;
    delete to_delete;
    size = size - 1;


    fixTheTree(temp_father);
    return;

}

template<class Key,class Data>
GNode<Key,Data> *RankTree<Key,Data>::find(Key &key)
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
    return nullptr;

}

template<class Key,class Data>
void RankTree<Key,Data>::TreeGrades_to_array(GNode<Key,Data>* node, int grades_arr[], long long int* cnt)
{
    if(node == nullptr)
        return;

    TreeGrades_to_array(node->left_son, grades_arr, cnt);
    grades_arr[(*cnt)] = (node->grade);
    (*cnt)++;
    TreeGrades_to_array(node->right_son, grades_arr, cnt);
}

template<class Key,class Data>
void RankTree<Key,Data>::TreeKey_to_array(GNode<Key,Data> *node,Key arr_key[], long long int *cnt)
{
    if(node == nullptr)
        return;

    TreeKey_to_array(node->left_son,arr_key,cnt);
    arr_key[(*cnt)] = (*node->key);
    (*cnt)++;
    TreeKey_to_array(node->right_son,arr_key,cnt);
}

template<class Key,class Data>
void RankTree<Key,Data>::TreeData_to_array(GNode<Key,Data>* node, Data arr_Data[], long long int* cnt)
{
    if(node == nullptr)
        return;

    TreeData_to_array(node->left_son,arr_Data,cnt);
    arr_Data[(*cnt)] = (*node->data);
    (*cnt)++;
    TreeData_to_array(node->right_son,arr_Data,cnt);
}

template<class Key,class Data>
GNode<Key,Data>* RankTree<Key,Data>::arrayToTree(Key arr_key[],Data arr_data[], int grades_arr[], long long int start, long long int end, long long int* cnt)
{
    /* Base Case */
    if (start > end) {
        return nullptr;
    }

    long long int mid = (start + end) / 2;
    auto newNode= new GNode<Key,Data>(arr_key[mid],arr_data[mid], grades_arr[mid]);

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

    newNode->left_son = arrayToTree(arr_key,arr_data,grades_arr, start, mid-1,cnt);
    if (newNode->left_son)
        newNode->left_son->father = newNode ;
    newNode->right_son = arrayToTree(arr_key,arr_data,grades_arr, mid+1, end,cnt);
    if (newNode->right_son)
        newNode->right_son->father = newNode ;

    return newNode;
}



#endif //WET2_RANKTREE_H