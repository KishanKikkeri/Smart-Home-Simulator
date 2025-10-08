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

    while (true) {
        cout << "\nSelect an option to simulate:" << endl;
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
                                continue;
                        }
                        appliance->turnOn();
                        appliance->status();
                        //delete appliance; 
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
                            //sensor->status();
                            delete sensor; 
                        }
                        break;
                    }

                    case 3:{
                        cout << "Select door type to add:" << endl;
                        cout << "1. Front Door" << endl;
                        cout << "2. Garage Door" << endl;
                        cout << "3. Back Door" << endl;
                        cin >> subchoice;
                        string location;
                        cout << "Enter door location: ";
                        cin >> location;
                        Doors* door = nullptr;
                        switch(subchoice){
                            case 1:
                                door = new SmartDoor(location, "1234");
                                break;
                            case 2:
                                door = new GarageDoor(location);
                                break;
                            case 3:
                                door = new LockerDoor(location, "1234");
                                break;
                            default:
                                cout << "Invalid choice." << endl;
                                continue;
                        }
                        door->lock();
                        door->status();
                        //delete door; 
                        break;
                    }

                    case 4:{
                        cout << "Select vehicle type to add:" << endl;
                        cout << "1. Car" << endl;
                        cout << "2. Bike" << endl;
                        cout << "3. Bicycle" << endl;
                        cin >> subchoice;
                        string model;
                        cout << "Enter vehicle model: ";
                        cin >> model;
                        Vehicle* vehicle = nullptr;
                        switch(subchoice){
                            case 1:
                                vehicle = new Car(model);
                                break;
                            case 2:
                                vehicle = new Scooter(model);
                                break;
                            case 3:
                                vehicle = new Bicycle(model);
                                break;
                            default:
                                cout << "Invalid choice." << endl;
                                continue;
                        }
                        vehicle->start();
                        vehicle->status();
                        delete vehicle; 
                        break;
                    }
                }
                break;
            }

            case 2:{
                cout << "Select device type to control:" << endl;
                cout << "1. Appliance" << endl;
                cout << "2. Sensor" << endl;
                cout << "3. Door" << endl;
                cout << "4. Vehicle" << endl;
                cin >> subchoice;
                switch(subchoice){
                    case 1:
                        cout << "Select Appliance." << endl;
                        cout << "1. Light" << endl;
                        cout << "2. Fan" << endl;
                        cout << "3. Air Conditioner" << endl;
                        cout << "4. Washing Machine" << endl;
                        cout << "5. Dishwasher" << endl;
                        cout << "6. Refrigerator" << endl;
                        cin >> subchoice;
                        switch(subchoice){
                            case 1:
                                cout << "Select Operation"<< endl;
                                cout << "1. Turn On" << endl;
                                cout << "2. Turn Off" << endl;
                                cin >> subchoice;
                                if(subchoice == 1){
                                    cout << "Light turned ON." << endl;
                                } 
                                else if(subchoice == 2){
                                    cout << "Light turned OFF." << endl;
                                } 
                                else {
                                    cout << "Invalid choice." << endl;
                                }
                                break;
                            case 2:
                                cout << "Select Operation" << endl;
                                cout << "1. Turn On" << endl;
                                cout << "2. Turn Off" << endl;
                                cin >> subchoice;
                                if(subchoice == 1){
                                    cout << "Fan turned ON." << endl;
                                } 
                                else if(subchoice == 2){
                                    cout << "Fan turned OFF." << endl;
                                } 
                                else {
                                    cout << "Invalid choice." << endl;
                                }
                                break;
                            case 3:
                                cout << "Select Operation" << endl;
                                cout << "1. Turn On" << endl;
                                cout << "2. Turn Off" << endl;
                                cin >> subchoice;
                                if(subchoice == 1){
                                    cout << "Air Conditioner turned ON." << endl;
                                } 
                                else if(subchoice == 2){
                                    cout << "Air Conditioner turned OFF." << endl;
                                } 
                                else {
                                    cout << "Invalid choice." << endl;
                                }
                                break;
                            case 4:
                                cout << "Washing Machine control not implemented in this demo." << endl;
                                break;
                            case 5:
                                cout << "Dishwasher control not implemented in this demo." << endl;
                                break;
                            case 6:
                                cout << "Refrigerator control not implemented in this demo." << endl;
                                break;
                            default:
                                cout << "Invalid choice." << endl;
                        }
                        break;
                    case 2:
                        cout << "Select Sensor." << endl;
                        cout << "1. Temperature Sensor" << endl;
                        cout << "2. Motion Sensor" << endl;
                        cout << "3. Humidity Sensor" << endl;
                        cout << "4. Rain Sensor" << endl;
                        cin >> subchoice;
                        switch(subchoice){
                            case 1:
                                cout << "Temperature Sensor control not implemented in this demo." << endl;
                                break;
                            case 2:
                                cout << "Motion Sensor control not implemented in this demo." << endl;
                                break;
                            case 3:
                                cout << "Humidity Sensor control not implemented in this demo." << endl;
                                break;
                            case 4:
                                cout << "Rain Sensor control not implemented in this demo." << endl;
                                break;
                            default:
                                cout << "Invalid choice." << endl;
                        }
                        break;
                    case 3:
                        cout << "Select Door" << endl;
                        cout << "1. Front Door" << endl;
                        cout << "2. Garage Door" << endl;
                        cout << "3. Back Door" << endl;
                        cin >> subchoice;
                        switch(subchoice){
                            case 1:
                                cout << "Front Door control not implemented in this demo." << endl;
                                break;
                            case 2:
                                cout << "Garage Door control not implemented in this demo." << endl;
                                break;
                            case 3:
                                cout << "Back Door control not implemented in this demo." << endl;
                                break;
                            default:
                                cout << "Invalid choice." << endl;
                        }
                        break;
                    case 4:
                        cout << "Select Vehicle" << endl;
                        cout << "1. Car" << endl;
                        cout << "2. Bike" << endl;
                        cout << "3. Bicycle" << endl;
                        cin >> subchoice;
                        switch(subchoice){
                            case 1:
                                cout << "Car control not implemented in this demo." << endl;
                                break;
                            case 2:
                                cout << "Bike control not implemented in this demo." << endl;
                                break;
                            case 3:
                                cout << "Bicycle control not implemented in this demo." << endl;
                                break;
                            default:
                                cout << "Invalid choice." << endl;
                        }
                        break;
                    default:
                        cout << "Invalid choice." << endl;
                }
                break;
            }

            case 3:{
                cout << "select device type to remove:" << endl;
                cout << "1. Appliance" << endl;
                cout << "2. Sensor" << endl;
                cout << "3. Door" << endl;
                cout << "4. Vehicle" << endl;
                cin >> subchoice;
                switch(subchoice){
                    case 1:
                        cout << "Appliance removed." << endl; 
                        break;
                    case 2:
                        cout << "Sensor removed." << endl;
                        break;
                    case 3:
                        cout << "Door removed." << endl;
                        break;
                    case 4:
                        cout << "Vehicle removed." << endl;
                        break;
                    default:
                        cout << "Invalid choice." << endl;
                }
                break;
            }

            case 4:{
                cout << "Exiting simulator." << endl;
                return 0;
            }

            default:
                cout << "Invalid choice." << endl;
        }
    }

    return 0;
}
