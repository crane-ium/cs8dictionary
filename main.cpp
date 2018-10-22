#include <iostream>
#include <algorithm>
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
void second_test();
void timer_test();
char menu();
void test_hash_map_interactive(const char& c);
template<class T>
void test_hash_map_random(T& table, size_t size
                            , const string& test_name="hash test");
char choose_test();
string choose_data();
int choose_key();
string choose_skey();

int main()
{
    second_test();
    old_test();
    cout << endl;
    timer_test(); //timed test of open vs double hash maps
    cout << endl;

    prime_generator pgen;
    size_t prime = pgen.get_prime(1100); //generate a prime < 1100
    ChainHash<string> chain_test(13);
    OpenHash<int,string> open_test(prime);
    DoubleHash<int,string> double_test(prime);
    srand(time(NULL));
    test_hash_map_random(open_test, 1000, "Open Test");
    test_hash_map_random(chain_test, 1000, "Chain Test");
    test_hash_map_random(double_test, 1000, "Double Test");
    cout << "Open Test collisions: " << open_test.collisions() << endl;
    cout << "Double Test collisions: " << double_test.collisions() << endl;

    char c = choose_test();
    test_hash_map_interactive(c);
    return 0;
}

void test_hash_map_interactive(const char& c){
    char input = ' ';
    int k;
    string s;
    string val;
    //tests Open, Double, and Chained Hash Maps
    if(c=='o'){
        OpenHash<int, string> open(17);
        while(input!='x'){
            cout << "OPEN HASH TEST" << endl;
            input = menu();
            switch(input){
            case 'i':
                k = choose_key();
                s = choose_data();
                if(!open.insert(k, s))
                    cout << "NO SPACE LEFT!!!\n";
                else
                    cout << k << ":" << s << " has been inserted\n";
                break;
            case 'd':
                k = choose_key();
                if(open.remove(k))
                    cout << k << " was removed\n";
                else
                    cout << "Cannot remove " << k << ", cannot find\n";
                break;
            case 's':
                cout << "Size: " << open.length() << endl;
                break;
            case 'f':
                k = choose_key();
                if(open.find(k, val))
                    cout << k << ":" << val << " was found\n";
                else
                    cout << k << " could not be located\n";
                break;
            case 'r':{
                size_t r = rand() % 100000;
                string rs = ""; //rand string
                size_t rsl = 2 + rand() % 20; //rand string length
                for(size_t i = 0; i < rsl; i++){
                    rs += char(48 + rand() % 74);
                }
                if(!open.insert(r,rs))
                    cout << "NO SPACE LEFT!!!\n";
                else
                    cout << ">>" << r << ":" << rs << endl;
                break;
            }
            case '?':
                k = choose_key();
                if(open.find(k))
                    cout << k << " exists\n";
                else
                    cout << k << " does not exist\n";
                break;
            case 'x':
                cout << "BYE BYE!\n";
                break;
            default:
                break;
            }
            cout << open << endl;
        }
    }else if(c=='d'){
        DoubleHash<int, string> d_hash(17);
        while(input!='x'){
            cout << "---- d_hash HASH TEST ----" << endl;
            input = menu();
            switch(input){
            case 'i':
                k = choose_key();
                s = choose_data();
                try{
                    d_hash.insert(k, s);
                    cout << k << ":" << s << " has been inserted\n";
                }catch(exception &e){
                    cout << e.what() << endl;
                    cout << "NO SPACE LEFT!!!\n";
                }
                break;
            case 'd':
                k = choose_key();
                try{
                    d_hash.remove(k);
                    cout << k << " was removed\n";
                }catch(exception &e){
                    cout << "ERROR: " << e.what() << endl;
                    cout << "Cannot remove " << k << ", cannot find\n";
                }
                break;
            case 's':
                cout << "Size: " << d_hash.length() << endl;
                break;
            case 'f':
                k = choose_key();
                try{
                    d_hash.find(k, val);
                    cout << k << ":" << val << " was found!\n";
                }catch(exception &e){
                    cout << "ERROR: " << e.what() << endl;
                    cout << "Cannot remove " << k << ", DNE\n";
                }
                break;
            case 'r':{
                size_t r = rand() % 100000;
                string rs = ""; //rand string
                size_t rsl = 2 + rand() % 20; //rand string length
                for(size_t i = 0; i < rsl; i++){
                    rs += char(48 + rand() % 74);
                }
                try{
                    d_hash.insert(r, rs);
                    cout << r << ":" << rs << " has been inserted\n";
                }catch(exception &e){
                    cout << e.what() << endl;
                    cout << "NO SPACE LEFT!!!\n";
                }
                break;
            }
            case '?':
                k = choose_key();
                try{
                    d_hash.find(k);
                    cout << k << " does exist\n";
                }catch(exception &e){
                    cout << "ERROR: " << e.what() << endl;
                    cout << "Cannot find " << k << ", DNE\n";
                }
                break;
            case 'x':
                cout << "BYE BYE!\n";
                break;
            default:
                break;
            }
            cout << d_hash << endl;
        }
    }else if(c=='c'){
        ChainHash<string> chain(17);
        string k;
        while(input!='x'){
            cin.clear();
            fflush(stdin);
            cout << "chain HASH TEST (Note: Uses string keys!)" << endl;
            input = menu();
            switch(input){
            case 'i':
                k = choose_skey();
                s = choose_data();
                chain[k] = s;
                cout << k << ":" << s << " has been inserted\n";
                break;
            case 'd':
                k = choose_skey();
                if(chain.remove(k))
                    cout << k << " was removed\n";
                else
                    cout << "Cannot remove " << k << ", cannot find\n";
                break;
            case 's':
                cout << "Size: " << chain.length() << endl;
                break;
            case 'f':
                k = choose_skey();
                if(chain.find(k, val))
                    cout << k << ":" << val << " was found\n";
                else
                    cout << k << " could not be located\n";
                break;
            case 'r':{
                string rs1 = "", rs2 = ""; //rand string
                size_t rsl = 2 + rand() % 20; //rand string length
                for(size_t i = 0; i < rsl; i++){
                    rs1 += char(48 + rand() % 74);
                }
                rsl = 2 + rand() % 20; //rand string length
                for(size_t i = 0; i < rsl; i++){
                    rs2 += char(48 + rand() % 74);
                }
                chain[rs1] = rs2;
                cout << rs1 << " : " << rs2 << " has been inserted\n";
                break;
            }
            case '?':
                k = choose_skey();
                if(chain.exists(k))
                    cout << k << " exists\n";
                else
                    cout << k << " does not exist\n";
                break;
            case 'x':
                cout << "BYE BYE!\n";
                break;
            default:
                break;
            }
            cout << chain << endl;
        }
    }
}
template<class T>
void test_hash_map_random(T& table
                          , size_t size
                          , const string& test_name){
    srand(0);
    cout << "----- " << (test_name) <<" ------" << endl;
    for(size_t i = 0; i < size; i++){
        size_t r = rand();
        table.insert(r, ("TEST "+to_string(r)));
    }
    cout << "Indeces: " << table.length() << endl;
}
//MENU CONTROLS
char menu(){
    char input;
    cin.clear();
    fflush(stdin);
    cout << "[S]ize() [I]nsert [R]andom [D]elete [F]ind value [?]exists?   e[X]it  :";
    cin >> input;
    if(string("sirdf?x").find(tolower(input),0))
        return input;
    else
        return menu();
}
string choose_data(){
    string data = "";
    cin >> data;
    cin.clear();
    fflush(stdin);
    return data;
}
string choose_skey(){
    string data = "";
    cin >> data;
    return data;
}
int choose_key(){
    int key = -1;
    try{
        cin >> key;
    }catch(...){
        cout << "input key: ";
        cin.clear();
        fflush(stdin);
        return choose_key()    ;
    }
    if(key>=0)
        return key;
    cout << "input key >0: ";
    cin.clear();
    fflush(stdin);
    return choose_key();
}
char choose_test(){
    char input;
    cout << "[O]pen [D]ouble [C]hained Interactive Test: ";
    cin >> input;
    cin.clear();
    fflush(stdin);
    if(tolower(input) == 'o' || tolower(input) == 'd' || tolower(input) == 'c')
        return tolower(input);
    else return choose_test();
}


void timer_test(){
    //---- TIME TEST ----
    //https://stackoverflow.com/questions/14337278/precise-time-measurement
    LARGE_INTEGER frequency;
    LARGE_INTEGER t_start, t_end;
    double elapsed;

    prime_generator gen;
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
    cout << "Open hash took " << elapsed << " milliseconds with "
         << open_test.collisions() << " collisions\n";
    //TEST INSERT DOUBLE HASH
    QueryPerformanceCounter(&t_start);
    for(size_t i = 0; i < 10000; i++){
        double_test.insert(randkeys[i], to_string(i));
    }
    QueryPerformanceCounter(&t_end);
    elapsed = (t_end.QuadPart - t_start.QuadPart) * 1000.0 / frequency.QuadPart;
    cout << "Double hash took " << elapsed << " milliseconds with "
         << double_test.collisions() << " collisions\n";
    //TEST FIND OPEN HASH
    QueryPerformanceCounter(&t_start);
    for(size_t i = 0; i < 10000; i++){
        open_test.find(randkeys[i]);
    }
    QueryPerformanceCounter(&t_end);
    elapsed = (t_end.QuadPart - t_start.QuadPart) * 1000.0 / frequency.QuadPart;
    cout << "Open find took " << elapsed << " milliseconds with "
         << open_test.collisions() << " collisions\n";
    //TEST FIND DOUBLE HASH
    QueryPerformanceCounter(&t_start);
    for(size_t i = 0; i < 10000; i++){
        double_test.find(randkeys[i]);
    }
    QueryPerformanceCounter(&t_end);
    elapsed = (t_end.QuadPart - t_start.QuadPart) * 1000.0 / frequency.QuadPart;
    cout << "Double find took " << elapsed << " milliseconds with "
         << double_test.collisions() << " collisions\n";

    cout << "Open Hash Map Length:   " << (int)open_test << endl;
    cout << "Double Hash Map Length: " << (int)double_test << endl;

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


    //Test indexing
    DoubleHash<int,string> dh4(11);
    dh4[1] = "Test";
    dh4[100] = "test2";
    dh4[1] = "Retest";
    dh4[3] = "no dupe";
    dh4[4]; //set a blank data
    dh4.remove(1); //remove "Test"
    cout << dh4;
    cout << "dh4 collisions: " << dh4.collisions() << endl;
}

void second_test(){
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
    cout << copy2.length() << endl;
}






