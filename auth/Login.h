#ifndef LOGIN_H
#define LOGIN_H

#include <string>
using namespace std;

class Login {
private:
    string username;
    string password;
    int role; 

public:
    Login();               
    void prosesLogin();    
    int getRole() const;  
};

#endif
