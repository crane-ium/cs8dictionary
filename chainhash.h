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
    //Return a reference to a data so it can be added or altered!
    T& operator [](const size_t& k);
    T& operator [](const string& s);
    //FRIENDS
    template<class U>
    friend ostream& operator <<(ostream& outs, ChainHash<U>& hash);
private:
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

//ChainHash(const ChainHash<T>& copy);
//ChainHash<T>& operator =(const ChainHash<T>& copy);

template<class T>
size_t ChainHash<T>::insert(const size_t& k, const T& d, const string& s){
    size_t i = hash_f(k);
    _hlist[i].insert(k, d, s);
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

//size_t remove(const size_t& k);
//size_t remove(const string& s);
template<class T>
T& ChainHash<T>::operator [](const size_t& k){
    size_t i = hash_f(k);
    HashNode<T>** node = (_hlist[i].find(k));
    if((*node) != NULL)
        return (*node)->data;
    else{
        (*node) = new HashNode<T>(k);
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
        return (*node)->data;
    }
}


#endif // CHAINHASH_H
