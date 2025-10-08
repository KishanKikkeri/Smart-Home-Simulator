# Smart Home Simulator (C++ | OOP Project)

## Overview
The **Smart Home Simulator** is a command-line based project built using **Object-Oriented Programming (OOP)** principles in C++.  
It simulates a real-world smart home system where you can add, control, automate, and monitor different smart devices — all through the terminal.  
The project emphasizes **class design, inheritance, polymorphism, encapsulation, file handling, STL, and operator overloading**.

---

## Features
### Core Functionality
- **Add / Remove Devices:** Manage smart devices dynamically (Lights, Fans, ACs, Sensors, etc.)
- **Device Groups:** Control multiple devices together (e.g., “Bedroom Lights”)
- **Automation Rules:** Simple “if-then” logic (e.g., *If temperature > 30°C → Turn on AC*)
- **Simulated Sensors:** Generate temperature and humidity readings (random/user-defined)
- **Scheduling:** Turn devices on/off at specific times
- **Energy Tracking:** Track power consumption of each device
- **Persistence:** Saves all devices, states, and configurations to file (loaded at next run)
- **Operator Overloading:** Custom operators to simplify object comparison and management
- **Interactive Menu:** Fully terminal-driven interface for user interaction

---

## OOP Concepts Implemented
| Concept | Usage |
|----------|--------|
| **Class & Object** | For each device type and module |
| **Encapsulation** | Private data with getters/setters |
| **Inheritance** | Device base class → Derived classes (Light, Fan, AC, etc.) |
| **Polymorphism** | Virtual methods for control and status display |
| **Abstraction** | High-level control interface for all devices |
| **Operator Overloading** | Equality & assignment operators for comparing devices |
| **File Handling** | Save and load system data using text files |
| **STL (Vectors, Maps)** | Efficient storage and retrieval of devices and groups |
| **Dynamic Memory** | Smart pointers / new-delete for dynamic objects |

---