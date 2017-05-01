/*
 * 
 * Exemple de code pour lire un unique capteur DS18B20 sur un bus 1-Wire.
 # Editeur : bngesp
 # Date   : 2017.04.29
 # Product: EC
*/

#ifndef WSNECSensor_h
#define WSNECSensor_h
#include "WSNTemperatureSensor.h"

#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>

// constantes

#define SensorPin A0  //le pin ou est branche le capteur
//unsigned int AnalogSampleInterval=25;printInterval=700,tempSampleInterval=850;  //analog sample interval;serial print interval;temperature sample interval
#define intervalMesure 25 //intervalle de ou on lit les valeurs sur le pin SensorPin
#define intervalConvertion 850 //intervalle ou les valeurs prises seront convertis en EC
#define tailleEchantillon  20
#define StartConvert 0
#define ReadTemperature 1
#define sensorTempPin 2
#define printInterval 700



class EC
{
public:
	EC();
	EC(uint8_t sensorECPin);
	EC(uint8_t sensorECPin,uint8_t TempPin );
	void updateEC();
	uint8_t getMesure();
	void init(uint8_t mesure, uint16_t conv ,uint8_t taille);
	void init();
private:
	uint8_t _sensorPin;
	uint8_t _sensorvalue;
	uint8_t _analogSampleInterval;
	uint16_t _tempSampleInterval;
	uint8_t tableauEC[];
	uint8_t _tailleEchantillon;
	uint16_t indexEC;
	uint32_t AnalogSampleTime;
	uint32_t tempSampleTime;
	uint16_t AnalogValueTotal, printTime;                  // the running total
	uint8_t AnalogAverage,averageVoltage; 
	uint8_t _sensorTempPin;  
 	Temperature *temperature;
	float temp;
	
};

#endif