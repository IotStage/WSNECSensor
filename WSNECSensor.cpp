#include "WSNECSensor.h"

EC::EC()
{
	_sensorPin = SensorPin;
	_sensorvalue = 0;
	_analogSampleInterval = intervalMesure;
	_tempSampleInterval = intervalConvertion;
	_tailleEchantillon = tailleEchantillon;
	indexEC = 0;
	temp = 0.0;
	_sensorTempPin = sensorTempPin;


};

EC::EC(uint8_t pin)
{
	_sensorPin = SensorPin;
	_sensorvalue = 0;
	_analogSampleInterval = intervalMesure;
	_tempSampleInterval = intervalConvertion;
	_tailleEchantillon = tailleEchantillon;
	//tableauEC[_tailleEchantillon];
	indexEC = 0;
	temp = 0.0;
	_sensorTempPin = sensorTempPin;

	
};
EC::EC(uint8_t pin, uint8_t TempPin)
{
	_sensorPin = SensorPin;
	_sensorvalue = 0;
	_analogSampleInterval = intervalMesure;
	_tempSampleInterval = intervalConvertion;
	_tailleEchantillon = tailleEchantillon;
	//tableauEC[_tailleEchantillon];
	indexEC = 0;
	temp = 0.0;
	_sensorTempPin = TempPin;

	
};
void EC::init()
{
	tableauEC[_tailleEchantillon];
	temperature = new Temperature(_sensorTempPin);
	temperature->updateTemperature(StartConvert);
	AnalogSampleTime = millis();
	tempSampleTime = millis();
	printTime = millis();
	AnalogValueTotal = 0;                  // the running total
	AnalogAverage = 0;
	averageVoltage=0;   
};

void EC::init(uint8_t mesure, uint16_t Convertion,uint8_t taille)
{
	_sensorvalue = 0;
	_analogSampleInterval = mesure;
	_tempSampleInterval = Convertion;
	_tailleEchantillon = taille;
	indexEC=0; 
	tableauEC[_tailleEchantillon];
	temperature = new Temperature(_sensorTempPin);
	temperature->updateTemperature(StartConvert);
	AnalogSampleTime = millis();
	tempSampleTime = millis();
	AnalogValueTotal = 0;                  // the running total
	AnalogAverage = 0;
	averageVoltage=0;
};

uint8_t EC::getMesure()
{
	return _sensorvalue;
};
/*

uint16_t EC::moyenne(uint8_t* arr, uint8_t number)
{
	uint8_t i;
	 int max,min;
	 uint16_t avg;
	 uint16_t amount=0;

	 if(number<5){  //less than 5, calculated directly statistics
	  for(i=0;i<number;i++){
	    amount+=arr[i];
	  }
	  avg = amount/number;
	  return avg;
	 }else{
	  
	   if(arr[0]<arr[1]){
	    min = arr[0];max=arr[1];
	   }
	   else{
	     min=arr[1];max=arr[0];
	    }
	    
	 for(i=2;i<number;i++){
	  if(arr[i]<min){
	    amount+=min;  //arr<min
	    min=arr[i];
	  }else {
	    if(arr[i]>max){
	      amount+=max;  //arr>max
	      max=arr[i];
	    }else{
	      amount+=arr[i]; //min<=arr<=max
	    }
	  }//if

	 }//for

	 avg = (uint16_t)amount/(number-2);

	 }//if

	 return avg;
};
*/
void EC::updateEC()
{
	
  /*
   Every once in a while,sample the analog value and calculate the average.
  */
  if(millis()-AnalogSampleTime>=_analogSampleInterval )  
  {
    AnalogSampleTime=millis();
     // subtract the last reading:
    AnalogValueTotal = AnalogValueTotal - tableauEC[indexEC];
    // read from the sensor:
    tableauEC[indexEC] = analogRead(_sensorPin);
    // add the reading to the total:
    AnalogValueTotal = AnalogValueTotal + tableauEC[indexEC];
    // advance to the next position in the array:
    indexEC = indexEC + 1;
    // if we're at the end of the array...
    if (indexEC >= _tailleEchantillon)
    // ...wrap around to the beginning:
    indexEC = 0;
    // calculate the average:
    AnalogAverage = AnalogValueTotal / _tailleEchantillon;
  }
  /*
   Every once in a while,MCU read the temperature from the DS18B20 and then let the DS18B20 start the convert.
   Attention:The interval between start the convert and read the temperature should be greater than 750 millisecond,or the temperature is not accurate!
  */
   if(millis()-tempSampleTime>=_tempSampleInterval) 
  {
    tempSampleTime=millis();
    temp = temperature->getTemperature();  // read the current temperature from the  DS18B20
    temperature->updateTemperature(StartConvert);                   //after the reading,start the convert for next reading
  }
  	
   /*
   Every once in a while,print the information on the serial monitor.
   */
  if(millis()-printTime>=printInterval)
  {
    printTime=millis();
    averageVoltage=AnalogAverage*(float)5000/1024;
    Serial.print("Analog value:");
    Serial.print(AnalogAverage);   //analog average,from 0 to 1023
    Serial.print("    Voltage:");
    Serial.print(averageVoltage);  //millivolt average,from 0mv to 4995mV
    Serial.print("mV    ");
    Serial.print("temp:");
    Serial.print(temp);    //current temperature
    Serial.print("^C     EC:");
    
    float TempCoefficient=1.0+0.0185*(temp-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
	float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
	if(CoefficientVolatge<150)Serial.println("No solution!");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
	else if(CoefficientVolatge>3300)Serial.println("Out of the range!");  //>20ms/cm,out of the range
	else
	{ 
	  if(CoefficientVolatge<=448)_sensorvalue=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
	  else if(CoefficientVolatge<=1457)_sensorvalue=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
	  else _sensorvalue=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm
	  _sensorvalue/=1000;    //convert us/cm to ms/cm
	  Serial.print(_sensorvalue,2);  //two decimal
	  Serial.println("ms/cm");
	}
  }

};


