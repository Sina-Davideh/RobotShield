#include "hc_sr04.hpp"

/* Convert units of measurement to each other (Cm -> Inch). */
void Ultrasonic::Measurement_Unit_Converter(void){
  _HC_SR04.Distance.Distance_Imperial = _HC_SR04.Distance.Distance_Metric * 0.3937;
}

/* Convert units of Temperature to each other (C -> F, C -> K). */
void Ultrasonic::Temperature_Unit_Converter(void){
  _HC_SR04.Environmen.Temperature_Fahrenheit = (_HC_SR04.Environmen.Temperature_Centigrade * 1.8) + 32;
  _HC_SR04.Environmen.Temperature_Kelvins = _HC_SR04.Environmen.Temperature_Centigrade + 273.15;
}

/* Class constructor Method */
Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin)
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    _HC_SR04 = Sensor_Init;
    _HC_SR04.Connection.Trig_Pin = trigPin;
    _HC_SR04.Connection.Echo_Pin = echoPin;
}

/* Send a 10 microsecond pulse through the trigger pin. */
void Ultrasonic::Ultrasonic_Send_Pulse(void)
{
  digitalWrite(_HC_SR04.Connection.Trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(_HC_SR04.Connection.Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_HC_SR04.Connection.Trig_Pin, LOW);
}

/* Get the distance to the obstacle through the ultrasonic module */
float Ultrasonic::Ultrasonic_Get_Distance(Measurement_Unit_e MeasurementUnit){
    float result;
    float Sound_Speed;

    if(_HC_SR04.Distance.Distance_Limit_Alarm == true){_HC_SR04.Distance.Distance_Limit_Alarm = false;}

    Ultrasonic_Send_Pulse();

    if(_HC_SR04.Sound_Speed.Correction_Active == true){
        Ultrasonic_Update_Correction();
        Sound_Speed = _HC_SR04.Sound_Speed.Corrected_Sound_Speed / 10000;
      }
    
    else{Sound_Speed = _HC_SR04.Sound_Speed.Basic_Sound_Speed / 10000;}

    _HC_SR04.Distance.Distance_Metric = (pulseIn(_HC_SR04.Connection.Echo_Pin, HIGH) * Sound_Speed)/2;
    Measurement_Unit_Converter();
    
    if(_HC_SR04.Distance.Distance_Limit_Active == true){
      if(_HC_SR04.Distance.Unit == Cm && _HC_SR04.Distance.Distance_Metric < _HC_SR04.Distance.Distance_Limit){_HC_SR04.Distance.Distance_Limit_Alarm = true;}
      else if(_HC_SR04.Distance.Unit == Inch && _HC_SR04.Distance.Distance_Imperial < _HC_SR04.Distance.Distance_Limit){_HC_SR04.Distance.Distance_Limit_Alarm = true;}
    }

    if(MeasurementUnit == Inch){result = _HC_SR04.Distance.Distance_Imperial;}
    else{result = _HC_SR04.Distance.Distance_Metric;}

    return result;
}

/* Set the distance limit to activate the alarm */
void  Ultrasonic::Ultrasonic_Set_Distance_Limit(float limit){
  if(_HC_SR04.Distance.Distance_Limit_Active != true){_HC_SR04.Distance.Distance_Limit_Active = true;}
  _HC_SR04.Distance.Distance_Limit = limit;
}

/* Active sound speed correction based on ambient temperature */
void  Ultrasonic::Ultrasonic_Active_Correction(void){
  _HC_SR04.Sound_Speed.Correction_Active = true;
}

/* Update sound speed based on ambient temperature */
void  Ultrasonic::Ultrasonic_Update_Correction(void){
  _HC_SR04.Sound_Speed.Corrected_Sound_Speed = 331.3 * (float)sqrt((1 +_HC_SR04.Environmen.Temperature_Kelvins) / 273.15);
}

/* Getter method to transferring private data structure to an external structure */
void  Ultrasonic::Ultrasonic_Get_Update(HC_SR04_t Data){
  Data = _HC_SR04;
}

/* Class destructor Method */
Ultrasonic::~Ultrasonic()
{
}
