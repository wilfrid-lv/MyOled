/**
    Class MyOledViewWifiAp
    @file MyOledViewWifiAp.h 
    @author Wilfrid Le Valégant
    @version 1.1 25/11/22 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      24/11/22   Wilfrid     Première version de la classe

**/
#ifndef MYOLEDVIEWWIFIAP_H
#define MYOLEDVIEWWIFIAP_H
#include "MyOledView.h"
#include <Adafruit_SSD1306.h>
#include <string>

class MyOledViewWifiAp : public MyOledView {
    public:
        void SetNomDuSysteme(std::string value);
        void SetSsIdDuSysteme(std::string value);
        void SetPassDuSysteme(std::string value);
        void SetSensibiliteBoutonAction(std::string value);

    private:
        std::string NomDuSysteme;
        std::string ssIdDuSysteme;
        std::string passDuSysteme;
        void update(Adafruit_SSD1306 *adafruit);
        void display( Adafruit_SSD1306 *adafruit);
};
#endif