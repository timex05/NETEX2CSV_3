#include <iostream>
#include "stopplace.h"

void _STOPPLACE::print(void)
{
    // dies ist der Quellcode f�r die Bildschirmausgabe der gespeicherten Daten
    // einer selbstdefinierten _STOPPLACE struct Variablen

    cout << "ID=" << this->ID
            << "\t"
            << "IFOPT=" << this->IFOPT
            << "\t"
            << "NAME=" << this->NAME
            << "\t"
            << "PublicCode=" << this->PublicCode
            << "\t"
            << "lon=" << this->lon
            << "\t"
            << "lat=" << this->lat
            << "\t"
            << "alt=" << this->alt

            << std::endl;
}
