#ifndef AVLHASH_H
#define AVLHASH_H
/* avlHash:
 * a hash map that stores data in each index in the hash array
 *  as an avl tree to allow for faster O(logn) access
 * */

#include "chainhash.h"

using namespace std;

template<class T>
class avlHash: private ChainHash<T>{
public:
    avlHash(const size_t& p=17);
private:
};

template<class T>
avlHash<T>::avlHash(const size_t& p):ChainHash<T>(p){

}

#endif // AVLHASH_H
