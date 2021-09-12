#pragma once
#include "SHA1.h"
#include "string.h"
#define SIZE 10
#define LOGINLENGTH 10

class Chat {
public:
    Chat();
    ~Chat();
    void reg(char _login[LOGINLENGTH], char _pass[], int pass_length);
    bool login(char _login[LOGINLENGTH], char _pass[], int pass_length);
    int hfunc(char* _login, int dif);
    void resize();
    void add(char _login[LOGINLENGTH], uint* hash);
    void deluser(char _login[LOGINLENGTH]);
    void get_data_count();

private:
    enum Status {
        free, 
        engaged, 
        deleted 
    };
    struct AuthData {
        AuthData() :
            login(""),
            pass_sha1_hash(0),
            status(free){}
        
        ~AuthData() {
            if (pass_sha1_hash)
                delete[] pass_sha1_hash;
        }
        AuthData(char _login[LOGINLENGTH], uint* sh1) {            
            strcpy_s  (login, _login);
            pass_sha1_hash = sh1;
            status = engaged;
        }
        AuthData& operator = (const AuthData& other) {
            memcpy(login, other.login, LOGINLENGTH);
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];
            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);
            status = other.status;
            return *this;
        }
        char login[LOGINLENGTH];
        uint* pass_sha1_hash;
        Status status;
    };    
    AuthData *data;
    uint data_count;
    uint mem_size = SIZE;
};