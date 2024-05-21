# Irrigation System with RTC, EEPROM, and TM1637 Display

This project is an irrigation system that uses a relay to control the watering schedule based on the current time and user-defined on/off times. The system includes features to adjust and display the current time, set on and off times for the relay, and persist these settings using EEPROM.

## Features

 **RTC Timekeeping**: Uses a DS3231 Real-Time Clock (RTC) module to keep track of the current time.
 **EEPROM Persistence**: Stores user-defined on/off times and current time settings in EEPROM, allowing the system to retain these settings even after power cycles.
 **TM1637 Display**: Displays current time, on time, and off time on a 4-digit 7-segment display.
 **User Controls**: Includes buttons to set the current time, on time, and off time.
 **Relay Control**: Controls a relay to switch the irrigation system on and off based on the user-defined schedule.
 **Visual Indicators**: Uses LEDs to indicate which setting (current time, on time, off time) is being adjusted.

## Hardware Requirements

 DS3231 RTC module
 TM1637 4-digit 7-segment display
 Relay module
 Push buttons for setting time and toggling modes
 LEDs for mode indication
 Arduino board (compatible with the libraries used)
 Resistors, breadboard, and connecting wires

## Pin Configuration

 **TM1637 Display**: DIO (2), CLK (3)
 **Buttons**: On Hours (4), On Minutes (5), Mode Toggle (7)
 **Relay**: Control Pin (12)
 **Mode Indicator LEDs**: Current Time (8), On Time (9), Off Time (10)
## Libraries Required

 RTClib: For interacting with the DS3231 RTC module.
 EEPROM: For storing and retrieving persistent settings.
 TM1637Display: For controlling the TM1637 4-digit 7-segment display.
