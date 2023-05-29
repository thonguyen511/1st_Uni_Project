#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FirebaseESP32.h>
#include <string.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define FIREBASE_HOST "https://do-an-1-2fc90-default-rtdb.firebaseio.com/" //đường dẫn từ firebase nhận dữ liệu
#define FIREBASE_AUTH "ez5skYpjPiJs6a9uibXYXWJOvKklGRjSwGuEAOfo"
#define WIFI_SSID "T"  // Tên wifi của bạn
#define WIFI_PASSWORD "mnbvcxz1"   // Password wifi của bạn 

int light_sensor = 27;//khởi tạo chân đọc dữ liệu từ light_sensor
int dirt_hum = 32;//khởi tạo chân đọc dữ liệu từ dirt_hum
const int lm35 = 34; // Analog input pin 34 for LM35

const int led = 2;
const int motor = 0;
const int fan = 4;

int x;  //Lấy giờ
char formattedTime[20]; // format time as YY/MM/DD HH:MM
int dirt_humidity;
int light_sensorST;
float temperature;

LiquidCrystal_I2C lcd(0x27, 16, 2);

FirebaseData firebaseData;
WiFiUDP ntpUDP;   // Define NTP Client to get time
NTPClient timeClient(ntpUDP);

bool wifiConnected = false;

void setup() {
  pinMode(dirt_hum,INPUT);//chân dirt_hum là chân vào
  pinMode(light_sensor,INPUT);//chân light_sensor là chân vào
  pinMode(lm35,INPUT);
  pinMode(led, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(fan, OUTPUT);

  digitalWrite(motor, HIGH); //tắt
  digitalWrite(led, HIGH); //tắt
  digitalWrite(fan, HIGH); //tắt

  Wire.begin(21, 22);
  lcd.begin();
  lcd.backlight();

  Serial.begin(115200);
  delay(500);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(1, 0);
  lcd.print("Connecting...!");
  Serial.print("Dang ket noi");
  while (!wifiConnected) {  // loop until WiFi is connected
    Serial.print(".");
    delay(1000);

    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;  // set the flag to true if WiFi is connected
    }
  }
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Connected !");
  delay(2000);
  Serial.println("");
  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);//khởi chạy firebase
  timeClient.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("First scanning !");
  lcd.setCursor(0, 1);
  lcd.print("Wait a minute...");
  /* we create a new task here */
  xTaskCreate(DIRT,"DIRT",10000,NULL,3,NULL);
  delay(20000);
  xTaskCreate(LIGHT,"LIGHT",10000,NULL,1,NULL);
  delay(20000);
  xTaskCreate(TEMP,"TEMP",10000,NULL,1,NULL);
  delay(20000);
}

void loop() {
    if(Firebase.getString(firebaseData, "Data/send_to_esp/WARNING"))
    {
       if(firebaseData.stringData() == "false"){
            // Get real time //
            timeClient.update();// Update NTP Client
            unsigned long epochTime = timeClient.getEpochTime() + (7 * 3600);
            struct tm timeinfo;
            gmtime_r((const time_t*)&epochTime, &timeinfo);// convert epoch time to struct tm format
            strftime(formattedTime, sizeof(formattedTime), "%d/%m/%Y %H:%M", &timeinfo);
            Serial.println("==============================");
            Serial.println(formattedTime);
            Serial.println("==============================");
            sscanf(formattedTime, "%*d/%*d/%*d %d:", &x);
            lcd.home();
            lcd.clear();
            delay(1000);
            lcd.setCursor(0, 0);
            lcd.print(formattedTime);
            delay(13250);
       } else if (firebaseData.stringData() == "true") {
                digitalWrite(motor, HIGH); //tắt
                digitalWrite(led, HIGH); //tắt
                digitalWrite(fan, HIGH); //tắt
                Firebase.setInt(firebaseData, "Data/status/motor", 0);
                Firebase.setInt(firebaseData, "Data/status/light", 0);
                Firebase.setInt(firebaseData, "Data/status/fan", 0);
            for(int i; i < 21; i++){
                lcd.home();
                lcd.clear();
                delay(300);
                lcd.setCursor(3, 0);
                lcd.print("WARNING !!");
                delay(700);
            }
       }
    }
    if(Firebase.getString(firebaseData, "Data/send_to_esp/WARNING"))
    {
       if(firebaseData.stringData() == "false"){
            lcd.home();
            lcd.clear();
            delay(1000);
            lcd.setCursor(1, 0);
            lcd.print("Dirt humidity: ");
            delay(1000);
            lcd.setCursor(6, 1);
            lcd.print(String(dirt_humidity) + " %");
            delay(13250);
       } else if (firebaseData.stringData() == "true") {
                digitalWrite(motor, HIGH); //tắt
                digitalWrite(led, HIGH); //tắt
                digitalWrite(fan, HIGH); //tắt
                Firebase.setInt(firebaseData, "Data/status/motor", 0);
                Firebase.setInt(firebaseData, "Data/status/light", 0);
                Firebase.setInt(firebaseData, "Data/status/fan", 0);
            for(int i; i < 21; i++){
                lcd.home();
                lcd.clear();
                delay(300);
                lcd.setCursor(3, 0);
                lcd.print("WARNING !!");
                delay(700);
            }
       }
    }
    if(Firebase.getString(firebaseData, "Data/send_to_esp/WARNING"))
    {
       if(firebaseData.stringData() == "false"){
            lcd.home();
            lcd.clear();
            delay(1000);
            if (light_sensorST == 1){
              lcd.setCursor(3, 0);
              lcd.print("Night Time");
              delay(14250);
            } else {
              lcd.setCursor(4, 0);
              lcd.print("Day Time");
              delay(14250);
            }
       } else if (firebaseData.stringData() == "true") {
                digitalWrite(motor, HIGH); //tắt
                digitalWrite(led, HIGH); //tắt
                digitalWrite(fan, HIGH); //tắt
                Firebase.setInt(firebaseData, "Data/status/motor", 0);
                Firebase.setInt(firebaseData, "Data/status/light", 0);
                Firebase.setInt(firebaseData, "Data/status/fan", 0);
            for(int i; i < 21; i++){
                lcd.home();
                lcd.clear();
                delay(300);
                lcd.setCursor(3, 0);
                lcd.print("WARNING !!");
                delay(700);
            }
       }
    }
    if(Firebase.getString(firebaseData, "Data/send_to_esp/WARNING"))
    {
       if(firebaseData.stringData() == "false"){
            lcd.home();
            lcd.clear();
            delay(1000);
            lcd.setCursor(0, 0);
            lcd.print("Air temperature:");
            delay(1000);
            lcd.setCursor(4, 1);
            lcd.print(String(temperature) + " \xDF"+"C");
            delay(13250);
       } else if (firebaseData.stringData() == "true") {
                digitalWrite(motor, HIGH); //tắt
                digitalWrite(led, HIGH); //tắt
                digitalWrite(fan, HIGH); //tắt
                Firebase.setInt(firebaseData, "Data/status/motor", 0);
                Firebase.setInt(firebaseData, "Data/status/light", 0);
                Firebase.setInt(firebaseData, "Data/status/fan", 0);
            for(int i; i < 21; i++){
                lcd.home();
                lcd.clear();
                delay(300);
                lcd.setCursor(3, 0);
                lcd.print("WARNING !!");
                delay(700);
            }
       }
    }
}

void DIRT( void * parameter )
{
  for(;;){
    if(Firebase.getString(firebaseData, "Data/send_to_esp/WARNING"))
    {
       if(firebaseData.stringData() == "false"){
          // Read dirt_hum //
          int dirt_humValue = analogRead(dirt_hum);//đọc dữ liệu từ dirt_hum
          dirt_humidity = map(dirt_humValue, 0, 4095, 100, 0);
          Serial.print("1. Dirt humidity: ");
          Serial.print(dirt_humidity);
          Serial.println(" %");
          Firebase.setInt(firebaseData, "Data/received_from_esp/dirt_humidity", dirt_humidity);//gửi lên firebase
          
          // Read Motor control form firebase //
          if(Firebase.getString(firebaseData, "Data/send_to_esp/motor"))//motor
          {
            if(firebaseData.stringData() == "ON"){
              digitalWrite(motor, LOW);
              Firebase.setInt(firebaseData, "Data/status/motor", 1);}
            else{
              // Motor control //
              Firebase.getString(firebaseData, "Data/send_to_esp/timeset");
              Serial.println(String("     Watering at: " +firebaseData.stringData()));
              if ( (firebaseData.stringData() == String(x) && (dirt_humidity <= 90))  || (dirt_humidity <= 10) ){
                digitalWrite(motor, LOW); //bật
                Firebase.setInt(firebaseData, "Data/status/motor", 1);
                } else {
                digitalWrite(motor, HIGH); //tắt
                Firebase.setInt(firebaseData, "Data/status/motor", 0);
                }
            }
          }
       }
    }
    delay(60000);
  }
  vTaskDelete( NULL );
}

void LIGHT( void * parameter )
{
  for(;;){
    if(Firebase.getString(firebaseData, "Data/send_to_esp/WARNING"))
    {
       if(firebaseData.stringData() == "false"){
          // Read light_sensor //
          light_sensorST = digitalRead(light_sensor);//đọc dữ liệu từ light_sensor
          Serial.print("2. Light_sensor: ");
          Serial.println(light_sensorST);
          Firebase.setInt(firebaseData, "Data/received_from_esp/light_sensor", light_sensorST);//gửi lên firebase
          
          // Read light control form firebase //
          if(Firebase.getString(firebaseData, "Data/send_to_esp/light"))//light
          {
            // Serial.println("Light: " + String(firebaseData.stringData()));
            if(firebaseData.stringData() == "ON"){
              digitalWrite(led, LOW);
              Firebase.setInt(firebaseData, "Data/status/light", 1);}
            else{
              // Light Control //
              if (light_sensorST == 1){
                digitalWrite(led, LOW);
                Firebase.setInt(firebaseData, "Data/status/light", 1);
              } else {
                digitalWrite(led, HIGH);
                Firebase.setInt(firebaseData, "Data/status/light", 0);
              }
            }
          }
       }
    }
    delay(61010);
  }
  vTaskDelete( NULL );
}

void TEMP( void * parameter )
{
  for(;;){
    if(Firebase.getString(firebaseData, "Data/send_to_esp/WARNING"))
    {
       if(firebaseData.stringData() == "false"){
          // Read LM35 ( Air temperature) //
          int lm35Value = analogRead(lm35);
          temperature = ((lm35Value * 3.3) / 4095.0) * 100.0; // Convert ADC value to voltage
          Serial.print("3. Air temperature: "); // Print the temperature
          Serial.print(temperature);
          Serial.println(" °C");
          Firebase.setInt(firebaseData, "Data/received_from_esp/temperature", temperature);//gửi lên firebase
          
          // Read Fan control form firebase  //
          if(Firebase.getString(firebaseData, "Data/send_to_esp/fan"))//fan
          {
            // Serial.println("Fan: " + String(firebaseData.stringData()));
            if(firebaseData.stringData() == "ON"){
              digitalWrite(fan, LOW);
              Firebase.setInt(firebaseData, "Data/status/fan", 1);}
            else{
              // Fan control //
              if (temperature >= 33 ){
                digitalWrite(fan, LOW);
                Firebase.setInt(firebaseData, "Data/status/fan", 1); 
              } else {
                digitalWrite(fan, HIGH);
                Firebase.setInt(firebaseData, "Data/status/fan", 0);
              }
            }
          }
       }
    }
    delay(61510); // Delay for 60 second
  }
  vTaskDelete( NULL );
}