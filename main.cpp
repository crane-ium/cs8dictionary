#include <iostream>
#include "dictionary.h"
#include "../prime_generator/prime_generator.h"
#include "doublehash.h"

using namespace std;

int main()
{
    prime_generator gen;
    long int prime = gen.get_prime(1000);
    Dictionary<int,string> dict(prime);
    DoubleHash<int,string> dh(prime);

    dict.insert(5,"data1");
    dict.insert(1500, "RANDOM DATA");
    dict.insert(1, "duplicate");
    for(int i = 0; i < 10; i++)
        dict.insert(i, ("test"));
    string data;
    cout << dict.find(1500,data) << ": " << data << endl;
    cout << dict << "Done with first dict\n";

    Dictionary<int,string> aDict(dict);
    cout << aDict << endl;
    aDict.insert(200500, "YUUUGE KEY");
    cout << aDict << endl;
    Dictionary<int, string> bDict;
    bDict = aDict;
    bDict.insert(103, ":)");
    cout << bDict << endl;

    dh.insert(100, "doublehash");
    cout << dh << endl;

    return 0;
}
