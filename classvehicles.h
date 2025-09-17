#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
using namespace std;

class Vehicle{
protected:
    string model;
    bool isRunning;
public:
    Vehicle(string m) : model(m), isRunning(false){}

    virtual ~Vehicle(){
        cout << "LOG: " << model << " vehicle has been removed." << endl;
    }

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

class Car : public Vehicle {
public:
    Car(string m) : Vehicle(m) {}
    
    void turnOnAC(){
        if(isRunning){
            cout << "AC has been turned on in the " << model << "." << endl;
        } else {
            cout << model << " is not running. Cannot turn on AC." << endl;
        }
    }
};

class Bicycle; // Forward declaration

class Scooter : public Vehicle {
public:
    friend class Bicycle;
    Scooter(string m) : Vehicle(m) {}
};

class Bicycle : public Vehicle {
public:
    friend class Scooter;
    Bicycle(string m) : Vehicle(m) {}

    void start(){
        isRunning = true;
        cout << "Started pedaling the " << model << "." << endl;
    }

    void stop(){
        isRunning = false;
        cout << "Stopped pedaling the " << model << "." << endl;
    }

    void charge(){
        cout << "The " << model << " is a bicycle and cannot be charged." << endl;
    }
};