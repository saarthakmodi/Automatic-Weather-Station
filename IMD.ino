/*
 Authors: Shail Vaidya, Saarthak Modi, Harsh Pardeshi, Digvijay Singh
 This code was written for the prototype developed for the Indian
 Meteorological Department, Pune.
 As part of the project for Practice School-1.
 */
#include<math.h>
/* Windwane Variables*/
const int pinWindDir = A0;            // Analog input pin for Windwane
int val1 = 0;
float Vin = 5;             // Input voltage (Windwane)
float Vout = 0;            // Vout default value (Windwane)
float Rref = 9999;          // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
float R = 0;               // Tested resistors default value
float theta = 0;
float oneDirection = 0;

/*Temperature and Humidity Variables*/
const int pinTemp = A1;               //Analog pin used for temperature
const int pinHumidity = A2;           //Analog pin used for Humidity  
float val2 = 0;
float val3 = 0;
float temp = 0;
float hum = 0;

/*Rainfall Variables*/
const int pinRainfall = 2;            // Digital pin used for rainfall
volatile int rainCount;
float totalraincount_mm=0;
float rainCount_mm=0;
/*Serial Port 2 is used for Anemometer*/
float windSpeed = 0;
String inwString = "";
String speedString = "";
float wind_speed_sqrt=0;
/*Serial Port 3 is used for Barometer */
String inbString = "";
String avString = "";
float avPress = 0;

int pressCount = 0;
int maincount=0;
float northComp = 0;
float eastComp = 0;

/*Sumation Variables*/
float sumOneNorthComp = 0;
float sumOneEastComp = 0;
float sumTemperature = 0;
float sumHumidity = 0;
float sumPressure = 0;
float totalraincount=0;
/*Average variables*/
double oneNorthComp = 0;
double oneEastComp = 0;
float dewPoint=0;
float oneTemperature = 0;
float oneHumidity = 0;
float onePressure = 0;
float QFE=0;
float QNE=0;
const String stationID = "Pune";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(4800);
  Serial3.begin(19200);
  
  pinMode(pinRainfall, INPUT);
  attachInterrupt(0, rain_count, RISING);
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(52,OUTPUT);
  pinMode(53,OUTPUT);
  digitalWrite(22,HIGH);
  digitalWrite(23,HIGH);
  digitalWrite(52,LOW);
  digitalWrite(53,LOW);

    Serial1.println("AT+CIPSHUT");
    delay(1000);
    printSerialData();
  
    Serial1.println("AT+CIPMUX=0");
   delay(2000);
   printSerialData();
   
   Serial1.println("AT+CGATT=1");
   delay(1000);
   printSerialData();
  
   Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
   delay(1000);
   printSerialData();
  
   Serial1.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
   delay(1000);
   printSerialData();
  
   Serial1.println("AT+SAPBR=1,1");
   delay(1000);
   printSerialData();
  
   Serial1.println("AT+SAPBR=2,1");
   delay(1000);
   printSerialData();
   
   Serial1.println("AT+CSQ"); //STRENGTH
   delay(2000); 
   printSerialData();

   Serial1.println("AT+HTTPTERM"); //TERMINATES HTTP CONNECTION, TO RESET THE MODULE
   delay(2000); 
   printSerialData();
   
  
   Serial1.println("AT+HTTPINIT"); //init the HTTP request
   delay(2000); 
   printSerialData();
  
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.println(millis());
    while(Serial2.available()>0){
    
    int inChar = Serial2.read();
    if(inChar != ' '){
      inwString += (char)inChar;
    }
    else{
      for(int i=inwString.length()-5; i<inwString.length(); i++){
        speedString += inwString[i];
      }
      windSpeed = speedString.toFloat();
     Serial.print("Wind speed = ");
      Serial.print(windSpeed);
      Serial.println(" Kts");
     inwString = "";
      speedString = ""; 
      break;
    
    }
  }
 
  delay(100);
    val1 = analogRead(pinWindDir);
    Vout = (Vin * val1) / 1023;    // Convert Vout to volts
    R = Rref * (1 / ((Vin / Vout) - 1));  // Formula to calculate tested resistor's value                          // Delay in milliseconds between reeds
    theta = (R/10000) * 360 + 180;
    if(theta > 360){
      theta = theta - 360;
    }
    Serial.print("Angle = ");
    Serial.print(theta);
    Serial.println(" °");
    
    delay(198); 

    while(Serial3.available()>0){
      int inChar = Serial3.read();
      if(inChar != '\n'){
        inbString += (char)inChar;
        
      }
      else{
        for(int i= inbString.length() - 23; i<inbString.length()-14; i++){
          avString += inbString[i];
        }
        avPress = avString.toFloat();
        sumPressure += avPress;
        Serial.print("Pressure =");
        Serial.print(inbString);
       
        Serial.println(" hPa");
        Serial.print(avPress); 
        inbString = "";
        avString = "";
        pressCount++;
        break;
      }
    }
  
   val2 = analogRead(pinTemp);
    val3 = analogRead(pinHumidity);
    temp = (val2*5/1023)*100-40;    //Temperature value mapped to -40 to +60 celsuis
    hum = (val3*5/1023)*100;        //Humidity value mapped to 0 - 100%
    sumTemperature += temp;
    sumHumidity += hum;
    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.println(" °C");
    Serial.print("Humidity = ");
    Serial.print(hum);
    Serial.println(" %");
    
    delay(100); 

  Serial2flush();
    
      Serial.print("Rain count = ");
      Serial.println(rainCount);
     delay(600);    
  Serial.println("");
     
    
  
    northComp = windSpeed * sin(theta * 3.141/180);
    sumOneNorthComp += northComp;
    eastComp = windSpeed * cos(theta*3.141/180);
    sumOneEastComp += eastComp;
    maincount++; 
    if(maincount%60==0)
      {three_min_average();
      sendData(); 
      delay(3000);
      }
    
      
    if(maincount%60==0)
      {ten_minute();
      sendData_rain();}
    
}

void ten_minute()
{ totalraincount_mm=totalraincount*0.5;
  rainCount_mm=rainCount*0.5;
  totalraincount=totalraincount+rainCount;
  Serial.print("Total rainfall = ");
  Serial.print(totalraincount*0.5);
  Serial.println(" mm");
  Serial.print("Rainfall in last 2 minutes = ");
  Serial.print(rainCount*0.5);
  Serial.println(" mm");
  rainCount=0;    //resetting rainCount variable
}  
void three_min_average()
{
        wind_speed_sqrt=sqrt(sq(oneNorthComp) + sq(oneEastComp));
        oneTemperature = sumTemperature/60;
        oneHumidity = sumHumidity /60;
        onePressure = sumPressure /pressCount;
        oneNorthComp = sumOneNorthComp / 60;
        oneEastComp = sumOneEastComp /60;
        oneDirection = atan2(oneNorthComp , oneEastComp)*180/3.141;
        if(oneDirection<0)
          { 
            oneDirection=oneDirection+360;
            }
        dewPoint=oneTemperature-((100-oneHumidity)/5);
        Serial.print("Three Min Average Temperature = ");
        Serial.print(oneTemperature);
        Serial.println(" °C");
        Serial.print("Three Min Average Humidity = ");
        Serial.print(oneHumidity);
        Serial.println(" %");
        Serial.print("Three minute average Pressure = ");
        Serial.print(onePressure);
        Serial.println(" hPa");
        Serial.print("Dew point temperature= ");
        Serial.print(dewPoint);
        Serial.println(" °C");
        Serial.print("Three minute average Wind = ");
        Serial.print(wind_speed_sqrt);
        Serial.println(" Kt");
        Serial.print("Angle = ");
        Serial.print(oneDirection);
        Serial.println(" °");
        sumTemperature=0;       //resetting all summation variables after every three minutes
        sumHumidity=0;
        sumPressure=0;
        sumOneNorthComp=0;
        sumOneEastComp=0;
        pressCount = 0;
        
        
}
/*
float asin(float c){
  float out;
  out= ((c+(c*c*c)/6+(3*c*c*c*c*c)/40+(5*c*c*c*c*c*c*c)/112+
(35*c*c*c*c*c*c*c*c*c)/1152 +(c*c*c*c*c*c*c*c*c*c*c*0.022)+
(c*c*c*c*c*c*c*c*c*c*c*c*c*.0173)+(c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*.0139)+
(c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*0.0115)+(c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*c*0.01)
));

//asin

  if(c>=.96 && c<.97){
    out=1.287+(3.82*(c-.96)); 
  }
  if(c>=.97 && c<.98){
    out=(1.325+4.5*(c-.97));
  } 
  if(c>=.98 && c<.99){
    out=(1.37+6*(c-.98));
  }
  if(c>=.99 && c<=1){
    out=(1.43+14*(c-.99));
  }
  return out;
}

float acos(float c){
  float out;
  out=asin(sqrt(1-c*c));
  return out;
}

float atan(float c){
  float out;
  out=asin(c/(sqrt(1+c*c)));
  return out;
}
  */
void rain_count(){
  rainCount++;
}

void Serial2flush()
{ while(Serial2.available())
        {char t = Serial2.read();}
}

void Serial3flush()
{ while(Serial3.available())
        {Serial3.read();}
}         

void sendData(){
   
  Serial1.println("AT+HTTPPARA=\"CID\",1");
  delay(3000);
  printSerialData();
 
  Serial1.print("AT+HTTPPARA=\"URL\",\"http://awsimd.000webhostapp.com/gprs.php?s=");
  // delay(500);
  printSerialData();
  Serial1.print(stationID);
  //delay(500);
  printSerialData();
  Serial1.print("&t=");
  //delay(500);
  printSerialData();
  Serial1.print(oneTemperature);
  //delay(500);
  printSerialData();
  Serial1.print("&h=");
  //delay(500);
  printSerialData();
  Serial1.print(oneHumidity);
  //delay(500);
  printSerialData();
  Serial1.print("&p=");
  //delay(500);
  printSerialData();
  Serial1.print(onePressure);
  //delay(500);
  printSerialData();
  Serial1.print("&ws=");
  //delay(500);
  printSerialData();
  Serial1.print(wind_speed_sqrt);
  //delay(500);
  printSerialData();
  Serial1.print("&wd=");
  //delay(500);
  printSerialData();
  Serial1.print(oneDirection);
  //delay(500);
  printSerialData();
  Serial1.print("&dp=");
  //delay(500);
  printSerialData();
  Serial1.print(dewPoint);
  //delay(500);
  printSerialData();
  Serial1.println("\"");
  delay(1000);
  printSerialData();

 
  Serial1.println("AT+HTTPACTION=1\r\n");
  delay(3000);
  printSerialData(); 

  
  Serial1.flush();
}


void printSerialData()
{
 while(Serial1.available()!=0)
 Serial.write(Serial1.read());
}

void sendData_rain(){
   
 // Serial1.println("AT+HTTPPARA=\"CID\",1");
  //delay(3000);
 // printSerialData();
 
  Serial1.print("AT+HTTPPARA=\"URL\",\"http://awsimd.000webhostapp.com/rain_gprs.php?s=");
  // delay(500);
  printSerialData();
  Serial1.print(stationID);
  //delay(500);
  //printSerialData();
  //Serial1.print("&s=");
  //delay(500);
  printSerialData();
  Serial1.print("&r=");
  //delay(500);
  printSerialData();
  Serial1.print(rainCount_mm);
  //delay(500);
  printSerialData();
  Serial1.print("&c=");
  //delay(500);
  printSerialData();
  Serial1.print(totalraincount/2);
  delay(1000);
  printSerialData();
  Serial1.println("\"");
  delay(1000);
  printSerialData();

 
  Serial1.println("AT+HTTPACTION=1\r\n");
  delay(3000);
  printSerialData(); 

  //delay(10000);
  Serial1.flush();
  rainCount_mm=0;
}
