# Coin Machine

This project is for filtering US coin using Arduino and reflective IR sensors.
This is a final project for CS 59000 Embedded System.

![demo](https://github.com/Ronaldzzzzz/coin-machine/blob/main/img/demo.gif)

## Features

* Sorting
* Counting
* Saving box
* Replaceable filter

## Hardware list

* Arduino Uno
* ITR20001 - Miniature Reflective Infrared Optical Sensor
* I2C LCD Display
* Servo Motor
* Buzzer
* 10K, 220 Resistors
* Button
* Power Shpply

## Library list

* [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
* [Servo](https://www.arduino.cc/reference/en/libraries/servo/)
* [EEPROMAnything](https://playground.arduino.cc/Code/EEPROMWriteAnything/)

## Architecture

![architecture](https://github.com/Ronaldzzzzz/coin-machine/blob/main/img/coinMachine.png)