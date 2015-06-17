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

#define test /* remove this line before uploading */
/* remember to add #ifdef test, #endif when adding test code */

/* general */
#define SUCCESS 1
#define FAIL    0

#define ID_NOT_FOUND       10001
#define WRONG_PASSWORD     10002

#define NO_ENOUGH_MONEY    60001


/* Constant */
const long long MAX_MONEY = 10000000000LL; /* remember the max money in an account is in long long */
const int BUFFER_SIZE = 256;
const int ID_SIZE = 128;
const int PWD_SIZE = 128;

/* Classes */

typedef pair<string,string> UserData;

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
    Account(){now_user = make_pair(string(),string());}
    bool add ( const string &id, const string &pwd );
    int remove ( const string &id, const string &pwd );
    void deposit ( long long num );
    bool withdraw ( long long num );
};

/* Global Functions */

void account_login    ( char *id, char *pwd );
void create_account   ( char *id, char *pwd );
void delete_account   ( char *id, char *pwd );
void merge_account    ( char *id1, char *pwd1, char *id2, char *pwd2 );
void account_deposit  ( long long num );
void account_withdraw ( long long num );
void account_transfer ( char *id, long long num);
void find_account     ( char *id );
void search_account   ( char *id );

/* Main */

int main () {
    
    char buf[BUFFER_SIZE];  /* char array buffer for inputting command type */
    char id[ID_SIZE], id1[ID_SIZE], id2[ID_SIZE];
    char pwd[PWD_SIZE], pwd1[PWD_SIZE], pwd2[PWD_SIZE];     /* password array */
    long long num;

    while ( ~scanf("%s", buf) ) {
        /* there is no logout command, so we can simply read-in until EOF */
        if ( buf[0] == 'l' ) {
            /* login : login to a specific account
             * login [ID] [password]
             */
            scanf("%s %s", id, pwd);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [password] = %s\n", "\"login\"", id, pwd);
            #endif
            account_login(id, pwd);

        } else if ( buf[0] == 'c' ) {
            /* create : create a specific account with password
             * create [ID] [password]
             */
            scanf("%s %s", id, pwd);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [password] = %s\n", "\"create\"", id, pwd);
            #endif
            create_account(id, pwd);

        } else if ( buf[0] == 'd' && buf[2] == 'l' ) { /* command conflicts with deposit */
            /* delete : delete a specific account with password
             * delete [ID] [password]
             */
            scanf("%s %s", id, pwd);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [password] = %s\n", "\"delete\"", id, pwd);
            #endif
            delete_account(id, pwd);

        } else if ( buf[0] == 'm' ) {
            /* merge : merge the second account into the first one; after merging, the second one is deleted
             * merge [ID1] [password1] [ID2] [password2]
             */
            scanf("%s %s %s %s", id1, pwd1, id2, pwd2);
            #ifdef test
                printf("Command %-10s : [ID1] = %s, [password1] = %s, [ID2] = %s, [password2] = %s\n", "\"merge\"", id1, pwd1, id2, pwd2);
            #endif
            merge_account(id1, pwd1, id2, pwd2);

        } else if ( buf[0] == 'd' && buf[2] == 'p' ) { /* command conflicts with delete */
            /* deposit : deposit money into [last-successful-login-ID]
             * deposit [num]
             */
            scanf("%lld", &num);
            #ifdef test
                printf("Command %-10s : [num] = %lld\n", "\"deposit\"", num);
            #endif
            account_deposit(num);

        } else if ( buf[0] == 'w' ) {
            /* withdraw : withdraw money from [last-successful-login-ID]
             * withdraw [num]
             */
            scanf("%lld", &num);
            #ifdef test
                printf("Command %-10s : [num] = %lld\n", "\"withdraw\"", num);
            #endif
            account_withdraw(num);

        } else if ( buf[0] == 't' ) {
            /* transfer : transfer money from [last-successful-login-ID] to a specific account]
             * transfer [ID] num
             */
            scanf("%s %lld", id, &num);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [num] = %lld\n", "\"transfer\"", id, num);
            #endif
            account_transfer(id, num);

        } else if ( buf[0] == 'f' ) {
            /* find : find all existing account IDs that matches [wildcard ID] but is different from [last-successful-login-ID]
             * find [wildcard_ID]
             */
            scanf("%s", id);
            #ifdef test
                printf("Command %-10s : [wildcard_ID] = %s\n", "\"find\"", id);
            #endif
            find_account(id);

        } else if ( buf[0] == 's' ) {
            /* search : search all transfer history of [last-successful-login-ID] from/to a specific account
             * search [ID]
             */
            scanf("%s", id);
            #ifdef test
                printf("Command %-10s : [ID] = %s\n", "\"search\"", id);
            #endif
            search_account(id);

        } 
    }

    return 0;
}

/* Member Functions */

inline bool Account :: exist ( const string &id ) {
    return name_used.find (id) != name_used.end();
}

inline bool Account :: islogin () {
    return now_user.first.length() > 0;
}

bool Account :: add ( const string &id, const string &pwd ) {
    if( exist(id) ) {
        /* name used */
        return FAIL;
    }
    name_used.insert(id);
    UserData tmp = make_pair(id, pwd);
    acct[tmp] = 0;
    return SUCCESS;
}

int Account :: remove ( const string &id, const string &pwd ) {
    if( !exist(id) ) {
        /* id not found */
        return ID_NOT_FOUND;
    }
    UserData tmp = make_pair(id, pwd);
    if( acct.find(tmp) == acct.end() ){
        /* wrong password */
        return WRONG_PASSWORD;
    }
    name_used.erase(id);
    acct.erase(tmp);
    return SUCCESS;
}

void Account :: deposit ( long long num ) {
    if( !islogin() ){
        /* not login yet */
        return;
    }
    acct[now_user] += num;
}

bool Account :: withdraw ( long long num ) {
    if( !islogin() ){
        /* not login yet */
        return FAIL;
    }
    acct_itr itr = acct.find(now_user);
    if(itr->second < num){
        return FAIL;
    }
    itr->second -= num;
    return SUCCESS;
}

/* Global Functions */

void account_login ( char *id, char *pwd ) {

}

void create_account ( char *id, char *pwd ) {

}

void delete_account ( char *id, char *pwd ) {

}

void merge_account ( char *id1, char *pwd1, char *id2, char *pwd2 ) {

}

void account_deposit ( long long num ) {

}

void account_withdraw ( long long num ) {

}

void account_transfer ( char *id, long long num){

}

void find_account ( char *id ) {

}

void search_account ( char *id ) {

}
