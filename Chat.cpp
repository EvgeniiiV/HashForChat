#include "Chat.h"
#include "iostream"

Chat::Chat() {
    data_count = 0;    
    data = new AuthData[mem_size];
}

Chat::~Chat()
{
    delete[]data;
}

void Chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {    
    uint* hash = sha1(_pass, pass_length);
    add(_login, hash); 
}

void Chat::add(char _login[LOGINLENGTH],  uint* hash) { 

    uint index = -1, i = 0;   
    for (i = 0; i < mem_size; i++) {
        index = hfunc(_login, i * i);
        if ( data[index].status == Status:: free) {           
            break;
        }
    }
    if (i >= mem_size)
    {
        resize(); 
        for (; i < mem_size; i++)
        {
            index = hfunc(_login, i*i);
            if (data[index].status == free)
                break;
        }
    }    
    data[index] = AuthData(_login, hash);
    data[index].status = engaged;
    cout << data[index].login << " was registered successfully" << endl;
    data_count++;
}

bool Chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    
    uint index, i = 0;
    for (; i < mem_size; i++) {
        index = hfunc(_login, i * i);
        if (data[index].status == free){   
            cout << "Error: invalid login or password" << endl;
            return false;
        }
        if (data[index].status == engaged
            && !strcmp(_login, data[index].login))                   
            break;        
    }
    if (i >= data_count){    
        cout << "Error: invalid login or password" << endl;
        return false;
    }
    uint* PAShash = sha1(_pass, pass_length);    
    if (!memcmp(data[index].pass_sha1_hash, PAShash, SHA1HASHLENGTHBYTES))
    {
        cout << "Hello " << data[index].login << "!" << endl;
        delete[] PAShash;
        return true;
    }
    cout << "Error: invalid login or password" << endl;    
    return false;
}

int Chat::hfunc(char* _login, int dif) {
    uint counter = 0;
    uint sum = 0;
    while (counter < strlen(_login))
    {
        sum += (uint)_login[counter];
        counter++;
    }
    const double A = 0.7;   
    return int((SIZE *(A * sum - int(A * sum)) + dif)) %SIZE;    
}

void Chat::resize() {    
    AuthData* temp = data; 
    mem_size *= 2 ;
    data = new AuthData[mem_size];
    data_count = 0;
    for (int i = 0; i < mem_size/2; i++) {        
        if ( temp[i].status == engaged) {
            uint* PASShash = new uint[SHA1HASHLENGTHUINTS];
            memcpy(PASShash, temp[i].pass_sha1_hash, SHA1HASHLENGTHBYTES);
            add(temp[i].login, PASShash);
        }
    }
    delete[] temp;
}
void Chat::deluser(char _login[LOGINLENGTH])
{
    int index = -1, i = 0;    
    for (; i < mem_size; i++)
    {
        index = hfunc(_login, i*i);
        if (data[index].status == engaged &&
            !strcmp(data[index].login, _login)){        
            data[index].status = deleted;
            data_count--;
            cout << "User " << data[index].login << " was deleted successfully" << endl;
            return;
        }
        else if (data[index].status == free)
            return;
    }
}
void Chat::get_data_count()
{
    cout << "Number of users is " << data_count << endl;
}

int main()
{
 Chat ch;
 ch.reg((char*)"Vasia", (char*)"hrthdgfh", strlen("hrthdgfh"));
 ch.reg((char*)"Masha", (char*)"fddgfnh", strlen("fddgfnh"));
 ch.reg((char*)"Misha", (char*)"hghghjj", strlen("hghghjj"));
 ch.reg((char*)"Petia", (char*)"fgjffjfgj", strlen("fgjffjfgj"));
 ch.reg((char*)"Valia", (char*)"fgnfgn", strlen("fgnfgn"));
 ch.reg((char*)"Serge", (char*)"tyru", strlen("tyru"));

 ch.login((char*)"Vasia", (char*)"hrthdgfh", strlen("hrthdgfh"));
 ch.login((char*)"Masha", (char*)"fddgfnh", strlen("fddgfnh"));
 ch.login((char*)"Misha", (char*)"hghghjj", strlen("hghghjj"));
 ch.login((char*)"Petia", (char*)"fgjffjfgj", strlen("fgjffjfgj"));
 ch.login((char*)"Valia", (char*)"fgnfgn", strlen("fgnfgn"));
 ch.login((char*)"Serge", (char*)"tyru", strlen("tyru"));

 ch.deluser((char*)"Vasia");
 ch.deluser((char*)"Masha");
 ch.deluser((char*)"Misha");
 ch.deluser((char*)"Petia");
 ch.deluser((char*)"Valia");
 ch.deluser((char*)"Serge");

 ch.get_data_count();
 
        
  return 0;
}

