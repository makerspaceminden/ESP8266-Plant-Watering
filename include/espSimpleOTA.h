/**
 * @file espSimpleOTA.h
 * @author Paul Schlarmann (paul.schlarmann@makerspace-minden.de)
 * @brief Simple OTA header for ESP
 * @version 0.1
 * @date 2022-08-04
 * 
 * @copyright Copyright (c) Paul Schlarmann 2022
 * 
 */
#ifndef ESPSIMPLEOTA_H
#define ESPSIMPLEOTA_H

// --- Includes ---
#include <Arduino.h>
#include <ArduinoOTA.h>

// --- Defines ---

// --- Marcos ---

// --- Typedefs ---

// --- Private Vars ---

// --- Private Functions ---

// --- Public Vars ---

// --- Public Functions ---

void espSimpleOTA_Init(int port, const char *chipID, const char *passwordHash){
    // Port defaults to 8266
    ArduinoOTA.setPort(port);
    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname(chipID);
    // Password set with it's md5 value
    ArduinoOTA.setPasswordHash(passwordHash);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_FS
            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {   
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {    
        Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
}



void espSimpleOTA_Loop(){
    ArduinoOTA.handle();
}

#endif /* ESPSIMPLEOTA_H */
