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
    DoubleHash(size_t l=811);
private:
    size_t hash_f(const K &key) const;
    size_t dhash_f(const K& key) const;
};

template<class K, class V>
DoubleHash<K,V>::DoubleHash(size_t l):Dictionary<K,V>(l){

}

template<class K, class V>
size_t DoubleHash<K,V>::hash_f(const K &key) const{

}

template<class K, class V>
size_t DoubleHash<K,V>::dhash_f(const K &key) const{

}

#endif // DOUBLEHASH_H
