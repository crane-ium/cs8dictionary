#ifndef CHAINHASH_H
#define CHAINHASH_H
/* ChainHash:
 * -Stores data in a hashtable where the table holds linked lists of records
 * -Going to remake this from the ground up just so I have a chance
 *  to remake it neater than the previous versions
 * */

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "hashlist.h"
#include "../prime_generator/prime_generator.h"

using namespace std;

template<class T> //T: type of data to store
class ChainHash{
public:
    //CTOR
    ChainHash(size_t l=811); //default prime set to 811
    //Big3
    ~ChainHash();
    ChainHash(const ChainHash<T>& copy);
    ChainHash<T>& operator =(const ChainHash<T>& copy);
    //MOD MEMBERS
    size_t insert(const size_t& k, const T &d, const string& s="");
    size_t insert(const string& s, const T& d); //insert string key
    size_t remove(const size_t& k); //remove size_t key
    size_t remove(const string& s);
    bool find(const size_t& k, T& d) const;
    bool find(const string& s, T& d) const;
    bool exists(const size_t& k) const;
    bool exists(const string& s) const;
    //Return a reference to a data so it can be added or altered!
    T& operator [](const size_t& k);
    T& operator [](const string& s);
    size_t length() const;
    //FRIENDS
    template<class U>
    friend ostream& operator <<(ostream& outs, ChainHash<U>& hash);
protected:
    size_t _prime, _size;
    HashList<T>* _hlist;
    size_t hash_f(const size_t& k) const;
    size_t dhash_f(const size_t& k) const;
    size_t string_hash(const string& s) const;
};

template<class T>
ChainHash<T>::ChainHash(size_t l)
        : _prime(l),_size(0),_hlist(new HashList<T>[l]){
}

template<class T>
ChainHash<T>::~ChainHash(){
    delete[] _hlist;
}

template<class T>
ChainHash<T>::ChainHash(const ChainHash<T>& copy){
    _prime = copy._prime;
    _size = copy._size;
    _hlist = new HashList<T>[_prime];
    for(size_t i = 0; i < _prime; i++){
        _hlist[i] = copy._hlist[i];
    }
}
template<class T>
ChainHash<T>& ChainHash<T>::operator =(const ChainHash<T>& copy){
    if(this == &copy)
        return (*this);
    ChainHash<T> temp(copy);
    swap(_prime, temp._prime);
    swap(_size, temp._size);
    swap(_hlist, temp._hlist);
    return(*this);
}

template<class T>
size_t ChainHash<T>::insert(const size_t& k, const T& d, const string& s){
    size_t i = hash_f(k);
    _hlist[i].insert(k, d, s);
    _size++;
    return i;
}

template<class T>
size_t ChainHash<T>::hash_f(const size_t &k) const{
    return k % _prime;
}

template<class T>
size_t ChainHash<T>::dhash_f(const size_t &k) const{
    return 1 + (k % (_prime - 2));
}

template<class T>
ostream& operator <<(ostream& outs, ChainHash<T>& hash){
    for(size_t i = 0; i < hash._prime; i++){
        outs << "[" << setw(5) << setfill('0') << i << "]: "
             << hash._hlist[i] << endl;
    }
    return outs;
}

template<class T>
size_t ChainHash<T>::string_hash(const string &s) const{
    size_t num = 0;
    for(size_t i = 0; i < s.length(); i++){
        num += s[i]; //generates a number based off of ascii values
    }
    return hash_f(num);
}
template<class T>
size_t ChainHash<T>::insert(const string& s, const T& d){
    size_t num = string_hash(s);
    this->insert(num, d, s);
    return num;
}
template<class T>
size_t ChainHash<T>::remove(const size_t& k){
    size_t i = hash_f(k);
    if(_hlist[i].remove(k))
        _size--;
    return i;
}
template<class T>
size_t ChainHash<T>::remove(const string& s){
    size_t i = string_hash(s);
    if(_hlist[i].remove(s))
        _size--;
    return i;
}
template<class T>
bool ChainHash<T>::find(const size_t& k, T& d) const{
    size_t i = hash_f(k);
    HashNode<T>* node = (*_hlist[i].find(k));
    if(node != NULL)
        d = node->data;
    return (node != NULL);
}
template<class T>
bool ChainHash<T>::find(const string& s, T& d) const{
    size_t i = string_hash(s);
    HashNode<T>* node = (*_hlist[i].find(s));
    if(node != NULL)
        d = node->data;
    return (node != NULL);
}
template<class T>
bool ChainHash<T>::exists(const size_t& k) const{
    T temp;
    return find(k, temp);
}
template<class T>
bool ChainHash<T>::exists(const string& s) const{
    T temp;
    return find(s, temp);
}
template<class T>
T& ChainHash<T>::operator [](const size_t& k){
    size_t i = hash_f(k);
    HashNode<T>** node = (_hlist[i].find(k));
    if((*node) != NULL)
        return (*node)->data;
    else{
        (*node) = new HashNode<T>(k);
        _size++;
        return (*node)->data;
    }
}
template<class T>
T& ChainHash<T>::operator [](const string& s){
    size_t i = string_hash(s);
    HashNode<T>** node = (_hlist[i].find(s));
    if((*node) != NULL)
        return (*node)->data;
    else{
        (*node) = new HashNode<T>(size_t(), T(), s);
        _size++;
        return (*node)->data;
    }
}

template<class T>
size_t ChainHash<T>::length() const{
    return _size;
}

#endif // CHAINHASH_H
