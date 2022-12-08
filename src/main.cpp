#include <iostream>
#include <Wire.h>
#include <String>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "myFunctions.cpp" //Fonctions utilitaires
#include <HTTPClient.h>
#include <WiFiManager.h>
#include "TemperatureStub.h"
#include "MyServer.h" //Pour la gestion du serveur ESP32
#include "MyOled.h"
#include "MyOledViewInitialisation.h"
#include "MyOledViewWorking.h"
#include "MyOledViewWorkingOFF.h"
#include "MyOledViewWifiAp.h"
#include "MyOledViewWorkingCOLD.h"
#include "MyOledViewWorkingHEAT.h"

using namespace std;

WiFiManager wm;
#define WEBSERVER_H

MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char * SSID = "SAC_";
const char * PASSWORD = "sac_";
String ssIDRandom;

int temps = 0;

// Définition des LEDs
#define GPIO_PIN_LED_LOCK_ROUGE 12 // Led Rouge GPIO12
#define GPIO_PIN_LED_OK_VERT 14 // Led Verte GPIO14
#define GPIO_PIN_LED_HEAT_JAUNE 27 // Led Jaune 27
 
// Définition des boutons
#include "MyButton.h"
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;
 
// Définition senseur température
#define DHTPIN  15   // Pin utilisée par le DHT22
#define DHTTYPE DHT22  //Le type de senseur utilisé
TemperatureStub *temperatureStub = NULL;

float tempDuFour = 20;
char buffer[100];

MyOledViewInitialisation *myOledViewInitialisation = NULL;
MyOledViewWorkingOFF *myOledViewWorkingOFF = NULL;
MyOledViewWifiAp *myOledViewWifiAp = NULL;
MyOledViewWorkingCOLD *myOledViewWorkingCOLD = NULL;
MyOledViewWorkingHEAT *myOledViewWorkingHEAT = NULL;

//Definition des éléments de l'ecran OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C // Adresse I2C de l'écran Oled
#define OLD_VEILLE_SECONDES 30  // Temps avant mise en veille de l'ecran Oled

std::string CallBackMessageListener(string message) {
   while(replaceAll(message, std::string("  "), std::string(" ")));
  //Décortiquer le message
  string actionToDo = getValue(message, ' ', 0);
  string arg1 = getValue(message, ' ', 1);
  string arg2 = getValue(message, ' ', 2);
  string arg3 = getValue(message, ' ', 3);
  string arg4 = getValue(message, ' ', 4);
  string arg5 = getValue(message, ' ', 5);
  string arg6 = getValue(message, ' ', 6);
  string arg7 = getValue(message, ' ', 7);
  string arg8 = getValue(message, ' ', 8);
  string arg9 = getValue(message, ' ', 9);
  string arg10 = getValue(message, ' ', 10);

//   if (string(actionToDo.c_str()).compare(string("button")) == 0) {
//     if(string(arg1.c_str()).compare(string("getTemp")) == 0) {
//       temps = atoi(arg2.c_str());
//       return(String("Ok").c_str());
//     }
//   }
//   std::string result = "";
//   return result;
    // std::string nomDuFour = "Four9394";
    // if (string(actionToDo.c_str()).compare(string("askNomFour")) == 0) {
    // return(nomDuFour.c_str()); }
    
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
        sprintf(buffer, "%4.1f C", tempDuFour);
        return(buffer);
    }
    return "";
 }

 //#define nomDuSysteme "SAC System"
 string idDuSysteme = "92834";
 string ssIdDuSysteme = "43829";
 string nomDuSysteme = "SAC System";
 string passDuSysteme = "Patate";


void setup() {
  Serial.begin(9600);
  
  //Connection au WifiManager
  String ssIDRandom, PASSRandom;
  String stringRandom;
  stringRandom = get_random_string(4).c_str();
  ssIDRandom = SSID;
  ssIDRandom = ssIDRandom + stringRandom;
  stringRandom = get_random_string(4).c_str();
  PASSRandom = PASSWORD;
  PASSRandom = PASSRandom + stringRandom;

  char strToPrint[128];
  sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom.c_str(), PASSRandom.c_str());
  Serial.println(strToPrint);

  if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())) {
    Serial.println("Erreur de connexion.");

  } else {
    Serial.println("Connexion Établie.");
  }

  // ----------- Routes du serveur ----------------
  myServer = new MyServer(80);
  myServer -> initAllRoutes();
  myServer->initCallback(&CallBackMessageListener);

  // Initialisation des LEDs
  pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);
  pinMode(GPIO_PIN_LED_OK_VERT, OUTPUT);
  pinMode(GPIO_PIN_LED_HEAT_JAUNE, OUTPUT);
 
  //Initialisation des boutons
  myButtonAction = new MyButton();
  myButtonAction->init(T8);
  int sensibilisationButtonAction = myButtonAction->autoSensibilisation();
 
  myButtonReset = new MyButton();
  myButtonReset->init(T9);
  int sensibilisationButtonReset = myButtonReset->autoSensibilisation();
 
  //Initiation du senseur de température
  temperatureStub = new TemperatureStub;
  temperatureStub->init(DHTPIN, DHTTYPE);


  MyOled *myOled = new MyOled(&Wire, OLED_RESET, SCREEN_HEIGHT, SCREEN_WIDTH);
        myOled->init(OLED_I2C_ADDRESS);
        myOled->veilleDelay(30); //En secondes

myOledViewInitialisation = new MyOledViewInitialisation();
  myOledViewInitialisation->SetNomDuSysteme(nomDuSysteme);
  myOledViewInitialisation->setParams("idDuSysteme",idDuSysteme.c_str());
  //myOledViewInitialisation->setParams("myButtonAction",myButtonAction.c_str());
  //myOledViewInitialisation->setParams("myButtonReset",myButtonReset.c_str());
  //myOled->displayView(myOledViewInitialisation);
  //mettre un delay

myOledViewWifiAp = new MyOledViewWifiAp();
  myOledViewWifiAp->SetNomDuSysteme(nomDuSysteme);
  myOledViewWifiAp->SetSsIdDuSysteme(ssIdDuSysteme.c_str());
  myOledViewWifiAp->SetPassDuSysteme(passDuSysteme.c_str());
  myOled->displayView(myOledViewWifiAp);
  //mettre un delay

//lancer un timer 
 myOledViewWorkingOFF = new MyOledViewWorkingOFF();
  myOledViewWorkingOFF->setParams("nomDuSysteme",nomDuSysteme.c_str());
  myOledViewWorkingOFF->setParams("idDuSysteme",idDuSysteme.c_str());
  myOledViewWorkingOFF->setParams("IpDuSysteme",WiFi.localIP().toString().c_str());
  myOledViewWorkingOFF->setParams("StatusDuSysteme","System OK");
  //myOled->displayView(myOledViewWorkingOFF);

//mettre le timer en pause
myOledViewWorkingCOLD = new MyOledViewWorkingCOLD();
  myOledViewWorkingCOLD->setParams("nomDuSysteme",nomDuSysteme.c_str());
  myOledViewWorkingCOLD->setParams("idDuSysteme",idDuSysteme.c_str());
  myOledViewWorkingCOLD->setParams("IpDuSysteme",WiFi.localIP().toString().c_str());
  myOledViewWorkingCOLD->setParams("StatusDuSysteme","System OK");
  //myOled->displayView(myOledViewWorkingCOLD);

//mettre le timer en pause
myOledViewWorkingHEAT = new MyOledViewWorkingHEAT();
  myOledViewWorkingHEAT->setParams("nomDuSysteme",nomDuSysteme.c_str());
  myOledViewWorkingHEAT->setParams("idDuSysteme",idDuSysteme.c_str());
  myOledViewWorkingHEAT->setParams("IpDuSysteme",WiFi.localIP().toString().c_str());
  myOledViewWorkingHEAT->setParams("StatusDuSysteme","System OK");
  //myOled->displayView(myOledViewWorkingHEAT);
}
 
void loop() {
  tempDuFour = temperatureStub->getTemperature();
  int i;

  Serial.print("Température : ");
  Serial.println(tempDuFour);
  delay(2000);
 
  //Gestion du bouton Action
  // int buttonAction = myButtonAction->checkMyButton();
  // if(buttonAction > 2)  {
  //   if(t > 25)
  //   {
  //     Serial.println("Vous avez appuyé sur le bouton d'action ");
  //     for (i = 0; i < 5; i++)
  //     {
  //       digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, HIGH);
  //       delay(500);
  //       digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
  //       delay(500);
  //     }
  //   }
  //   else{
  //     digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, HIGH);
  //     delay(3000);
  //     digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, LOW);
  //   }
  // }

  // if ((myOled->veilleCheck()) cout << "\nEst en veille"){
  //   myOled->veilleCheck();
  //           myOled->updateCurrentView(myOledViewWorking); //Pour les animations dans la vue si utilisées

  // }
}