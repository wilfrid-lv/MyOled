/**
    
    @file MyOledViewWifiAp.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyOledViewWifiAp.h"

void MyOledViewWifiAp::SetNomDuSysteme(std::string value){
    NomDuSysteme = value;
}

void MyOledViewWifiAp::SetSsIdDuSysteme(std::string value){
    ssIdDuSysteme= value;
}

void MyOledViewWifiAp::SetPassDuSysteme(std::string value){
    passDuSysteme= value;
}

void MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWifiAp");
    }

void MyOledViewWifiAp::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWifiAp");
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(NomDuSysteme.c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("ssId: ");
    adafruit->print(ssIdDuSysteme.c_str());
    
    adafruit->display();
    }
