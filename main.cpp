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

using namespace std;

#define test /* remove this line before uploading */
/* remember to add #ifdef test, #endif when adding test code */

/* Constant */
const long long MAX_MONEY = 10000000000LL; /* remember the max money in an account is in long long */
const int BUFFER_SIZE = 256;
const int ID_SIZE = 128;
const int PWD_SIZE = 128;

/* Classes */

/* Global Functions */

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

        } else if ( buf[0] == 'c' ) {
            /* create : create a specific account with password
             * create [ID] [password]
             */
            scanf("%s %s", id, pwd);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [password] = %s\n", "\"create\"", id, pwd);
            #endif

        } else if ( buf[0] == 'd' && buf[2] == 'l' ) { /* command conflicts with deposit */
            /* delete : delete a specific account with password
             * delete [ID] [password]
             */
            scanf("%s %s", id, pwd);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [password] = %s\n", "\"delete\"", id, pwd);
            #endif

        } else if ( buf[0] == 'm' ) {
            /* merge : merge the second account into the first one; after merging, the second one is deleted
             * merge [ID1] [password1] [ID2] [password2]
             */
            scanf("%s %s %s %s", id1, pwd1, id2, pwd2);
            #ifdef test
                printf("Command %-10s : [ID1] = %s, [password1] = %s, [ID2] = %s, [password2] = %s\n", "\"merge\"", id1, pwd1, id2, pwd2);
            #endif

        } else if ( buf[0] == 'd' && buf[2] == 'p' ) { /* command conflicts with delete */
            /* deposit : deposit money into [last-successful-login-ID]
             * deposit [num]
             */
            scanf("%lld", &num);
            #ifdef test
                printf("Command %-10s : [num] = %lld\n", "\"deposit\"", num);
            #endif

        } else if ( buf[0] == 'w' ) {
            /* withdraw : withdraw money from [last-successful-login-ID]
             * withdraw [num]
             */
            scanf("%lld", &num);
            #ifdef test
                printf("Command %-10s : [num] = %lld\n", "\"withdraw\"", num);
            #endif

        } else if ( buf[0] == 't' ) {
            /* transfer : transfer money from [last-successful-login-ID] to a specific account]
             * transfer [ID] num
             */
            scanf("%s %lld", id, &num);
            #ifdef test
                printf("Command %-10s : [ID] = %s, [num] = %lld\n", "\"transfer\"", id, num);
            #endif

        } else if ( buf[0] == 'f' ) {
            /* find : find all existing account IDs that matches [wildcard ID] but is different from [last-successful-login-ID]
             * find [wildcard_ID]
             */
            scanf("%s", id);
            #ifdef test
                printf("Command %-10s : [wildcard_ID] = %s\n", "\"find\"", id);
            #endif

        } else if ( buf[0] == 's' ) {
            /* search : search all transfer history of [last-successful-login-ID] from/to a specific account
             * search [ID]
             */
            scanf("%s", id);
            #ifdef test
                printf("Command %-10s : [ID] = %s\n", "\"search\"", id);
            #endif

        } 
    }

    return 0;
}

/* Member Functions */

/* Global Functions */


