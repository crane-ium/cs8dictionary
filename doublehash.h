#ifndef DOUBLEHASH_H
#define DOUBLEHASH_H
/* DoubleHash:
 * -Instead of linear probing, uses a double hash
 * --This should speed up hash and access times
 * */

#include <exception>
#include "openhash.h"

template<class K, class V>
class DoubleHash: public OpenHash<K,V>{
public:
    DoubleHash(size_t l=811, size_t m=809);
    //Big3
//    ~DoubleHash(); //Just uses base destructor
    DoubleHash(const DoubleHash<K,V>& copy);
    DoubleHash<K,V>& operator =(const DoubleHash<K,V>& copy);
    //MOD MEMBER FUNCTOINS
    size_t insert(const K &key, const V& val);
    size_t find(const K& key, V &val) const;
    size_t find(const K& key) const;
    V& operator [](const K& key);
    struct DataException: public exception{
        const char* what() const throw(){return "No Vacancy Hash Map";}
    };
    struct KeyException: public exception{
        const char* what() const throw(){return "Undefined Key";}
    };
protected:
//    size_t hash_f(const K &key) const; //Hash based off of the hash_size
    size_t dhash_f(const K& key) const; //hsah for the twin relative prime
    void change_data(const size_t& i, dictnode<K,V>*& dn); //changes data at i
    size_t check_data(const K& key, V& val) const; //used to find key, returns index

};

template<class K, class V>
DoubleHash<K,V>::DoubleHash(size_t l, size_t m)
        : OpenHash<K,V>(l){
}

template<class K, class V>
DoubleHash<K,V>::DoubleHash(const DoubleHash<K, V> &copy)
        : OpenHash<K,V>(copy){
}

template<class K, class V>
DoubleHash<K,V>& DoubleHash<K,V>::operator =(const DoubleHash<K, V> &copy){
    OpenHash<K,V>::operator=(copy);
    return (*this);
}

template<class K, class V>
size_t DoubleHash<K,V>::dhash_f(const K& key) const{
    return 1 + ((size_t)key % (this->_hash_size - 2));
}

template<class K, class V>
size_t DoubleHash<K,V>::insert(const K &key, const V &val){
    size_t current_i = 0;
    for(size_t i = 0; i < this->_hash_size; i++){
        current_i = (this->hash_f(key) + i*dhash_f(key)) % this->_hash_size;
        if(this->data[current_i] == NULL || this->data[current_i]->key == key){
            dictnode<K,V>* new_node = new dictnode<K,V>(key, val);
            change_data(current_i, new_node);
            return current_i;
        }
    }
    //end means there was no space left
    throw DataException();
}

template<class K, class V>
size_t DoubleHash<K,V>::find(const K& key, V& val) const{
    return check_data(key,val);
}

template<class K, class V>
size_t DoubleHash<K,V>::find(const K& key) const{
    V temp;
    return check_data(key,temp);
}

template<class K, class V>
void DoubleHash<K,V>::change_data(const size_t& i, dictnode<K,V>*& dn){
    //This is important so it makes the hash function more flexible later
    // if we swap this to accept a chain hash
    delete this->data[i];
    this->data[i] = dn;
    this->_indeces++;
}

template<class K, class V>
size_t DoubleHash<K,V>::check_data(const K& key, V& val) const{
    //changes based on using an open vs chained hash
    //Since this is open hash, we will check every index until the
    // array is exhausted
    size_t current_i = 0;
    for(size_t i = 0; i < this->_hash_size; i++){
        current_i = (this->hash_f(key) + i*dhash_f(key)) % this->_hash_size;
//        current_i = (this->hash_f(key) + i*_hash_d) % this->_hash_size;
        if(this->data[current_i] != NULL && this->data[current_i]->key == key){
            val = this->data[current_i]->data;
            return current_i;
        }
    }
    throw KeyException(); //throw an error for checking invalid key
}

template<class K, class V>
V& DoubleHash<K,V>::operator [](const K& key){
    try{
        return this->data[find(key)]->data; //try find a key to access data
    }catch(KeyException& e){
        //KeyException means there was no corresponding key, therefore
        // create a new index!
        size_t i;
        i = this->insert(key, V());
        return this->data[i]->data;
    }
}
#endif // DOUBLEHASH_H
