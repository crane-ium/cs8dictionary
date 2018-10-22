#ifndef avl_H
#define avl_H

#include <iostream>
#include "node.h"

using namespace std;

template <class T>
class avl{
public:
    avl():_head(NULL), _size(0){}
    //BIG 3
    ~avl();
    avl(const avl& other);
    avl& operator =(const avl& rhs);
    //MOD MEMBER FUNCTIONS
    void insert(T item=T()); //insert item of type T
    bool erase(const T &item); //delete item if it exists
    void clear(); //delete the whole tree
    bool search(const T &item); //search without returning ptr
    bool search(const T& item, tree_node<T>* &ptr);
    avl<T>& operator +=(const avl<T> &rhs); //concatenate two trees
    //CONST MEMBER FUNCTIONS
    void print_inorder() const; //Prints downwards and inorder
    void print() const; //Basic sideways print of tree with labels
    void print_list() const; //print it like a list
    int get_size() const; //Would return the number of nodes in avl tree. Doesn't work atm
    //FRIEND FUNCTIONS
    template<class U>
    friend ostream& operator << (ostream& outs, const avl<U> &tree);//sideways output
    template<class U> //Puts a tree consisting from node, into an array arr
    friend void tree_to_array(U* &arr, size_t &index, const tree_node<U>* node);
private:
    tree_node<T>* _head; //Tree
    size_t _size; //not functional atm
};

//a_merge: array function that merges two sorted arrays
template<class T>
T* a_merge(T* arr1, T* arr2, const size_t s_a1, const size_t s_a2);

template<class T>
avl<T>::~avl(){
    _head->clear(_head);
}

template<class T>
avl<T>::avl(const avl& other){
    //Copy the other's tree
    if(this==&other)
        return;
    tree_copy(other._head, this->_head); //call friend function
    this->_size = other._size;
}

template <class T>
avl<T>& avl<T>::operator =(const avl& rhs){
    if(this==&rhs)
        return *this;
    tree_copy(rhs._head, this->_head);
    this->_size = rhs._size;
    return(*this);
}

template<class T>
void avl<T>::insert(T item){
    _head->tree_insert(_head, item);
}

template<class T>
void avl<T>::print_inorder() const{
    _head->tree_print_inorder(_head, 0, cout);
}

template<class T>
void avl<T>::print_list() const{
    _head->tree_print_list(_head, 0, cout);
}

template <class T>
void avl<T>::print() const{
    cout << "~~~~~~Tree in sideways view~~~~~~\n";
    _head->tree_print(_head, 0, cout, true);
    cout << "Done printing\n";
}

template<class T>
bool avl<T>::erase(const T& item){
    return _head->delete_node(_head, item);
}

template<class T>
void avl<T>::clear(){
    _head->clear(_head);
}

template<class T>
bool avl<T>::search(const T &item){
    tree_node<T>* blank;
    if(!_head) //check that there is a head
        return false;
    return _head->find_node(item,blank);
}
template<class T>
bool avl<T>::search(const T &item, tree_node<T> *&ptr){
    if(!_head)
        return false;
    return _head->find_node(item, ptr);
}

template<class T>
ostream& operator <<(ostream& outs, const avl<T> &tree){
    tree._head->tree_print(tree._head, 0, outs, true);
    return outs;
}

template<class T>
int avl<T>::get_size() const{
    return this->_size;
}

template<class T>
avl<T>& avl<T>::operator +=(const avl<T> &rhs){
    //Method: Using two sorted arrays holding the items of each tree, merge and make new tree
    size_t h1, h2;
    h1 =        ((this->_head)?(this->_head->_height):(0)) + 1;
    T* a_t1 =   new T[(int)pow(2,h1) - 1]; //total size equals potential max size
    h2 =        ((rhs._head)?(rhs._head->_height):(0)) + 1;
    T* a_t2 =   new T[(int)pow(2,h2) - 1];
    T* a_both;

    //SIZES of arrays, which is the size of each array
    size_t a_t1_s = 0;
    size_t a_t2_s = 0;

    tree_to_array(a_t1, a_t1_s, this->_head);
    tree_to_array(a_t2, a_t2_s, rhs._head);
    //a_both holds the sorted array of both trees
    a_both = a_merge(a_t1, a_t2, a_t1_s, a_t2_s);

    tree_node<T>* new_head = NULL;
    //To do a pre-order insert, to do O(h)* time, start from middle of the sorted array
    //  and work outwards  *h is height of tree
    size_t middle = (int)((a_t1_s + a_t2_s)/2); //rounds up, meaning if size is even, middle will be towards top side
    new_head->tree_insert(new_head,a_both[middle]);
    for(size_t i = 1; i <= middle; i++){
        if(middle+i < a_t1_s + a_t2_s)
            new_head->tree_insert(new_head,a_both[middle+i]);
        if(middle-i >= 0)
            new_head->tree_insert(new_head,a_both[middle-i]);
    }
    this->clear(); //mem management
    _head = new_head;
    return *this;
}

template<class T>
void tree_to_array(T* &arr, size_t &index, const tree_node<T>* node){
    if(!node)
        return;
    //inorder insertions into arr to create a sorted arr
    if(node->_left)
        tree_to_array(arr, index, node->_left);
    arr[index] = node->_item;
    index++; //e.g. if size of arr is 5, index will return to be 5
    if(node->_right)
        tree_to_array(arr, index, node->_right);
}

template<class T>
T* a_merge(T* arr1, T* arr2, const size_t s_a1, const size_t s_a2){
//    cout << __FUNCTION__ << "s_a1: "<<s_a1<<" s_a2: "<<s_a2<< endl;
    T* mrg = new T[s_a1 + s_a2 + 1]; //allocate new space for the merged array
    size_t n1 = 0, n2 = 0;
    for(size_t i = 0; i < s_a1 + s_a2; i++){
        if(n1 < s_a1 && n2 < s_a2){ //Keep checking it's within the boundaries of each array
            if(arr1[n1] < arr2[n2]){
                mrg[i] = arr1[n1];
                n1++;
            }else{
                mrg[i] = arr2[n2];
                n2++;
            }
        }else if(n2 < s_a2){
            mrg[i] = arr2[n2];
            n2++;
        }else if(n1 < s_a1){
            mrg[i] = arr1[n1];
            n1++;
        }else{
            cout <<"Reached a bad end in merge\n"<<endl;
            assert(false);
        }
    }
    return mrg;
}
#endif // avl_H
