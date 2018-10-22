#ifndef AVLHASH_H
#define AVLHASH_H
/* avlHash:
 * a hash map that stores data in each index in the hash array
 *  as an avl tree to allow for faster O(logn) access
 * */

#include "chainhash.h"
#include "../bst/bst.h"
#include "record.h"

using namespace std;

template<class T>
class avlHash: private ChainHash<T>{
public:
    avlHash(const size_t& p=17);
    //big3
    ~avlHash();
    avlHash(const avlHash<T>& copy);
    avlHash<T>& operator =(const avlHash<T>& copy);
    //MOD MEMBERS
    // insert/remove return the index they were inserted at
    size_t insert(const size_t& k, const T &d, const string& s="");
    size_t insert(const string& s, const T& d); //insert string key
    bool remove(const size_t& k); //remove size_t key
    bool remove(const string& s);
    //Return a reference to a data so it can be added or altered!
    T& operator [](const size_t& k);
    T& operator [](const string& s);
    //CONST MEMBERS
    bool find(const size_t& k, T& d) const;
    bool find(const string& s, T& d) const;
    bool exists(const size_t& k) const;
    bool exists(const string& s) const;
    size_t length() const;
    //FRIENDS
    template<class U>
    friend ostream& operator <<(ostream& outs, avlHash<U>& hash);
private:
    avl<record<T> >* _avl;
};

template<class T>
avlHash<T>::avlHash(const size_t& p):ChainHash<T>(p)
                , _avl(new avl<record<T> >[p]){

}
template<class T>
avlHash<T>::~avlHash(){
    delete[] _avl;
}
template<class T>
avlHash<T>::avlHash(const avlHash<T>& copy)
        : ChainHash<T>(copy){
    this->_prime = copy._prime;
    _avl = new avl<record<string> >[this->_prime];
    for(size_t i=0; i < this->_prime; i++)
        this->_avl[i] = copy._avl[i];
}
template<class T>
avlHash<T>& avlHash<T>::operator =(const avlHash<T>& copy){
    if(this == &copy)
        return (*this);
    ChainHash<T>::operator=(copy);
    avlHash<T> temp(copy);
    swap(this->_avl, temp._avl);
    return (*this);
}
template<class T>
size_t avlHash<T>::insert(const size_t& k, const T &d, const string& s){
    if(s!="")
        return insert(s,d);
    record<T>* temp = new record<T>(k, d, s);
    size_t i = this->hash_f(k);
    _avl[i].insert((*temp));
    this->_size++;
    return i;
}
template<class T>
size_t avlHash<T>::insert(const string& s, const T& d){
    size_t si = this->string_hash(s);
    record<T>* temp = new record<T>(si, d, s);
    size_t i = this->hash_f(si);
    _avl[i].insert((*temp));
    this->_size++;
    return i;
}
template<class T>
bool avlHash<T>::remove(const size_t& k){
    size_t i = this->hash_f(k);
    record<T>* temp = new record<T>(k);
    if(_avl[i].erase((*temp))){
        this->_size--;
        delete temp;
        return true;
    }
    delete temp;
    return false;
}
template<class T>
bool avlHash<T>::remove(const string& s){
    size_t si = this->string_hash(s);
    size_t i = this->hash_f(si);
    record<T>* temp = new record<T>(si,T(),s);
    if(_avl[i].erase((*temp))){
        this->_size--;
        delete temp;
        return true;
    }
    delete temp;
    return false;
}
template<class T>
T& avlHash<T>::operator [](const size_t& k){
    size_t i = this->hash_f(k);
    record<T>* temp = new record<T>(k);
    tree_node<record<T> >* rec;
    if(_avl[i].search((*temp), rec)){
        return rec->_item.data;
    }
    insert(k,T());
    _avl[i].search((*temp), rec);
    return rec->_item.data;
}
template<class T>
T& avlHash<T>::operator [](const string& s){
    size_t si = this->string_hash(s);
    size_t i = this->hash_f(si);
    record<T>* temp = new record<T>(si,T(),s);
    tree_node<record<T> >* rec;
    if(_avl[i].search((*temp), rec)){
        return rec->_item.data;
    }
    insert(s,T());
    _avl[i].search((*temp), rec);
    return rec->_item.data;
}
template<class T>
bool avlHash<T>::find(const size_t& k, T& d) const{
    size_t i = this->hash_f(k);
    record<T> rec(k);
    tree_node<record<T> >* temp;
    if(_avl[i].search(rec, temp)){
        d = temp->_item.data;
        return true;
    }
    return false;
}
template<class T>
bool avlHash<T>::find(const string& s, T& d) const{
    size_t si = this->string_hash(s);
    size_t i = this->hash_f(si);
    record<T> rec(si,T(),s);
    tree_node<record<T> >* temp;
    if(_avl[i].search(rec, temp)){
        d = temp->_item.data;
        return true;
    }
    return false;
}
template<class T>
bool avlHash<T>::exists(const size_t& k) const{
    size_t i = this->hash_f(k);
    record<T> rec(k);
    return _avl[i].search(rec);
}
template<class T>
bool avlHash<T>::exists(const string& s) const{
    size_t si = this->string_hash(s);
    size_t i = this->hash_f(si);
    record<T> rec(si,T(),s);
    return _avl[i].search(rec);
}
template<class T>
size_t avlHash<T>::length() const{
    return this->_size;
}

template<class T>
ostream& operator <<(ostream& outs, avlHash<T>& hash){
    for(size_t i = 0; i < hash._prime; i++){
        outs << "[" << setw(5) << setfill('0') << i << "]: ";
        hash._avl[i].print_list();
        outs << endl;
    }
    return outs;
}

#endif // AVLHASH_H
