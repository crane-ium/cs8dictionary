#ifndef DICTIONARY_H
#define DICTIONARY_H
/*OpenHash:
 * Takes keys in either string or integer
 * Then stores the item
 *
 * V1 Uses an open hash table
 * V2 Uses a double hash table
 * */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "../prime_generator/prime_generator.h" //self-explanatory

using namespace std;

template<class K, class V>
struct dictnode{
    //basic node storage
    dictnode(K k, V v):key(k),data(v){}
    K key;
    V data;
    template<class U, class T>
    friend ostream& operator <<(ostream& outs, const dictnode<U,T>& dn);
};

template<class K, class V>
ostream& operator <<(ostream& outs, const dictnode<K,V>& dn){
    outs << "[" << dn.key << "]: " << dn.data;
    return outs;
}

template<class K, class V>
class OpenHash{
public:
    //CTORS
    OpenHash(size_t length=11); //set the length of the hash table
    //BIG3
    ~OpenHash();
    OpenHash(const OpenHash& copy);
    OpenHash<K,V>& operator =(const OpenHash& copy);
    //MOD MEMBER FUNCTIONS
    bool insert(const K& key, const V& val);
        //searches for key, and if found, places it in val
    bool find(const K& key, V& val) const;
    bool find(const K& key) const;
    bool remove(const K& key);
    operator size_t() const;
    size_t length() const;
    size_t collisions() const; //counts collisions that occured in insertion
    //FRIEND MEMBMER FUNCTIONSS;AFDS
    template<class U, class T>
    friend ostream& operator <<(ostream& outs, const OpenHash<U,T>& dict);
protected:
    size_t _hash_size, _indeces, _collisions; //hash_size better be prime or u have problems
        //Stores data in array of dictnode pointers so u can check for NULL
        // rather than using a 2nd array to check
    dictnode<K,V>** data;
    //PRIVATE MEMBER FUNCTIONS
    size_t hash_f(const K& key) const;
};

template<class K, class V>
OpenHash<K,V>::OpenHash(size_t length):_hash_size(length),_indeces(0),_collisions(0){
    data = new dictnode<K,V>*[_hash_size];
    for(size_t i = 0; i < _hash_size; i++)
        data[i] = NULL;
}

template<class K, class V>
OpenHash<K,V>::~OpenHash(){
    for(size_t i = 0; i < _hash_size; i++)
        if(data[i] != NULL)
            delete data[i];
}

template<class K, class V>
OpenHash<K,V>::OpenHash(const OpenHash &copy)
        : _hash_size(copy._hash_size), _indeces(copy._indeces)
        , _collisions(copy._collisions){
    this->data = new dictnode<K,V>*[_hash_size];
    for(size_t i = 0; i < copy._hash_size; i++){
        if(copy.data[i] != NULL){
            this->data[i] = new dictnode<K,V>(
                    copy.data[i]->key, copy.data[i]->data);
        }else
            this->data[i] = NULL;
    }
}

template<class K, class V>
OpenHash<K,V>& OpenHash<K,V>::operator =(const OpenHash<K,V>& copy){
    //Copy-Swap Idiom
    OpenHash<K,V> temp(copy);
    swap(this->_hash_size,temp._hash_size);
    swap(this->_indeces, temp._indeces);
    swap(this->_collisions, temp._collisions);
    swap(this->data, temp.data);
    return (*this);
}

template<class K, class V>
bool OpenHash<K, V>::insert(const K &key, const V &val){
    size_t index = hash_f(key);
    size_t current_i;
    for(size_t i = 0; i < _hash_size; i++){
        //get hash, then check all indeces until you reach the end
        // for a space to place the key and val
        current_i = (index + i) % _hash_size;
        if(data[current_i] == NULL || data[current_i]->key == key){
            dictnode<K,V>* new_dn = new dictnode<K,V>(key,val);
            if(data[current_i] == NULL)
                _indeces++;\
            else
                delete data[current_i];
            data[current_i] = new_dn;
            return true;
        }
        _collisions++;
    }
    //Reaches end means no space for new data
    return false;
}

template<class K, class V>
bool OpenHash<K,V>::find(const K &key, V &val) const{
    size_t hash_i = hash_f(key);
    size_t current_i;
    for(size_t i = 0; i < _hash_size; i++){
        current_i = (hash_i + i) % _hash_size;
        if(data[current_i] != NULL && data[current_i]->key == key){
            val = data[current_i]->data;
            return true;
        }
    }
    return false;
}

template<class K, class V>
bool OpenHash<K,V>::find(const K& key) const{
    V temp;
    return find(key,temp);
}
template<class K, class V>
size_t OpenHash<K,V>::hash_f(const K& key) const{
    size_t index = (size_t)key % _hash_size;
    return index;
}

template<class K, class V>
ostream& operator <<(ostream& outs, const OpenHash<K,V>& dict){
    for(size_t i = 0; i < dict._hash_size; i++){
        if(dict.data[i] != NULL || dict._hash_size < 20){
            outs << "[" << setw(4) << setfill('0') << i <<setfill(' ')
                 << "]  ";
            if(dict.data[i] != NULL){
                outs << (*dict.data[i]);
                if(dict.hash_f(dict.data[i]->key) != i)
                    outs << " *";
            }
            outs << endl;
        }
    }
    return outs;
}

template<class K, class V>
OpenHash<K,V>::operator size_t() const{
    return _indeces;
}

template<class K, class V>
bool OpenHash<K,V>::remove(const K& key){
    size_t hash_i = hash_f(key);
    size_t current_i;
    for(size_t i = 0; i < _hash_size; i++){
        current_i = (hash_i + i) % _hash_size;
        if(data[current_i] != NULL && data[current_i]->key == key){
            data[current_i] = NULL;
            _indeces--;
            return true;
        }
    }
    return false; //default, didn't remove anything
}

template<class K, class V>
size_t OpenHash<K,V>::collisions() const{
    return _collisions;
}
template<class K, class V>
size_t OpenHash<K,V>::length() const{
    return _indeces;
}

#endif // DICTIONARY_H
