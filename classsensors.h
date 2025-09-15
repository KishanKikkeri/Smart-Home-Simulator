#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
using namespace std;

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