/**
    Class MyOledViewInitialisation : Initialiser données nécessaire pour l'affichage
    @file MyOledViewInitialisation.h 
    @author Wilfrid Le Valégant
    @version 1.1 25/11/22 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      24/11/22   Wilfrid     Première version de la classe

**/
#ifndef MYOLEDVIEWINITIALISATION_H
#define MYOLEDVIEWINITIALISATION_H
#include "MyOledView.h"
#include <Adafruit_SSD1306.h>
#include <string>


class MyOledViewInitialisation : public MyOledView {
    public:
        void SetNomDuSysteme(std::string value);
        void SetIdDuSysteme(std::string value);
        void SetSensibiliteBoutonAction(std::string value);
        void SetSensibiliteBoutonReset(std::string value);

    private:
        std::string NomDuSysteme;
        std::string idDuSysteme;
        std::string sensibilisationButtonAction;
        std::string sensibilisationButtonReset;
        void update(Adafruit_SSD1306 *adafruit);
        void display( Adafruit_SSD1306 *adafruit);

};
#endif
