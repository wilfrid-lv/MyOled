
/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
using namespace std;

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;


//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
    }

void MyServer::initAllRoutes() { 
    currentTemperature = 3.3f;

    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/home.html", "text/html");
    });

    //Route pour l'image
     this->on("/sac.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/sac.png", "image/png");
    });

    //Route du script JavaScript
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    this->on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (ptrToCallBackFunction) (*ptrToCallBackFunction)("test "); 
    });

    this->on("/getNomEsp", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askNomFour");
        String lireNomDuFour =String(repString.c_str());
        request->send(200, "text/plain", lireNomDuFour );
    });

    //mettre dans un intervalle pour la temperature
    this->on("/lireTemp", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTempFour");
        String lireTempDuFour =String(repString.c_str());
        request->send(200, "text/plain", lireTempDuFour );
    });

   //route si celle tapé n'existe pas
    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
    });

    this->begin();

    this->on("/getTemp", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        AsyncWebParameter* p = request->getParam(0);
        char buffer[1024];

        sprintf(buffer, "%s %s", "changement getTemp", p->value().c_str());

        if (ptrToCallBackFunction) (*ptrToCallBackFunction) (buffer);
        
    });

    // Recupère la liste des bois
    this->on("/getListeWood", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askListeWood");
        DynamicJsonDocument doc(2048);
        deserializeJson(doc,repString);
        String lesBois;
        for(JsonObject elem : doc.as<JsonArray>()){
            String woodName = elem["name"];
            lesBois += woodName + String(";");
        }
        request->send(200, "text/plain", lesBois);
        });
    
    // Recupère les information d'un Bois
    this->on("/afficherBois", HTTP_POST, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        String nomBois = request->getParam("nomBois", true)->value();
        char buffer[100];
        sprintf(buffer, "afficherBois %S", nomBois);
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)(buffer);
        DynamicJsonDocument doc(2048);
        deserializeJson(doc,repString);
        String leBois;
        for(JsonObject elem : doc.as<JsonArray>()){  
            String woodName = elem["name"];
            String woodType = elem["type"];
            String woodOrigine = elem["origin"];
            String woodDryingTime = elem["dryingTime"];
            String woodTemperature = elem["temperature"];
            leBois += woodName + String(";") +  woodType + String(";") + woodOrigine + String(";") + woodDryingTime + String(";") + woodTemperature + String(";");
        }
        request->send(200, "text/plain", leBois);
        });
 
};

//Appeler l API
// HTTPClient http;
// String woodApiRestAdress = "51.79.68.103:3000/api/woods/getAllWoods/"; //adresse web de l API
// woodApiRestAdress += wood; //nom de la BD
// http.begin(woodApiRestAdress);
// http.addHeader("Authorization", "");

// http.addHeader("Content-Type", "application/x-www-form-urlencoded");

// int httpResponseCode = http.GET();
// Serial.print("HTTP Response code");
// Serial.println(httpResponseCode);

// String response;
// if(httpResponseCode){
//     response = http.getString();
//     Serial.println(response);
// }
// http.end();

// DynamicJsonDocument doc(2048);
// deserializeJson(doc, response);
// String tempToSend;
// for (JsonObject elem : doc.as<JsonArray>()){
//     String woodName         = elem["name"];
//     String woodType         = elem["type"];
//     String woodOrigin       = elem["origin"];
//     String woodDryingTime   = elem["dryingTime"];
//     String woodTemperature  = elem["temperature"];
// }
    
 