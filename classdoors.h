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

