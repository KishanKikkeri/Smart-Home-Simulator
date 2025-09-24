#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
using namespace std;

class Appliance{ 
protected:
    string name;
    bool isOn;
public:
    Appliance(string n) : name(n), isOn(false){} 

    virtual ~Appliance(){
        cout << "LOG: Appliance '" << name << "' has been removed." << endl;
    }

    virtual void turnOn(){
        isOn = true;
        cout << name << " is now ON." << endl;
    }
    virtual void turnOff(){
        isOn = false;
        cout << name << " is now OFF." << endl;
    }
    virtual void status(){
        cout << name << " is " << (isOn ? "ON" : "OFF") << endl; 
    }
};


class Light : public Appliance{
public:
    Light(string n) : Appliance(n) {}
    void dim(){
        if(isOn){
            cout << name << " light is dimmed." << endl;
        }
        else {
            cout << name << " light is OFF. Cannot dim." << endl;
        }
    }
};

class Fan : public Appliance{
public:
    Fan(string n) : Appliance(n) {}
    void setSpeed(int speed){
        if(isOn){
            cout << name << " fan speed set to " << speed << "." << endl;
        }
        else {
            cout << name << " fan is OFF. Cannot set speed." << endl;
        }
    }
};

class AirConditioner : public Appliance{
public:
    AirConditioner(string n) : Appliance(n) {}
    void setTemperature(int temp){
        if(isOn){
            cout << name << " air conditioner temperature set to " << temp << " degrees." << endl;
        }
        else {
            cout << name << " air conditioner is OFF. Cannot set temperature." << endl;
        }
    }
    void setMode(string mode){
        if(isOn){
            cout << name << " air conditioner mode set to " << mode << "." << endl;
        }
        else {
            cout << name << " air conditioner is OFF. Cannot set mode." << endl;
        }
    }
};

class WashingMachine : public Appliance{
public:
    friend class Dishwasher; //declaring friend class Dishwasher to access private members of WashingMachine
    WashingMachine(string n) : Appliance(n) {}
    void startWashCycle(string cycle){
        if(isOn){
            cout << name << " washing machine started " << cycle << " wash cycle." << endl;
        }
        else {
            cout << name << " washing machine is OFF. Cannot start wash cycle." << endl;
        }
    }

};

//declaring friend class Dishwasher to access private members of WashingMachine
class Dishwasher : public Appliance{
public:
    Dishwasher(string n) : Appliance(n) {}
    void startDishwashCycle(string cycle){
        if(isOn){
            cout << name << " dishwasher started " << cycle << " dishwash cycle." << endl;
        }
        else {
            cout << name << " dishwasher is OFF. Cannot start dishwash cycle." << endl;
        }
    }
};

class Refrigerator : public Appliance{
public:
    Refrigerator(string n) : Appliance(n) {}
    void setTemperature(int temp){
        if(isOn){
            cout << name << " refrigerator temperature set to " << temp << " degrees." << endl;
        }
        else {
            cout << name << " refrigerator is OFF. Cannot set temperature." << endl;
        }
    }
};

