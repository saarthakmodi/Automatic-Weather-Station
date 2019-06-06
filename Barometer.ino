String inString = "";
String avString = "";
float avPress = 0;
void setup()  
{
  Serial.begin(9600);
  Serial.println("Ready");

 
 // Open serial communications and wait for port to open:
  Serial3.begin(19200);
  Serial.println("Start");

}
void loop(){ // run over and over

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
      Serial.println(avPress);
      inString = "";
      avString = "";
    }
  }
  }

