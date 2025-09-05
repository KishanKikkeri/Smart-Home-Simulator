//Smart Home simulator
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
using namespace std;

class Appliance{ //defining a base class, that other classes will inherit from.
protected:
    string name;
    bool isOn;
public:
    Appliance(string n) : name(n), isOn(false){} //constructor
    virtual void turnOn(){
        isOn = true;
        cout << name << " is now ON." << endl;
    }
    virtual void turnOff(){
        isOn = false;
        cout << name << " is now OFF." << endl;
    }
    virtual void status(){
        cout << name << " is " << (isOn ? "ON" : "OFF") << endl; //using ternary operator
    }
};

class Sensors{ //defining a second base class for sensors
protected:
    string type;
    bool isActive;
public:
    Sensors(string t) : type(t), isActive(false){} 
    void activate(){
        isActive = true;
        cout << type << " sensor activated." << endl;
    }
    void deactivate(){
        isActive = false;
        cout << type << " sensor deactivated." << endl;
    }
    void status(){
        cout << type << " sensor is " << (isActive ? "active" : "inactive") << endl; 
    }
    void readData(){
        if(isActive){
            cout << "Reading data from " << type << " sensor." << endl;
        } else {
            cout << type << " sensor is inactive. Cannot read data." << endl;
        }
    }
};

class Doors{//defining a third base class for doors
protected:
    string location;
    bool isLocked;
public:
    Doors(string loc) : location(loc), isLocked(true){}
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

class Vehicle{//defining a fourth base class for vehicles
protected:
    string model;
    bool isRunning; 
public:
    Vehicle(string m) : model(m), isRunning(false){}
    void start(){
        isRunning = true;
        cout << model << " vehicle started." << endl;
    }
    void stop(){
        isRunning = false;
        cout << model << " vehicle stopped." << endl;
    }
    void status(){
        cout << model << " vehicle is " << (isRunning ? "running" : "stopped") << endl;
    }
    void drive(){
        if(isRunning){
            cout << "Driving the " << model << " vehicle." << endl;
        } else {
            cout << model << " vehicle is not running. Cannot drive." << endl;
        }
    }
    void park(){
        if(isRunning){
            cout << "Cannot park while the " << model << " vehicle is running." << endl;
        } else {
            cout << "Parking the " << model << " vehicle." << endl;
        }
    }
    void charge(){
        cout << "Charging the " << model << " vehicle." << endl;
    }
};

//defining derived classes for specific appliances
class Light : public Appliance{
public:
    Light(string n) : Appliance(n) {}
    void dim(){
        if(isOn){
            cout << name << " light is dimmed." << endl;
        } else {
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
        } else {
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
        } else {
            cout << name << " air conditioner is OFF. Cannot set temperature." << endl;
        }
    }
    void setMode(string mode){
        if(isOn){
            cout << name << " air conditioner mode set to " << mode << "." << endl;
        } else {
            cout << name << " air conditioner is OFF. Cannot set mode." << endl;
        }
    }
};

class WashingMachine : public Appliance{
public:
    WashingMachine(string n) : Appliance(n) {}
    void startWashCycle(string cycle){
        if(isOn){
            cout << name << " washing machine started " << cycle << " wash cycle." << endl;
        } else {
            cout << name << " washing machine is OFF. Cannot start wash cycle." << endl;
        }
    }
    
};

class Dishwasher : public Appliance{
public:
    Dishwasher(string n) : Appliance(n) {}
    void startDishwashCycle(string cycle){
        if(isOn){
            cout << name << " dishwasher started " << cycle << " dishwash cycle." << endl;
        } else {
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
        } else {
            cout << name << " refrigerator is OFF. Cannot set temperature." << endl;
        }
    }
};

//defining derived classes for specific sensors
