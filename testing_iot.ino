#include <Adafruit_NeoPixel.h>

// NYC DOT Traffic Speeds Demo for Nodemcu board 12E

#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
#define PIN D4
#define SSD        "iot168"    // Wifi Credential
#define PASS        "rutabaga"    // Wifi Credential
#define BRIGHTNESS  128

int UVIndex = 0, Tree = 0, Mold = 0;
uint32_t color_UV, color_Tree, color_Mold;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);


ESP8266WiFiMulti WiFiMulti;



// ******** SETUP ********
void setup()
{

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //connect to the wifi
  
    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

//    WiFiMulti.addAP("SSD", "PASS");

    WiFi.begin(SSD, PASS);      // use WiFi.begin here because WiFiMulti would not reconnect if previously-stored AP is not present
     
    for(uint8_t t = 9; t > 0; t--) {              //    wait for DHCP to complete
        USE_SERIAL.printf("[SETUP] WIFI %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

}
    
// ******** LOOP ********
void loop()
{
  
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");

        // configure server and url - UV Index request
        http.begin("http://dataservice.accuweather.com/forecasts/v1/daily/1day/2843_pc.json?apikey=fapRnf57vISCFALOvzJrwjGqa89dlYJl&details=true"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");

        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {

                // get length of document (is -1 when Server sends no Content-Length header)
                int len = http.getSize();

                // get tcp stream
                WiFiClient * stream = http.getStreamPtr();

                // read data from server
                while(http.connected() && (len > 0 || len == -1)) {

                     while(!stream->findUntil("UVIndex","///"));  //  read through stream to find minimum travel time
                     UVIndex = stream->parseInt();
                     USE_SERIAL.println(UVIndex);

                     break;
                     }
                     
                     
                    delay(1);
                }

                USE_SERIAL.println();
                USE_SERIAL.print("[HTTP] connection closed or file end.\n");

            
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

        delay(1000);

    // configure server and url - Tree Pollen request
        http.begin("http://dataservice.accuweather.com/forecasts/v1/daily/1day/2843_pc.json?apikey=fapRnf57vISCFALOvzJrwjGqa89dlYJl&details=true"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");

        // start connection and send HTTP header
        httpCode = http.GET();
        if(httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

          // file found at server
          if(httpCode == HTTP_CODE_OK) {

            // get length of document (is -1 when Server sends no Content-Length header)
            int len = http.getSize();

            // get tcp stream
            WiFiClient * stream = http.getStreamPtr();

            // read data from server
            while(http.connected() && (len > 0 || len == -1)) {

               while(!stream->findUntil("Tree","///"));  //  read through stream to find minimum travel time
               Tree = stream->parseInt();
               USE_SERIAL.println(Tree);

               break;
               }
               
               
              delay(1);
            }

            USE_SERIAL.println();
            USE_SERIAL.print("[HTTP] connection closed or file end.\n");

          
        } else {
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

        delay(1000);
        
    
    // configure server and url - Mold Level request
        http.begin("http://dataservice.accuweather.com/forecasts/v1/daily/1day/2843_pc.json?apikey=fapRnf57vISCFALOvzJrwjGqa89dlYJl&details=true"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");

        // start connection and send HTTP header
        httpCode = http.GET();
        if(httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

          // file found at server
          if(httpCode == HTTP_CODE_OK) {

            // get length of document (is -1 when Server sends no Content-Length header)
            int len = http.getSize();

            // get tcp stream
            WiFiClient * stream = http.getStreamPtr();

            // read data from server
            while(http.connected() && (len > 0 || len == -1)) {

               while(!stream->findUntil("Mold","///"));  //  read through stream to find minimum travel time
               Mold = stream->parseInt();
               USE_SERIAL.println(Mold);

               break;
               }
               
               
              delay(1);
            }

            USE_SERIAL.println();
            USE_SERIAL.print("[HTTP] connection closed or file end.\n");

          
        } else {
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

        delay(1000);


      Serial.print(UVIndex);
      Serial.println(" : is the UV Index");
    Serial.print(Tree);
      Serial.println(" : is the Tree pollen count");
      Serial.print(Mold);
      Serial.println(" : is the Mold level");

      if (UVIndex == 0) {
        color_UV = strip.Color(1, 1, 1);
      } else if (UVIndex < 4) {
        color_UV = strip.Color(0, 255, 0);
      } else if (UVIndex < 7) {
        color_UV = strip.Color(255, 255, 0);
    } else {
        color_UV = strip.Color(255, 0, 0);
    }

    if (Tree == 0) {
        color_Tree = strip.Color(1, 1, 1);
      } else if (Tree < 4) {
        color_Tree = strip.Color(0, 255, 0);
      } else if (Tree < 8) {
        color_Tree = strip.Color(255, 255, 0);
    } else {
        color_Tree = strip.Color(255, 0, 0);
    }
    
    if (Mold == 0) {
        color_Mold = strip.Color(255, 255, 255);
      } else if (Mold < 4) {
        color_Mold = strip.Color(0, 255, 0);
      } else if (Mold < 8) {
        color_Mold = strip.Color(255, 255, 0);
    } else {
        color_Mold = strip.Color(255, 0, 0);
    }
    
      strip.setPixelColor(0, color_UV);
      strip.setPixelColor(1, color_Tree);
      strip.setPixelColor(2, color_Mold);
      strip.show();

delay(300000);
       
 }
  
}
