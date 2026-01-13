#ifndef _NETWORK_H_INCLUDED
#define _NETWORK_H_INCLUDED

#include "stopplace.h"      // muessen wir einbinden, um die Stopplace-Liste im struct _NETWORK verwenden zu k�nnen
#include "quay.h"           // muessen wir einbinden, um die Quay-Liste im struct _NETWORK verwenden zu k�nnen
#include "route.h"          // muessen wir einbinden, um die Linienverl�ufe mit struct ROUTE zu speichern
#include "pugixml.hpp"      // muessen wir einbinden, um PUGIXML nutzen zu k�nnen

#include <string>
#include <list>

using namespace std;

struct _NETWORK {

    // Attribute

    // Liste der NeTEx-Dateinamen, die zu durchsuchen sind
    list<string> DateiNamen;

    // Liste zum Speichern der gefundenen STOPPLACES
    list<struct _STOPPLACE> StopPlaces;

    // Liste zum Speichern der gefundenen QUAYs
    list<struct _QUAY> Quays;

    // Liste zum Speichern der gefundenen Linienverl�ufe
    list<struct _ROUTE> Routes;

    // METHODEN

    // Default-Konstruktor
    _NETWORK();

    // Speichern der Datei-Namen der NeTEx-Dateien
    void SpecifyNeTExFiles(string _FILENAME);

    // Lesen der Inhalte der XML-Datei mit dem Namen _XMLFILENAME
    void AnalyzeNeTExFile(string _XMLFILENAME);

    // Routine/Funktion, die pr�ft, ob der StopPlace SP schon in der Liste this->StopPlaces enthalten ist
    int CheckIfStopplaceExists(pugi::xml_node _SP);

    // Routine/Funktion, die pr�ft, ob der Quay _Q schon in der Liste this->Quays enthalten ist
    int CheckIfQuayExists(pugi::xml_node _Q);

    // Funktion zur Ausgabe der gespeicherten Stopplace
    void PrintStopplaceListOnSCreen(void);

    // Funktion zur Ausgabe der Quays in der einer txt-Datei
    void ExportQuayInformation(string _FILENAME);

    // Funktion zur R�ckgabe von StopPlace-Daten bei vorliegender IFOPT-Nummer
    struct _STOPPLACE GetStopPlaceDataByIfopt(string _SPIFOPT);

    // Funktion zur Ausgabe aller gespeicherten Routen / Linienverl�ufe auf der Kommandozeile
    void print_routes_on_console(void);
    void ExportStopPlacesForContour(string filename);
    void ExportRoutesForContour(string filename);
};

#endif // _NETWORK_H_INCLUDED
