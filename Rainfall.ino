const int rain = 2;     //pin used for rainfall sensor
volatile int pulse;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rain,INPUT);
  attachInterrupt(0, rain_count, RISING);   //interrupt attached to the interrupt pin 0 i.e pin 2
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  pulse = 0;
  delay(20000);
  Serial.print("Pulses per second: ");
  Serial.println(pulse/7);                  //dividing by 7 as one transition is giving 7 rising edges
  
  
}

void rain_count(){
  pulse++;
}

