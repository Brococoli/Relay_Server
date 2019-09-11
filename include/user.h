#ifndef USER_H_
#define USER_H_


class User
{
public:
    User();
    User(int user_id, int sockfd = -1, bool online = false);
    virtual ~User();

    bool online() const;
    void set_online(bool online);

    int user_id() const;
    void set_user_id(int user_id);

    int sockfd() const;
    void set_sockfd(int);
    /* bool operator < (const User& user) const; */
    /* bool operator = (const User& user) const; */
    /* bool operator == (const User& user) const; */
    /* bool operator != (const User& user) const; */
private:
    int user_id_;
    bool online_;
    int sockfd_;

};

#endif
