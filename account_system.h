#include <functional>
#include <string>
#include <vector>
#include <map>

class TransferRecord{
  public:
    enum Type {Deposit, Withdraw};
    Type type;
    int hashID;
    long long money;
    TransferRecord(Type type, int hashID, long long money) : 
      type(type), hashID(hashID), money(money) {}
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

class AccountSystem{
  public:
    int timeStamp;
    int currentLoginHashID;

    std::set<std::string> IDs;  // set of used IDs.
    std::map<std::string, int> __toHashID;
    std::vector<std::string> __fromHashID;
    std::vector<Account> accounts;
    
    // It doesn't check if ID exists or not.
    int toHashID(const std::string& ID) const
    {
      return __toHashID[ID];
    }

    // This one doesn't check, either;
    const std::string& fromHashID(int hashID) const
    {
      return __fromHashID[hashID];
    }
    
    bool exist(const std::string& ID) const
    {
      return IDs.find(ID) != IDs.end();
    }

    // All passwords should be hashed before calling functions.
    bool create(const std::string& ID, const std::string& hashPWD){
      if(exist(ID)) return false;
      int hashID = timeStamp++;
      accounts.emplace_back(ID, hashID, hashPWD, 0);
      IDs.insert(ID);
      __toHashID[ID] = hashID;
      __fromHashID.push_back(ID);
      return true;
    }

    bool 

    AccountSystem() : timeStamp(0), currentLoginHashID(0) {}
};
