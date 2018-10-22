#ifndef RECORD_H
#define RECORD_H

#include <cstdlib>
#include <iostream>

using namespace std;

template<class T>
struct record{
    record(const size_t& k=0, const T& d=T()
         , const string& s="");

    size_t key;
    T data;
    string skey;

    //big3
    ~record();
    record(const record<T>& copy);
    record<T>& operator =(const record<T>& copy);

    size_t string_hash() const;
    bool string_hash_ex(const string& s2) const;

    template<class U>
    friend bool operator <(const record<U>& lhs, const record<U>& rhs);
    template<class U>
    friend bool operator >(const record<U>& lhs, const record<U>& rhs);
    template<class U>
    friend bool operator ==(const record<U>& lhs, const record<U>& rhs);
    template<class U>
    friend bool operator !=(const record<U>& lhs, const record<U>& rhs);


    template<class U>
    friend ostream& operator <<(ostream& outs, const record<U>& rec);
};

template<class T>
record<T>::record(const size_t& k, const T& d
        , const string& s):key(k),data(d),skey(s){

}
template<class T>
record<T>::~record(){

}
template<class T>
record<T>::record(const record<T>& copy){
    this->key = copy.key;
    this->skey = copy.skey;
    this->data = copy.data;
}
template<class T>
record<T>& record<T>::operator =(const record<T>& copy){
    record<T> temp(copy);
    swap(this->key, temp.key);
    swap(this->skey, temp.skey);
    swap(this->data, temp.data);
    return (*this);
}

template<class T>
size_t record<T>::string_hash() const{
    size_t int_hash = 0;
    for(size_t i=0; i < skey.length(); i++){
        int_hash += (int)skey[i];
    }
    return int_hash;
}

template<class T> //compare two strings that aren't equal and make sure they have a comparison
bool record<T>::string_hash_ex(const string& s2) const{
    if(skey.length() != s2.length())
        return skey.length() < s2.length();
    else
        for(size_t i=0; i < skey.length(); i++){
            if((int)skey[i] != (int)s2[i])
                return (int)skey[i] < (int)s2[i];
        }
    return 0;
}

template<class T>
bool operator <(const record<T>& lhs, const record<T>& rhs){
    if(lhs == rhs)
        return false;
    if(lhs.key == rhs.key){ //means strings != rhs.stringkey
        if(lhs.string_hash() < rhs.string_hash())
            return true;
        else
            return lhs.string_hash_ex(rhs.skey);
    }
    return lhs.key < rhs.key;
}

template<class T>
bool operator >(const record<T>& lhs, const record<T>& rhs){
    return rhs < (lhs);
}

template<class T>
bool operator ==(const record<T> &lhs, const record<T>& rhs){
    if(lhs.key == rhs.key && lhs.skey == rhs.skey)
        return true;
    return false;
}

template<class T>
ostream& operator <<(ostream& outs, const record<T>& rec){
    if(rec.skey == "")
        outs << rec.key;
    else
        outs << "\"" << rec.skey << "\"";
    outs << " : " << rec.data;
    return outs;
}
template<class T>
bool operator !=(const record<T>& lhs, const record<T>& rhs){
    return !(lhs==rhs);
}

#endif // RECORD_H
