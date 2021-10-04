#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE DHT11
#define DHTPIN 13

/*WIFI*/
SoftwareSerial myWifi(2, 3);
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
char ssid[] = "mh";
char pass[] = "19991006";
int status = WL_IDLE_STATUS; 
WiFiEspClient esp_client;
void printWifiData();
void printCurrentNet();
/*WIFI*/

int dhtPin = 13;
const int MotorIn1 = 5;
const int MotorIn2 = 6;
int LED = 9;
DHT dht(dhtPin, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

int STATUE=0;

void setup()  
{
  Serial.begin(9600);
  Serial.println(F("DHT11 test!"));
  dht.begin();
  lcd.init(); // initialize the lcd
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  pinMode(LED, OUTPUT);
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Hello !");
  delay(1000);

  myWifi.begin(9600);
  myWifi.listen();
  WiFi.init(& myWifi);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }// check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  printCurrentNet();
  printWifiData();
  
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  String sh = String(h, 2);// using a float and the decimal places
  String st = String(t, 2);// using a float and the decimal places

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }else{
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.println(F("%"));
    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.println(F("°C "));
    // Print a message to the LCD.  
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(F("Humidity:"));
    lcd.print(h);
    lcd.print(F("%"));
    lcd.setCursor(0,1);
    lcd.print(F("Temp:"));
    lcd.print(t);
    lcd.print(F(" oC"));     
    }
    delay(1000); 
    if(t>=31){ 
    spin3();
    }else if(t>=29&&t<31){ 
    spin2();
    }else if(t>=27.5&&t<29){ 
    spin1();
    }else if(t<27.5){
      motorstop();
    }
    ///555before(26/28/30)
    //after{27.5/29/31)
    myWifi.listen();
  if (esp_client.connectSSL("docs.google.com", 443)) {
       esp_client.println("GET /forms/d/e/1FAIpQLSdMFuuUFGngaemfvvylB_8peBO5RmMxNLXQlJnhq6o8mkTxGg/formResponse?entry.87393030="+ sh +"&entry.565758616="+ st +"&entry.1335350502="+ String(STATUE)+"&submit=Submit HTTP/1.1");
       esp_client.println("Host: docs.google.com");
       esp_client.println("Connection: close");
       esp_client.println();
       esp_client.stop();
  }
}

void motorstop()
{
  analogWrite(MotorIn1, 0);
  analogWrite(MotorIn2, 0);
  digitalWrite(LED, LOW);
  Serial.println(F("OFF"));
  lcd.setCursor(0,1);
  lcd.print("OFF");
  STATUE=0;
}

void spin1()
{
  analogWrite(MotorIn1,0);
  analogWrite(MotorIn2,50);
  analogWrite(LED,50);
  Serial.println(F("ON1"));
  STATUE=1;
}
void spin2()
{
  analogWrite(MotorIn1,0);
  analogWrite(MotorIn2,100);
  analogWrite(LED,150);
  Serial.println(F("ON2"));
  STATUE=2;
}
void spin3()
{
  analogWrite(MotorIn1,0);
  analogWrite(MotorIn2,200);
  analogWrite(LED,255);
  Serial.println(F("ON3"));
  STATUE=3;
}
void printWifiData()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

void printCurrentNet()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to
  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print("BSSID: ");
  Serial.println(buf);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}
