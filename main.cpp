#include <iostream>
#include "openhash.h"
#include "../prime_generator/prime_generator.h"
#include "doublehash.h"
#include "Windows.h" //For timer QueryPerformanceFrequency/Counter
#include <random>
#include <time.h>
#include "hashlist.h"
#include "chainhash.h"

using namespace std;

void old_test();

int main()
{
    ChainHash<int> chain(13);
    chain.insert(1000, 123);
    chain.insert(":)", 123);
    cout << chain << endl;
    cout << chain[":)"] << endl;
    chain[":)"] = 1234;
    cout << chain[":)"] << endl;
    chain["new"] = 321;
    chain[123] = 111111;
    chain[124] = 222222;
    chain[124]++;
    chain[124];
    chain[125];
    chain.remove(125);
    cout << chain << endl;
    ChainHash<string> schain(17);
    schain["First1"] = "Hello";
    schain["Second"] = ", ";
    schain["Third"] = "World";
    schain["Fourth"] = "!";
    schain["(!JCZ(ps..m]    \""] = "CRAZY";
    cout << schain << endl;
    ChainHash<string> copy1(schain);
    copy1[0] = "zero";
    copy1["SMALL ADDITIONS"] = "noice";
    cout << copy1 << endl;
    ChainHash<string> copy2;
    copy2["before"] = "WORSE?";
    copy2 = copy1;
    copy2["after"] = "BETTER?";
    copy2.remove("after");
    copy2.remove("before"); //should have no effect

    cout << copy2 << endl;
    return 0;
}

void old_test(){
    prime_generator gen;
    long int prime = gen.get_prime(1000);
    OpenHash<int,string> dict(prime);
    DoubleHash<int,string> dh(11);

    dict.insert(5,"data1");
    dict.insert(1500, "RANDOM DATA");
    dict.insert(1, "duplicate");
    for(int i = 0; i < 10; i++)
        dict.insert(i, ("test"));
    string data;
    cout << dict.find(1500,data) << ": " << data << endl;
    cout << dict << "Done with first dict\n";

    OpenHash<int,string> aDict(dict);
    cout << aDict << endl;
    aDict.insert(200500, "YUUUGE KEY");
    cout << aDict << endl;
    OpenHash<int, string> bDict;
    bDict = aDict;
    bDict.insert(103, ":)");
    cout << bDict << endl;

//    dh = dynamic_cast<OpenHash<int,string> >(aDict);
    dh.insert(100, "doublehash");
    cout << dh << endl;
    DoubleHash<int,string> dh2(11);
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
//    size_t prime3 = gen.get_prime(150);
    size_t iterations = 10000;
    size_t* randkeys = new size_t[iterations];
    srand(time(NULL)); //comment out to make it repeatable tests
    for(size_t i = 0; i < iterations; i++)
        randkeys[i] = rand();
    OpenHash<int,string> open_test(prime1);
    DoubleHash<int,string> double_test(prime1);
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

    cout << "Open Hash Map Length:   " << (int)open_test << endl;
    cout << "Double Hash Map Length: " << (int)double_test << endl;

    //Test indexing
    DoubleHash<int,string> dh4(11);
    dh4[1] = "Test";
    dh4[100] = "test2";
    dh4[1] = "Retest";
    dh4[3] = "no dupe";
    dh4[4]; //set a blank data
    dh4.remove(1); //remove "Test"
    cout << dh4;
}






