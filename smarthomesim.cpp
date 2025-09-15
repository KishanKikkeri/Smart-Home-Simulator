//Smart Home simulator
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
using namespace std;
//including class headers
#include "classappliances.h"
#include "classsensors.h"
#include "classdoors.h"
#include "classvehicles.h"


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
