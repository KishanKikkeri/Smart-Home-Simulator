#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
using namespace std;

class Doors{
protected:
    string location;
    bool isLocked;
public:
    Doors(string loc) : location(loc), isLocked(true){}

    virtual ~Doors(){
        cout << "LOG: " << location << " door has been removed." << endl;
    }

    void lock(){
        isLocked = true;
        cout << location << " door is now LOCKED." << endl;
    }
    void unlock(){
        isLocked = false;
        cout << location << " door is now UNLOCKED." << endl;
    }
    void status(){
        cout << location << " door is " << (isLocked ? "LOCKED" : "UNLOCKED") << endl;
    }
};

class SmartDoor : public Doors {
private:
    string accessCode;
public:
    SmartDoor(string loc, string code) : Doors(loc), accessCode(code) {}

    void unlock(string codeAttempt){
        if (codeAttempt == accessCode) {
            isLocked = false;
            cout << location << " door is now UNLOCKED." << endl;
        } else {
            cout << "Incorrect access code for " << location << " door." << endl;
        }
    }
};

class GarageDoor : public Doors {
public:
    GarageDoor(string loc) : Doors(loc) {}

    void open(){
        unlock();
        cout << location << " garage door is OPENING." << endl;
    }
    void close(){
        lock();
        cout << location << " garage door is CLOSING." << endl;
    }
};

class LockerDoor : public Doors {
private:
    string securityKey;
public:
    LockerDoor(string loc, string key) : Doors(loc), securityKey(key) {}

    void unlock(){
        cout << "Extra security required for " << location << " door. Cannot unlock directly." << endl;
    }

    void unlockWithKey(string keyAttempt){
        if (keyAttempt == securityKey) {
            isLocked = false;
            cout << location << " locker is now UNLOCKED." << endl;
        } else {
            cout << "Invalid security key for " << location << " locker." << endl;
        }
    }
};