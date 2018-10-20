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
    DoubleHash<int,string> dh(11,13);

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

//    dh = dynamic_cast<Dictionary<int,string> >(aDict);
    dh.insert(100, "doublehash");
    cout << dh << endl;
    DoubleHash<int,string> dh2(11,13);
//    dh2 = dh;
    dh2.insert(1, "dh2");
    dh2.insert(12, "test");
    dh2.insert(36,"test2");
    dh2.insert(71, "testing 5");
    dh2.insert(115, "testing 5");
    dh2.insert(11, "11");
    dh2.insert(13, "13");
    dh2.insert(65, "65");
    dh2.insert(3, "3");
    dh2.insert(200, "two-hundred");
    dh2.insert(150, "150");
    dh2.insert(55, "OVERFLOW");
    cout << dh2 << endl;
    cout << (int)dh2 << endl;
    cout << dh2[200] << endl;
    dh2[200] = "new value";
    cout << dh2[200] << endl;
    cout << dh2[201] << endl;
    return 0;
}
