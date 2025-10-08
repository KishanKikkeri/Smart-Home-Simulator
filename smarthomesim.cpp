#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <chrono>
#include <thread>
#include "classappliances.h"
#include "classsensors.h"
#include "classdoors.h"
#include "classvehicles.h"
using namespace std;

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

int main()
{
    srand(time(0));
    SmartHome home;
    home.loadFromFile();

    int choice;
    do
    {
        cout << "\n=== Smart Home Simulator ===\n";
        cout << "1. Add Device\n2. Control Device\n3. Device Groups\n4. Automation Rules\n5. Show Status\n6. Show Energy\n7. Scheduling\n8. Exit\nChoice: ";
        // Add new menu option for operator overloading demo
        cout << "9. Combine Energy of Two Devices (operator+)\n";
        cout << "10. Increase Device Energy (operator+=)\n";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int t;
            cout << "1.Appliance 2.Sensor 3.Door 4.Vehicle: ";
            cin >> t;
            string name;
            cout << "Enter name: ";
            cin >> name;
            if (t == 1)
            {
                Appliance *a = new Appliance(name);
                a->turnOn();
                home.appliances.push_back(a);
            }
            else if (t == 2)
            {
                Sensors *s = new Sensors(name);
                s->activate();
                home.sensors.push_back(s);
            }
            else if (t == 3)
            {
                Doors *d = new Doors(name);
                d->unlock();
                home.doors.push_back(d);
            }
            else if (t == 4)
            {
                Vehicle *v = new Vehicle(name);
                v->start();
                home.vehicles.push_back(v);
            }
            break;
        }
        case 2:
            cout << "Control device (to be expanded with selection)\n";
            break;
        case 3:
        {
            string gname;
            cout << "Enter group name: ";
            cin >> gname;
            DeviceGroup *g = new DeviceGroup(gname);
            g->appliances = home.appliances;
            g->sensors = home.sensors;
            g->doors = home.doors;
            g->vehicles = home.vehicles;
            g->turnOnAll();
            home.groups.push_back(g);
            break;
        }
        case 4:
        {
            string sname, dname, cond;
            int val;
            bool turnOn;
            cout << "Enter sensor name: ";
            cin >> sname;
            cout << "Enter device name: ";
            cin >> dname;
            cout << "Condition > < == : ";
            cin >> cond;
            cout << "Value to compare: ";
            cin >> val;
            cout << "Turn device ON(1) or OFF(0): ";
            cin >> turnOn;
            home.rules.push_back({sname, dname, cond, val, turnOn});
            break;
        }
        case 5:
        {
            cout << "--- All Devices ---\n";
            for (auto a : home.appliances)
                a->status();
            for (auto s : home.sensors)
                s->status();
            for (auto d : home.doors)
                d->status();
            for (auto v : home.vehicles)
                v->status();
            break;
        }
        case 6:
            home.showEnergy();
            break;
        case 7:
        {
            string dname;
            int hr, min;
            bool on;
            cout << "Enter device name: ";
            cin >> dname;
            cout << "Hour(0-23) Minute(0-59): ";
            cin >> hr >> min;
            cout << "Turn ON(1) or OFF(0): ";
            cin >> on;
            home.schedules.push_back({dname, hr, min, on});
            break;
        }
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
                cout << "Device not found.\n";
            break;
        }
        }

        // Simulate time passing for automation and scheduling
        time_t now = time(0);
        tm *ltm = localtime(&now);
        home.checkSchedules(ltm->tm_hour, ltm->tm_min);
        home.applyAutomation();
        home.updateEnergy();

    } while (choice != 8);

    home.saveToFile();
    cout << "Data saved. Exiting...\n";
}
