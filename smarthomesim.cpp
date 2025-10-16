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

using namespace std;

// Scheduling structure
struct Schedule
{
    string deviceName;
    int hour;
    int minute;
    bool turnOn;
};

// Automation structure
struct AutomationRule
{
    string sensorName;
    string deviceName;
    string condition;
    int value;
    bool turnOn;
};

// DeviceGroup class
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
        for (int i = 0; i < (int)appliances.size(); i++)
            if (appliances[i])
                appliances[i]->status();
        for (int i = 0; i < (int)sensors.size(); i++)
            if (sensors[i])
                sensors[i]->status();
        for (int i = 0; i < (int)doors.size(); i++)
            if (doors[i])
                doors[i]->status();
        for (int i = 0; i < (int)vehicles.size(); i++)
            if (vehicles[i])
                vehicles[i]->status();
    }

    void turnOnAll()
    {
        for (int i = 0; i < (int)appliances.size(); i++)
            if (appliances[i])
                appliances[i]->turnOn();
    }

    void turnOffAll()
    {
        for (int i = 0; i < (int)appliances.size(); i++)
            if (appliances[i])
                appliances[i]->turnOff();
    }
};

// SmartHome class
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
        for (int i = 0; i < (int)appliances.size(); i++)
            fout << "Appliance " << appliances[i]->getName() << " " << (appliances[i]->isOnDevice() ? "ON" : "OFF") << "\n";
        for (int i = 0; i < (int)sensors.size(); i++)
            fout << "Sensor " << sensors[i]->getName() << " " << (sensors[i]->isActiveSensor() ? "ON" : "OFF") << "\n";
        for (int i = 0; i < (int)doors.size(); i++)
            fout << "Door " << doors[i]->getName() << " " << (doors[i]->isLockedDoor() ? "LOCKED" : "UNLOCKED") << "\n";
        for (int i = 0; i < (int)vehicles.size(); i++)
            fout << "Vehicle " << vehicles[i]->getName() << " " << (vehicles[i]->isRunningVehicle() ? "ON" : "OFF") << "\n";
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
        for (int i = 0; i < (int)schedules.size(); i++)
        {
            Schedule &sch = schedules[i];
            if (sch.hour == curHour && sch.minute == curMin)
            {
                for (int j = 0; j < (int)appliances.size(); j++)
                {
                    if (appliances[j]->getName() == sch.deviceName)
                    {
                        if (sch.turnOn)
                            appliances[j]->turnOn();
                        else
                            appliances[j]->turnOff();
                    }
                }
                for (int j = 0; j < (int)vehicles.size(); j++)
                {
                    if (vehicles[j]->getName() == sch.deviceName)
                    {
                        if (sch.turnOn)
                            vehicles[j]->start();
                        else
                            vehicles[j]->stop();
                    }
                }
            }
        }
    }

    void applyAutomation()
    {
        for (int i = 0; i < (int)rules.size(); i++)
        {
            AutomationRule &rule = rules[i];
            Sensors *s = nullptr;
            Appliance *a = nullptr;

            for (int j = 0; j < (int)sensors.size(); j++)
                if (sensors[j]->getName() == rule.sensorName)
                    s = sensors[j];

            for (int j = 0; j < (int)appliances.size(); j++)
                if (appliances[j]->getName() == rule.deviceName)
                    a = appliances[j];

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
                else if (rule.condition == "==" && ts->getTemperature() == rule.value)
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
        for (int i = 0; i < (int)appliances.size(); i++)
            if (appliances[i]->isOnDevice())
                energyTracker[appliances[i]->getName()] += appliances[i]->getEnergy();

        for (int i = 0; i < (int)vehicles.size(); i++)
            if (vehicles[i]->isRunningVehicle())
                energyTracker[vehicles[i]->getName()] += vehicles[i]->getEnergy();
    }

    void showEnergy()
    {
        cout << "--- Energy Consumption ---\n";
        for (map<string, double>::iterator it = energyTracker.begin(); it != energyTracker.end(); ++it)
        {
            cout << it->first << ": " << it->second << " kWh\n";
        }
    }
};

// ---------- Device Group Management ----------
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

        while (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

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
            for (int i = 0; i < (int)home.groups.size(); i++)
                cout << i + 1 << ". " << home.groups[i]->getName() << endl;

            int groupChoice;
            cin >> groupChoice;
            if (groupChoice <= 0 || groupChoice > (int)home.groups.size())
            {
                cout << "Invalid selection.\n";
                break;
            }

            DeviceGroup *selectedGroup = home.groups[groupChoice - 1];
            cout << "Select a device to add:\n";
            for (int i = 0; i < (int)home.appliances.size(); i++)
                cout << i + 1 << ". " << home.appliances[i]->getName() << endl;

            int deviceChoice;
            cin >> deviceChoice;
            if (deviceChoice > 0 && deviceChoice <= (int)home.appliances.size())
            {
                selectedGroup->addAppliance(home.appliances[deviceChoice - 1]);
                cout << "Device added.\n";
            }
            else
                cout << "Invalid device selection.\n";
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
            for (int i = 0; i < (int)home.groups.size(); i++)
                cout << i + 1 << ". " << home.groups[i]->getName() << endl;

            int groupChoice;
            cin >> groupChoice;
            if (groupChoice <= 0 || groupChoice > (int)home.groups.size())
            {
                cout << "Invalid selection.\n";
                break;
            }

            DeviceGroup *selectedGroup = home.groups[groupChoice - 1];
            cout << "1. Turn On All\n2. Turn Off All\n";
            int action;
            cin >> action;
            if (action == 1)
                selectedGroup->turnOnAll();
            else if (action == 2)
                selectedGroup->turnOffAll();
            else
                cout << "Invalid action.\n";
            break;
        }
        case 4:
        {
            if (home.groups.empty())
            {
                cout << "No groups.\n";
                break;
            }
            cout << "Select a group to show status:\n";
            for (int i = 0; i < (int)home.groups.size(); i++)
                cout << i + 1 << ". " << home.groups[i]->getName() << endl;

            int groupChoice;
            cin >> groupChoice;
            if (groupChoice > 0 && groupChoice <= (int)home.groups.size())
                home.groups[groupChoice - 1]->showStatusAll();
            else
                cout << "Invalid selection.\n";
            break;
        }
        case 5:
        {
            if (home.groups.empty())
                cout << "No groups created.\n";
            else
            {
                cout << "--- All Groups ---\n";
                for (int i = 0; i < (int)home.groups.size(); i++)
                    cout << "- " << home.groups[i]->getName() << endl;
            }
            break;
        }
        }
    } while (choice != 6);
}

// -------------------- MAIN FUNCTION --------------------
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
        cout << "9. Combine Energy (operator+)\n10. Increase Device Energy (operator+=)\nChoice: ";

        while (!(cin >> choice))
        {
            cout << "Invalid input. Enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice)
        {
        case 1:
        {
            cout << "Add device logic here (already implemented in your original code).\n";
            break;
        }

        case 2:
            cout << "Control device logic here.\n";
            break;

        case 3:
            manageDeviceGroups(home);
            break;

        case 4:
        {
            string sname, dname, cond;
            int val;
            bool turnOn;
            cout << "Enter sensor name: ";
            cin >> sname;
            cout << "Enter device name: ";
            cin >> dname;
            cout << "Condition (> < ==): ";
            cin >> cond;
            cout << "Value: ";
            cin >> val;
            cout << "Turn ON(1) or OFF(0): ";
            cin >> turnOn;
            home.rules.push_back({sname, dname, cond, val, turnOn});
            break;
        }

        case 5:
        {
            cout << "--- Device Status ---\n";
            for (int i = 0; i < (int)home.appliances.size(); i++)
                home.appliances[i]->status();
            for (int i = 0; i < (int)home.sensors.size(); i++)
                home.sensors[i]->status();
            for (int i = 0; i < (int)home.doors.size(); i++)
                home.doors[i]->status();
            for (int i = 0; i < (int)home.vehicles.size(); i++)
                home.vehicles[i]->status();
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
                cout << "Need at least 2 appliances.\n";
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

            for (int i = 0; i < (int)home.appliances.size(); i++)
            {
                if (home.appliances[i]->getName() == name1)
                    a1 = home.appliances[i];
                if (home.appliances[i]->getName() == name2)
                    a2 = home.appliances[i];
            }

            if (a1 && a2)
            {
                Appliance combined = *a1 + *a2;
                cout << "\n[Combined Appliance Energy Info]\n";
                combined.status();
            }
            else
            {
                cout << "Device(s) not found.\n";
            }
            break;
        }

        case 10:
        {
            try
            {
                if (home.appliances.empty())
                    throw runtime_error("No devices to modify.");

                string name;
                double addEnergy;

                cout << "Enter device name to increase energy: ";
                cin >> ws;
                getline(cin, name);

                cout << "Enter amount to add (kWh): ";
                cin >> addEnergy;

                bool found = false;
                for (int i = 0; i < (int)home.appliances.size(); i++)
                {
                    if (home.appliances[i]->getName() == name)
                    {
                        *home.appliances[i] += addEnergy;
                        cout << "Updated Energy: " << home.appliances[i]->getEnergy() << " kWh\n";
                        found = true;
                        break;
                    }
                }

                if (!found)
                    throw invalid_argument("Device not found.");
            }
            catch (const invalid_argument &e)
            {
                cerr << "Error: " << e.what() << "\n";
            }
            catch (const runtime_error &e)
            {
                cerr << "Error: " << e.what() << "\n";
            }
            catch (...)
            {
                cerr << "Unexpected error.\n";
            }

            time_t now = time(0);
            tm *ltm = localtime(&now);
            home.checkSchedules(ltm->tm_hour, ltm->tm_min);
            home.applyAutomation();
            home.updateEnergy();
            break;
        }

        case 8:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid option.\n";
            break;
        }

    } while (choice != 8);

    home.saveToFile();
    cout << "Data saved. Goodbye!\n";
    return 0;
}
