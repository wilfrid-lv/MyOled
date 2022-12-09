/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWorkingCold.cpp
    @author Alain Dubé
            Wilfrid Le Valégant
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyOledViewWorkingCold.h"

using namespace std;

void MyOledViewWorkingCOLD::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWifiCold");
    }

void MyOledViewWorkingCOLD::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWorkingCOLD");
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(getTag("nomDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(getTag("idDuSysteme").c_str());
    
    // adafruit->setCursor(40, 50);
    // adafruit->print(getTag("ipDuSysteme").c_str());
    
    adafruit->display();
    }