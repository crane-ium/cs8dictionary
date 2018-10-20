#include <iostream>
#include "dictionary.h"

using namespace std;

int main()
{
    Dictionary<int,string> dict;

    dict.insert(5,"data1");
    dict.insert(12, "RANDOM DATA");
    dict.insert(1, "duplicate");
    for(int i = 0; i < 10; i++)
        dict.insert(i, ("test"));

    cout << dict;

    return 0;
}
