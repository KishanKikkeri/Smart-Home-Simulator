//Smart Home simulator
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

class Sensors{ 
protected:
    string type;
    bool isActive;
public:
    Sensors(string t) : type(t), isActive(false){}

    virtual ~Sensors(){
        cout << "LOG: " << type << " sensor has been removed." << endl;
    }

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
class dishwasher : public Appliance{
public:
    dishwasher(string n) : Appliance(n) {}
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

class TemperatureSensor : public Sensors{
private:
    int temperature = 35;
public:
    TemperatureSensor() : Sensors("Temperature") {}
    void readData(){
        if(isActive){
            cout << "Current temperature: " << temperature << " degrees." << endl;
        }
        else {
            cout << type << " sensor is inactive. Cannot read data." << endl;
        }
    }

    void setTemperature(int temp){
        temperature = temp;
        cout << "Temperature set to " << temperature << " degrees." << endl;
    }

};

class MotionSensor : public Sensors{
public:
    MotionSensor() : Sensors("Motion") {}
    void detectMotion(){
        if(isActive){
            cout << "Motion detected!" << endl;
        }
        else {
            cout << type << " sensor is inactive. Cannot detect motion." << endl;
        }
    }

};

class HumiditySensor : public Sensors{
private:
    int humidity = 50;  //default humidity
public:
    HumiditySensor() : Sensors("Humidity") {}
    void readData(){
        if(isActive){
            cout << "Current humidity: " << humidity << "%" << endl;
        }
        else {
            cout << type << " sensor is inactive. Cannot read data." << endl;
        }
    }

    void setHumidity(int hum){
        humidity = hum;
        cout << "Humidity set to " << humidity << "%" << endl;
    }

};

class rainSensor : public Sensors{
private:
    bool isRaining;
public:
    rainSensor() : Sensors("Rain"), isRaining(false) {}
    void detectRain(){
        if(isActive){
            isRaining = true;
            cout << "Rain detected!" << endl;
        }
        else {
            cout << type << " sensor is inactive. Cannot detect rain." << endl;
        }
    }
    void setRainStatus(bool status){
        isRaining = status;
        cout << "Rain status set to: " << (isRaining ? "Raining" : "Not Raining") << endl;
    }
};

int main(){
    int choice, subchoice, temp, speed, cycle;
    string mode;
    cout << "Welcome to the Smart Home Simulator!" << endl;
    cout << "Select an option to simulate:" << endl;
    cout << "1. Add Devices" << endl;
    cout << "2. Control Devices" << endl;
    cout << "3. Remove Devices" << endl;
    cout << "4. Exit" << endl;
    cin >> choice;
    switch(choice){
        case 1:{
            cout << "Select device type to add:" << endl;
            cout << "1. Appliance" << endl;
            cout << "2. Sensor" << endl;
            cout << "3. Door" << endl;
            cout << "4. Vehicle" << endl;
            cin >> subchoice;
            switch(subchoice){
                case 1:{
                    cout << "Select appliance type to add:" << endl;
                    cout << "1. Light" << endl;
                    cout << "2. Fan" << endl;
                    cout << "3. Air Conditioner" << endl;
                    cout << "4. Washing Machine" << endl;
                    cout << "5. Dishwasher" << endl;
                    cout << "6. Refrigerator" << endl;
                    cin >> subchoice;
                    string name;
                    cout << "Enter appliance name: ";
                    cin >> name;
                    Appliance* appliance = nullptr;
                    switch(subchoice){
                        case 1:
                            appliance = new Light(name);
                            break;
                        case 2:
                            appliance = new Fan(name);
                            break;
                        case 3:
                            appliance = new AirConditioner(name);
                            break;
                        case 4:
                            appliance = new WashingMachine(name);
                            break;
                        case 5:
                            appliance = new Dishwasher(name);
                            break;
                        case 6:
                            appliance = new Refrigerator(name);
                            break;
                        default:
                            cout << "Invalid choice." << endl;
                            return 0;
                    }
                    appliance->turnOn();
                    appliance->status();
                    delete appliance; // only change: delete added to match unique_ptr scope
                    break;
                }
                
                case 2:{
                    cout << "Select sensor type to add:" << endl;
                    cout << "1. Temperature Sensor" << endl;
                    cout << "2. Motion Sensor" << endl;
                    cout << "3. Humidity Sensor" << endl;
                    cout << "4. Rain Sensor" << endl;
                    cin >> subchoice;
                    Sensors* sensor = nullptr;
                    switch(subchoice){
                        case 1:
                            sensor = new TemperatureSensor();
                            break;
                        case 2:
                            sensor = new MotionSensor();
                            break;
                        case 3:
                            sensor = new HumiditySensor();
                            break;
                        case 4:
                            sensor = new rainSensor();
                            break;
                        default:
                            cout << "Invalid choice." << endl;
                    }
                    if(sensor){
                        sensor->activate(); 
                        sensor->status();
                        delete sensor; 
                    }
                }
                
            }
        }
        case 2:{
            cout << "Control Devices feature is under development." << endl;
            break;
        }

        case 3:{
            cout << "Remove Devices feature is under development." << endl;
            break;
        }

        case 4:{
            cout << "Exiting simulator." << endl;
            return 0;
        }
    }
    cout << "\nSimulator session ended." << endl;
    return 0;
}
