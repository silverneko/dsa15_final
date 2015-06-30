/* NTU DSA Spring 2015 Final Project
 * Author : silverneko bigelephant29 carolinetychen
 */

#include <cstdlib>
#include <string>
#include <algorithm>
#include <iostream>
#include <regex>
#include <functional>
#include <queue>
#include "account_system.h"
#include "md5.h"

using namespace std;

//#define test /* remove this line before uploading */
/* remember to add #ifdef test, #endif when adding test code */

/* general
#define SUCCESS 1
#define FAIL    0

#define ID_NOT_FOUND       10001
#define WRONG_PASSWORD     10002

#define ID_ALREADY_USED    20001

#define NOT_LOGIN_YET      30001

#define NO_ENOUGH_MONEY    60001
*/

/* Constant */
const long long MAX_MONEY = 10000000000LL; /* remember the max money in an account is in long long */
const int BUFFER_SIZE = 256;
const int ID_SIZE = 128;
const int PWD_SIZE = 128;
const int INF = 1029384756;

/* Global Functions */

void account_login    ( AccountSystem &mng, char *id, char *pwd );
void create_account   ( AccountSystem &mng, char *id, char *pwd );
void delete_account   ( AccountSystem &mng, char *id, char *pwd );
void merge_account    ( AccountSystem &mng, char *id1, char *pwd1, char *id2, char *pwd2 );
void account_deposit  ( AccountSystem &mng, long long num );
void account_withdraw ( AccountSystem &mng, long long num );
void account_transfer ( AccountSystem &mng, char *id, long long num);
void find_account     ( AccountSystem &mng, char *id );
void search_account   ( AccountSystem &mng, char *id );

/* Main */

int main () {
    ios_base::sync_with_stdio(false);
    char buf[BUFFER_SIZE];  /* char array buffer for inputting command type */
    char id[ID_SIZE], id1[ID_SIZE], id2[ID_SIZE];
    char pwd[PWD_SIZE], pwd1[PWD_SIZE], pwd2[PWD_SIZE];     /* password array */
    long long num;
    AccountSystem acctmng;

    while ( cin >> buf ) {
        /* there is no logout command, so we can simply read-in until EOF */
        if ( buf[0] == 'l' ) {
            /* login : login to a specific account
             * login [ID] [password]
             */
            cin >> id >> pwd;
            account_login(acctmng, id, pwd);

        } else if ( buf[0] == 'c' ) {
            /* create : create a specific account with password
             * create [ID] [password]
             */
            cin >> id >> pwd;
            create_account(acctmng, id, pwd);

        } else if ( buf[0] == 'd' && buf[2] == 'l' ) { /* command conflicts with deposit */
            /* delete : delete a specific account with password
             * delete [ID] [password]
             */
            cin >> id >> pwd;
            delete_account(acctmng, id, pwd);

        } else if ( buf[0] == 'm' ) {
            /* merge : merge the second account into the first one; after merging, the second one is deleted
             * merge [ID1] [password1] [ID2] [password2]
             */
            cin >> id1 >> pwd1 >> id2 >> pwd2;
            merge_account(acctmng, id1, pwd1, id2, pwd2);

        } else if ( buf[0] == 'd' && buf[2] == 'p' ) { /* command conflicts with delete */
            /* deposit : deposit money into [last-successful-login-ID]
             * deposit [num]
             */
            cin >> num;
            account_deposit(acctmng, num);

        } else if ( buf[0] == 'w' ) {
            /* withdraw : withdraw money from [last-successful-login-ID]
             * withdraw [num]
             */
            cin >> num;
            account_withdraw(acctmng, num);

        } else if ( buf[0] == 't' ) {
            /* transfer : transfer money from [last-successful-login-ID] to a specific account]
             * transfer [ID] num
             */
            cin >> id >> num;
            account_transfer(acctmng, id, num);

        } else if ( buf[0] == 'f' ) {
            /* find : find all existing account IDs that matches [wildcard ID] but is different from [last-successful-login-ID]
             * find [wildcard_ID]
             */
            cin >> id;
            find_account(acctmng, id);

        } else if ( buf[0] == 's' ) {
            /* search : search all transfer history of [last-successful-login-ID] from/to a specific account
             * search [ID]
             */
            cin >> id;
            search_account(acctmng, id);

        } 
    }

    return 0;
}

/* Global Functions */

int score(const string& a, const string& b)
{
  int l = min(a.size(), b.size()), dl = abs((int)(a.size()) - (int)(b.size()));
  int res = dl * (dl + 1) / 2;
  for(int i = 0; i < l; ++i){
    if(a[i] != b[i])
      res += l-i;
  }
  return res;
}

int score_(const string& a, const string& b, int ub)
{
  int l = min(a.size(), b.size()), dl = abs((int)(a.size()) - (int)(b.size()));
  int res = dl * (dl + 1) / 2;
  if(res >= ub) return INF;
  for(int i = 0; i < l; ++i){
    if(a[i] != b[i]){
      res += l-i;
      if(res >= ub) return INF;
    }
  }
  return res;
}

void idNotFound(const string& ID)
{
  cout << "ID " << ID << " not found\n";
}

void create_account ( AccountSystem &mng, char *id, char *pwd ) {
    string ID(id), hashPWD(md5(pwd));
    Status status = mng.create(ID, hashPWD);
    if(status == Fail){
      cout << "ID " << ID << " exists, ";
      // Needs to recommend IDs here.
      vector<string> rmd;
      mng.get_recommend(rmd, ID);
      for(int i = 0 ; i < (int)rmd.size() ; i++){
        if(i != 0) cout << ',';
        cout << rmd[i];
      }
      cout << endl;
    }else{
      cout << "success\n";
    }
}

void account_login ( AccountSystem &mng, char *id, char *pwd ) {
    string ID(id), hashPWD(md5(pwd));
    Status status = mng.login(ID, hashPWD);
    if(status == IDNotFound){
      idNotFound(ID);
    }else if(status == WrongPassword){
      cout << "wrong password\n";
    }else{
      cout << "success\n";
    }
}

void delete_account ( AccountSystem &mng, char *id, char *pwd ) {
    string ID(id), hashPWD(md5(pwd));
    Status status = mng.destroy(ID, hashPWD);
    if(status == IDNotFound){
      idNotFound(ID);
    }else if(status == WrongPassword){
      cout << "wrong password\n";
    }else{
      cout << "success\n";
    }
}

void merge_account ( AccountSystem &mng, char *id1, char *pwd1, char *id2, char *pwd2 ) {
    string ID1(id1), hashPWD1(md5(pwd1));
    string ID2(id2), hashPWD2(md5(pwd2));
    Status status;
    long long balance;
    tie(status, balance) = mng.merge(ID1, hashPWD1, ID2, hashPWD2);
    if(status == IDNotFound){
      if(balance == 1){
        idNotFound(ID1);
      }else{
        idNotFound(ID2);
      }
    }else if(status == WrongPassword){
      if(balance == 1){
        cout << "wrong password1\n";
      }else{
        cout << "wrong password2\n";
      }
    }else{
      cout << "success, " << ID1 << " has " << balance << " dollars\n";
    }
}

void account_deposit ( AccountSystem &mng, long long num ) {
    long long balance;
    tie(std::ignore, balance) = mng.deposit(num);
    cout << "success, " << balance << " dollars in current account\n";
}

void account_withdraw ( AccountSystem &mng, long long num ) {
    Status status;
    long long balance;
    tie(status, balance) = mng.withdraw(num);
    if(status == Fail){
      cout << "fail, " << balance << " dollars only in current account\n";
    }else{
      cout << "success, " << balance << " dollars left in current account\n";
    }
}

void account_transfer ( AccountSystem &mng, char *id, long long num){
    string ID(id);
    Status status;
    long long balance;
    tie(status, balance) = mng.transfer(ID, num);
    if(status == IDNotFound){
      cout << "ID " << ID << " not found, ";
      priority_queue<tuple<int, int, string>> Q;
      int timeStamp = 0;
      for(auto &candidate : mng.IDs){
        if(Q.size() >= 10){
          int scr = score_(ID, candidate, get<0>(Q.top()));
          if(scr >= get<0>(Q.top())){
            continue;
          }else{
            Q.push(make_tuple(scr, timeStamp, candidate));
            Q.pop();
          }
        }else{
          Q.push(make_tuple(score(ID, candidate), timeStamp, candidate));
        }
        ++timeStamp;
      }
      vector<string> candidates;
      while(!Q.empty()){
        candidates.emplace_back(get<2>(Q.top()));
        Q.pop();
      }
      reverse(candidates.begin(), candidates.end());
      if(!candidates.empty()){
        cout << candidates[0];
        for(int i = 1; i < candidates.size(); ++i){
          cout << ',' << candidates[i];
        }
      }
      cout << '\n';
    }else if(status == Fail){
      cout << "fail, " << balance << " dollars only in current account\n";
    }else{
      cout << "success, " << balance << " dollars left in current account\n";
    }
}

void find_account ( AccountSystem &mng, char *id ) {
  vector<const string*> matches(mng.find(id));
  if(matches.size() > 0){
    cout << *matches[0];
    for(int i = 1; i < matches.size(); ++i)
      cout << ',' << *matches[i];
  }
  cout << '\n';
}

void search_account ( AccountSystem &mng, char *id ) {
  vector<const TransferRecord*> records(mng.search(id));
  if(records.size() == 0){
    cout << "no record\n";
  }else{
    for(auto &record : records){
      if(record->type == TransferRecord::Deposit){
        cout << "From ";
      }else if(record->type == TransferRecord::Withdraw){
        cout << "To ";
      }
      cout << mng.fromHashID(record->hashID) << ' ' << record->money << '\n';
    }
  }
}
