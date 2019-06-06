float val1 = 0;
float val2 = 0;
float temp = 0;
float hum = 0;
const int temperature = A1;       //Analog pin used for temperature
const int humidity = A2;          //Analog pin used for Humidity  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val1 = analogRead(temperature);
  val2 = analogRead(humidity);
  temp = (val1*5/1023)*100-40;    //Temperature value mapped to -40 to +60 celsuis
  hum = (val2*5/1023)*100;        //Humidity value mapped to 0 - 100%
  Serial.println("Temperature = ");
  Serial.println(temp);
  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.print('%');
  delay(1000);
}
