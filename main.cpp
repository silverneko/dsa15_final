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

#define ID_ALREADY_USED    20001

#define NOT_LOGIN_YET      30001

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
    int login ( const string &id, const string &pwd );  /* ID_NOT_FOUND WRONG_PASSWORD SUCCESS*/
    int add ( const string &id, const string &pwd );    /* ID_ALREADY_USED SUCCESS */
    int remove ( const string &id, const string &pwd ); /* ID_NOT_FOUND WRONG_PASSWORD SUCCESS*/
    int deposit ( long long num );                      /* NOT_LOGIN_YET SUCCESS */ 
    int withdraw ( long long num );                     /* NOT_LOGIN_YET NO_ENOUGH_MONEY SUCCESS */
    long long money ();
};

/* Global Functions */

void account_login    ( Account &mng, char *id, char *pwd );
void create_account   ( Account &mng, char *id, char *pwd );
void delete_account   ( Account &mng, char *id, char *pwd );
void merge_account    ( Account &mng, char *id1, char *pwd1, char *id2, char *pwd2 );
void account_deposit  ( Account &mng, long long num );
void account_withdraw ( Account &mng, long long num );
void account_transfer ( Account &mng, char *id, long long num);
void find_account     ( Account &mng, char *id );
void search_account   ( Account &mng, char *id );

/* Main */

int main () {
    
    char buf[BUFFER_SIZE];  /* char array buffer for inputting command type */
    char id[ID_SIZE], id1[ID_SIZE], id2[ID_SIZE];
    char pwd[PWD_SIZE], pwd1[PWD_SIZE], pwd2[PWD_SIZE];     /* password array */
    long long num;
    Account acctmng;

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
            account_login(acctmng, id, pwd);

        } else if ( buf[0] == 'c' ) {
            /* create : create a specific account with password
             * create [ID] [password]
             */
            scanf("%s %s", id, pwd);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [password] = %s\n", "\"create\"", id, pwd);
            #endif
            create_account(acctmng, id, pwd);

        } else if ( buf[0] == 'd' && buf[2] == 'l' ) { /* command conflicts with deposit */
            /* delete : delete a specific account with password
             * delete [ID] [password]
             */
            scanf("%s %s", id, pwd);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [password] = %s\n", "\"delete\"", id, pwd);
            #endif
            delete_account(acctmng, id, pwd);

        } else if ( buf[0] == 'm' ) {
            /* merge : merge the second account into the first one; after merging, the second one is deleted
             * merge [ID1] [password1] [ID2] [password2]
             */
            scanf("%s %s %s %s", id1, pwd1, id2, pwd2);
            #ifdef test
                printf("Command %-10s : [ID1] = %s, [password1] = %s, [ID2] = %s, [password2] = %s\n", "\"merge\"", id1, pwd1, id2, pwd2);
            #endif
            merge_account(acctmng, id1, pwd1, id2, pwd2);

        } else if ( buf[0] == 'd' && buf[2] == 'p' ) { /* command conflicts with delete */
            /* deposit : deposit money into [last-successful-login-ID]
             * deposit [num]
             */
            scanf("%lld", &num);
            #ifdef test
                printf("Command %-10s : [num] = %lld\n", "\"deposit\"", num);
            #endif
            account_deposit(acctmng, num);

        } else if ( buf[0] == 'w' ) {
            /* withdraw : withdraw money from [last-successful-login-ID]
             * withdraw [num]
             */
            scanf("%lld", &num);
            #ifdef test
                printf("Command %-10s : [num] = %lld\n", "\"withdraw\"", num);
            #endif
            account_withdraw(acctmng, num);

        } else if ( buf[0] == 't' ) {
            /* transfer : transfer money from [last-successful-login-ID] to a specific account]
             * transfer [ID] num
             */
            scanf("%s %lld", id, &num);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [num] = %lld\n", "\"transfer\"", id, num);
            #endif
            account_transfer(acctmng, id, num);

        } else if ( buf[0] == 'f' ) {
            /* find : find all existing account IDs that matches [wildcard ID] but is different from [last-successful-login-ID]
             * find [wildcard_ID]
             */
            scanf("%s", id);
            #ifdef test
                printf("Command %-10s : [wildcard_ID] = %s\n", "\"find\"", id);
            #endif
            find_account(acctmng, id);

        } else if ( buf[0] == 's' ) {
            /* search : search all transfer history of [last-successful-login-ID] from/to a specific account
             * search [ID]
             */
            scanf("%s", id);
            #ifdef test
                printf("Command %-10s : [ID] = %s\n", "\"search\"", id);
            #endif
            search_account(acctmng, id);

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

int Account :: login ( const string &id, const string &pwd ) {
    if( !exist(id) ) {
        /* id not found */
        return ID_NOT_FOUND;
    }
    UserData tmp = make_pair(id, pwd);
    if( acct.find(tmp) == acct.end() ) {
        /* wrong password */
        return WRONG_PASSWORD;
    }
    now_user = tmp;
    return SUCCESS;
}

int Account :: add ( const string &id, const string &pwd ) {
    if( exist(id) ) {
        /* name used */
        return ID_ALREADY_USED;
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
    if( acct.find(tmp) == acct.end() ) {
        /* wrong password */
        return WRONG_PASSWORD;
    }
    name_used.erase(id);
    acct.erase(tmp);
    return SUCCESS;
}

int Account :: deposit ( long long num ) {
    if( !islogin() ){
        /* not login yet */
        return NOT_LOGIN_YET;
    }
    acct[now_user] += num;
    return SUCCESS;
}

int Account :: withdraw ( long long num ) {
    if( !islogin() ){
        /* not login yet */
        return NOT_LOGIN_YET;
    }
    acct_itr itr = acct.find(now_user);
    if(itr->second < num){
        return NO_ENOUGH_MONEY;
    }
    itr->second -= num;
    return SUCCESS;
}

long long Account :: money (){
    if( !islogin() ){
        /* not login yet */
        return -1LL;
    }
    return acct[now_user];
}

/* Global Functions */

void account_login ( Account &mng, char *id, char *pwd ) {
    string id_str = string(id);
    string pwd_str = string(pwd);
    int status = mng.login(id_str, pwd_str);
    if( status == ID_NOT_FOUND ) {
        printf("ID %s not found\n", id);
    } else if ( status == WRONG_PASSWORD ) {
        puts("wrong password");
    } else if ( status == SUCCESS){
        puts("success");
    } else puts("ERROR RETURN VALUE");
}

void create_account ( Account &mng, char *id, char *pwd ) {
    string id_str = string(id);
    string pwd_str = string(pwd);
    int status = mng.add(id_str, pwd_str);
    if ( status == ID_ALREADY_USED ) {
        /* todo */
    } else if ( status == SUCCESS ) {
         puts("success");
    } else puts("ERROR RETURN VALUE");
}

void delete_account ( Account &mng, char *id, char *pwd ) {
    string id_str = string(id);
    string pwd_str = string(pwd);
    int status = mng.remove(id_str, pwd_str);
    if ( status == ID_NOT_FOUND ) {
        printf("ID %s not found\n", id);
    } else if ( status == WRONG_PASSWORD ) {
        puts("wrong password");
    } else if ( status == SUCCESS ) {
        puts("success");
    } else puts("ERROR RETURN VALUE");
}

void merge_account ( Account &mng, char *id1, char *pwd1, char *id2, char *pwd2 ) {
    /* todo */
}

void account_deposit ( Account &mng, long long num ) {
    int status = mng.deposit(num);
    if ( status == NOT_LOGIN_YET ) {
        /* not login yet */
    } else if ( status == SUCCESS ) {
        printf("success, %lld dollars in current account\n", mng.money());
    } else puts("ERROR RETURN VALUE");
}

void account_withdraw ( Account &mng, long long num ) {
    int status = mng.withdraw(num);
    if ( status == NOT_LOGIN_YET ) {
        /* not login yet */
    } else if ( status == NO_ENOUGH_MONEY ) {
        printf("fail, %lld dollars only in current account\n", mng.money());
    } else if ( status == SUCCESS ) {
        printf("success, %lld dollars left in current account\n", mng.money());
    } else puts("ERROR RETURN VALUE");
}

void account_transfer ( Account &mng, char *id, long long num){
    
}

void find_account ( Account &mng, char *id ) {
    
}

void search_account ( Account &mng, char *id ) {
    
}
