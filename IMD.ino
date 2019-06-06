
/* Windwane Variables*/
const int pinWindDir = A0;            // Analog input pin for Windwane
int val1 = 0;
float Vin = 5;             // Input voltage (Windwane)
float Vout = 0;            // Vout default value (Windwane)
float Rref = 9999;          // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
float R = 0;               // Tested resistors default value
float theta = 0;

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

/*Serial Port 2 is used for Anemometer*/
float windSpeed = 0;

/*Serial Port 3 is used for Barometer */
String inString = "";
String avString = "";
float avPress = 0;

float northComp = 0;
float eastComp = 0;

/*Sumation Variables*/
float sumOneNorthComp = 0;
float sumOneEastComp = 0;
float sumThreeNorthComp = 0;
float sumThreeEastComp = 0;
float sumTemperature = 0;
float sumHumidity = 0;
float sumPressure = 0;

/*Average variables*/
float oneNorthComp = 0;
float oneEastComp = 0;
float threeNorthComp = 0;
float threeEastComp = 0;
float oneTemperature = 0;
float oneHumidity = 0;
float onePressure = 0;
const String latitude = "";
const String longitude = "";
const String stationID = "";
String Time = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(4800);
  Serial3.begin(19200);

  pinMode(pinRainfall, INPUT);
  attachInterrupt(0, rainCount, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i<55; i++){
    val1 = analogRead(pinWindDir);
    Vout = (Vin * val1) / 1023;    // Convert Vout to volts
    R = Rref * (1 / ((Vin / Vout) - 1));  // Formula to calculate tested resistor's value                          // Delay in milliseconds between reeds
    theta = (R/10000) * 360 + 180;
    if(theta > 360){
      theta = theta - 360;
    }
    delay(100);
  
    val2 = analogRead(pinTemp);
    val3 = analogRead(pinHumidity);
    temp = (val1*5/1023)*100-40;    //Temperature value mapped to -40 to +60 celsuis
    hum = (val2*5/1023)*100;        //Humidity value mapped to 0 - 100%
    sumTemperature += temp;
    sumHumidity += hum;
    delay(100);
    
  
    while(Serial3.available()>0){
      int inChar = Serial3.read();
      if(inChar != '\n'){
        inString += (char)inChar;
        
      }
      else{
        for(int i=25; i<31; i++){
          avString += inString[i];
        }
        avPress = avString.toFloat();
        sumPressure += avPress;
        inString = "";
        avString = "";
        break;
      }
    }
  
    northComp = windSpeed * cos(theta);
    sumOneNorthComp += northComp;
    eastComp = windSpeed * sin(theta);
    sumOneEastComp += eastComp;

      if(i == 55){
        oneTemperature = sumTemperature/55;
        oneHumidity = sumHumidity / 55;
        onePressure = sumPressure / 55;
        oneNorthComp = sumOneNorthComp / 55;
        oneEastComp = sumOneEastComp / 55;
        Serial.print("Temperature = ");
        Serial.print(oneTemperature);
        Serial.println(" °C");
        Serial.print("Humidity = ");
        Serial.print(oneHumidity);
        Serial.println(" %");
        Serial.print("Pressure = ");
        Serial.print(onePressure);
        Serial.println(" hPa");
        Serial.print("Temperature = ");
        Serial.print(oneTemperature);
        Serial.println(" °C");
        Serial.print("Wind = ");
        Serial.print(sqrt(sq(oneNorthComp) + sq(oneEastComp)));
        Serial.println(" Kt");
        Serial.print("Angle = ");
        Serial.print(atan(oneNorthComp / oneEastComp));
        Serial.println(" °");
        
      }
  }
  
  
}

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
  
void rain_count(){
  rainCount++;
}
