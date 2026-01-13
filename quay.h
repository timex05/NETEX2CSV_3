#ifndef QUAY_H_INCLUDED
#define QUAY_H_INCLUDED

#include <string>

using namespace std;

struct _QUAY {

    // Attribute
    string ID;
    string NAME;
    string IFOPT;
    double lon;
    double lat;

    double alt;

    // Methoden

    // Ausgabe der gespeicherten Daten auf der Kommandozeile / Shell
    void print(void);

    // Standard-Konstruktor-Methode
    _QUAY();  // wird immer ausgef�hrt, wenn eine struct _QUAY Variablen-Deklaration erfolgt

    // ein �berladener Konstruktor f�r das struct _QUAY
    _QUAY(string _ID, string _NAME, string _IFOPT, double _lon, double _lat, double _alt);

};

#endif // QUAY_H_INCLUDED
