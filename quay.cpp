#include <iostream>
#include "quay.h"

// Hinweis: durch die Vorsile _QUAY:: werden die Funktionen an das Objekt _QUAY gekoppelt

_QUAY::_QUAY()
{
    // dies ist der Quellcode des STandard-Konstruktors f�r das struct _QUAY
    // beachten Sie, dass weder ein Eingabe-Kanal noch ein R�ckgabedatentyp spezifiziert werden darf
}

_QUAY::_QUAY(string _ID, string _NAME, string _IFOPT, double _lon, double _lat, double _alt)
{
    // dies ist der Quellcode eines �berladenen-Konstruktors f�r das struct _QUAY
    // beachten Sie, dass kein R�ckgabedatentyp spezifiziert werden darf
    // im Gegensatz zum Standard-Konstruktor werden hier aber verschiedene Eingabe-Kan�le definiert,
    // �ber die Informationen an die Funktion �bergeben werden m�ssen

    this->ID = _ID;
    this->NAME = _NAME;
    this->IFOPT = _IFOPT;
    this->lon = _lon;
    this->lat = _lat;

    this->alt = _alt;
}

void _QUAY::print(void)
{
    // dies ist der Quellcode f�r die Bildschirmausgabe der gespeicherten Daten
    // einer selbstdefinierten _QUAY struct Variablen

    cout << "ID=" << this->ID
            << "\t"
            << "IFOPT=" << this->IFOPT
            << "\t"
            << "NAME=" << this->NAME
            << "\t"
            << "lon=" << this->lon
            << "\t"
            << "lat=" << this->lat
            << "\t"
            << "alt=" << this->alt
            << std::endl;
}
