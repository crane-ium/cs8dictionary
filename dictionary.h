#ifndef DICTIONARY_H
#define DICTIONARY_H
/*Dictionary:
 * Takes keys in either string or integer
 * Then stores the item
 *
 * V1 Uses an open hash table
 * */

#include <iostream>
#include <iomanip>

using namespace std;

template<class K, class V>
struct dictnode{
    //basic node storage
    dictnode(K k, V v):key(k),data(v){}
    K key;
    V data;
};

template<class K, class V>
class Dictionary{
public:
    //CTORS
    Dictionary(size_t length=11);
    //BIG3
    ~Dictionary();
    Dictionary(const Dictionary& copy);
    Dictionary<K,V>& operator =(const Dictionary& copy);
    //MOD MEMBER FUNCTIONS
    void insert(const K& key, const V& val);
    //FRIEND MEMBMER FUNCTIONSS;AFDS
    template<class U, class T>
    friend ostream& operator <<(ostream& outs, const Dictionary<U,T>& dict);
private:
    size_t _hash_size, _indeces;
    dictnode<K,V>** data;
    //PRIVATE MEMBER FUNCTIONS
    size_t hash_f(const K& key);
};

template<class K, class V>
Dictionary<K,V>::Dictionary(size_t length):_hash_size(length),_indeces(0){
    data = new dictnode<K,V>*[_hash_size];
    for(size_t i = 0; i < _hash_size; i++)
        data[i] = NULL;
}

template<class K, class V>
Dictionary<K,V>::~Dictionary(){
    for(size_t i = 0; i < _hash_size; i++)
        if(data[i] != NULL)
            delete data[i];
}

template<class K, class V>
void Dictionary<K,V>::insert(const K &key, const V &val){
    size_t index = hash_f(key);
    size_t current_i;
    for(size_t i = 0; i < _hash_size; i++){
        //get hash, then check all indeces until you reach the end
        // for a space to place the key and val
        current_i = (index + i) % _hash_size;
        if(data[current_i] == NULL){
            dictnode<K,V>* new_dn = new dictnode<K,V>(key,val);
            data[current_i] = new_dn;
            return;
        }
    }
    //Reaches end means no space for new data
    cout << "Error: No space\n";
}

template<class K, class V>
size_t Dictionary<K,V>::hash_f(const K& key){
    size_t index = (size_t)key % _hash_size;
    return index;
}

template<class K, class V>
ostream& operator <<(ostream& outs, const Dictionary<K,V>& dict){
    for(size_t i = 0; i < dict._hash_size; i++){
        if(dict.data[i] != NULL)
            outs << "[" << setw(4) << setfill('0') << i <<setfill(' ')
                 << "]" << setw(8) << dict.data[i]->key << ": "
                 << dict.data[i]->data << endl;
    }
    return outs;
}

#endif // DICTIONARY_H
