This is an automatic translation, may be incorrect in some places. See sources and examples!

# Gyverds3231
Library for working with a real time microcircuit (RTC) DS3231
- time is considered on the basis of Millis () and synchronized with RTC by timer
- Built -in second timer for the convenience of automation of processes
- works on the basis of the library [Stamp] (https://github.com/gyverlibs/stamp)
- the ability to use the external object wire

## compatibility
Compatible with all arduino platforms (used arduino functions)

### Dependencies
- [Stamp] (https://github.com/gyverlibs/stamp)

## Content
- [use] (#usage)
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id="usage"> </a>

## Usage
The library considers time using Millis () and synchronizes the time from RTC by timer (by default every hour).

`` `CPP
// inherits Stampticker

// transfer the period of synchronization in seconds, silence.1 hour
Gyverds3231 (Uint16_t Syncprd = 60 * 60);

// Launch and synchronize.You can specify another Wire and I2C driver.Will return True with success
Bool Begin (Twowire* Wire = & Wire, Uint8_t Addr = 0x68);

// synchronize time with RTC.True with success, FALSE with a tire error or after resetting RTC power
Bool updatatenow ();

// Read the time with RTC for debugging
Datime Gettime ();

// Set the RTC time equal to the local firmware compilation time
Bool setbuildtime ();

// set the time RTC:
// ("hh: mm: ss") or ("yyyy-mm-dd") or ("yyyy-mm-ddthh: mm: ss")
// (unix)
// Datime (year, Month, Day, Hour, Minute, Second)
Bool settime (Datime DT);

// ticker, call in LOOP.Returns True every second if time is synchronized
Bool Tick ();

// get the chip temperature
Float gettemp ();

// get the chip temperature
int Gettempint ();

// Get a calibration displacement (value * 0.1PPM)
int8_t getoffset ();

// Set the calibration displacement (value * 0.1PPM)
Bool Setoffset (Int8_T Offseet);
`` `

### Examples
`` `CPP
#include <gyverds3231.h>
Gyverds3231 ds;

VOID setup () {
setstampzone (3);// specify a time zone if the program needs a real unix
// if not needed, you can not specify (remain 0)

Serial.Begin (115200);
Wire.begin ();

// Launch and synchronize time, if possible
DS.Begin ();

// Launch on another driver i2c
// DS.Begin (& Customwire);

// OR
// If the start is unsuccessful (for example, there was a power supply RTC)
// set time equal to compilation time
// if (! ds.begin ()) ds.Setbuildtime ();

// Time installation manually
// ds.Settime (123456789ul);// unix
// ds.Settime (Datime (2024, 7, 27, 17, 6, 30));// y.m.d h: m: s

// Datime DT;
// dt.second = 34;
// ... set the date and time ...
// and send to RTC
// ds.Settime (DT);
}

VOID loop () {
// call a ticker in look
// The ticker will also return True every second, which is convenient for comparing time
if (ds.tick ()) {
Serial.println (ds.tostring ());// Date and time withdrawal
Serial.println (ds.datetostosting ());// Date of dates by lineCranberry
// can be compared directly with UNIX
if (ds> = 12345) {}
if (ds == 123456) {}

ds.getunix ();// Get Unix seconds

// Unix Parsing on date and time
DS.Second ();// seconds
ds.minute ();// minutes and so on

// more efficiently use Datime Parser
Datime DT (DS);// ds itself converts into Datime

dt.year;
DT.MONTH;
dt.day;
DT.Hour;
dt.minute;
DT.Second;
DT.Weekday;
dt.yearday;

// for automation inside the day, it is convenient to use
// seconds from the beginning of the day, Dayseconds ()
DS.DaySeconds ();

// For convenience, there is also a class Dayseconds class that allows you to set time inside the day
Dayseconds DSEC (5, 10, 0);// 5 hours, 10 minutes, 0 seconds

// gyverds3231 can be compared directly to Dayseconds
if (ds == dsec) {}
}
}
`` `

<a id="versions"> </a>

## versions
- V1.0

<a id="install"> </a>
## Installation
- The library can be found by the name ** gyverds3231 ** and installed through the library manager in:
- Arduino ide
- Arduino ide v2
- Platformio
- [download the library] (https://github.com/gyverlibs/gyverds3231/archive/refs/heads/main.zip). Zip archive for manual installation:
- unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
- unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
- unpack and put in *documents/arduino/libraries/ *
- (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id="feedback"> </a>

## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code