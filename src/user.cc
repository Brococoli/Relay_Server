#include "user.h"

inline 
User::User(){
    online_ = false;
    user_id_ = -1;
}
inline 
User::User(int user_id, bool online){
    online_ = user_id;
    user_id_ = online;
}

inline
User::~User(){}

inline
int User::user_id() const{
    return user_id_;
}

inline
void User::set_user_id(int user_id){
    user_id_ = user_id;
}

inline 
bool User::online() const{
    return online_;
}

inline
void User::set_online(bool online){
    online_ = online;
}

/* inline */
/* bool User::operator < (const User& user) const{ */
/*     return this->user_id_ < user.user_id_; */
/* } */

/* inline */
/* bool User::operator = (const User& user) const{ */
/*     return this->user_id_ == user.user_id_; */
/* } */

inline
bool User::operator == (const User& user) const{
    return this->user_id_ == user.user_id_;
}
inline
bool User::operator != (const User& user) const{
    return this->user_id_ != user.user_id_;
}
