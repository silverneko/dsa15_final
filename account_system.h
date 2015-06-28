#include <functional>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <regex>
#include <unordered_map>
#include <unordered_set>

const std::string alphabets("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

static int toIndex(char c)
{
  if(c <= '9') return c - '0';
  if(c <= 'Z') return c - 'A' + 10;
  return c - 'a' + 36;
}

class Trie{
  public:
    bool endHere;
    int count;
    Trie *branches[62];

    Trie() : endHere(false), count(0), branches {nullptr} {}

    bool exist(const std::string& str, int pos = 0) const
    {
      if(pos >= str.size()){
        return endHere;      // Return if some string end at this node
      }
      int i = toIndex(str[pos]);
      auto branch = branches[i];
      if(branch == nullptr){
        return false;
      }
      return branch->exist(str, pos+1);
    }

    // You should check if str already exists or not before inserting or erasing
    void insert(const std::string& str, int pos = 0)
    {
      if(pos >= str.size()){
        endHere = true;
        return ;
      }
      ++count;
      int i = toIndex(str[pos]);
      if(branches[i] == nullptr){
        branches[i] = new Trie();
      }
      branches[i]->insert(str, pos+1);
    }

    void erase(const std::string& str, int pos = 0)
    {
      if(pos >= str.size()){
        endHere = false;
        return ;
      }
      --count;
      int i = toIndex(str[pos]);
      branches[i]->erase(str, pos+1);
    }

};

class TransferRecord{
  public:
    enum Type {Deposit, Withdraw};
    int timeStamp;
    Type type;
    int hashID;
    long long money;
    TransferRecord() {}
    TransferRecord(int timeStamp, Type type, int hashID, long long money) : 
      timeStamp(timeStamp), type(type), hashID(hashID), money(money) {}
};

class Account{
  public:
    const std::string ID;           // This may collide.
    const int hashID;               // This should be unique all the time.
    const std::string hashPWD;
    long long balance;
    std::vector<TransferRecord> records;
    Account(const std::string& ID, int hashID, const std::string& hashPWD, long long balance = 0) : 
      ID(ID), hashID(hashID), hashPWD(hashPWD), balance(balance), records() {}
};

enum Status {IDNotFound, WrongPassword, Success, Fail};

class AccountSystem{
/* Trie version
  private:
    void dfs_find(Trie *now, std::string &str, std::vector<std::string> &vec, const std::string &pattern, std::set<int, std::greater<int>> &ctr){
        if(now == nullptr) return;
        if(now->endHere && ctr.find(pattern.length()) != ctr.end()){
            vec.push_back(str);
        }
        ctr.erase(pattern.length());
        for(int i = 0 ; i < alphabets.length() ; i++) if(now->branches[i] && now->branches[i]->count > 0) {
            char nc = alphabets[i];
            std::vector<int> add;
            std::vector<int> rmv;
            for(auto itr = ctr.begin() ; itr != ctr.end() ; ){
                int now = *(itr++);
                if(pattern[now] == '?' || pattern[now] == nc){
                    if(ctr.insert(now+1).second == true){
                        //insert succeed
                        add.push_back(now+1);
                    }
                    ctr.erase(now);
                    rmv.push_back(now);
                }else if(pattern[now] == '*'){
                    if(ctr.insert(now+1).second == true){
                        //insert succeed
                        add.push_back(now+1);
                    }
                }else{
                    ctr.erase(now);
                    rmv.push_back(now);
                }
            }
            if(ctr.size() > 0){
                str.push_back(nc);
                dfs_find(now->branches[i], str, vec, pattern, ctr);
                str.pop_back();
            }
            for(int i = 0 ; i < (int)add.size() ; i++) ctr.erase(add[i]);
            for(int i = 0 ; i < (int)rmv.size() ; i++) ctr.insert(rmv[i]);
        }
    }   
*/
  public:
    int timeStamp;
    int unusedHashID;
    int lastLoginHashID;

    std::set<std::string> IDs;                // set of used IDs.
    //std::unordered_set<std::string> __IDs;
    Trie __IDs;
    std::unordered_map<std::string, int> __toHashID;
    std::vector<std::string> __fromHashID;
    std::vector<int> __parent;          // Use disjoint-set to maintain.
    std::vector<Account> accounts;

    AccountSystem() : timeStamp(0), unusedHashID(0), lastLoginHashID(0) {}
    
    // It doesn't check if ID exists or not.
    int toHashID(const std::string& ID) const
    {
      return __toHashID.at(ID);
    }

    int find(int hashID)
    {
      return __parent[hashID] == hashID ? hashID : __parent[hashID] = find(__parent[hashID]);
    }

    void merge(int hashID1, int hashID2)
    {
      hashID1 = find(hashID1);
      hashID2 = find(hashID2);
      __parent[hashID2] = hashID1;
    }

    // This one doesn't check, either;
    const std::string& fromHashID(int hashID)
    {
      return __fromHashID[find(hashID)];
    }
    
    bool exist(const std::string& ID) const
    {
      return __IDs.exist(ID);
      //return __IDs.find(ID) != __IDs.end();
    }

    // All passwords should be hashed before calling functions.
    Status create(const std::string& ID, const std::string& hashPWD)
    {
      if(exist(ID)) return Fail;
      int hashID = unusedHashID++;
      accounts.emplace_back(ID, hashID, hashPWD, 0);
      IDs.insert(ID);
      __IDs.insert(ID);
      __toHashID[ID] = hashID;
      __fromHashID.push_back(ID);
      __parent.push_back(hashID);
      return Success;
    }

    Status login(const std::string& ID, const std::string& hashPWD)
    {
      if(!exist(ID)) return IDNotFound;
      int hashID = toHashID(ID);
      if(hashPWD != accounts[hashID].hashPWD){
        return WrongPassword;
      }
      lastLoginHashID = hashID;
      return Success;
    }
    
    void __destroy(const std::string& ID)
    {
      IDs.erase(ID);
      __IDs.erase(ID);
      __toHashID.erase(ID);
    }

    Status destroy(const std::string& ID, const std::string& hashPWD)
    {
      if(!exist(ID)) return IDNotFound;
      int hashID = toHashID(ID);
      Account &account = accounts[hashID];
      if(hashPWD != account.hashPWD) return WrongPassword;
      __destroy(ID);
      return Success;
    }

    std::tuple<Status, long long> merge(const std::string& ID1, const std::string& hashPWD1
                                      , const std::string& ID2, const std::string& hashPWD2)
    {
      if(!exist(ID1)) return std::make_tuple(IDNotFound, 1);
      if(!exist(ID2)) return std::make_tuple(IDNotFound, 2);
      int hashID1 = toHashID(ID1);
      int hashID2 = toHashID(ID2);
      Account &account1 = accounts[hashID1], &account2 = accounts[hashID2];
      if(hashPWD1 != account1.hashPWD) return std::make_tuple(WrongPassword, 1);
      if(hashPWD2 != account2.hashPWD) return std::make_tuple(WrongPassword, 2);
      account1.balance += account2.balance;
      merge(hashID1, hashID2);
      std::vector<TransferRecord> records(account1.records.size() + account2.records.size());
      auto it1 = account1.records.begin(), it2 = account2.records.begin();
      auto end1 = account1.records.end(), end2 = account2.records.end();
      for(auto &record : records){
        if(it1 == end1){
          record = *it2++;
        }else if(it2 == end2){
          record = *it1++;
        }else{
          if(it1->timeStamp <= it2->timeStamp){
            record = *it1++;
          }else{
            record = *it2++;
          }
        }
      }
      account1.records = records;
      __destroy(ID2);
      return std::make_tuple(Success, account1.balance);
    }
    
    std::tuple<Status, long long> deposit(long long money)
    {
      Account &account = accounts[lastLoginHashID];
      account.balance += money;
      return std::make_tuple(Success, account.balance);
    }

    std::tuple<Status, long long> withdraw(long long money)
    {
      Account &account = accounts[lastLoginHashID];
      if(account.balance < money) return std::make_tuple(Fail, account.balance);
      account.balance -= money;
      return std::make_tuple(Success, account.balance);
    }

    std::tuple<Status, long long> transfer(const std::string& ID, long long money)
    {
      if(!exist(ID)) return std::make_tuple(IDNotFound, 0);
      int hashID1 = lastLoginHashID, hashID2 = toHashID(ID);
      Account &account1 = accounts[hashID1], &account2 = accounts[hashID2];
      if(account1.balance < money) return std::make_tuple(Fail, account1.balance);
      account1.balance -= money;
      account2.balance += money;
      account1.records.emplace_back(timeStamp, TransferRecord::Withdraw, hashID2, money);
      account2.records.emplace_back(timeStamp, TransferRecord::Deposit , hashID1, money);
      timeStamp++;
      return std::make_tuple(Success, account1.balance);
    }

    std::vector<std::string> find(const std::string& pattern)
    {
      std::string reg;//("^");
      for(char p : pattern){
        if(p == '*'){
          reg += ".*";
        }else if(p == '?'){
          reg += '.';
        }else{
          reg += p;
        }
      }
      //reg += '$';
      std::regex e(reg);
      std::vector<std::string> matches;
      for(auto &ID : IDs){
        if((toHashID(ID) != lastLoginHashID) && std::regex_match(ID, e)){
          matches.push_back(ID);
        }
      }
      return matches;
    }

    /* Trie version
    std::vector<std::string> find(const std::string& pattern)
    {
        std::vector<std::string> matches;
        std::string tmp;
        std::set<int, std::greater<int>> container;
        container.insert(0);
        dfs_find(&__IDs, tmp, matches, pattern, container);
        return matches;
    }
    */
    
    std::vector<TransferRecord> search(const std::string& ID)
    {
      std::vector<TransferRecord> records;
      Account &account = accounts[lastLoginHashID];
      for(auto &record : account.records){
        if(ID == fromHashID(record.hashID)){
          records.push_back(record);
        }
      }
      return records;
    }

    void get_recommend(std::vector<std::string> &rmd, std::string &now){
        Trie *ptr = &__IDs;
        int cnt = 0;
        std::string tmp;
        char obs = now.back();
        for(int i = 0 ; i < now.length() - 1 ; i++){
            ptr = ptr->branches[ toIndex(now[i]) ];
            tmp += now[i];
        }
        if(ptr != &__IDs && ptr->endHere == false) rmd.push_back(tmp), cnt++;
        for(int i = 0 ; cnt < 10 && alphabets[i] != obs && i < alphabets.length() ; i++){
            char nc = alphabets[i];
            if(ptr->branches[ i ] == nullptr || ptr->branches[ i ]->endHere == false){
                tmp.push_back(nc);
                rmd.push_back(tmp);
                cnt++;
                tmp.pop_back();
            }
        }
        tmp.push_back(obs);
        ptr = ptr->branches[ toIndex(obs) ];
        for(int i = 0 ; cnt < 10 && i < alphabets.length() ; i++){
            char nc = alphabets[i];
            if(ptr->branches[ i ] == nullptr || ptr->branches[ i ]->endHere == false){
                tmp.push_back(nc);
                rmd.push_back(tmp);
                cnt++;
                tmp.pop_back();
            }
        }
    }
/*
    void get_recommend(std::vector<std::string> &rmd, std::string &now){
        std::string tmp = now;
        char obs = tmp.back();
        bool get_obs = false;
        tmp.pop_back();
        int cnt = 0;
        if(tmp.length() > 0 && !exist(tmp)) rmd.push_back(tmp), cnt++;
        tmp.push_back('0');
        if(cnt < 10){
            for(char nc = '0' ; !get_obs && cnt < 10 && nc <= '9' ; nc++){
                if(nc == obs){
                    get_obs = true;
                    break;
                }
                tmp.pop_back();
                tmp.push_back(nc);
                if(!exist(tmp)) rmd.push_back(tmp), cnt++;
            }
            for(char nc = 'A' ; !get_obs && cnt < 10 && nc <= 'Z' ; nc++){
                if(nc == obs){
                    get_obs = true;
                    break;
                }
                tmp.pop_back();
                tmp.push_back(nc);
                if(!exist(tmp)) rmd.push_back(tmp), cnt++;
            }
            for(char nc = 'a' ; !get_obs && cnt < 10 && nc <= 'z' ; nc++){
                if(nc == obs){
                    get_obs = true;
                    break;
                }
                tmp.pop_back();
                tmp.push_back(nc);
                if(!exist(tmp)) rmd.push_back(tmp), cnt++;
            }
        }
        tmp.pop_back();
        tmp.push_back(obs);
        tmp.push_back(obs);
        if(cnt < 10){
            for(char nc = '0' ; cnt < 10 && nc <= '9' ; nc++){
                tmp.pop_back();
                tmp.push_back(nc);
                rmd.push_back(tmp), cnt++;
            }
        }
    }
    */
};



