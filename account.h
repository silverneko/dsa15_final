/* NTU DSA Spring 2015 Final Project
 * Author : silverneko bigelephant29 carolinetychen
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <regex>
#include <functional>
#include <unordered_map> /* we can use either map or unordered_map */
#include <unordered_set> /* we can use either set or unordered_set */

using namespace std;

/* general */
#define SUCCESS 1
#define FAIL    0

#define ID_NOT_FOUND       10001
#define WRONG_PASSWORD     10002

#define ID_ALREADY_USED    20001

#define NOT_LOGIN_YET      30001

#define NO_ENOUGH_MONEY    60001

/* Classes */

typedef pair<string,string> UserData; // <accountName, hashedPWD>

class Account {
private:
    struct StringPairHash {
        size_t operator()(const UserData &s) const{
            size_t h1 = hash<string>()(s.first);
            size_t h2 = hash<string>()(s.second);
            return h1 ^ (h2 << 1);
        }
    };
    unordered_set<string> name_used;
    unordered_map<UserData, long long, StringPairHash> acct;
    typedef unordered_map<UserData, long long, StringPairHash> :: iterator acct_itr;
    UserData now_user;
    inline bool exist( const string &id );
    inline bool islogin();
public:
    Account() : now_user(string(),string()) {}
    int login ( const string &id, const string &pwd );  /* ID_NOT_FOUND WRONG_PASSWORD SUCCESS*/
    int add ( const string &id, const string &pwd );    /* ID_ALREADY_USED SUCCESS */
    int remove ( const string &id, const string &pwd ); /* ID_NOT_FOUND WRONG_PASSWORD SUCCESS*/
    int deposit ( long long num );                      /* NOT_LOGIN_YET SUCCESS */ 
    int withdraw ( long long num );                     /* NOT_LOGIN_YET NO_ENOUGH_MONEY SUCCESS */
    long long money ();
};

