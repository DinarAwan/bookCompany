#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include "ManajemenUser.h"

using namespace std;

class Login {
private:
    long long int nik;
    string password;
    int role;
    ManajemenUser* manajemenUser;

public:
    Login();
    ~Login();
    void prosesLogin();
    int getRole() const;
};

#endif
