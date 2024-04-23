![Banner](./repoBanner.png)


# Smart Medibox

Smart medibox is a smart device that,
1. Remind the user to take medicine at time through alarms.
2. Help to keep medicine in the required conditions via monitoring temperature and humidity continously and notifying the user if there is a bad condition.

# Technologies and Components

*   This project is developed to use
    *   ADAFRUIT SSD 1306 OLED Monochrome Display (128x64)
    *   ESP32 Devkit V1
    *   DHT11 Temperature and Humidity Sensor. (Easily configurable for DHT22 aswell.)

out of the box.
If the components change, you may have to change the parameters accordingly. 
<br><br>

<div align="center">
	<code><img width="50" src="./192108891-d86b6220-e232-423a-bf5f-90903e6887c3.png" alt="Visual Studio Code" title="Visual Studio Code"/></code>
	<code><img width="50" src="./192106073-90fffafe-3562-4ff9-a37e-c77a2da0ff58.png" alt="C++" title="C++"/></code>
	<code><img width="50" src="./274697405_404517471478609_1469547816363376057_n-removebg-preview.png" alt="Wokwi" title="Wokwi"/></code>
	<code><img width="50" src="./PlatformIO_logo.svg" alt="PlatformIO" title="UPlatformIO"/></code>
	<code><img width="50" src="https://github.com/marwin1991/profile-technology-icons/assets/136815194/a57a85ba-e2dd-4036-85b6-7e1532391627" alt="Arduino" title="Arduino"/></code>
</div>

## platform.ini

```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
lib_deps = 
	adafruit/Adafruit GFX Library@^1.11.9
	adafruit/Adafruit SSD1306@^2.5.9
	adafruit/DHT sensor library@^1.4.6
```


*   This was developed using Platform IO with Arduino Framework. (You can use the code in branch [`Wokwi`](https://github.com/chathura-de-silva/Smart-Medibox/tree/Wokwi), if you are looking forward for a simulation.)

# Getting Started

## Prerequisites
*   Git
*   Platform I/O with Arduino Framework Set up.
*   Relavant hardware for testing if you are not intended to do simulations.
<br>
<br>

*   Clone the repository.
    ```bash 
    git clone https://github.com/chathura-de-silva/Smart-Medibox     
    ```

 >[!IMPORTANT]<br>
> If you are willing to simulate the project using Wokwi extension, then clone the relavant branch using this, instead. (Or you can switch branches later.)
>    ```bash 
>     git clone -b Wokwi https://github.com/chathura-de-silva/Smart-Medibox     
>  ```


* Typically platform I/O will install required libraries itself. If not you have to install them referring the [`platform.ini`](#platform.ini)
 file displayed above.

* Compile and Upload/Simulate.
## Configuration and Structure

* Three seperate header files inside `./include` directory :
	*	`Constants.h` : Contains all constants.
	*	`Functions.h` : Contains declarations of all global functions.
	*	`Globals.h` : Contains declarations of all the global constants.
*	The initialisation and definitions of variables and functions are done in the relavant `.cpp` files.

*	`main.cpp` contains the arduino *loop()* and *setup()* functions.

* The device code is developed in such a way that it's actively listening(polling) for user inputs and for collecting sensor data from the DHT sensor. 
*	For an instance the three function calls in loop() function is responsible for,
	1.	`update_time_with_check_alarm()` - Updates the the time on the display. Checks whether there are any alarms to be rung at the moment. Calls `ring_alarm()` inside it if there is any.

	2.	`go_to_menu()` - Activelly listening for button presses and then keeps track of the selected mode by user via an abstract variable. When user confirms a menu item via pressing OK button, the `run_mode(mode)` is called. `run_mode(mode)` handles the rest via somr another nested function calls.

	3.	`check_temp()` - Reads the data from the `DHT11` sensor, prints a warning in the display if there is any parameter that is out of the specified limits. If there isn't any, plays an animation on the screen indicating it is actively monitoring those parameters.


# Known issues

Currently there are no known issues. If there is any, please open an issue in the `issus` section with a relavant label.

# Screenshot Gallery

<img width="1604" alt="Gallery Image" src="./1.jpg"> |  <img width="1604" alt="Gallery Image" src="./2.jpg"> |<img width="1604" alt="Gallery Image" src="./3.jpg">
:-------------------------:|:-------------------------:|:-------------------------:
<img width="1604" alt="Gallery Image" src="./4.jpg">  |  <img width="1604" alt="Gallery Image" src="./5.jpg">|<img width="1604" alt="Gallery Image" src="./6.jpg">
<img width="1604" alt="Gallery Image" src="./7.jpg">  |  <img width="1604" alt="Gallery Image" src="./8.jpg">|<img width="1604" alt="Gallery Image" src="./9.jpg">

# About

This project was first developed as the Semester 4 Project in the Module `EN2853 - Embedded Systems & Applications`, UoM, CSE.