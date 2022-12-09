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

WiFiClient client;
HTTPClient http;
WiFiManager wm;
#define WEBSERVER_H
#define woodApiRestAdress "http:/51.79.68.103:3000/api/woods/getAllWoods"

MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char * SSID = "SAC_";
const char * PASSWORD = "sac_";
String ssIDRandom;

int temps = 0;
//temperature min du four
int tempGoal = 26;
//temperature max du four
int tempMax = tempGoal+(tempGoal*0.1);

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
char strTempFour[100];


//Definition des éléments de l'ecran OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C // Adresse I2C de l'écran Oled
#define OLD_VEILLE_SECONDES 30  // Temps avant mise en veille de l'ecran Oled

MyOled *myOled = new MyOled(&Wire, OLED_RESET, SCREEN_HEIGHT, SCREEN_WIDTH);
        
MyOledViewInitialisation *myOledViewInitialisation = NULL;
MyOledViewWorkingOFF *myOledViewWorkingOFF = NULL;
MyOledViewWifiAp *myOledViewWifiAp = NULL;
MyOledViewWorkingCOLD *myOledViewWorkingCOLD = NULL;
MyOledViewWorkingHEAT *myOledViewWorkingHEAT = NULL;


//#define nomDuSysteme "SAC System"
string idDuSysteme = "92834";
string ssIdDuSysteme = "43829";
string nomDuSysteme = "SAC System";
string etatDuSysteme = "Heating";
string passDuSysteme = "Patate";
string JsonListeBois;
string JsonLeBois;
int nbSecondes = 10;        
char lesSecondes[100];
float tempDemander = 24; 
int tempsSechage = 10;
int four = 0; //0=OFF ,1=HEAT ,2=COLD


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
        sprintf(strTempFour, "%4.1f C", tempDuFour);
        return(strTempFour);
    }
    return "";

  if (string(actionToDo.c_str()).compare(string("changement")) == 0) 
  {
      if(string(arg1.c_str()).compare(string("getTemp")) == 0) 
      {
        tempGoal = atoi(arg2.c_str());
        return(String("Ok").c_str());
      }
  }
    // Recupère la liste des bois.
      if (string(actionToDo.c_str()).compare(string("askListeWood")) == 0) {
          http.begin(client, woodApiRestAdress);
          http.GET();
          JsonListeBois = http.getString();
          http.end();
          return(JsonListeBois.c_str()); }

  // Recupère les informations d'un bois.
      if (string(actionToDo.c_str()).compare(string("afficherBois")) == 0) {
          char buffer[100];
          sprintf(buffer, "http:/51.79.68.103:3000/api/woods/getAllWoods/%S", arg1.c_str());
          http.begin(client, buffer);
            
  http.addHeader("Authorization","eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2Njg3ODUyMjF9.jhT6LpcaUyx5w0gXGldjC9TZxymvArrzvPt6GG2WukM");
          http.GET();
          JsonLeBois = http.getString();
          http.end();
          DynamicJsonDocument doc(2048);
          deserializeJson(doc,JsonLeBois);
          for(JsonObject elem : doc.as<JsonArray>()){
            tempDemander = elem["temperature"];
            tempsSechage = elem["dryingTime"];
            nbSecondes = tempsSechage;
          }
          return(JsonLeBois.c_str()); } 
 }

 void RefreshMyOledParams(){
  switch(four){
      case 0:
        //s'affiche avant que l'on appuie sur le bouton démarrage du four
        myOledViewWorkingOFF = new MyOledViewWorkingOFF();
        myOledViewWorkingOFF->setParams("nomDuSysteme",nomDuSysteme.c_str());
        myOledViewWorkingOFF->setParams("idDuSysteme",idDuSysteme.c_str());
        myOledViewWorkingHEAT->setParams("strTempFour",strTempFour);
        myOledViewWorkingOFF->setParams("IpDuSysteme",WiFi.localIP().toString().c_str());
        myOledViewWorkingOFF->setParams("StatusDuSysteme","System OK");
        myOled->displayView(myOledViewWorkingOFF);
        break;

      case 1:
        sprintf(strTempFour,"%g",tempDuFour);
        myOledViewWorkingHEAT = new MyOledViewWorkingHEAT();
        myOledViewWorkingHEAT->setParams("nomDuSysteme",nomDuSysteme.c_str());
        myOledViewWorkingHEAT->setParams("idDuSysteme",idDuSysteme.c_str());
        myOledViewWorkingHEAT->setParams("strTempFour",strTempFour);
        myOledViewWorkingHEAT->setParams("IpDuSysteme",WiFi.localIP().toString().c_str());
        myOledViewWorkingHEAT->setParams("StatusDuSysteme","System OK");
        myOled->displayView(myOledViewWorkingHEAT);
        break;

      case 2:
        myOledViewWorkingCOLD = new MyOledViewWorkingCOLD();
        myOledViewWorkingCOLD->setParams("nomDuSysteme",nomDuSysteme.c_str());
        myOledViewWorkingCOLD->setParams("idDuSysteme",idDuSysteme.c_str());
        myOledViewWorkingCOLD->setParams("IpDuSysteme",WiFi.localIP().toString().c_str());
        myOledViewWorkingCOLD->setParams("StatusDuSysteme","System OK");
        myOled->displayView(myOledViewWorkingCOLD);
        break;
  }
}

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

  //initialisation myOled
  myOled->init(OLED_I2C_ADDRESS);
  myOled->veilleDelay(30); //En secondes
  myOledViewInitialisation = new MyOledViewInitialisation();
  myOledViewInitialisation->setNomDuSysteme(nomSysteme);
  myOledViewInitialisation->setIdDuSysteme(idSysteme.c_str());
  myOledViewInitialisation->setSensibiliteBoutonAction(sensibiliteBoutonStart);
  myOledViewInitialisation->setSensibiliteBoutonReset(sensibiliteBoutonReset);
  myOled->displayView(myOledViewInit);


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

 //WifiAp s'affiche si il ne trouve pas le nom du réseau
  myOledViewWifiAp = new MyOledViewWifiAp();
  myOledViewWifiAp->SetNomDuSysteme(nomDuSysteme);
  myOledViewWifiAp->SetSsIdDuSysteme(ssIdDuSysteme.c_str());
  myOledViewWifiAp->SetPassDuSysteme(passDuSysteme.c_str());
  myOled->displayView(myOledViewWifiAp);
      
 for (int i = 0; i < 2; i++){
  digitalWrite(GPIO_PIN_LED_OK_VERT, HIGH);
  delay(200);
  digitalWrite(GPIO_PIN_LED_OK_VERT, LOW);
  delay(100);
  digitalWrite(GPIO_PIN_LED_OK_VERT, HIGH);
  delay(200);
  digitalWrite(GPIO_PIN_LED_OK_VERT, LOW);
}

 digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,HIGH);

}

 
void loop() {
  tempDuFour = temperatureStub->getTemperature();
  //int i;

  Serial.print("Température : ");
  Serial.println(tempDuFour);
  delay(5000);

  // int buttonAction = myButtonAction->checkMyButton();
  // //mettre le bouton démarrage lorsque l'on pourra le récupérer et led qui clignote quand systeme affiche ready
  // if(buttonAction > 2){

  // digitalWrite(GPIO_PIN_LED_OK_VERT, HIGH);
  //        delay(200);
  //        digitalWrite(GPIO_PIN_LED_OK_VERT, LOW);
  //        delay(100);
  //        digitalWrite(GPIO_PIN_LED_OK_VERT, HIGH);
  //        delay(200);
  //        digitalWrite(GPIO_PIN_LED_OK_VERT, LOW);
 //}

int buttonAction = myButtonAction->checkMyButton();
int temperature = temperatureStub->getTemperature();
if(buttonAction > 2){
  if(temperature > tempGoal && temperature < tempMax){
    four = 1;
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,LOW);
    digitalWrite(GPIO_PIN_LED_OK_VERT,HIGH);

    sprintf(lesSecondes, "%i secondes.", nbSecondes);
            nbSecondes--;
            if(nbSecondes == 0){
                buttonAction = 1;
                nbSecondes = tempsSechage;
                four = 0;
                digitalWrite(GPIO_PIN_LED_OK_VERT,LOW);
                digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,HIGH);
            }
  }
  if(temperature < tempGoal){
    four = 2;
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,LOW);
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
    nbSecondes = tempsSechage;
  }
  if(temperature > tempMax){
    four = 2;
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE,LOW);
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE,HIGH);
    nbSecondes = nbSecondes;
  }
}
delay(2000);
}
