#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <map>
#include "classappliances.h"
#include "classsensors.h"
#include "classdoors.h"
#include "classvehicles.h"
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
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string gname;
            cout << "Enter group name: ";
            cin >> gname;
            home.groups.push_back(new DeviceGroup(gname));
            cout << "Group '" << gname << "' created.\n";
            break;
        }
        case 2:
        {
            if (home.groups.empty())
            {
                cout << "No groups available. Please create a group first.\n";
                break;
            }
            cout << "Select a group to add a device to:\n";
            for (int i = 0; i < home.groups.size(); ++i)
            {
                cout << i + 1 << ". " << home.groups[i]->getName() << endl;
            }
            int groupChoice;
            cin >> groupChoice;
            if (groupChoice > 0 && groupChoice <= home.groups.size())
            {
                DeviceGroup *selectedGroup = home.groups[groupChoice - 1];
                cout << "Select a device to add:\n";
                for (int i = 0; i < home.appliances.size(); ++i)
                {
                    cout << i + 1 << ". " << home.appliances[i]->getName() << endl;
                }
                int deviceChoice;
                cin >> deviceChoice;
                if (deviceChoice > 0 && deviceChoice <= home.appliances.size())
                {
                    selectedGroup->addAppliance(home.appliances[deviceChoice - 1]);
                    cout << "Device added to group.\n";
                }
                else
                {
                    cout << "Invalid device selection.\n";
                }
            }
            else
            {
                cout << "Invalid group selection.\n";
            }
            break;
        }
        case 3:
        {
            if (home.groups.empty())
            {
                cout << "No groups available.\n";
                break;
            }
            cout << "Select a group to control:\n";
            for (int i = 0; i < home.groups.size(); ++i)
            {
                cout << i + 1 << ". " << home.groups[i]->getName() << endl;
            }
            int groupChoice;
            cin >> groupChoice;
            if (groupChoice > 0 && groupChoice <= home.groups.size())
            {
                DeviceGroup *selectedGroup = home.groups[groupChoice - 1];
                cout << "1. Control All Devices in Group\n";
                cout << "2. Control an Individual Device in Group\n";
                int controlChoice;
                cin >> controlChoice;
                if (controlChoice == 1)
                {
                    cout << "1. Turn On All\n2. Turn Off All\n";
                    int actionChoice;
                    cin >> actionChoice;
                    if (actionChoice == 1)
                        selectedGroup->turnOnAll();
                    else if (actionChoice == 2)
                        selectedGroup->turnOffAll();
                }
                else if (controlChoice == 2)
                {
                    cout << "Select a device to control:\n";
                    for (int i = 0; i < selectedGroup->appliances.size(); ++i)
                    {
                        cout << i + 1 << ". " << selectedGroup->appliances[i]->getName() << endl;
                    }
                    int deviceChoice;
                    cin >> deviceChoice;
                    if (deviceChoice > 0 && deviceChoice <= selectedGroup->appliances.size())
                    {
                        Appliance *app = selectedGroup->appliances[deviceChoice - 1];
                        cout << "1. Turn On\n2. Turn Off\n";
                        int action;
                        cin >> action;
                        if (action == 1)
                            app->turnOn();
                        else if (action == 2)
                            app->turnOff();
                    }
                }
            }
            break;
        }
        case 4:
        {
            if (home.groups.empty())
            {
                cout << "No groups available.\n";
                break;
            }
            cout << "Select a group to show status:\n";
            for (int i = 0; i < home.groups.size(); ++i)
            {
                cout << i + 1 << ". " << home.groups[i]->getName() << endl;
            }
            int groupChoice;
            cin >> groupChoice;
            if (groupChoice > 0 && groupChoice <= home.groups.size())
            {
                home.groups[groupChoice - 1]->showStatusAll();
            }
            break;
        }
        case 5:
        {
            if (home.groups.empty())
            {
                cout << "No groups have been created yet.\n";
            }
            else
            {
                cout << "--- All Groups ---\n";
                for (auto g : home.groups)
                {
                    cout << "- " << g->getName() << endl;
                }
            }
            break;
        }
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
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int typeChoice;
            cout << "Select device type:\n1. Appliance\n2. Sensor\n3. Door\n4. Vehicle\nChoice: ";
            cin >> typeChoice;
            string name;
            cout << "Enter a name for the device: ";
            cin >> name;

            if (typeChoice == 1)
            {
                int applianceChoice;
                cout << "Select appliance type:\n1. Light\n2. Fan\n3. Air Conditioner\n4. Washing Machine\n5. Dishwasher\n6. Refrigerator\nChoice: ";
                cin >> applianceChoice;
                Appliance *a = nullptr;
                switch (applianceChoice)
                {
                case 1:
                    a = new Light(name);
                    break;
                case 2:
                    a = new Fan(name);
                    break;
                case 3:
                    a = new AirConditioner(name);
                    break;
                case 4:
                    a = new WashingMachine(name);
                    break;
                case 5:
                    a = new Dishwasher(name);
                    break;
                case 6:
                    a = new Refrigerator(name);
                    break;
                default:
                    cout << "Invalid appliance type.\n";
                    break;
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
                cout << "Select sensor type:\n1. Temperature\n2. Motion\n3. Humidity\n4. Rain\nChoice: ";
                cin >> sensorChoice;
                Sensors *s = nullptr;
                switch (sensorChoice)
                {
                case 1:
                    s = new TemperatureSensor(name);
                    break;
                case 2:
                    s = new MotionSensor(name);
                    break;
                case 3:
                    s = new HumiditySensor(name);
                    break;
                case 4:
                    s = new rainSensor();
                    break;
                default:
                    cout << "Invalid sensor type.\n";
                    break;
                }
                if (s)
                {
                    s->activate();
                    home.sensors.push_back(s);
                }
            }
            else if (typeChoice == 3)
            {
                int doorChoice;
                cout << "Select door type:\n1. Standard Door\n2. Smart Door\n3. Garage Door\nChoice: ";
                cin >> doorChoice;
                Doors *d = nullptr;
                switch (doorChoice)
                {
                case 1:
                    d = new Doors(name);
                    break;
                case 2:
                {
                    string code;
                    cout << "Enter a code for the smart door: ";
                    cin >> code;
                    d = new SmartDoor(name, code);
                    break;
                }
                case 3:
                    d = new GarageDoor(name);
                    break;
                default:
                    cout << "Invalid door type.\n";
                    break;
                }
                if (d)
                {
                    d->unlock();
                    home.doors.push_back(d);
                }
            }
            else if (typeChoice == 4)
            {
                int vehicleChoice;
                cout << "Select vehicle type:\n1. Car\n2. Bicycle\nChoice: ";
                cin >> vehicleChoice;
                Vehicle *v = nullptr;
                switch (vehicleChoice)
                {
                case 1:
                    v = new Car(name);
                    break;
                case 2:
                    v = new Bicycle(name);
                    break;
                default:
                    cout << "Invalid vehicle type.\n";
                    break;
                }
                if (v)
                {
                    v->start();
                    home.vehicles.push_back(v);
                }
            }
            break;
        }
        case 2:
            cout << "Control device (to be expanded with selection)\n";
            break;
        case 3:
        {
            manageDeviceGroups(home);
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