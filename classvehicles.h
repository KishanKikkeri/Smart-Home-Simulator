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

