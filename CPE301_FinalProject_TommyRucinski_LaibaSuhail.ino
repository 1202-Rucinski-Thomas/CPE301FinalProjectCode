//CPE 301 Final Project Code
//Tommy Rucinski and Laiba Suhail
// May 10, 2024

#include <LiquidCrystal.h> 
 #include <DHT.h> 
 #include <DHT_U.h> 
 #include <Stepper.h> 
 #define DHTPIN 7 
 #define DHTTYPE DHT11 
 #define tempMax 78 
 const int buttonPin = 2; 
 const int motorPin = 18; 
 const int fanPin = 20; 
 const int rPin = 14; 
 const int gPin = 15; 
 const int bPin = 16; 
 bool buttonPressed; 
 int lastButtonState; 
 int currentButtonState; 
//In the above portion of the code (lines 5-20), we initialized some elements, as well as created some variables and pin number assignments.

 LiquidCrystal lcd(3,4,5,6,12,13); 
 DHT dht(DHTPIN, DHTTYPE); 
 const int stepsperRevolution = 32; 
 Stepper myStepper(stepsperRevolution, 8, 9, 10, 11); 
#define RDA 0x80 
#define TBE 0x20  
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0; volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2; volatile unsigned int *myUBRR0 = (unsigned int *) 0x00C4; volatile unsigned char *myUDR0 = (unsigned char *)0x00C6; 
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C; volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B; volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A; volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78; 
//In lines 23-31, we defined the unsigned variables, and set up more of our initializations. This includes the liquid crystal library and DHT library.

void setup() { 

  //Serial.begin(9600); 
  U0init(9600); 
  adc_init(); 
  pinMode(buttonPin, INPUT); 
  pinMode(fanPin, OUTPUT); 
  pinMode(motorPin, INPUT); 
  pinMode(rPin, OUTPUT); 
  pinMode(gPin, OUTPUT); 
  pinMode(bPin, OUTPUT); 
  myStepper.setSpeed(200); 
  lcd.begin(16, 2); 
  lcd.setCursor(7,1); 
  lcd.print("---"); 
  dht.begin(); 
  DDRB |= (1 << 5); 
    
  lcd.setCursor(0,0); 
  lcd.print("Temp:"); 
  lcd.setCursor(0,1); 
  lcd.print("Humid:"); 
  delay(1500);

 } 
//In the above setup code, the function is to hold the action of the output and input pins and to print the humidity and temperature on our LCD display.
void loop() { 

  if(digitalRead(buttonPin)==HIGH){  buttonPressed = true; 
  delay(2000); 
  } 
  while(buttonPressed){ 
  digitalWrite(gPin, HIGH); 
  digitalWrite(bPin, HIGH); 
  digitalWrite(rPin, LOW); 
  digitalWrite(fanPin, LOW); 
  motor_move(); 
  if(digitalRead(buttonPin)==HIGH){  buttonPressed = false;  
  } 
  } 
  //set green light 
  digitalWrite(gPin, HIGH); 
  digitalWrite(bPin, LOW); 
  digitalWrite(rPin, LOW); 
    
  unsigned int adc_value = adc_read(0); if (digitalRead(motorPin) == HIGH) { 
  motor_move(); 
  } 
  // if (buttonState != HIGH){ 
  lcd.setCursor(12,1); 
  lcd.print("%"); 
  delay(550);
  lcd.setCursor(13,0); 
  lcd.print("<"); 
  delay(50); 
  lcd.setCursor(14,0); 
  lcd.print("-"); 
  delay(50); 
  lcd.setCursor(15,0); 
  lcd.print("-"); 
  delay(50); 
  lcd.setCursor(15,1); 
  lcd.print("-"); 
  delay(50); 
  lcd.setCursor(14,1); 
  lcd.print("-"); 
  delay(50); 
  lcd.setCursor(13,1); 
  lcd.print("<"); 
  lcd.setCursor(12,0); 
  lcd.print(" "); 
  float f = DHT.Humidity; 
  float c = DHT.Temperature; 
  //After implementing this code into our circuit, we have an error on the display, which we emailed Prof. Anima about. Unfortunately, due to time constraints with other classes, we were unable to fix this issue, as shown in the video.
  //This initial part of the loop function (lines 60-106) initializes our motor, along with sets up the green led to be active. The delay of the button press is also contained here in this code.
 
  if (isnan(f) && (c)) { 
  lcd.clear(); 
  lcd.setCursor(4,1); 
  lcd.print("|SENSOR|"); 
  lcd.setCursor(4,0); 
  lcd.print("|ERROR |");  
  delay(1000); 
  return; 
  } 
  
  lcd.setCursor(7,0); 
  lcd.print(c); 
  lcd.setCursor(7,1); 
  lcd.print(f); 
  //Serial.println("- Result -"); 
  Serial.print("Temp: ");  
  Serial.println(c); 
  Serial.print("Humid: "); 
  Serial.println(f); 
  Serial.println("- - - - - -"); 
  Serial.println(" "); 
  Serial.println(" "); 
    
  //adc lvl 
  Serial.println(adc_value);
  // print the value over UART 
  char str[8]; 
  sprintf(str, "%d\r\n", adc_value); 
  for (int i = 0; str[i] != '\0'; i++) { 
  U0putchar(str[i]); 
  } 
  if(adc_value < 100){ 
  digitalWrite(gPin, LOW); 
  digitalWrite(bPin, LOW); 
  digitalWrite(rPin, HIGH); 
  delay(500); 
  // print the value over UART 
  char str[] = {'E','r','r','o','r', '-', 'W', 'a', 't', 'e', 'r', ' ', 't', 'o', 'o', ' ', 'L', 'o', 'w', '\n'};  
  for (int i = 0; str[i] != '\0'; i++) { 
  U0putchar(str[i]); 
  } 
  } 
  if(c > 77 || c < 25){
  digitalWrite(fanPin, HIGH); 
  digitalWrite(gPin, LOW); 
  digitalWrite(bPin, HIGH); 
  digitalWrite(rPin, LOW); 
  delay(500); 
  }else{ 
  digitalWrite(fanPin, LOW); 
    
  } 
  delay(400); 
  lcd.setCursor(15,1); 
  lcd.print(" "); 
  delay(50); 
  lcd.setCursor(14,1); 
  lcd.print(" "); 
  delay(50); 
  lcd.setCursor(13,1); 
  lcd.print(" "); 
  delay(50); 
  lcd.setCursor(13,0); 
  lcd.print(" "); 
  delay(50); 
  lcd.setCursor(14,0); 
  lcd.print(" "); 
  delay(50); 
  lcd.setCursor(15,0); 
  lcd.print(" "); 
  delay(50); 
} 
void adc_init() 
{ 
 // setup the A register 
 *my_ADCSRA |= 0b10000000; // set bit 7 to 1 to enable the ADC  *my_ADCSRA &= 0b11011111; // clear bit 6 to 0 to disable the ADC trigger mode
 *my_ADCSRA &= 0b11110111; // clear bit 5 to 0 to disable the ADC interrupt  *my_ADCSRA &= 0b11111000; // clear bit 0-2 to 0 to set prescaler selection to slow reading  // setup the B register 
 *my_ADCSRB &= 0b11110111; // clear bit 3 to 0 to reset the channel and gain bits  *my_ADCSRB &= 0b11111000; // clear bit 2-0 to 0 to set free running mode  // setup the MUX Register 
 *my_ADMUX &= 0b01111111; // clear bit 7 to 0 for AVCC analog reference  *my_ADMUX |= 0b01000000; // set bit 6 to 1 for AVCC analog reference  *my_ADMUX &= 0b11011111; // clear bit 5 to 0 for right adjust result  *my_ADMUX &= 0b11100000; // clear bit 4-0 to 0 to reset the channel and gain bits } 
unsigned int adc_read(unsigned char adc_channel_num) {

  // clear the channel selection bits (MUX 4:0) 
  *my_ADMUX &= 0b11100000; 
  // clear the channel selection bits (MUX 5) 
  *my_ADCSRB &= 0b11110111; 
  // set the channel number 
  if(adc_channel_num > 7) 
  { 
  // set the channel selection bits, but remove the most significant bit (bit 3)  adc_channel_num -= 8; 
  // set MUX bit 5 
  *my_ADCSRB |= 0b00001000; 
  } 
  // set the channel selection bits 
  *my_ADMUX += adc_channel_num; 
  // set bit 6 of ADCSRA to 1 to start a conversion 
  *my_ADCSRA |= 0x40; 
  // wait for the conversion to complete 
  while((*my_ADCSRA & 0x40) != 0); 
  // return the result in the ADC data register 
  return *my_ADC_DATA; 
} 

void U0init(int U0baud) { 

  unsigned long FCPU = 16000000;
  unsigned int tbaud; 
  tbaud = (FCPU / 16 / U0baud - 1); // Same as (FCPU / (16 * U0baud)) - 1; *myUCSR0A = 0x20; 
  *myUCSR0B = 0x18; 
  *myUCSR0C = 0x06; 
  *myUBRR0 = tbaud; 
} 
unsigned char U0kbhit() {

return *myUCSR0A & RDA; 

} 
unsigned char U0getchar() { 

 return *myUDR0; 

} 
void U0putchar(unsigned char U0pdata) { 

while((*myUCSR0A & TBE)==0);  *myUDR0 = U0pdata; 

} 
// stepper move function
void motor_move() {

 myStepper.step(1); 
 Serial.print("motor stepped");  delay(500); 

}
