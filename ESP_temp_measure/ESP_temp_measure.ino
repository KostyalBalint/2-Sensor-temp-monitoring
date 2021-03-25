/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//For monitoring the current battery voltage
ADC_MODE(ADC_VCC);

// Replace with your network details
const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";

// Time to sleep (in minutes):
const int sleepTimeM = 15;


// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS 5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.

//---------------------Devices---------------------//
  //Device 1: 0x28, 0x91, 0xA9, 0xE2, 0x08, 0x00, 0x00, 0x33
  //Device 2: 0x28, 0x13, 0x67, 0xE3, 0x08, 0x00, 0x00, 0xB8
//-------------------------------------------------//

DeviceAddress TempSensor1 = { 0x28, 0x91, 0xA9, 0xE2, 0x08, 0x00, 0x00, 0x33 };
DeviceAddress TempSensor2 = { 0x28, 0x13, 0x67, 0xE3, 0x08, 0x00, 0x00, 0xB8 };

DallasTemperature DS18B20(&oneWire);
char temperatureCDevice1[6];
char temperatureCDevice2[6];

// Web Server on port 80
//WiFiServer server(80);
char server[] = "WEBSERVER DOMAIN NAME";
WiFiClient client;



void setup() {        // only runs once on boot
  unsigned long start_time =  millis();       // to measure the awake time
  int wifi_connect_time =  10000;       // WiFi connecting allowed time in milisecond

  // Initializing serial port for debugging purposes
  Serial.begin(74880);
  delay(10);

  //Read the current voltage
  int Mvoltage_length = 10;
  uint32_t Mvoltage[Mvoltage_length];
  for(int i = 0; i < Mvoltage_length; i++){
    Mvoltage[i] =   ESP.getVcc() + 180; // Add 0.18 volt, because of the measuring error
  }
  sort(Mvoltage, Mvoltage_length);
  float voltage = Mvoltage[Mvoltage_length/2];
  voltage = voltage/1000;

  Serial.print("Current input voltage is: ");
  Serial.print(voltage);
  Serial.println(" V");

  DS18B20.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

 /* getTemperature();
  Serial.println("//----------------------------//");
  Serial.println(temperatureCString);
  Serial.println("//----------------------------//");
*/
  getTemperature();

  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  unsigned long wifi_connect_start_time =  millis();
  while ((WiFi.status() != WL_CONNECTED) && (wifi_connect_start_time + wifi_connect_time) >= millis()) {
    delay(100);            // original value: delay(500)
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi connected");
  }
  else{
    Serial.println();
    Serial.println("WiFi connection failed - goes to sleep");
    Serial.print("Awake time: ");
    Serial.println((millis() - start_time)); //In miliseconds
    ESP.deepSleep(sleepTimeM * 60 * 1e6);    // microseconds
  }

  // Starting the web server
//  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  //delay(5000);            // original value: delay(10000)

  // Printing the ESP IP address
  Serial.println(WiFi.localIP());


// because of using deep sleep mode, the communication is made here in the setup
  if (client.connect(server, 80)) {
    getTemperature();
    Serial.println("--> connection ok\n");
    client.print("GET /solarpanel/insert_data.php?"); // This
    client.print("temp_1="); // This
    client.print(temperatureCDevice1);// And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.print("&temp_2=");
    client.print(temperatureCDevice2);
    client.print("&voltage=");
    client.print(voltage);
    client.println(" HTTP/1.1"); // Part of the GET request
    client.print( "Host: " );
    client.println(server);
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.println(); // Empty line
    client.stop();    // Closing connection to server
    Serial.println("--> finished transmission\n");
  }
    else {
    // If Arduino can't connect to the server (your computer or web page)
    Serial.println("--> connection failed\n");
  }


    // Sleep

  Serial.print("Awake time: ");
  Serial.println((millis() - start_time)); //In milisecondss
  Serial.println("ESP goes sleep mode");
  ESP.deepSleep(sleepTimeM * 60 * 1e6);    // microseconds

}

void getTemperature() {
  float TempC1;
  float TempC2;
  do {
    /*DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);*/
    oneWire.reset();

    oneWire.select(TempSensor1);
    oneWire.write(0x44);
    float TempC1 = DS18B20.getTempC(TempSensor1);

    oneWire.select(TempSensor2);
    oneWire.write(0x44);
    float TempC2 = DS18B20.getTempC(TempSensor2);

    dtostrf(TempC1, 2, 2, temperatureCDevice1);
    dtostrf(TempC2, 2, 2, temperatureCDevice2);

    delay(100);
  } while ((TempC2 == 85.0 || TempC2 == (-127.0)) || (TempC1 == 85.0 || TempC1 == (-127.0)));
}

// runs over and over again
void loop() {     // In case of deep sleep mode, the main loop is empty
}

/*void loop() {
  // Listenning for new clients
  //SearchSensors();
//  WiFiClient client = server.available();

  if (client.connect(server, 80)) {
    getTemperature();
    Serial.println("--> connection ok\n");
    client.print("GET /solarpanel/insert_data.php?"); // This
    client.print("temp_1="); // This
    client.print(temperatureCDevice1);// And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.print("&temp_2=");
    client.print(temperatureCDevice2);
    client.println(" HTTP/1.1"); // Part of the GET request
    client.print( "Host: " );
    client.println(server);
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.println(); // Empty line
    client.stop();    // Closing connection to server
    Serial.println("--> finished transmission\n");
  }
    else {
    // If Arduino can't connect to the server (your computer or web page)
    Serial.println("--> connection failed\n");
  }
  delay(10000);   // 10 seconds

  */

  /*if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && blank_line) {
            getTemperature();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP8266 - Temperature</h1>");
            client.println("<h3>Sensore 1 temperature in Celsius: ");
            client.println(temperatureCDevice1);
            client.println("</h3><br>");
            client.println("<h3>Sensore 2 temperature in Celsius: ");
            client.println(temperatureCDevice2);
            client.println("</h3>");
            client.println("</h3></body></html>");
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }*/


/*void SearchSensors() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !oneWire.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    oneWire.reset_search();
    delay(250);
    return;
  }

  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  }

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = oneWire.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}*/

void sort(uint32_t a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    uint32_t t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}
