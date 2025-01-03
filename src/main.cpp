#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "config.h"
#include "espMQTTpub.h"
#include "espSimpleOTA.h"


// every 10 seconds
#define EXECUTION_STEP_INTERVAL (1 * 10 * 1000)

#define WATER_LEVEL_PIN_1 14 // D5
#define WATER_LEVEL_PIN_2 12 // D6
#define WATER_LEVEL_INPUT A0

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
bool isConnected = false;
uint32_t waterLevel = 0;

uint32_t oldTime = 0;

uint16_t counter = 0;

// sleep about every 59 minutes a.k.a. 3,540,000,000 µs (59*60*1000*1000)
uint64_t sleepTimeMicroSeconds = 3540000000UL;

int measureSoilMoisture()
{
	int sum = 0;

	for (int i = 0; i < SOIL_MOISTURE_SENSOR_READ_TIMES; i++)
	{

		digitalWrite(WATER_LEVEL_PIN_1, HIGH);
		digitalWrite(WATER_LEVEL_PIN_2, LOW);
		delayMicroseconds(25);
		sum += analogRead(WATER_LEVEL_INPUT);

		digitalWrite(WATER_LEVEL_PIN_1, LOW);
		digitalWrite(WATER_LEVEL_PIN_2, HIGH);
		delayMicroseconds(25);
		sum += 1023 - analogRead(WATER_LEVEL_INPUT);
	}

	digitalWrite(WATER_LEVEL_PIN_1, LOW);
	digitalWrite(WATER_LEVEL_PIN_2, LOW);

	return sum / (SOIL_MOISTURE_SENSOR_READ_TIMES * 2);
}
int measureSoilMoisture()
{
	int sum = 0;

	for (int i = 0; i < SOIL_MOISTURE_SENSOR_READ_TIMES; i++)
	{

		digitalWrite(WATER_LEVEL_PIN_1, HIGH);
		digitalWrite(WATER_LEVEL_PIN_2, LOW);
		delayMicroseconds(25);
		sum += analogRead(WATER_LEVEL_INPUT);

		digitalWrite(WATER_LEVEL_PIN_1, LOW);
		digitalWrite(WATER_LEVEL_PIN_2, HIGH);
		delayMicroseconds(25);
		sum += 1023 - analogRead(WATER_LEVEL_INPUT);
	}

	digitalWrite(WATER_LEVEL_PIN_1, LOW);
	digitalWrite(WATER_LEVEL_PIN_2, LOW);

	return sum / (SOIL_MOISTURE_SENSOR_READ_TIMES * 2);
}
void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("Plant Watering");

	pinMode(WATER_LEVEL_PIN_1, OUTPUT);
	pinMode(WATER_LEVEL_PIN_2, OUTPUT);

	IPAddress ip;

	if (ip.fromString(IP_ADDRESS)) {
			Serial.println(IP_ADDRESS);
	} else {
			Serial.print("UnParsable IP");
			Serial.println(IP_ADDRESS);
	}

	IPAddress dns;

	if (ip.fromString(DNS)) {
			Serial.println(DNS);
	} else {
			Serial.print("UnParsable IP");
			Serial.println(DNS);
	}

	IPAddress gateway;

	if (ip.fromString(GATEWAY)) {
			Serial.println(GATEWAY);
	} else {
			Serial.print("UnParsable IP");
			Serial.println(GATEWAY);
	}

	IPAddress subnet;

	if (ip.fromString(SUBNET)) {
			Serial.println(SUBNET);
	} else {
			Serial.print("UnParsable IP");
			Serial.println(SUBNET);
	}

	//WiFi.hostname(MQTT_ID);
	// Configures static IP address
	if (!WiFi.config(ip, dns, gateway, subnet))
	{
		Serial.println("STA Failed to configure");
	}

	Serial.print("Connecting to ");
	Serial.println(WIFI_SSID);
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	espMQTTpub_Init(&mqttClient);
	espSimpleOTA_Init(8266, "PLANT_WATERING_ESP8266", NULL);
}

void loop()
{
	espMQTTpub_Loop();
	espSimpleOTA_Loop();

	if (millis() - oldTime > EXECUTION_STEP_INTERVAL)
	{

		switch (counter)
		{
		case 0:
			espMQTTpub_PublishInt(ESPMQTTPUB_MQTT_TOPIC_WATER_LEVEL, measureSoilMoisture());
			break;
		default:
			// sleep
			counter = -1;
			ESP.deepSleep(3600000000UL);
			break;
		}

		oldTime = millis();

		counter++;
	}
}