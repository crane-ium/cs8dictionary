#ifndef HASHLIST_H
#define HASHLIST_H
/* HashList:
 * -Quick little module specialized for storing chainhash data
 * */

#include <iostream>
#include <stdlib.h>

using namespace std;

template<class T>
struct HashNode{
    HashNode(const size_t& k    = 0
             , const T& d       = T()
             , const string& s  = ""
             , HashNode<T>* n=NULL)
                : next(n), key(k)
                , skey(s), data(d){}

    size_t key;
    string skey; //if given, use a string as a key instead (key still serves as reference)
    T data;
    HashNode<T>* next;
    friend ostream& operator <<(ostream& outs, HashNode<T>& node){
        outs << ((node.skey=="") ? (to_string(node.key)) : ("\""+node.skey+"\""))
             << " : " << node.data;
        return outs;
    }
};

template<class T>
class HashList{
public:
    HashList();
    //big3
    ~HashList();
    HashList(const HashList<T> &copy);
    HashList<T>& operator =(const HashList<T>& copy);
    //MOD MEMBERS
    void insert(const size_t &k, const T& d, string s="");
    bool remove(const size_t& k); //remove a node based on key
    bool remove(const string& s); //remove a node based on skey
    HashNode<T>** find(const size_t& k);
    HashNode<T>** find(const string& s);
    //friends
    template<typename U>
    friend ostream& operator <<(ostream& outs, HashList<U>& list);
private:
    HashNode<T>* _head;
};

template<class T>
HashList<T>::HashList():_head(NULL){}

template<class T>
HashList<T>::~HashList(){
    while(_head != NULL){
        HashNode<T>* temp = _head;
        _head = temp->next;
        delete temp;
    }
}
template<class T>
HashList<T>::HashList(const HashList<T>& copy){
    HashNode<T>** this_w = &_head;
    for(HashNode<T>* w = copy._head; w != NULL; w = w->next){
        (*this_w) = new HashNode<T>(w->key,w->data,w->skey);
        this_w = &(*this_w)->next;
    }
}
template<class T>
HashList<T>& HashList<T>::operator =(const HashList<T>& copy){
    if(this == &copy)
        return (*this);
    HashList<T> temp(copy);
    swap(_head, temp._head);
    return (*this);
}
template<class T>
void HashList<T>::insert(const size_t& k, const T& d, string s){
    HashNode<T>* node;
    if(s=="")
        node = (*find(k));
    else
        node = (*find(s));

    if(node == NULL){
        HashNode<T>* new_node = new HashNode<T>(k, d, s, this->_head);
        _head = new_node;
    }else{
        node->key = k;
        node->data = d;
        node->skey = s;
    }
}

template<class T>
bool HashList<T>::remove(const size_t& k){
    HashNode<T>** walker = find(k);
    if((*walker) != NULL){
        HashNode<T>* temp = (*walker);
        (*walker) = (*walker)->next;
        delete temp;
        return true;
    }
    return false;
}
template<class T>
bool HashList<T>::remove(const string& s){
    HashNode<T>** walker = find(s);
    if((*walker) != NULL){
        HashNode<T>* temp = (*walker);
        (*walker) = (*walker)->next;
        delete temp;
        return true;
    }
    return false;
}
template<typename T>
ostream& operator <<(ostream& outs, HashList<T>& list){
    HashNode<T>* walker = list._head;
    while(walker != NULL){
        outs << "[" << (*walker) << "] -> ";
        walker = walker->next;
    }
    outs << "|||" << endl;
    return outs;
}
template<class T>
HashNode<T>** HashList<T>::find(const size_t& k){
    HashNode<T>** walker = &this->_head;
    while((*walker) != NULL){
        if((*walker)->key == k){
            return walker;
        }
        walker = &(*walker)->next;
    }
    return walker;
}
template<class T>
HashNode<T>** HashList<T>::find(const string& s){
    HashNode<T>** walker = &this->_head;
    while((*walker) != NULL){
        if((*walker)->skey == s){
            return walker;
        }
        walker = &(*walker)->next;
    }
    return walker;
}

#endif // HASHLIST_H
