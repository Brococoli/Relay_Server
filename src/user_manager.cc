#include "user_manager.h"

inline
UserManager::UserManager(){
    user_map_.clear();
}

inline
UserManager::~UserManager(){
    for(auto int_and_pUser_pair: user_map_){
        delete int_and_pUser_pair.second;
    }
}

inline
int UserManager::Register(int user_id, bool online){
    user_map_[user_id] = new User(user_id, online);
    return 0;
}

inline
int UserManager::Register(User user){
    user_map_[user.user_id()] = new User(user);
    return 0;
}

inline
int UserManager::ModUserInfo(User user){
    user_map_[user.user_id()] = new User(user);
    return 0;
}

inline
int UserManager::ModUserInfo(int user_id, bool online){
    user_map_[user_id] = 
}
int UserManager::Logout(int user_id){
    return ModUserInfo(User(user_id, false));
}

int UserManager::Login(int user_id){
    return ModUserInfo(User(user_id, true));
}

int UserManager::Unregister(int user_id){
    return user_set_.erase(User(user_id, true));
}

inline
std::unordered_set<User>::iterator UserManager::GetUser(int user_id){
    return 
    return  user_set_.find(User(user_id, false));
}

inline
std::unordered_set<User>::iterator UserManager::GetUser(User user){
    return  user_set_.find(user);
}

bool UserManager::Exist(int user_id){
    return GetUser(user_id) != user_set_.end();
}
bool UserManager::Online(int user_id){
    std::unordered_set<User>::iterator it = GetUser(user_id);
    if(it != user_set_.end()){
        /* return it->online(); */
        /* return (*it).Online(); */
        return 1;
    }
    else 
        return -1;
}
