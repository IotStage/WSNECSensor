#include <WSN.h>
EC ec(A1, 3);


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println("debut initialisation");
  ec.init();
  Serial.println("initialisation terminee");
 
}

void loop() {
   ec.updateEC();
 
  // put your main code here, to run repeatedly:
  Serial.println(ec.getMesure());
}
