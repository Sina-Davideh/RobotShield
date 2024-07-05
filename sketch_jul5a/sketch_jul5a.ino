#include "hc_sr04.hpp"

/* Make object from Ultrasonic class */
Ultrasonic sonar(9, 10);

/* Define global variables */
HC_SR04_t Ultersonic_Data;
float distance;
const int led = 13;
int Led_State = HIGH;

/* Timer-1 overflow function */
ISR(TIMER1_COMPA_vect)
{
  OCR1A += 25000; // Advance The COMPA Register
  // Handle The Timer Interrupt
  sonar.Ultrasonic_Get_Distance(Cm);
  sonar.Ultrasonic_Get_Update(Ultersonic_Data);
  digitalWrite(led, Led_State);
  Led_State = !Led_State;
}

/* Config timer-1 to make 100ms interrupt to update ultrasonic data automatically.
 * In this config assumed ATMEGA328P working with 16Mhz frequency.
 */
void Timer1_Config(void){
  TCCR1A = 0;      // Init Timer1A
  TCCR1B = 0;      // Init Timer1B
  TCCR1B |= 0x03;  // Prescaler = 64
  OCR1A = 25000;   // Timer Compare1A Register
  TIMSK1 |= 0x02;  // Enable Timer COMPA Interrupt
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Timer1_Config();
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = sonar.Ultrasonic_Get_Distance(Cm);
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(150);
}
