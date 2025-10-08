#ifndef CLASSSENSORS_H
#define CLASSSENSORS_H

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
using namespace std;

class Sensors
{
protected:
    string type;
    bool isActive;

public:
    Sensors(string t) : type(t), isActive(false) {}

    virtual ~Sensors()
    {
        cout << "LOG: " << type << " sensor has been removed." << endl;
    }

    void activate()
    {
        isActive = true;
        cout << type << " sensor activated." << endl;
    }
    void deactivate()
    {
        isActive = false;
        cout << type << " sensor deactivated." << endl;
    }
    void status()
    {
        cout << type << " sensor is " << (isActive ? "active" : "inactive") << endl;
    }

    string getName()
    {
        return type;
    }
    bool isActiveSensor()
    {
        return isActive;
    }
};

class TemperatureSensor : public Sensors
{
private:
    int temperature;

public:
    TemperatureSensor(string n = "TempSensor") : Sensors(n), temperature(25) {}
    void setTemperature(int t) { temperature = t; }
    int getTemperature() { return temperature; }
    void randomTemperature() { temperature = 15 + rand() % 20; } // simulate
};

class MotionSensor : public Sensors
{
private:
    bool motionDetected;

public:
    MotionSensor(string n = "MotionSensor") : Sensors(n), motionDetected(false) {}
    void detectMotion()
    {
        if (isActive)
        {
            motionDetected = rand() % 2;
            cout << type << (motionDetected ? " detected motion!\n" : " no motion.\n");
        }
        else
        {
            cout << type << " sensor is inactive. Cannot detect motion." << endl;
        }
    }
    bool isMotionDetected() { return motionDetected; }
};

class HumiditySensor : public Sensors
{
private:
    int humidity;

public:
    HumiditySensor(string n = "HumiditySensor") : Sensors(n), humidity(50) {}
    void setHumidity(int hum)
    {
        humidity = hum;
        cout << "Humidity set to " << humidity << "%" << endl;
    }

    int getHumidity() { return humidity; }
    void randomHumidity() { humidity = 30 + rand() % 40; }
};

class rainSensor : public Sensors
{
private:
    bool isRaining;

public:
    rainSensor() : Sensors("Rain"), isRaining(false) {}
    void detectRain()
    {
        if (isActive)
        {
            isRaining = true;
            cout << "Rain detected!" << endl;
        }
        else
        {
            cout << type << " sensor is inactive. Cannot detect rain." << endl;
        }
    }
    bool isRaining() { return isRaining; }
    void setRainStatus(bool status)
    {
        isRaining = status;
        cout << "Rain status set to: " << (isRaining ? "Raining" : "Not Raining") << endl;
    }
};

#endif // CLASSSENSORS_H