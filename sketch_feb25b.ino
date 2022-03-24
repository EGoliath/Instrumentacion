#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
String apiKey = "P8OG46UT3E5M1DDG";  
const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";
int duration = 5; 
int m = 0;
int Con_t = 0;
float mktlist[11];
float mkt ;
float et;
int num;

  
const char* server = "api.thingspeak.com";

WiFiClient client;
HTTPClient http;  //Declare an object of class HTTPClient
HTTPClient http2;  //Declare an object of class HTTPClient

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado a red WiFi");
  

}

void loop() {

  // put your main code here, to run repeatedly:
        
float BALANCE_RESISTOR   = 47000.0;
float MAX_ADC = 1024;
float BETA = 3450.0;
float ROOM_TEMP          = 298.15;
float RESISTOR_ROOM_TEMP = 100000.0;
float rThermistor = 0;            
float tKelvin     = 0;            
float t = 0;           
float adcAverage  = 0;
float vol = analogRead(A0);
float DH = 83.14472; //KJ / mole
float R =0.008314472;//kJ/mole/degree

pinMode(13,OUTPUT);
        
rThermistor = BALANCE_RESISTOR * ( (MAX_ADC / vol ));
tKelvin = (BETA * ROOM_TEMP) / (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));
t = tKelvin - 273.15; 
Con_t = Con_t +1;
mktlist[Con_t]= tKelvin;
Serial.println(Con_t);
Serial.println(mktlist[Con_t]);
num= sizeof(mktlist)/sizeof(int);
Serial.println(num);
if (Con_t==10){
     for(int i = 1; i < num ; i++){
       et= et+ exp(-DH/(R*mktlist[i]));
       Serial.print("L ");
       Serial.println (mktlist[i]);
          
    }
      mkt =(DH/R)/(-log(et/(num-1)));
       if (client.connect(server, 80)) {
        String postStr2 = apiKey;
        postStr2 += "&field2=";
        postStr2 += String(mkt);
        postStr2 += "\r\n\r\n";
    
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr2.length());
        client.print("\n\n");
        client.print(postStr2);
    
        Serial.print("MKT: ");
        Serial.print(mkt);
        Serial.println("°C enviado a BROKER Thingspeak");
        
        
        }
       Con_t=0;
      
}

        


    if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println("°C enviado a BROKER Thingspeak");
    
    //Serial.print(m);
    //Serial.print(vol);
    
    }
    
  
  client.stop();

  Serial.println("Esperando a siguiente publicacion…"); 
  //Serial.println(Con_t);
  delay(duration*1000);
  
  if (t > 27){

    if (WiFi.status() == WL_CONNECTED) { 

    

    http.begin("https://maker.ifttt.com/trigger/alarmtemp/with/key/d4WshdPhWtws-fNJzAnHtP");  //Specify request destination
    int httpCode = http.GET();                                                              

    if (httpCode > 0) { 

      String payload = http.getString();   
      Serial.println(payload);                     

    }

    http.end();   //Close connection
     Serial.println(" Mande correo");
     delay(5000);
     Serial.println("Esperando 10 seg");

  }
  m = 1;

     
    } else{


      if (m == 1) {


         if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    Serial.println(" si hay wifi");

    http2.begin("https://maker.ifttt.com/trigger/alarmnormal/with/key/d4WshdPhWtws-fNJzAnHtP");  //Specify request destination
    int httpCode2 = http2.GET();     //Send the request                                                           

    if (httpCode2 > 0) { //Check the returning code

      String payload2 = http2.getString();   //Get the request response payloadsw
      Serial.println(payload2); 
     //Print the response payload

    }

    http2.end();   //Close connection
    Serial.println(" Mande correo low");
    
  }
      m= 0;
      
      
      
      
    }
    if(t<-60){
      Serial.print("ERROR sensor desconectado");
      
      
      }
    

      
      
      
      
      
      }

}
    
    
    

      
    
  
  


    
    
