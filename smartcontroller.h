#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <limits>
#include <stdexcept>

#include "classappliances.h"
#include "classsensors.h"
#include "classdoors.h"
#include "classvehicles.h"
#include "classmedia.h"

//Scheduling structure
struct Schedule{
    string deviceName;
    int hour;
    int minute;
    bool turnOn;
};

// Automation structure
struct AutomationRule{
    string sensorName;
    string deviceName;
    string condition;
    int value;
    bool turnOn;
};

// DeviceGroup class
class DeviceGroup{
    string groupName;
public:
    vector<Appliance*>appliances;
    vector<Sensors*>sensors;
    vector<Doors*>doors;
    vector<Vehicle*>vehicles;

    DeviceGroup(string name):groupName(name){}

    string getName(){return groupName;}

    void addAppliance(Appliance *appliance){appliances.push_back(appliance);}

    void showStatusAll(){
        cout<<"\n--- Device Group: "<<groupName<<" ---\n";
        for(int i=0;i<(int)appliances.size();i++)
            if(appliances[i])
                appliances[i]->status();
        for(int i=0;i<(int)sensors.size();i++)
            if(sensors[i])
                sensors[i]->status();
        for(int i=0;i<(int)doors.size();i++)
            if(doors[i])
                doors[i]->status();
        for(int i=0;i<(int)vehicles.size();i++)
            if(vehicles[i])
                vehicles[i]->status();
    }

    void turnOnAll(){
        for(int i=0;i<(int)appliances.size();i++)
            if(appliances[i])
            appliances[i]->turnOn();
    }

    void turnOffAll(){
        for(int i=0;i<(int)appliances.size();i++)
            if(appliances[i])
                appliances[i]->turnOff();
    }
};

// SmartHome class
class SmartHome{
public:
    vector<Appliance*>appliances;
    vector<Sensors*>sensors;
    vector<Doors*>doors;
    vector<Vehicle*>vehicles;
    vector<DeviceGroup*>groups;

    vector<Schedule>schedules;
    vector<AutomationRule>rules;
    map<string,double>energyTracker;

    void saveToFile(string fname ="smarthome.txt"){
        ofstream fout(fname);
        for(int i= 0;i<(int)appliances.size();i++)
            fout<<"Appliance "<<appliances[i]->getName()<<" "<<(appliances[i]->isOnDevice() ? "ON":"OFF")<<"\n";
        for(int i=0;i<(int)sensors.size();i++)
            fout<<"Sensor "<<sensors[i]->getName()<<" "<<(sensors[i]->isActiveSensor() ? "ON":"OFF")<<"\n";
        for(int i=0;i<(int)doors.size();i++)
            fout<<"Door "<<doors[i]->getName()<<" "<<(doors[i]->isLockedDoor() ?"LOCKED":"UNLOCKED")<<"\n";
        for(int i=0;i<(int)vehicles.size();i++)
            fout<<"Vehicle "<<vehicles[i]->getName()<<" "<<(vehicles[i]->isRunningVehicle() ? "ON":"OFF")<<"\n";
        fout.close();
    }

    void loadFromFile(string fname = "smarthome.txt"){
        ifstream fin(fname);
        if(!fin)
            return;
        string line;
        while(getline(fin,line)){
            if(line.empty())
                continue;
            istringstream iss(line);
            string type,name,status;
            iss>>type>>name>>status;

            if(type == "Appliance"){
                Appliance*a=new Appliance(name);
                if(status=="ON")
                    a->turnOn();
                appliances.push_back(a);
            }
            else if (type == "Sensor"){
                Sensors*s=new Sensors(name);
                if(status=="ON")
                    s->activate();
                sensors.push_back(s);
            }
            else if (type == "Door"){
                Doors*d=new Doors(name);
                if(status=="UNLOCKED")
                    d->unlock();
                doors.push_back(d);
            }
            else if(type=="Vehicle"){
                Vehicle*v=new Vehicle(name);
                if(status=="ON")
                    v->start();
                vehicles.push_back(v);
            }
        }
        fin.close();
    }

    void checkSchedules(int curHour,int curMin){
        for(int i=0;i<(int)schedules.size();i++){
            Schedule &sch=schedules[i];
            if(sch.hour==curHour&&sch.minute==curMin)
            {
                for(int j=0;j<(int)appliances.size();j++){
                    if(appliances[j]->getName()==sch.deviceName){
                        if(sch.turnOn)
                            appliances[j]->turnOn();
                        else
                            appliances[j]->turnOff();
                    }
                }
                for(int j=0;j<(int)vehicles.size();j++){
                    if(vehicles[j]->getName() == sch.deviceName){
                        if(sch.turnOn)
                            vehicles[j]->start();
                        else
                            vehicles[j]->stop();
                    }
                }
            }
        }
    }

    void applyAutomation(){
        for(int i=0;i<(int)rules.size();i++){
            AutomationRule&rule=rules[i];
            Sensors*s=nullptr;
            Appliance*a=nullptr;

            for(int j=0;j<(int)sensors.size();j++)
                if(sensors[j]->getName()==rule.sensorName)
                    s=sensors[j];

            for(int j=0;j<(int)appliances.size();j++)
                if(appliances[j]->getName()==rule.deviceName)
                    a=appliances[j];

            if(!s || !a)
                continue;

            TemperatureSensor*ts=dynamic_cast<TemperatureSensor*>(s);
            if(ts){
                bool trigger=false;
                if(rule.condition==">"&&ts->getTemperature()>rule.value)
                    trigger=true;
                else if(rule.condition=="<"&& ts->getTemperature()<rule.value)
                    trigger=true;
                else if(rule.condition=="=="&&ts->getTemperature()==rule.value)
                    trigger=true;

                if (trigger){
                    if(rule.turnOn)
                        a->turnOn();
                    else
                        a->turnOff();
                }
            }
        }
    }

    void updateEnergy(){
        for(int i=0; i<(int)appliances.size();i++)
            if(appliances[i]->isOnDevice())
                energyTracker[appliances[i]->getName()]+=appliances[i]->getEnergy();

        for(int i=0;i<(int)vehicles.size();i++)
            if(vehicles[i]->isRunningVehicle())
                energyTracker[vehicles[i]->getName()]+=vehicles[i]->getEnergy();
    }
    void showEnergy(){
        cout<<"---Energy Consumption ---\n";
        for(map<string,double>::iterator it=energyTracker.begin(); it !=energyTracker.end();++it){
            cout<<it->first<<": "<< it->second<<"kWh\n";
        }
    }
};
// ---------- Device Group Management ----------
void manageDeviceGroups(SmartHome &home){
    int choice;
    do{
        cout<<"\n--- Device Group Management ---\n";
        cout<<"1. Create New Group\n";
        cout<<"2. Add Device to Group\n";
        cout<<"3. Control Group\n";
        cout<<"4. Show Group Status\n";
        cout<<"5. List All Groups\n";
        cout<<"6. Back to Main Menu\n";
        cout<<"Choice: ";

        while(!(cin>>choice)){
            cout<<"Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }
        switch (choice){
        case 1:{
            string gname;
            cout<<"Enter group name: ";
            cin>>gname;
            home.groups.push_back(new DeviceGroup(gname));
            cout<<"Group '"<<gname<<"'created.\n";
            break;
        }
        case 2:{
            if(home.groups.empty()){
                cout<<"No groups available. Please create a group first.\n";
                break;
            }
            cout<<"Select a group to add a device to:\n";
            for(int i=0;i<(int)home.groups.size();i++)
                cout<<i + 1<<". "<< home.groups[i]->getName()<< endl;

            int groupChoice;
            cin>>groupChoice;
            if(groupChoice<= 0||groupChoice>(int)home.groups.size()){
                cout<<"Invalid selection.\n";
                break;
            }

            if(home.appliances.empty()){
                cout<<"No appliances available to add.\n";
                break;
            }

            DeviceGroup*selectedGroup=home.groups[groupChoice-1];
            cout<<"Select a device to add:\n";
            for(int i=0;i<(int)home.appliances.size();i++)
                cout<<i + 1<<". "<<home.appliances[i]->getName()<< endl;

            int deviceChoice;
            cin>>deviceChoice;
            if(deviceChoice>0 &&deviceChoice<=(int)home.appliances.size()){
                selectedGroup->addAppliance(home.appliances[deviceChoice-1]);
                cout<<"Device added.\n";
            }
            else
                cout<<"Invalid device selection.\n";
            break;
        }
        case 3:{
            if(home.groups.empty()){
                cout<<"No groups available.\n";
                break;
            }
            cout<<"Select a group to control:\n";
            for(int i = 0;i<(int)home.groups.size();i++)
                cout<<i + 1<<". "<<home.groups[i]->getName()<< endl;

            int groupChoice;
            cin>>groupChoice;
            if(groupChoice<=0||groupChoice>(int)home.groups.size()){
                cout<<"Invalid selection.\n";
                break;
            }

            DeviceGroup*selectedGroup=home.groups[groupChoice - 1];
            cout<<"1. Turn On All\n2. Turn Off All\n3. Control Individual Device\nChoice: ";
            int action;
            cin>>action;
            if(action==1)
                selectedGroup->turnOnAll();
            else if(action==2)
                selectedGroup->turnOffAll();
            else if(action==3){
                if (selectedGroup->appliances.empty()){
                    cout<<"No appliances in this group.\n";
                    break;
                }
                cout<<"Select a device:\n";
                for(int i =0;i<(int)selectedGroup->appliances.size();i++)
                    cout<<i + 1<<". "<<selectedGroup->appliances[i]->getName()<<endl;
                int devChoice;
                cin>>devChoice;
                if(devChoice<=0||devChoice>(int)selectedGroup->appliances.size()){
                    cout<<"Invalid device selection.\n";
                    break;
                }
                Appliance*app=selectedGroup->appliances[devChoice-1];
                cout<<"1. Turn On\n2. Turn Off\nChoice:";
                int act2;
                cin>>act2;
                if(act2==1)
                    app->turnOn();
                else if(act2==2)
                    app->turnOff();
                else
                    cout<<"Invalid action.\n";
            }
            else
                cout<<"Invalid action.\n";
            break;
        }
        case 4:{
            if(home.groups.empty()){
                cout<<"No groups.\n";
                break;
            }
            cout<<"Select a group to show status:\n";
            for(int i=0;i<(int)home.groups.size();i++)
                cout<<i + 1 <<". "<<home.groups[i]->getName()<<endl;

            int groupChoice;
            cin>>groupChoice;
            if(groupChoice>0&&groupChoice<=(int)home.groups.size())
                home.groups[groupChoice-1]->showStatusAll();
            else
                cout<<"Invalid selection.\n";
            break;
        }
        case 5:{
            if(home.groups.empty())
                cout<<"No groups created.\n";
            else{
                cout<<"--- All Groups ---\n";
                for(int i=0;i<(int)home.groups.size();i++)
                    cout<<"- "<< home.groups[i]->getName()<<endl;
            }
            break;
        }
        }
    }while(choice!=6);
}