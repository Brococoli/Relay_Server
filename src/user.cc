#include "../include/user.h"

/* inline */ 
User::User(){
    online_ = false;
    user_id_ = -1;
}
/* inline */ 
User::User(int user_id, int sockfd, bool online){
    online_ = online;
    user_id_ = user_id;
    sockfd_ = sockfd;
}

/* inline */
User::~User(){}

/* inline */
int User::user_id() const{
    return user_id_;
}

/* inline */
void User::set_user_id(int user_id){
    user_id_ = user_id;
}

/* inline */ 
bool User::online() const{
    return online_;
}

/* inline */
void User::set_online(bool online){
    online_ = online;
}

int User::sockfd() const{
    return sockfd_;
}

void User::set_sockfd(int sockfd){
    sockfd_ = sockfd;
}

/* inline */
/* bool User::operator < (const User& user) const{ */
/*     return this->user_id_ < user.user_id_; */
/* } */

/* inline */
/* bool User::operator = (const User& user) const{ */
/*     return this->user_id_ == user.user_id_; */
/* } */

/* inline */
/* bool User::operator == (const User& user) const{ */
/*     return this->user_id_ == user.user_id_; */
/* } */
/* inline */
/* bool User::operator != (const User& user) const{ */
/*     return this->user_id_ != user.user_id_; */
/* } */
