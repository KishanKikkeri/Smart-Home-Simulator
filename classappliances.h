#ifndef CLASSAPPLIANCES_H
#define CLASSAPPLIANCES_H

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
    double energyConsumption;

public:
    Appliance(string n, double energy = 1.0) : name(n), isOn(false), energyConsumption(energy) {}
    virtual ~Appliance() { cout << "LOG: Appliance '" << name << "' removed.\n"; }

    virtual void turnOn(){
        isOn = true;
        cout << name << " is now ON." << endl;
    }
    virtual void turnOff(){
        isOn = false;
        cout << name << " is now OFF." << endl;
    }
    virtual void status(){
        cout << name << " is " << (isOn ? "ON" : "OFF") << " | Energy: " << energyConsumption << " kWh\n";
    }

    string getName(){return name;}
    bool isOnDevice(){return isOn;}
    double getEnergy(){return energyConsumption;}
    void setEnergy(double e){energyConsumption = e;}

     Appliance operator+(const Appliance& other) const{
        Appliance temp("Combined");
        temp.energyConsumption = this->energyConsumption + other.energyConsumption;
        return temp;
    }

    Appliance& operator+=(double energy){
        this->energyConsumption += energy;
        return *this;
    }
};

class Light : public Appliance{
private:
    int brightness;

public:
    Light(string n, double e = 0.1) : Appliance(n, e), brightness(100) {}
    void dim(int b){
        if (isOn){
            brightness = b;
            cout << name << " brightness set to " << b << "%\n";
        }
        else cout<<name <<" light is OFF. Cannot dim."<<endl;
    }
};

class Fan : public Appliance{
private:
    int speed;

public:
    Fan(string n, double e = 0.2) : Appliance(n, e), speed(0) {}
    void setSpeed(int sp){
        if (isOn){
            this->speed = sp;
            cout << name << " speed set to " << sp << endl;
        }
        else cout << name << " fan is OFF. Cannot set speed." << endl;
    }
};

class AirConditioner : public Appliance{
private:
    int temperature;
    string mode;

public:
    AirConditioner(string n, double e = 1.5) : Appliance(n, e), temperature(25), mode("Cool") {}
    void setTemperature(int temp){
        if (isOn){
            temperature = temp;
            cout << name << " air conditioner temperature set to " << temp << " degrees." << endl;
        }
        else cout << name << " air conditioner is OFF. Cannot set temperature." << endl;
    }
    void setMode(string mode)
    {
        if (isOn){
            this->mode = mode;
            cout << name << " air conditioner mode set to " << mode << "." << endl;
        }
        else cout << name << " air conditioner is OFF. Cannot set mode." << endl;
    }
    int getTemperature() {return temperature; }
};

class WashingMachine : public Appliance
{
public:
    friend class Dishwasher; // declaring friend class Dishwasher to access private members of WashingMachine
    WashingMachine(string n, double e = 0.8) : Appliance(n, e) {}
    void startWashCycle(string cycle)
    {
        if (isOn)
        {
            cout << name << " washing machine started " << cycle << " wash cycle." << endl;
        }
        else
        {
            cout << name << " washing machine is OFF. Cannot start wash cycle." << endl;
        }
    }
};

// declaring friend class Dishwasher to access private members of WashingMachine
class Dishwasher : public Appliance
{
public:
    Dishwasher(string n, double e = 0.7) : Appliance(n, e) {}
    void startDishwashCycle(string cycle)
    {
        if (isOn)
        {
            cout << name << " dishwasher started " << cycle << " dishwash cycle." << endl;
        }
        else
        {
            cout << name << " dishwasher is OFF. Cannot start dishwash cycle." << endl;
        }
    }
};

class Refrigerator : public Appliance
{
private:
    int temperature;

public:
    Refrigerator(string n, double e = 1.2) : Appliance(n, e), temperature(5) {}
    void setTemperature(int temp)
    {
        if (isOn)
        {
            temperature = temp;
            cout << name << " refrigerator temperature set to " << temp << " degrees." << endl;
        }
        else
        {
            cout << name << " refrigerator is OFF. Cannot set temperature." << endl;
        }
    }
};

#endif // CLASSAPPLIANCES_H