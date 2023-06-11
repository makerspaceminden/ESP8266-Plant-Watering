/**
 * @file espMQTTpub.h
 * @author Paul Schlarmann (paul.schlarmann@makerspace-minden.de)
 * @brief simple MQTT publish library. Requires pubsubclient lib!
 * @version 0.1
 * @date 2022-08-04
 * 
 * @copyright Copyright (c) Paul Schlarmann 2022
 * 
 */
#ifndef ESPMQTTPUB_H
#define ESPMQTTPUB_H

// --- Includes ---
#include <Arduino.h>
#include <PubSubClient.h>

// --- Public Vars ---
PubSubClient *espMQTTpub_mqttClient;
char espMQTTpub_mqttDataBuffer[ESPMQTTPUB_MQTT_DATA_BUFFER_LEN];

// --- Public Functions ---

void espMQTTpub_Init(PubSubClient *mqttClient){
    espMQTTpub_mqttClient = mqttClient;
    espMQTTpub_mqttClient->setServer(ESPMQTTPUB_MQTT_SERVER, ESPMQTTPUB_MQTT_PORT);

    if (espMQTTpub_mqttClient->connect(ESPMQTTPUB_MQTT_ID, ESPMQTTPUB_MQTT_USER, ESPMQTTPUB_MQTT_PASS)) {
        Serial.println("Connected to MQTT");
    } else {    
        Serial.println("Could not connect to MQTT server!!");
        Serial.print("Current state: ");
        Serial.println(espMQTTpub_mqttClient->state());
        
        delay(1000);
    }
}

// Reconnect to MQTT server
void espMQTTpub_Reconnect(){
    // Loop until we're reconnected
    if(!espMQTTpub_mqttClient->connected()) {
        Serial.print("Attempting MQTT connection..., rc=");
        Serial.print(espMQTTpub_mqttClient->state());
        // Attempt to connect
        if (espMQTTpub_mqttClient->connect(ESPMQTTPUB_MQTT_ID, ESPMQTTPUB_MQTT_USER, ESPMQTTPUB_MQTT_PASS)) {
            Serial.println("connected");
            delay(1000);
        } else {
            Serial.print("failed, rc=");
            Serial.print(espMQTTpub_mqttClient->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void espMQTTpub_Loop(){
    espMQTTpub_Reconnect();
    if (espMQTTpub_mqttClient->connected()){
        espMQTTpub_mqttClient->loop();
    }
}

// Publish int value to MQTT
void espMQTTpub_PublishInt(const char *const topic, uint32_t value){
    // Turn int into string
    sprintf(espMQTTpub_mqttDataBuffer, "%d", value);
#ifdef ESPMQTTPUB_DEBUG
    Serial.print("MQTT: ");
    Serial.print(topic);
    Serial.print(" Int: ");
    Serial.print(espMQTTpub_mqttDataBuffer);
#endif
    // Publish if connected
    if (espMQTTpub_mqttClient->connected()){
#ifdef ESPMQTTPUB_DEBUG
        Serial.print("  OK");
#endif
        espMQTTpub_mqttClient->publish(topic, espMQTTpub_mqttDataBuffer);
    } else {
#ifdef ESPMQTTPUB_DEBUG
        Serial.print("  NC");
#endif
    }
#ifdef ESPMQTTPUB_DEBUG
    Serial.println("");
#endif
}
// Publish str value to MQTT (value _must_ be null terminated)
void espMQTTpub_PublishStr(const char *const topic, char *value){
#ifdef ESPMQTTPUB_DEBUG
    Serial.print("MQTT: ");
    Serial.print(topic);
    Serial.print(" Str: ");
    Serial.print(value);
#endif
    // Publish if connected
    if (espMQTTpub_mqttClient->connected()){
#ifdef ESPMQTTPUB_DEBUG
        Serial.print("  OK");
#endif
        espMQTTpub_mqttClient->publish(topic, value);
    } else {
#ifdef ESPMQTTPUB_DEBUG
        Serial.print("  NC");
#endif
    }
#ifdef ESPMQTTPUB_DEBUG
    Serial.println("");
#endif
}
// Publish float value to MQTT
void espMQTTpub_PublishFloat(const char *const topic, double value){
    // Check for nan
    if(isnan(value)){
        return;
    }
    // Turn float into string
    dtostrf( value, ESPMQTTPUB_MQTT_DATA_BUFFER_LEN, ESPMQTTPUB_FLOAT_PRECISION, espMQTTpub_mqttDataBuffer);
#ifdef ESPMQTTPUB_DEBUG
    Serial.print("MQTT: ");
    Serial.print(topic);
    Serial.print(" Float: ");
    Serial.print(espMQTTpub_mqttDataBuffer);
#endif
    // Publish if connected
    if (espMQTTpub_mqttClient->connected()){
#ifdef ESPMQTTPUB_DEBUG
        Serial.print("  OK");
#endif
        espMQTTpub_mqttClient->publish(topic, espMQTTpub_mqttDataBuffer);
    } else {
#ifdef ESPMQTTPUB_DEBUG
        Serial.print("  NC");
#endif
    }
#ifdef ESPMQTTPUB_DEBUG
    Serial.println("");
#endif
}


#endif /* ESPMQTTPUB_H */
