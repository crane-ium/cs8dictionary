#ifndef DOUBLEHASH_H
#define DOUBLEHASH_H
/* DoubleHash:
 * -Instead of linear probing, uses a double hash
 * --This should speed up hash and access times
 * */

#include "dictionary.h"

template<class K, class V>
class DoubleHash: public Dictionary<K,V>{
public:
    DoubleHash(size_t l=811, size_t m=809);
    //Big3
//    ~DoubleHash(); //Just uses base destructor
    DoubleHash(const DoubleHash<K,V>& copy);
    DoubleHash<K,V>& operator =(const DoubleHash<K,V>& copy);
    //MOD MEMBER FUNCTOINS
    bool insert(const K &key, const V& val);
    bool find(const K& key, V &val) const;
    bool find(const K& key) const;
protected:
    size_t _hash_d;
//    size_t hash_f(const K &key) const; //Hash based off of the hash_size
    size_t dhash_f(const K& key) const; //hsah of _hash_d
    void change_data(const size_t& i, dictnode<K,V>*& dn); //changes data at i
};

template<class K, class V>
DoubleHash<K,V>::DoubleHash(size_t l, size_t m)
        : Dictionary<K,V>(l), _hash_d(m){
}

template<class K, class V>
DoubleHash<K,V>::DoubleHash(const DoubleHash<K, V> &copy)
        : Dictionary<K,V>(copy), _hash_d(copy._hash_d){
}

template<class K, class V>
DoubleHash<K,V>& DoubleHash<K,V>::operator =(const DoubleHash<K, V> &copy){
    Dictionary<K,V>::operator=(copy);
    _hash_d = copy._hash_d;
    return (*this);
}

template<class K, class V>
size_t DoubleHash<K,V>::dhash_f(const K& key) const{
    return (this->hash_f(key) + _hash_d) % this->_hash_size;
}

template<class K, class V>
bool DoubleHash<K,V>::insert(const K &key, const V &val){
    size_t current_i;
    K k_c = key; //key current
    for(size_t i = 0; i < this->_hash_size; i++){
        k_c = (K)dhash_f(k_c);
//        current_i = (this->hash_f(key) + i*_hash_d) % this->_hash_size;
        current_i = (size_t)k_c;
        if(this->data[current_i] == NULL || this->data[current_i]->key == key){
            dictnode<K,V>* new_node = new dictnode<K,V>(key, val);
            change_data(current_i, new_node);
            return true;
        }
    }
    //end means there was no space left
    return false;
}

template<class K, class V>
bool DoubleHash<K,V>::find(const K& key, V& val) const{

}

template<class K, class V>
bool DoubleHash<K,V>::find(const K& key) const{

}

template<class K, class V>
void DoubleHash<K,V>::change_data(const size_t& i, dictnode<K,V>*& dn){
    //This is important so it makes the hash function more flexible later
    // if we swap this to accept a chain hash
    delete this->data[i];
    this->data[i] = dn;
    this->_indeces++;
}
#endif // DOUBLEHASH_H
