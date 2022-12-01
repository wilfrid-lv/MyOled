/**
    Class MyOledViewWorkingOFF : Vue utilisée par MyOled (Affiche la page montrant le système en opération)
    @file MyOledViewWorkingOFF.h 
    @author Alain Dubé, Le Valegant Wilfrid
    @version 1.1 21/09/20 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      21/09/20   Ald         Première version de la classe
                            1.2      01/12/22   Willv       Version WorkingOFF
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
**/
#ifndef MYOLEDVIEWWORKINGOFF_H
#define MYOLEDVIEWWORKINGOFF_H
#include "MyOledView.h"
#include "MyOledViewWorking.h"
#include <string>

class MyOledViewWorkingOFF: public MyOledViewWorking {
    public:
        void init(std::string _id);
        
    
    protected:
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        unsigned char *Fire24x24Pointers[6];

        int indexFire = 1;
        int indexFireDelay = 1;

        void displayGifFire(Adafruit_SSD1306 *adafruitint, int positionX, int positionY); 
};
#endif