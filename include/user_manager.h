#ifndef USER_MANAGER_H_
#define USER_MANAGER_H_

#include "user.h"
#include <map>

class UserManager
{
    /* typedef std::unordered_set<User>::iterator unordered_set_it; */ 
public:
    UserManager();
    ~UserManager();

    int Register(int user_id, bool online = false);
    virtual int Register(User* user);
    virtual int Unregister(User* user);
    virtual int ModUserInfo(User* user);
    int ModUserInfo(int user_id, bool online);

    int Logout(int user_id);
    int Login(int user_id);
    int Unregister(int user_id);
    bool Online(int user_id);
    bool Exist(int user_id);
private:
    std::map<int, User*> user_map_;

    User* GetUserPtr(int user_id);
    void SetUserPtr(int user_id, User* user);

};

#endif
