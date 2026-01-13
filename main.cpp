#include <iostream>
#include "network.h"
#include "pugixml.hpp"

using namespace std;



int main(int argc, char *argv[])
{
    std::cout << "argc=" << argc << std::endl;

    // ausgabe der Kommandozeilen Parameter
    for(int i=0 ; i < argc ; i++ )
    {
        std::cout << "i=" << i << "\t" << argv[i] << std::endl;
    }

    // Deklaration der zentralen Variablen zum Speichern aller relevanten Daten/Informationen
    struct _NETWORK MyNetwork;

    // F�llen der Liste der NeTEx-Datei-Namen
    MyNetwork.SpecifyNeTExFiles(argv[2]);

    // Analysiere nacheinander alle gespeicherten NeTEx-Dateien
    list<string>::iterator IT;
    for( IT= MyNetwork.DateiNamen.begin() ; IT != MyNetwork.DateiNamen.end() ; IT++ )
    {
        cout << "Analysiere NeTEx-Datei " << *IT << endl;
        MyNetwork.AnalyzeNeTExFile(*IT);
    }

    // Gebe die duplikatsfreie Stopplace liste am Bildschirm aus
    MyNetwork.PrintStopplaceListOnSCreen();

    // Gebe die Daten der gefundenen Routen auf dem Bildschirm aus
    MyNetwork.print_routes_on_console();

    // exportiere die gefundenen Information im Format der Aufgabenstellung
    MyNetwork.ExportQuayInformation(argv[4]);

    MyNetwork.ExportStopPlacesForContour(argv[6]);
    MyNetwork.ExportRoutesForContour(argv[8]);

    return 0;
}
