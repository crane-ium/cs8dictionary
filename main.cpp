#include <iostream>
#include "dictionary.h"
#include "../prime_generator/prime_generator.h"
#include "doublehash.h"
#include "Windows.h"
#include <random>
#include <time.h>

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
//    dh2.insert(55, "OVERFLOW");
    cout << dh2 << endl;
    cout << (int)dh2 << endl;
    cout << dh2[200] << endl;
    dh2[200] = "new value";
    cout << dh2[200] << endl;
    try{
        cout << dh2[201] << endl;
    }catch(exception& e){
        cout << "Oops\n" << e.what()<<endl;
    }

    //---- TIME TEST ----
    //https://stackoverflow.com/questions/14337278/precise-time-measurement
    LARGE_INTEGER frequency;
    LARGE_INTEGER t_start, t_end;
    double elapsed;

    size_t prime1 = gen.get_prime(15000);
    size_t prime2 = gen.get_prime(prime1-1);
//    size_t prime3 = gen.get_prime(150);
    size_t iterations = 10000;
    size_t* randkeys = new size_t[iterations];
    srand(time(NULL)); //comment out to make it repeatable tests
    for(size_t i = 0; i < iterations; i++)
        randkeys[i] = rand();
    Dictionary<int,string> open_test(prime1);
    DoubleHash<int,string> double_test(prime1, prime2);
    //TEST OPEN HASH
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t_start);
    for(size_t i = 0; i < 10000; i++){
        open_test.insert(randkeys[i], to_string(i));
    }
    QueryPerformanceCounter(&t_end);
    elapsed = (t_end.QuadPart - t_start.QuadPart) * 1000.0 / frequency.QuadPart;
    cout << "Open hash took " << elapsed << " milliseconds\n";
    //TEST INSERT DOUBLE HASH
    QueryPerformanceCounter(&t_start);
    for(size_t i = 0; i < 10000; i++){
        double_test.insert(randkeys[i], to_string(i));
    }
    QueryPerformanceCounter(&t_end);
    elapsed = (t_end.QuadPart - t_start.QuadPart) * 1000.0 / frequency.QuadPart;
    cout << "Double hash took " << elapsed << " milliseconds\n";
    //TEST FIND OPEN HASH
    QueryPerformanceCounter(&t_start);
    for(size_t i = 0; i < 10000; i++){
        open_test.find(randkeys[i]);
    }
    QueryPerformanceCounter(&t_end);
    elapsed = (t_end.QuadPart - t_start.QuadPart) * 1000.0 / frequency.QuadPart;
    cout << "Open find took " << elapsed << " milliseconds\n";
    //TEST FIND DOUBLE HASH
    QueryPerformanceCounter(&t_start);
    for(size_t i = 0; i < 10000; i++){
        double_test.find(randkeys[i]);
    }
    QueryPerformanceCounter(&t_end);
    elapsed = (t_end.QuadPart - t_start.QuadPart) * 1000.0 / frequency.QuadPart;
    cout << "Double find took " << elapsed << " milliseconds\n";

    //Test indexing
    DoubleHash<int,string> dh4(11);
    dh4[1] = "Test";
    dh4[100] = "test2";
    dh4[1] = "Retest";
    dh4[3] = "no dupe";
    dh4[4];
    cout << dh4;

    return 0;
}








