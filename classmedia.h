#ifndef CLASSMEDIA_H
#define CLASSMEDIA_H

#include "classappliances.h"

class SmartTV : public Appliance {
private:
    int volume;
    string currentChannel;

public:
    SmartTV(string n, double e = 0.5) : Appliance(n, e), volume(10), currentChannel("News") {}

    void changeChannel(string channel) {
        if (isOn) {
            currentChannel = channel;
            cout << name << " channel changed to " << channel << ".\n";
        } else {
            cout << name << " is OFF. Cannot change channel.\n";
        }
    }

    void setVolume(int vol) {
        if (isOn) {
            volume = vol;
            cout << name << " volume set to " << vol << ".\n";
        } else {
            cout << name << " is OFF. Cannot set volume.\n";
        }
    }
};

class Speaker : public Appliance {
private:
    int volume;
    string currentSong;

public:
    Speaker(string n, double e = 0.3) : Appliance(n, e), volume(15), currentSong("Ambient Music") {}

    void playSong(string song) {
        if (isOn) {
            currentSong = song;
            cout << name << " is now playing " << song << ".\n";
        } else {
            cout << name << " is OFF. Cannot play song.\n";
        }
    }

    void setVolume(int vol) {
        if (isOn) {
            volume = vol;
            cout << name << " volume set to " << vol << ".\n";
        } else {
            cout << name << " is OFF. Cannot set volume.\n";
        }
    }
};

#endif // CLASSMEDIA_H