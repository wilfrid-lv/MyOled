/**
    Gestion d'une VUE pour le OLed : Base pour les écrans de travail
    @file MyOledViewWorking.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyOledViewInitialisation.h"

using namespace std;

void MyOledViewInitialisation::SetNomDuSysteme(std::string value){
    NomDuSysteme = value;
}

void MyOledViewInitialisation::SetIdDuSysteme(std::string value){
    idDuSysteme= value;
}


void MyOledViewInitialisation::SetSensibiliteBoutonAction(std::string value){
    sensibilisationButtonAction = value;
}


void MyOledViewInitialisation::SetSensibiliteBoutonReset(std::string value){
    sensibilisationButtonReset = value;
}

void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewInitialisation");
    }

void MyOledViewInitialisation::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewInitialisation");
    adafruit->setTextColor(WHITE);

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(NomDuSysteme.c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(idDuSysteme.c_str());
    adafruit->display();
    }