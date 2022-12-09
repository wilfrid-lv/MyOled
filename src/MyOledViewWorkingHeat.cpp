/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWorkingHeat.cpp
    @author Alain Dubé
            Wilfrid Le Valégant
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyOledViewWorkingHeat.h"

using namespace std;

void MyOledViewWorkingHEAT::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWifiHeat");
    }

void MyOledViewWorkingHEAT::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWorkingHEAT");
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(getTag("nomDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(getTag("idDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(60, 20);
    adafruit->print(getTag("etatDuSysteme").c_str());

    adafruit->setTextSize(2);
    adafruit->setCursor(30, 40);
    adafruit->print(getTag("strTempFour").c_str());
    
    adafruit->display();
    }