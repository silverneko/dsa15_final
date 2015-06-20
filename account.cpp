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
#include "account.h"
#include "md5.h"

using namespace std;

/* general
#define SUCCESS 1
#define FAIL    0

#define ID_NOT_FOUND       10001
#define WRONG_PASSWORD     10002

#define ID_ALREADY_USED    20001

#define NOT_LOGIN_YET      30001

#define NO_ENOUGH_MONEY    60001
*/

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
    UserData tmp = make_pair(id, md5(pwd));
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
    UserData tmp = make_pair(id, md5(pwd));
    acct[tmp] = 0;
    return SUCCESS;
}

int Account :: remove ( const string &id, const string &pwd ) {
    if( !exist(id) ) {
        /* id not found */
        return ID_NOT_FOUND;
    }
    UserData tmp = make_pair(id, md5(pwd));
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

