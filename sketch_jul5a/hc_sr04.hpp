#ifndef _HC_SR04_H
#define _HC_SR04_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

/* Definition the basic data for initialization */

#define Connection_Init     (Sensor_Physical_Connection_t){.Trig_Pin = 0x00, \
                                                           .Echo_Pin = 0x00}

#define Measurement_Init    (Measurement_t){.Unit = Cm,                     \
                                            .Distance_Metric = 0,           \
                                            .Distance_Imperial = 0,         \
                                            .Distance_Limit = 0,            \
                                            .Distance_Limit_Active = false, \
                                            .Distance_Limit_Alarm = false}

#define Correction_Init     (Sound_Velocity_Correction_t){.Correction_Active = false,   \
                                                          .Basic_Sound_Speed = 343.14,  \
                                                          .Corrected_Sound_Speed = 0}

#define Environmen_Init     (Environmental_Conditions_t){.Temperature_Unit = Centigrade,  \
                                                         .Temperature_Centigrade = 0,     \
                                                         .Temperature_Fahrenheit = 0,     \
                                                         .Temperature_Kelvins = 0,        \
                                                         .Humidity = 0}

#define Sensor_Init         (HC_SR04_t){.Connection = Connection_Init,  \
                                        .Sound_Speed = Correction_Init, \
                                        .Environmen = Environmen_Init,  \
                                        .Distance = Measurement_Init}

/* Enumerations type definition */
typedef enum{
    Cm = 0,
    Inch
} Measurement_Unit_e;

typedef enum{
    Centigrade = 0,
    Fahrenheit,
    Kelvins
} Temperature_Unit_e;

/* Structures type definition */
typedef struct
{
    uint8_t Trig_Pin;
    uint8_t Echo_Pin;
} Sensor_Physical_Connection_t;

typedef struct
{
    Measurement_Unit_e Unit;
    float Distance_Metric;
    float Distance_Imperial;
    float Distance_Limit;
    bool  Distance_Limit_Active;
    bool  Distance_Limit_Alarm;
} Measurement_t;

typedef struct
{
    bool  Correction_Active;
    float Basic_Sound_Speed;
    float Corrected_Sound_Speed;
} Sound_Velocity_Correction_t;

typedef struct
{
    Temperature_Unit_e Temperature_Unit;
    // float Temperature;
    float Temperature_Centigrade;
    float Temperature_Fahrenheit;
    float Temperature_Kelvins;
    float Humidity;
} Environmental_Conditions_t;

typedef struct
{
    Sensor_Physical_Connection_t Connection;
    Sound_Velocity_Correction_t Sound_Speed;
    Environmental_Conditions_t Environmen;
    Measurement_t Distance;
} HC_SR04_t;


/* Class definition */

class Ultrasonic
{
private:
    HC_SR04_t _HC_SR04;
    void Measurement_Unit_Converter(void);  /* Convert units of measurement to each other (Cm -> Inch). */
    void Temperature_Unit_Converter(void);  /* Convert units of Temperature to each other (C -> F, C -> K). */
    void Ultrasonic_Send_Pulse(void);       /* Send a 10 microsecond pulse through the trigger pin. */

public:
    Ultrasonic(uint8_t trigPin, uint8_t echoPin); /* Class constructor Method */
    float Ultrasonic_Get_Distance(Measurement_Unit_e MeasurementUnit); /* Get the distance to the obstacle through the ultrasonic module */
    void  Ultrasonic_Set_Distance_Limit(float limit); /* Set the distance limit to activate the alarm */
    void  Ultrasonic_Active_Correction(void); /* Active sound speed correction based on ambient temperature */
    void  Ultrasonic_Update_Correction(void); /* Update sound speed based on ambient temperature */
    void  Ultrasonic_Get_Update(HC_SR04_t Data); /* Getter method to transferring private data structure to an external structure */
    ~Ultrasonic(); /* Class destructor Method */
};

#endif /* _HC_SR04_H */
