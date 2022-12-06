/**
    Class MyOledViewWorkingOFF : Vue utilisée par MyOled (Affiche la page montrant le système en opération)
    @file MyOledViewWorkingCOLD.h 
    @author Alain Dubé, Le Valegant Wilfrid
    @version 1.1 21/09/20 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      21/09/20   Ald         Première version de la classe
                            1.2      01/12/22   Willv       Version WorkingCold
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
**/
#ifndef MYOLEDVIEWWORKINGCOLD_H
#define MYOLEDVIEWWORKINGCOLD_H
#include "MyOledView.h"
#include "MyOledViewWorking.h"
#include <string>

class MyOledViewWorkingCOLD: public MyOledViewWorking {
        
    
    protected:
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
};
#endif