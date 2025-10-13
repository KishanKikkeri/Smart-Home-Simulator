#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <limits> 
#include "classappliances.h"
#include "classsensors.h"
#include "classdoors.h"
#include "classvehicles.h"
#include "classmedia.h" 
using namespace std;

// Scheduling structu
// Scheduling structure
struct Schedule
{
    string deviceName;
    int hour;    // 0-23
    int minute;  // 0-59
    bool turnOn; // true=ON, false=OFF
};

// Automation structure
struct AutomationRule
{
    string sensorName;
    string deviceName;
    string condition; // ">" "<" "==" etc.
    int value;
    bool turnOn; // action
};

class DeviceGroup
{
    string groupName;

public:
    vector<Appliance *> appliances;
    vector<Sensors *> sensors;
    vector<Doors *> doors;
    vector<Vehicle *> vehicles;

    DeviceGroup(string name) : groupName(name) {}

    string getName()
    {
        return groupName;
    }

    void addAppliance(Appliance *appliance)
    {
        appliances.push_back(appliance);
    }

    void showStatusAll()
    {
        cout << "\n--- Device Group: " << groupName << " ---\n";
        for (auto a : appliances)
            if (a)
                a->status();
        for (auto s : sensors)
            if (s)
                s->status();
        for (auto d : doors)
            if (d)
                d->status();
        for (auto v : vehicles)
            if (v)
                v->status();
    }

    void turnOnAll()
    {
        for (auto a : appliances)
            if (a)
                a->turnOn();
    }

    void turnOffAll()
    {
        for (auto a : appliances)
            if (a)
                a->turnOff();
    }
};

class SmartHome
{
public:
    vector<Appliance *> appliances;
    vector<Sensors *> sensors;
    vector<Doors *> doors;
    vector<Vehicle *> vehicles;
    vector<DeviceGroup *> groups;

    vector<Schedule> schedules;
    vector<AutomationRule> rules;
    map<string, double> energyTracker;

    void saveToFile(string fname = "smarthome.txt")
    {
        ofstream fout(fname);
        for (auto a : appliances)
            fout << "Appliance " << a->getName() << " " << (a->isOnDevice() ? "ON" : "OFF") << "\n";
        for (auto s : sensors)
            fout << "Sensor " << s->getName() << " " << (s->isActiveSensor() ? "ON" : "OFF") << "\n";
        for (auto d : doors)
            fout << "Door " << d->getName() << " " << (d->isLockedDoor() ? "LOCKED" : "UNLOCKED") << "\n";
        for (auto v : vehicles)
            fout << "Vehicle " << v->getName() << " " << (v->isRunningVehicle() ? "ON" : "OFF") << "\n";
        fout.close();
    }

    void loadFromFile(string fname = "smarthome.txt")
    {
        ifstream fin(fname);
        if (!fin)
            return;
        string line;
        while (getline(fin, line))
        {
            if (line.empty())
                continue;
            istringstream iss(line);
            string type, name, status;
            iss >> type >> name >> status;
            if (type == "Appliance")
            {
                Appliance *a = new Appliance(name);
                if (status == "ON")
                    a->turnOn();
                appliances.push_back(a);
            }
            else if (type == "Sensor")
            {
                Sensors *s = new Sensors(name);
                if (status == "ON")
                    s->activate();
                sensors.push_back(s);
            }
            else if (type == "Door")
            {
                Doors *d = new Doors(name);
                if (status == "UNLOCKED")
                    d->unlock();
                doors.push_back(d);
            }
            else if (type == "Vehicle")
            {
                Vehicle *v = new Vehicle(name);
                if (status == "ON")
                    v->start();
                vehicles.push_back(v);
            }
        }
        fin.close();
    }

    void checkSchedules(int curHour, int curMin)
    {
        for (auto &sch : schedules)
        {
            if (sch.hour == curHour && sch.minute == curMin)
            {
                for (auto a : appliances)
                    if (a->getName() == sch.deviceName)
                    {
                        if (sch.turnOn)
                            a->turnOn();
                        else
                            a->turnOff();
                    }
                for (auto v : vehicles)
                    if (v->getName() == sch.deviceName)
                    {
                        if (sch.turnOn)
                            v->start();
                        else
                            v->stop();
                    }
            }
        }
    }

    void applyAutomation()
    {
        for (auto &rule : rules)
        {
            Sensors *s = nullptr;
            Appliance *a = nullptr;
            for (auto sens : sensors)
                if (sens->getName() == rule.sensorName)
                    s = sens;
            for (auto dev : appliances)
                if (dev->getName() == rule.deviceName)
                    a = dev;
            if (!s || !a)
                continue;
            TemperatureSensor *ts = dynamic_cast<TemperatureSensor *>(s);
            if (ts)
            {
                bool trigger = false;
                if (rule.condition == ">" && ts->getTemperature() > rule.value)
                    trigger = true;
                else if (rule.condition == "<" && ts->getTemperature() < rule.value)
                    trigger = true;
                else if (rule.condition == "== " && ts->getTemperature() == rule.value)
                    trigger = true;
                if (trigger)
                {
                    if (rule.turnOn)
                        a->turnOn();
                    else
                        a->turnOff();
                }
            }
        }
    }

    void updateEnergy()
    {
        for (auto a : appliances)
            if (a->isOnDevice())
                energyTracker[a->getName()] += a->getEnergy();
        for (auto v : vehicles)
            if (v->isRunningVehicle())
                energyTracker[v->getName()] += v->getEnergy();
    }

    void showEnergy()
    {
        cout << "--- Energy Consumption ---\n";
        for (auto &e : energyTracker)
            cout << e.first << ": " << e.second << " kWh\n";
    }
};

void manageDeviceGroups(SmartHome &home)
{
    int choice;
    do
    {
        cout << "\n--- Device Group Management ---\n";
        cout << "1. Create New Group\n";
        cout << "2. Add Device to Group\n";
        cout << "3. Control Group\n";
        cout << "4. Show Group Status\n";
        cout << "5. List All Groups\n";
        cout << "6. Back to Main Menu\n";
        cout << "Choice: ";

        // Input validation
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice)
        {
        // ... (rest of the cases remain the same)
        }
    } while (choice != 6);
}

int main()
{
    srand(time(0));
    SmartHome home;
    home.loadFromFile();

    int choice;
    do
    {
        cout << "\n=== Smart Home Simulator ===\n";
        cout << "1. Add Device\n2. Control Device\n3. Device Groups\n4. Automation Rules\n5. Show Status\n6. Show Energy\n7. Scheduling\n8. Exit\n";
        cout << "9. Combine Energy of Two Devices (operator+)\n";
        cout << "10. Increase Device Energy (operator+=)\nChoice:";

        // Input validation
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice)
        {
        case 1:
        {
            int typeChoice;
            cout << "Select device type:\n1. Appliance\n2. Sensor\n3. Door\n4. Vehicle\n5. Media\nChoice: "; // Added Media
            // Input validation
            while (!(cin >> typeChoice)) {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            string name;
            cout << "Enter a name for the device: ";
            cin >> name;

            if (typeChoice == 1)
            {
                int applianceChoice;
                cout << "Select appliance type:\n1. Light\n2. Fan\n3. Air Conditioner\n4. Washing Machine\n5. Dishwasher\n6. Refrigerator\nChoice: ";
                // Input validation
                while (!(cin >> applianceChoice)) {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                Appliance *a = nullptr;
                switch (applianceChoice)
                {
                // ... (rest of the cases remain the same)
                }
                if (a)
                {
                    a->turnOn();
                    home.appliances.push_back(a);
                }
            }
            else if (typeChoice == 2)
            {
                int sensorChoice;
                cout << "Select sensor type:\n1. Temperature\n2. Motion\n3. Humidity\n4. Rain\n5. Thermostat\nChoice: "; // Added Thermostat
                // Input validation
                while (!(cin >> sensorChoice)) {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                Sensors *s = nullptr;
                switch (sensorChoice)
                {
                // ... (rest of the cases remain the same)
                case 5:
                    s = new Thermostat(name);
                    break;
                }
                if (s)
                {
                    s->activate();
                    home.sensors.push_back(s);
                }
            }
            // ... (rest of the cases for Door and Vehicle remain the same)
            else if (typeChoice == 5) // Added Media
            {
                int mediaChoice;
                cout << "Select media device type:\n1. Smart TV\nChoice: ";
                // Input validation
                while (!(cin >> mediaChoice)) {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                Appliance *a = nullptr;
                switch (mediaChoice)
                {
                case 1:
                    a = new SmartTV(name);
                    break;
                default:
                    cout << "Invalid media device type.\n";
                    break;
                }
                if (a)
                {
                    a->turnOn();
                    home.appliances.push_back(a);
                }
            }

            break;
        }
        // ... (rest of the cases remain the same)
        case 9:
        {
            if (home.appliances.size() < 2)
            {
                cout << "Need at least 2 devices to combine energy.\n";
                break;
            }
            string name1, name2;
            cout << "Enter first device name: ";
            cin >> ws;
            getline(cin, name1);
            cout << "Enter second device name: ";
            getline(cin, name2);

            Appliance *a1 = nullptr;
            Appliance *a2 = nullptr;

            for (auto &a : home.appliances)
            {
                if (a->getName() == name1)
                    a1 = a;
                if (a->getName() == name2)
                    a2 = a;
            }

            if (a1 && a2)
            {
                Appliance combined = *a1 + *a2;
                cout << "\n[Combined Appliance Energy Info]\n";
                combined.status();
            }
            else
            {
                cout << "One or both devices not found.\n";
                cout << "Available appliances:\n";
                for (const auto& a : home.appliances) {
                    cout << "- " << a->getName() << endl;
                }
            }
            break;
        }

        case 10:
        {
            if (home.appliances.empty())
            {
                cout << "No devices to modify.\n";
                break;
            }
            string name;
            double addEnergy;
            cout << "Enter device name to increase energy: ";
            cin >> ws;
            getline(cin, name);
            cout << "Enter amount to add (kWh): ";
            cin >> addEnergy;

            bool found = false;
            for (auto &a : home.appliances)
            {
                if (a->getName() == name)
                {
                    *a += addEnergy;
                    cout << "Updated Energy: " << a->getEnergy() << " kWh\n";
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << "Device not found.\n";
                cout << "Available appliances:\n";
                for (const auto& a : home.appliances) {
                    cout << "- " << a->getName() << endl;
                }
            }
            break;
        }
        }

        // ... (rest of the main function remains the same)

    } while (choice != 8);

    home.saveToFile();
    cout << "Data saved. Exiting...\n";
}