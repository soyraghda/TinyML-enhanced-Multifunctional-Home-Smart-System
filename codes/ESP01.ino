
#include <ESP8266WiFi.h>
#define ssid "Raghda"                          //SSID and password of the router
#define password "raghda99"
#define webhookskey "cFTqxR7im4WBbH3lAWmoMh"      //the key of our event      
#define eventname1 "fire_detected"               //IFTTT event name - fire_detected

const int API_TIMEOUT = 15000;
const int httpsPort = 443;
const char* trigger1 = "/trigger/" eventname1 "/with/key/" webhookskey;
char const* event = trigger1;
const char* server = "maker.ifttt.com";
bool result = true;
bool first_test=true; //boolean that is checked true to indicate that it is the first data transmission from the arduino to the ESP-01
unsigned long delayStart = 0; // timer

void setup() {

  delay(5000);
  Serial.begin(115200);                         
  delay(100);

}    
// Establish WiFi connection to the router

static bool connectToWifi() {
  WiFi.disconnect();
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);                           //connect as client
  WiFi.begin(ssid, password);                    //connect to router
  Serial.print("Attempting to connect: ");

  int i = 16;                                    //try to connect to wifi for 15 seconds
  while((WiFi.status() != WL_CONNECTED) && (i-- > 0)) {
    delay(1000);
    Serial.print(i);
    Serial.print(", ");
  }
  Serial.println();

  //print connection result
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    result = true;
  }
  else {
    Serial.println("Connection failed - check your credentials or connection");
    result = false;
  }
  return result;
}


// Make an HTTP request to the IFTTT web service

void makeIFTTTRequest() {
  Serial.print("Connecting to ");
  Serial.print(server);

  BearSSL::WiFiClientSecure client;

  for (int tries = 0; tries < 5; tries++) {        //try up to 5 times to connect to server
    client.setTimeout(API_TIMEOUT);
    client.setInsecure();                          
    if(client.connect(server, httpsPort)) break;   //exit FOR loop if connection was established
    Serial.print(".");                             //  else wait, try again
    delay(2000);
  }
  Serial.println();

  if(!client.connected()) {
    Serial.println("Failed to connect to server");
    client.stop();
    return;                                      //if no connection, leave the function
  }
  Serial.print("Request event: ");               //if connection was established, send GET request
  Serial.println(event);
  client.print(String("GET ") + event +
                  " HTTP/1.1\r\n" +
                  "Host: " + server + "\r\n" +
                  "Connection: close\r\n\r\n");

  int timeout = 50;                              //wait 5 seconds for a response from server
  while(!client.available() && (timeout-- > 0)){
    delay(100);
  }

  if(!client.available()) {                     //if there was no response from the server  to the request
     Serial.println("No response to GET");
     client.stop();
     return;
  }
  while(client.available()){ //while there was a response from the server to the request
    Serial.write(client.read()); //it will read the data from the server
  }
}

void loop(){
   if(Serial.available( ) > 0) //  It will only send data when the received data is greater than 0.  
   {  
    int receivedData  = Serial.read();  // It will read the incoming or arriving data byte 
    char state = char(receivedData); // convert the received data to char
    if(state == '1' && first_test) //it will check if the transmitted data is equal to 1 & if it is the first time that  data is transmitted from the arduino
      {
        if(connectToWifi()) {                          //it will try to connect to wifi first
            makeIFTTTRequest(); }                      //if the wifi connection was successful it will try to send an ifttt request

        delayStart = millis();   // start delay (timer) by using the millis() function
        first_test=false;        
      }
    else if(state == '1' && ((millis() - delayStart) >= 1500000)){ //it will check if the transmitted data is equal to 1 & if the timer has surpassed 25 mins (1,500,000 ms) of not 

          if(connectToWifi()) {                          //it will try to connect to wifi first
            makeIFTTTRequest(); }                      //if the wifi connection was successful it will try to send an ifttt request

          delayStart = millis();   // restart delay (timer)
    }
   }
}

