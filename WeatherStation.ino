#include <math.h>
// Schematic:
// (+5V) ---- (10k-Resistor) -------|------- (Thermistor) ---- (0V)
//                                  |
//                             Analog Pin 0
 
int ThermistorPin = A0; // select the input pin for the thermistor
int HumidityPin = A1;// select the input pin for the humidity sensor
float R2 = 9785; // set the resistance of the second resistor, in Ω
float V_IN = 4.85; // set the e.m.f., in V
float R25 = 10000; // set the Resistance. in Ω, of the thermistor at @5 degrees Celsius
// set the Steinhart-Hart coefficients
//float A = 1.129148e-3; //good
//float B = 2.34125e-4; //good
//float C = 8.76741e-8; //good
float A = 3.354016e-03;
float B = 2.569850e-04;
float C = 2.620131e-06;
float D = 6.383091e-08;

void setup() {
  Serial.begin(9600);
}

float Humidity(float Temperature) {
  float ZeroPercentV_OUT = 0.8; // set the V_OUT at 0% RH
  // read the value from the humidity sensor
  float a5 = analogRead(HumidityPin);
  
  // calculate the pd across the humidity sensor, in V
  float V_OUT = (a5*V_IN)/1024;
  // The max voltage value drops down 0.006705882 for each degree C over 0C. The voltage at 0C is 3.27 (corrected for zero precent voltage)
  float maxV_OUT = (3.27-(0.006706*Temperature));
  float RH = ((V_OUT - ZeroPercentV_OUT)/maxV_OUT)*100;
  return RH;

}

float Temperature() {
  // read the value from the thermistor
  float a0 = analogRead(ThermistorPin);
  
  // calculate the pd across the thermistor, in V
  float V_OUT = (a0*V_IN)/1024;
  
  // calculate the resistance of the thermistor, in Ω
  float Rtherm = (R2*(V_IN - V_OUT))/V_OUT;
  
  //calculate the temperature in K using the Seinhart-Hart equation
//  float lnR = log(Rtherm);
  float lnROverR25 = log(Rtherm/R25);
//  float T = 1/(A + B*lnR + C*(pow(lnR, 3)));
  float T = 1/(A+(B*lnROverR25)+(C*(pow(lnROverR25,2)))+(D*(pow(lnROverR25,3))));
  // Convert Kelvin to degrees Celsius
  T = T - 273.15;
  return T;
}

void loop() {
  float T = Temperature();
  float RH = Humidity(T);
  // print the temperature on the PC monitor
  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.println(" degrees C");
  Serial.print("Relative Humidity: ");
  Serial.print(RH);
  Serial.println("%");
//  Serial.print("Resistance: ");
//  Serial.println(Rtherm);
//  Serial.print("Outpout Voltage: ");
//  Serial.println(V_OUT);
//  Serial.print("Analog Outpout: ");
//  Serial.println(a0);
  Serial.println("");
  delay(6000);
  
}
