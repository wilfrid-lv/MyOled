/**
    
    @file MyOledViewWifiAp.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyOledViewWifiAp.h"

void MyOledViewWifiAp::SetNomDuSysteme(std::string value){
    NomDuSysteme = value;
}

void MyOledViewWifiAp::SetSsIdDuSysteme(std::string value){
    ssIdDuSysteme= value;
}

void MyOledViewWifiAp::SetPassDuSysteme(std::string value){
    passDuSysteme= value;
}
