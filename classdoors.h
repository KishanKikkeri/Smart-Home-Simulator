#ifndef CLASSDOORS_H
#define CLASSDOORS_H

#include <iostream>
#include <string>
using namespace std;

class Doors{
protected:
    string name;
    bool locked;

public:
    Doors(string n) : name(n), locked(true) {}
    virtual ~Doors(){cout<<"LOG: Door'"<<name<<"' removed.\n";}
    void lock(){locked = true; cout<<name<<" locked.\n";}
    void unlock(){locked = false; cout<<name<<" unlocked.\n";}
    void status(){cout<<name<<" is "<< (locked ? "Locked" : "Unlocked")<<endl;}
    string getName(){return name;}
    bool isLockedDoor(){return locked;}
};

class SmartDoor : public Doors{
private:
    string code;

public:
    SmartDoor(string n, string c = "0000") : Doors(n), code(c) {}
    void unlock(string c){
        if(c==code){locked = false; cout<<name<< " unlocked.\n";}
        else cout << "Wrong code.\n";}
};

class GarageDoor : public Doors{
public:
    GarageDoor(string n) : Doors(n) {}
    void open(){unlock(); cout<<name<< " opening.\n";}
    void close(){lock(); cout<<name<<" closing.\n";}
};

#endif