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

int UVIndex = 5;
int gwb_green, gwb_red, blue=255;
//int linc_south = 100.0, holland = 100.0;
//int gwb_red, gwb_green, linc_south_red, linc_south_green, holland_red, holland_green, blue=255;



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

        // configure server and url - GWB request
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
                     int UVIndex = stream->parseInt();
                     USE_SERIAL.println(UVIndex);

//                     while(!stream->findUntil("travelDurationTraffic","///"));  //  read through stream to find current travel time
//                     int travelDurationTraffic = stream->parseInt();
//                     USE_SERIAL.println(travelDurationTraffic);
// 
//                     blue = 0;
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

//                // configure server and url - Lincoln Tunnel request
//        http.begin("http://dataservice.accuweather.com/forecasts/v1/daily/1day/2843_pc.json?apikey=fapRnf57vISCFALOvzJrwjGqa89dlYJl&details=true"); //HTTP
//
//        USE_SERIAL.print("[HTTP] GET...\n");
//
//        // start connection and send HTTP header
//        httpCode = http.GET();
//        if(httpCode > 0) {
//            // HTTP header has been send and Server response header has been handled
//            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
//
//            // file found at server
//            if(httpCode == HTTP_CODE_OK) {
//
//                // get length of document (is -1 when Server sends no Content-Length header)
//                int len = http.getSize();
//
//                // get tcp stream
//                WiFiClient * stream = http.getStreamPtr();
//
//                // read data from server
//                while(http.connected() && (len > 0 || len == -1)) {
//
//                     while(!stream->findUntil("UVIndex","///"));  //  read through stream to find minimum travel time
//                     int travelDuration = stream->parseInt();
//                     USE_SERIAL.println(UVIndex);
//
//                     while(!stream->findUntil("travelDurationTraffic","///"));  //  read through stream to find current travel time
//                     int travelDurationTraffic = stream->parseInt();
//                     USE_SERIAL.println(travelDurationTraffic);
// 
//                     linc_south = (travelDurationTraffic - travelDuration);
//                     blue = 0;
//                     USE_SERIAL.println(linc_south);
//
//                     break;
//                     }
//                     
//                     
//                    delay(1);
//                }
//
//                USE_SERIAL.println();
//                USE_SERIAL.print("[HTTP] connection closed or file end.\n");
//
//            
//        } else {
//            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//        }
//
//        http.end();
//
//
//        delay(1000);
//
//        
//                // configure server and url - Holland Tunnel request
//        http.begin("http://dataservice.accuweather.com/forecasts/v1/daily/1day/2843_pc.json?apikey=fapRnf57vISCFALOvzJrwjGqa89dlYJl&details=true"); //HTTP
//
//        USE_SERIAL.print("[HTTP] GET...\n");
//
//        // start connection and send HTTP header
//        httpCode = http.GET();
//        if(httpCode > 0) {
//            // HTTP header has been send and Server response header has been handled
//            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
//
//            // file found at server
//            if(httpCode == HTTP_CODE_OK) {
//
//                // get length of document (is -1 when Server sends no Content-Length header)
//                int len = http.getSize();
//
//                // get tcp stream
//                WiFiClient * stream = http.getStreamPtr();
//
//                // read data from server
//                while(http.connected() && (len > 0 || len == -1)) {
//
//                     while(!stream->findUntil("travelDuration","///"));  //  read through stream to find minimum travel time
//                     int travelDuration = stream->parseInt();
//                     USE_SERIAL.println(travelDuration);
//
//                     while(!stream->findUntil("travelDurationTraffic","///"));  //  read through stream to find current travel time
//                     int travelDurationTraffic = stream->parseInt();
//                     USE_SERIAL.println(travelDurationTraffic);
// 
//                     holland = (travelDurationTraffic - travelDuration);
//                     blue = 0;
//                     USE_SERIAL.println(holland);
//
//                     break;
//                     }
//                     
//                     
//                    delay(1);
//                }
//
//                USE_SERIAL.println();
//                USE_SERIAL.print("[HTTP] connection closed or file end.\n");
//
//            
//        } else {
//            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//        }
//
//        http.end();
//    }    

    

      Serial.print(UVIndex);
      Serial.println(" : is the UV Index");
//      Serial.print(linc_south);
//      Serial.println(" : delay Lincoln Tunnel");
//      Serial.print(holland);
//      Serial.println(" : delay Holland Tunnel");

      if (UVIndex < 4) {
        color_UV = strip.Color(300, 0, 0)
      } else if (UVIndex < number) {
        
      }

      gwb_green = map(UVIndex, 300, 0, 0, BRIGHTNESS);
      gwb_red = BRIGHTNESS - gwb_green;
//
//      linc_south_green = map(linc_south, 300, 0, 0, BRIGHTNESS);
//      linc_south_red = BRIGHTNESS - linc_south_green;
//
//      holland_green = map(holland, 300, 0, 0, BRIGHTNESS);
//      holland_red = BRIGHTNESS - holland_green;
//
      strip.setPixelColor(0, color_UV);
//      strip.setPixelColor(1, strip.Color(linc_south_red, linc_south_green, blue));
//      strip.setPixelColor(2, strip.Color(holland_red, holland_green, blue));
//      strip.show();

delay(300000);
       
 }

  
}

