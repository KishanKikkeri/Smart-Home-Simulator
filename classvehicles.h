#ifndef CLASSVEHICLES_H
#define CLASSVEHICLES_H

#include <iostream>
#include <string>
using namespace std;

class Vehicle
{
protected:
    string name;
    bool running;
    double energyConsumption;

public:
    Vehicle(string n, double e = 1.0) : name(n), running(false), energyConsumption(e) {}
    virtual ~Vehicle() { cout << "LOG: Vehicle '" << name << "' removed.\n"; }

    void start()
    {
        running = true;
        cout << name << " started.\n";
    }
    void stop()
    {
        running = false;
        cout << name << " stopped.\n";
    }
    void status() { cout << name << " is " << (running ? "Running" : "Stopped") << " | Energy: " << energyConsumption << " kWh\n"; }

    string getName() { return name; }
    bool isRunningVehicle() { return running; }
    double getEnergy() { return energyConsumption; }
};

class Car : public Vehicle
{
public:
    Car(string n, double e = 1.5) : Vehicle(n, e) {}
    void turnOnAC()
    {
        if (running)
            cout << "AC on in " << name << endl;
    }
};

class Bicycle : public Vehicle
{
public:
    Bicycle(string n) : Vehicle(n, 0) {}
    void start()
    {
        running = true;
        cout << "Pedaling " << name << endl;
    }
    void stop()
    {
        running = false;
        cout << "Stopped pedaling " << name << endl;
    }
};

#endif
