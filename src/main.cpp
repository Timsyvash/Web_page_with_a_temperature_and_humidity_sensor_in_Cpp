#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

const char *ssid = "EpsilonNET";
const char *password = "zaq123456";

unsigned long last_time_serial = 0;

#define pin_sensor_dht D2
#define type_sensor DHT11

ESP8266WebServer server(80);
DHT dht(pin_sensor_dht, type_sensor);

void getHum()
{
  float hum = dht.readHumidity();
  String message = "Humidity: " + String(hum) + " %";
  server.send(200, "text/plain", message);
}

void getTemp()
{
  float temp = dht.readTemperature();
  String message = "Temperature: " + String(temp) + " C";
  server.send(200, "text/plain", message);
}

void setup()
{
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print(" ");

  while (WiFi.status() != WL_CONNECTED)
  {
    unsigned long cur_time = millis();
    if (cur_time - last_time_serial >= 500)
    {
      last_time_serial = cur_time;
      Serial.print(".");
    }
    yield(); // Запобігає зависанню та WDT reset
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Розділяємо маршрути для вебсерверу
  server.on("/temp", HTTP_GET, getTemp);
  server.on("/hum", HTTP_GET, getHum);

  server.begin();
  Serial.println("Web server started!");
}

void loop()
{
  server.handleClient();
}