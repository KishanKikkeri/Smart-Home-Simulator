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

            if (home.appliances.empty())
            {
                cout << "No appliances available to add.\n";
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
            cout << "1. Turn On All\n2. Turn Off All\n3. Control Individual Device\nChoice: ";
            int action;
            cin >> action;
            if (action == 1)
                selectedGroup->turnOnAll();
            else if (action == 2)
                selectedGroup->turnOffAll();
            else if (action == 3)
            {
                if (selectedGroup->appliances.empty())
                {
                    cout << "No appliances in this group.\n";
                    break;
                }
                cout << "Select a device:\n";
                for (int i = 0; i < (int)selectedGroup->appliances.size(); i++)
                    cout << i + 1 << ". " << selectedGroup->appliances[i]->getName() << endl;
                int devChoice;
                cin >> devChoice;
                if (devChoice <= 0 || devChoice > (int)selectedGroup->appliances.size())
                {
                    cout << "Invalid device selection.\n";
                    break;
                }
                Appliance *app = selectedGroup->appliances[devChoice - 1];
                cout << "1. Turn On\n2. Turn Off\nChoice: ";
                int act2;
                cin >> act2;
                if (act2 == 1)
                    app->turnOn();
                else if (act2 == 2)
                    app->turnOff();
                else
                    cout << "Invalid action.\n";
            }
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
            int typeChoice;
            cout << "Select device type:\n1. Appliance\n2. Sensor\n3. Door\n4. Vehicle\n5. Media\nChoice: ";
            while (!(cin >> typeChoice))
            {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            string name;
            cout << "Enter a name for the device: ";
            cin >> ws;
            getline(cin, name);

            if (typeChoice == 1)
            {
                int applianceChoice;
                cout << "Select appliance type:\n1. Light\n2. Fan\n3. Air Conditioner\n4. Washing Machine\n5. Dishwasher\n6. Refrigerator\nChoice: ";
                while (!(cin >> applianceChoice))
                {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                Appliance *a = nullptr;
                if (applianceChoice == 1)
                    a = new Light(name);
                else if (applianceChoice == 2)
                    a = new Fan(name);
                else if (applianceChoice == 3)
                    a = new AirConditioner(name);
                else if (applianceChoice == 4)
                    a = new WashingMachine(name);
                else if (applianceChoice == 5)
                    a = new Dishwasher(name);
                else if (applianceChoice == 6)
                    a = new Refrigerator(name);
                else
                    cout << "Invalid appliance type.\n";

                if (a)
                {
                    a->turnOn();
                    home.appliances.push_back(a);
                    cout << "Appliance '" << name << "' added and turned ON.\n";
                }
            }
            else if (typeChoice == 2)
            {
                int sensorChoice;
                cout << "Select sensor type:\n1. Temperature\n2. Motion\n3. Humidity\n4. Rain\n5. Thermostat\nChoice: ";
                while (!(cin >> sensorChoice))
                {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                Sensors *s = nullptr;
                if (sensorChoice == 1)
                    s = new TemperatureSensor(name);
                else if (sensorChoice == 2)
                    s = new MotionSensor(name);
                else if (sensorChoice == 3)
                    s = new HumiditySensor(name);
                else if (sensorChoice == 4)
                    s = new rainSensor(name);
                else if (sensorChoice == 5)
                    s = new Thermostat(name);
                else
                    cout << "Invalid sensor type.\n";

                if (s)
                {
                    s->activate();
                    home.sensors.push_back(s);
                    cout << "Sensor '" << name << "' added and activated.\n";
                }
            }
            else if (typeChoice == 3)
            {
                int doorChoice;
                cout << "Select door type:\n1. Standard Door\n2. Smart Door\n3. Garage Door\nChoice: ";
                while (!(cin >> doorChoice))
                {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                Doors *d = nullptr;
                if (doorChoice == 1)
                    d = new Doors(name);
                else if (doorChoice == 2)
                {
                    string code;
                    cout << "Enter a code for the smart door: ";
                    cin >> ws;
                    getline(cin, code);
                    d = new SmartDoor(name, code);
                }
                else if (doorChoice == 3)
                    d = new GarageDoor(name);
                else
                    cout << "Invalid door type.\n";

                if (d)
                {
                    d->unlock();
                    home.doors.push_back(d);
                    cout << "Door '" << name << "' added and unlocked.\n";
                }
            }
            else if (typeChoice == 4)
            {
                int vehicleChoice;
                cout << "Select vehicle type:\n1. Car\n2. Bicycle\nChoice: ";
                while (!(cin >> vehicleChoice))
                {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                Vehicle *v = nullptr;
                if (vehicleChoice == 1)
                    v = new Car(name);
                else if (vehicleChoice == 2)
                    v = new Bicycle(name);
                else
                    cout << "Invalid vehicle type.\n";

                if (v)
                {
                    v->start();
                    home.vehicles.push_back(v);
                    cout << "Vehicle '" << name << "' added and started.\n";
                }
            }
            else if (typeChoice == 5)
            {
                int mediaChoice;
                cout << "Select media device type:\n1. Smart TV\n2. Speaker\nChoice: ";
                while (!(cin >> mediaChoice))
                {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                Appliance *a = nullptr;
                if (mediaChoice == 1)
                    a = new SmartTV(name);
                else if (mediaChoice == 2)
                    a = new Speaker(name);
                else
                    cout << "Invalid media device type.\n";

                if (a)
                {
                    a->turnOn();
                    home.appliances.push_back(a);
                    cout << "Media device '" << name << "' added and turned ON.\n";
                }
            }
            else
            {
                cout << "Invalid device type selection.\n";
            }
            break;
        }

        case 2:
        {
            if (home.appliances.empty() && home.sensors.empty() && home.doors.empty() && home.vehicles.empty())
            {
                cout << "No devices available to control.\n";
                break;
            }

            cout << "Control which category?\n1. Appliance\n2. Sensor\n3. Door\n4. Vehicle\nChoice: ";
            int cat;
            while (!(cin >> cat))
            {
                cout << "Invalid input. Enter a number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (cat == 1)
            {
                if (home.appliances.empty())
                {
                    cout << "No appliances available.\n";
                    break;
                }
                cout << "Select appliance:\n";
                for (int i = 0; i < (int)home.appliances.size(); i++)
                    cout << i + 1 << ". " << home.appliances[i]->getName() << endl;
                int sel;
                cin >> sel;
                if (sel <= 0 || sel > (int)home.appliances.size())
                {
                    cout << "Invalid selection.\n";
                    break;
                }
                Appliance *app = home.appliances[sel - 1];
                cout << "1. Turn On\n2. Turn Off\n3. Show Status\n4. Increase Energy (+=)\nChoice: ";
                int act;
                cin >> act;
                if (act == 1)
                    app->turnOn();
                else if (act == 2)
                    app->turnOff();
                else if (act == 3)
                    app->status();
                else if (act == 4)
                {
                    double eAdd;
                    cout << "Enter amount to add (kWh): ";
                    cin >> eAdd;
                    *app += eAdd;
                    cout << "Updated energy: " << app->getEnergy() << " kWh\n";
                }
                else
                    cout << "Invalid action.\n";
            }
            else if (cat == 2)
            {
                if (home.sensors.empty())
                {
                    cout << "No sensors available.\n";
                    break;
                }
                cout << "Select sensor:\n";
                for (int i = 0; i < (int)home.sensors.size(); i++)
                    cout << i + 1 << ". " << home.sensors[i]->getName() << endl;
                int sel;
                cin >> sel;
                if (sel <= 0 || sel > (int)home.sensors.size())
                {
                    cout << "Invalid selection.\n";
                    break;
                }
                Sensors *s = home.sensors[sel - 1];
                cout << "1. Activate\n2. Deactivate\n3. Show Status\nChoice: ";
                int act;
                cin >> act;
                if (act == 1)
                    s->activate();
                else if (act == 2)
                    s->deactivate();
                else if (act == 3)
                    s->status();
                else
                    cout << "Invalid action.\n";
            }
            else if (cat == 3)
            {
                if (home.doors.empty())
                {
                    cout << "No doors available.\n";
                    break;
                }
                cout << "Select door:\n";
                for (int i = 0; i < (int)home.doors.size(); i++)
                    cout << i + 1 << ". " << home.doors[i]->getName() << endl;
                int sel;
                cin >> sel;
                if (sel <= 0 || sel > (int)home.doors.size())
                {
                    cout << "Invalid selection.\n";
                    break;
                }
                Doors *d = home.doors[sel - 1];
                cout << "1. Lock\n2. Unlock\n3. Show Status\nChoice: ";
                int act;
                cin >> act;
                if (act == 1)
                    d->lock();
                else if (act == 2)
                    d->unlock();
                else if (act == 3)
                    d->status();
                else
                    cout << "Invalid action.\n";
            }
            else if (cat == 4)
            {
                if (home.vehicles.empty())
                {
                    cout << "No vehicles available.\n";
                    break;
                }
                cout << "Select vehicle:\n";
                for (int i = 0; i < (int)home.vehicles.size(); i++)
                    cout << i + 1 << ". " << home.vehicles[i]->getName() << endl;
                int sel;
                cin >> sel;
                if (sel <= 0 || sel > (int)home.vehicles.size())
                {
                    cout << "Invalid selection.\n";
                    break;
                }
                Vehicle *v = home.vehicles[sel - 1];
                cout << "1. Start\n2. Stop\n3. Show Status\nChoice: ";
                int act;
                cin >> act;
                if (act == 1)
                    v->start();
                else if (act == 2)
                    v->stop();
                else if (act == 3)
                    v->status();
                else
                    cout << "Invalid action.\n";
            }
            else
            {
                cout << "Invalid category.\n";
            }
            break;
        }

        case 3:
            manageDeviceGroups(home);
            break;

        case 4:
        {
            string sname, dname, cond;
            int val;
            bool turnOn;
            cout << "Enter sensor name: ";
            cin >> ws;
            getline(cin, sname);
            cout << "Enter device name: ";
            getline(cin, dname);
            cout << "Condition (> < ==): ";
            cin >> cond;
            cout << "Value: ";
            cin >> val;
            cout << "Turn ON(1) or OFF(0): ";
            cin >> turnOn;
            home.rules.push_back({sname, dname, cond, val, turnOn});
            cout << "Automation rule added.\n";
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
            home.updateEnergy();
            home.showEnergy();
            break;

        case 7:
        {
            string dname;
            int hr, min;
            bool on;
            cout << "Enter device name: ";
            cin >> ws;
            getline(cin, dname);
            cout << "Hour(0 - 23) Minute(0 - 59): ";
            cin >> hr >> min;
            cout << "Turn ON(1) or OFF(0): ";
            cin >> on;
            home.schedules.push_back({dname, hr, min, on});
            cout << "Schedule added.\n";
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
                Appliance combined = *a1 + *a2; // assumes operator+ returns Appliance
                cout << "\n[Combined Appliance Energy Info]\n";
                combined.status();
            }
            else
            {
                cout << "Device(s) not found. Available appliances:\n";
                for (int i = 0; i < (int)home.appliances.size(); i++)
                    cout << "- " << home.appliances[i]->getName() << endl;
            }
            break;
        }

        case 10: //added exexption handling to handle invalid inputs
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
                if (!(cin >> addEnergy))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Invalid energy amount input.");
                }

                bool found = false;
                for (int i = 0; i < (int)home.appliances.size(); i++)
                {
                    if (home.appliances[i]->getName() == name)
                    {
                        *home.appliances[i] += addEnergy; // assumes operator+= exists
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
