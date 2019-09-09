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
int UserManager::Register(User* user){
    user_map_[user->user_id()] = user;
    return 0;
}

inline
int UserManager::ModUserInfo(User* user){
    user_map_[user->user_id()] = user;
    return 0;
}

inline
int UserManager::ModUserInfo(int user_id, bool online){
    User* user = GetUserPtr(user_id);
    if(user != NULL){
        user->set_online(online);
    }
    else{
        SetUserPtr(user_id, new User(user_id, online));
    }
    return 0;
}
int UserManager::Logout(int user_id){
    return user_map_.erase(user_id);
}

int UserManager::Login(int user_id){
    User* user = NULL;
    if((user = GetUserPtr(user_id)) == NULL) return -1;
    user->set_online(true);
    return 0;
}

int UserManager::Unregister(int user_id){
    User* user = NULL;
    if((user = GetUserPtr(user_id)) == NULL) return -1;
    delete user;
    return user_map_.erase(user_id);
}

int UserManager::Unregister(User* user){
    if(user == NULL) return -1;
    delete user;
    return user_map_.erase(user->user_id());
}


bool UserManager::Exist(int user_id){
    return GetUserPtr(user_id) != NULL;
}
bool UserManager::Online(int user_id){
    User* user = NULL;
    if((user = GetUserPtr(user_id)) == NULL) return -1;
    return user->online();
}
