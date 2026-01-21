#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define byte win_byte_override

#include <iostream>
#include <fstream>
#include <sstream>

#undef byte

#include "network.h"
#include "pugixml.hpp"

#include "curl/curl.h"
#include "cJSON/cJSON.h"

#include <map>


// Callback-Funktion zum Schreiben der empfangenen Daten in einen std::string
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}



double elevation(double lat, double lon)
{
    // curl initialisieren
    CURL* curl = curl_easy_init();
    // Fehler überprüfen
    if (!curl) return -1.0;

    // url initialisieren
    std::ostringstream url;
    url << "https://api.open-elevation.com/api/v1/lookup?locations="
        << lat << "," << lon;

    std::string response;

    // url setzen
    curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
    // write callback setzen
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    // response variable setzen
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    // User-Agent setzen
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "elevation-client/1.0");
    // Timeout setzen
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    // SSL Prüfung deaktivieren
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Ergebnis Variable
    double value = -1.0;

    // 3 Versuche
    for (int i = 0; i < 3; ++i) {
        response.clear(); // alte Response löschen
        CURLcode res = curl_easy_perform(curl); // Abfrage durchführen
        // Fehler überprüfen
        if (res != CURLE_OK) {
            std::cerr << "curl error: " << curl_easy_strerror(res) << std::endl; // Fehler Ausgabe
            Sleep(3000); // Pause 3 Sekunden
            continue; // Nächster Loop
        }
        cJSON* root = cJSON_Parse(response.c_str()); // Response in JSON parsen
        // Fehler überprüfen
        if (!root) {
            std::cerr << "JSON parse error\n"; // Fehler Ausgabe
            Sleep(3000); // Pause 3 Sekunden
            continue; // Nächster Loop
        }
        // Daten aus JSON extrahieren
        cJSON* results = cJSON_GetObjectItem(root, "results");
        cJSON* first = cJSON_GetArrayItem(results, 0);
        cJSON* elevation = cJSON_GetObjectItem(first, "elevation");
        if (cJSON_IsNumber(elevation)) {
            value = elevation->valuedouble; // Ergebnis setzen
            cJSON_Delete(root); // JSON Objekt löschen
            break; // Erfolgreich, Schleife abbrechen
        }
        cJSON_Delete(root); // JSON Objekt löschen
        Sleep(3000); // Pause 3 Sekunden
    }

    // curl aufräumen
    curl_easy_cleanup(curl);
    return value;
}



_NETWORK::_NETWORK()
{
    // leeren der Dateinamen-Liste
    this->DateiNamen.clear();

    // leeren der Stopplace-Liste
    this->StopPlaces.clear();

    // leeren der Quay-Liste
    this->Quays.clear();

    // leeren der ROute-Liste
    this->Routes.clear();
}

void _NETWORK::SpecifyNeTExFiles(string _FILENAME)
{
    // analyse der Datei die mit der Kommandozeilenoption -N festgelegt wurde

    // deklariere eine String-Variable f�r den Datei-Namen
    std::string EingabeDateiName = _FILENAME;

    // �ffnen der der Textdatei
    std::fstream MyFile;        // file handler

    MyFile.open(EingabeDateiName.c_str(),ios::in);

    std::string MyLine;

    // Zeilenweises auslesen der Datei
    while ( getline (MyFile,MyLine) )
    {
      cout << "gelesene Zeile " << MyLine << '\n';
      this->DateiNamen.push_back(MyLine);
    }
    // schlie�en der Textdatei
    MyFile.close();

}

void _NETWORK::AnalyzeNeTExFile(string _XMLFILENAME)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);



    // Variable XMLFILE vom Typ pugi::xml_document zum Speichern des XML-Attributs-Baums wird deklariert
    pugi::xml_document XMLFILE;

    // �ffnen der �bergebenen NeTEx-Datei
    XMLFILE.load_file(_XMLFILENAME.c_str());

    // Variable zum Speichern der ersten Stopplace-Instanz in _XMLFILENAME wird deklariert
    pugi::xml_node FIRST_STOPPLACE;

    // Navigiere zum ersten Stopplace in _XMLFILENAME und Speichere diesen in FIRST_STOPPLACE
    FIRST_STOPPLACE = XMLFILE.child("PublicationDelivery").child("dataObjects").child("CompositeFrame").child("frames").child("SiteFrame").child("stopPlaces").child("StopPlace");

    // Laufe nacheinander durch alle gespeicherten Stopplace-Instanzen in _XMLFILENAME
    for( pugi::xml_node SP=FIRST_STOPPLACE ; SP ; SP = SP.next_sibling() )
    {
        // Speichern der ID des Stopplaces
        string ID = SP.attribute("id").as_string();
        // Speichern des Names des Stopplaces
        string SPName = SP.child_value("Name");
        // Speichern der IFOPT2-Nummer des STopplaces
        string IFOPT = SP.child("keyList").child("KeyValue").child_value("Value");
        // Speichern des L�ngengrads (atof wandelt string in double um)
        double SPLon = atof(SP.child("Centroid").child("Location").child_value("Longitude"));
        // Speichern des Breitengrads (atof wandelt string in double um)
        double SPLat = atof(SP.child("Centroid").child("Location").child_value("Latitude"));

        double SPAlt = elevation(SPLat, SPLon);

        // Speichern des PublicCodes
        string SPPublicCode = "---";    // set the default value
        if( (string)(SP.child_value("PublicCode")) != "" )
        {
            SPPublicCode = (string)(SP.child_value("PublicCode"));
        }
        if( this->CheckIfStopplaceExists(SP) < 1 )
        {
            // fuege SP in die Liste der Stopplaces ein
            cout << "Stopplace mit der ID "
                << SP.attribute("id").as_string()
                << "\t"
                << SPName
                << "\t"
                << IFOPT
                << " wird gespeichert" << endl;

            struct _STOPPLACE AuxiliaryStopPlace;
            AuxiliaryStopPlace.ID = ID;
            AuxiliaryStopPlace.IFOPT = IFOPT;
            AuxiliaryStopPlace.lat = SPLat;
            AuxiliaryStopPlace.lon = SPLon;
            AuxiliaryStopPlace.NAME = SPName;
            AuxiliaryStopPlace.PublicCode = SPPublicCode;
            AuxiliaryStopPlace.alt = SPAlt;

            this->StopPlaces.push_back(AuxiliaryStopPlace);
        }

        // Erg�nzen Sie hier
        // Durchlaufen Sie alle zum aktuellen Stopplace geh�renden Quays


        // ... und hier die for-Schleife, die �ber alle zu SP zugeh�rigen Quay-Instanzen l�uft
        for( pugi::xml_node Q=SP.child("quays").child("Quay") ; Q ; Q = Q.next_sibling() )
        {
            if( this->CheckIfQuayExists(Q) < 1 )
            {
                // speichere notwendige Werte
                string QID = Q.attribute("id").as_string();
                string QNAME = Q.child_value("Name");

                // iterate over all stored key-value-attributes to identify key GLobalID
                string QIFOPT = "";
                for( pugi::xml_node KV=Q.child("keyList").child("KeyValue") ; KV ; KV = KV.next_sibling() )
                {
                    if( (std::string)((KV.child_value("Key"))) == "GlobalID" )
                        {
                            QIFOPT = KV.child_value("Value");
                        }
                }

                double QLon = atof(Q.child("Centroid").child("Location").child_value("Longitude"));
                double QLat = atof(Q.child("Centroid").child("Location").child_value("Latitude"));

                double QAlt = elevation(QLat, QLon);

                // Ausgabe der gefundenen Werte am Bildschirm
                std::cout << "Quay "
                << QID
                << "\t"
                << QNAME
                << "\t"
                << QIFOPT
                << " muss gespeichert werden" << std::endl;

                // Speichern des Quays in this->Quays
                struct _QUAY AuxiliaryQuay;
                AuxiliaryQuay.ID = QID;
                AuxiliaryQuay.IFOPT = QIFOPT;
                AuxiliaryQuay.lat = QLat;
                AuxiliaryQuay.lon = QLon;
                AuxiliaryQuay.NAME = QNAME;

                AuxiliaryQuay.alt = QAlt;

                this->Quays.push_back(AuxiliaryQuay);
            }

        }
    }

    // Identifikation der gespeicherten Linienverl�ufe

    // First, Store the list of lines
    struct _ShortLine {
        std::string Name;
        std::string Id;
    };

    std::list<struct _ShortLine> ShortLines;
    ShortLines.clear();

    // Navigiere zur ersten Line in _XMLFILENAME und Speichere diesen in FIRST_LINE
    pugi::xml_node FIRST_LINE;
    FIRST_LINE = XMLFILE.child("PublicationDelivery").child("dataObjects").child("CompositeFrame").child("frames").child("ServiceFrame").child("lines").child("Line");

    for( pugi::xml_node LINE=FIRST_LINE ; LINE ; LINE = LINE.next_sibling() )
    {
        struct _ShortLine TmpLine;

        TmpLine.Id = LINE.attribute("id").as_string();
        TmpLine.Name = LINE.child_value("PublicCode");

        std::cout << "found Line " << TmpLine.Id << ": " << TmpLine.Name << std::endl;
        ShortLines.push_back(TmpLine);
    }

    // second, store the PassengerStopAssignment entities
    struct _ShortPSA {
        std::string id;
        std::string SSPRef;
        std::string QuayRef;
    };

    std::list<struct _ShortPSA> ShortPSAs;
    ShortPSAs.clear();

    // Navigiere zum ersten PSA in _XMLFILENAME und Speichere diesen in FIRST_PSA
    pugi::xml_node FIRST_PSA;
    FIRST_PSA = XMLFILE.child("PublicationDelivery").child("dataObjects").child("CompositeFrame").child("frames").child("ServiceFrame").child("stopAssignments").child("PassengerStopAssignment");

    for( pugi::xml_node PSA=FIRST_PSA ; PSA ; PSA = PSA.next_sibling() )
    {
        struct _ShortPSA TmpPSA;

        TmpPSA.id = PSA.attribute("id").as_string();
        TmpPSA.QuayRef = PSA.child("QuayRef").attribute("ref").as_string();
        TmpPSA.SSPRef = PSA.child("ScheduledStopPointRef").attribute("ref").as_string();

        std::cout << "found PSA " << TmpPSA.id << ": " << TmpPSA.QuayRef << " / " << TmpPSA.SSPRef << std::endl;

        ShortPSAs.push_back(TmpPSA);
    }

    // third, store the ServiceJourneyPattern (SJP) entities

    // Navigiere zum ersten SJP in _XMLFILENAME und Speichere diesen in FIRST_SJP
    pugi::xml_node FIRST_SJP;
    FIRST_SJP = XMLFILE.child("PublicationDelivery").child("dataObjects").child("CompositeFrame").child("frames").child("ServiceFrame").child("journeyPatterns").child("ServiceJourneyPattern");

    for( pugi::xml_node SJP=FIRST_SJP ; SJP ; SJP = SJP.next_sibling() )
    {
        // create a new route entity
        struct _ROUTE TmpRoute;
        TmpRoute.QuaySeq.clear();

        TmpRoute.NeTEX_ID = SJP.attribute("id").as_string();

        // identified the route reference
        std::string RouteRef = SJP.child("RouteView").child("LineRef").attribute("ref").as_string();

        // find the associated Line information
        std::list<struct _ShortLine>::iterator ShortLineIT;
        for( ShortLineIT=ShortLines.begin() ; ShortLineIT !=ShortLines.end() ; ShortLineIT++)
        {
            if( (std::string)(ShortLineIT->Id) == RouteRef )
            {
                TmpRoute.Name = ShortLineIT->Name + " ("+ ShortLineIT->Id+")";
            }
        }

        // print to screen what we have so far
        std::cout << "\t" << TmpRoute.NeTEX_ID << "->" << TmpRoute.Name << std::endl;

        // identify the stops in the current Pattern SJP
        for( pugi::xml_node SPIJP= SJP.child("pointsInSequence").child("StopPointInJourneyPattern") ;
            SPIJP ;
            SPIJP = SPIJP.next_sibling() )
            {
                std::cout << "\t\t" << SPIJP.child("ScheduledStopPointRef").attribute("ref").as_string() << std::endl;

                std::string TmpQuayRef = "";
                std::string TmpQuayID = "";

                // welcher Quay geh�rt zum ScheduledStopPoint? = gucke in der PSA-Liste nach QuayRef
                std::list<struct _ShortPSA>::iterator ShortPSAIT;
                for( ShortPSAIT=ShortPSAs.begin() ; ShortPSAIT !=ShortPSAs.end() ; ShortPSAIT++)
                {
                    // std::cout << "AUX=" << ShortPSAIT->SSPRef << std::endl;
                    if( (std::string)(ShortPSAIT->SSPRef) == SPIJP.child("ScheduledStopPointRef").attribute("ref").as_string() )
                    {
                        TmpQuayRef = ShortPSAIT->QuayRef;
                        // Eintrag gefunden => gehe zum Ende der Liste (um Zeit zu sparen)
                        ShortPSAIT = std::prev(ShortPSAs.end());
                    }
                }

                std::cout << "\t\t\t" << "zug. QuayRef=" << TmpQuayRef << std::endl;
                // nun ermittle die IFOPT-Nummer des Quays, der zu TmpQuayRef geh�rt

                struct _QUAY NextStopPoint;

                std::list<struct _QUAY>::iterator QuayIT;
                for( QuayIT=this->Quays.begin() ; QuayIT != this->Quays.end() ; QuayIT++)
                {
                    if( (string)(QuayIT->ID) == TmpQuayRef )
                    {
                        NextStopPoint = *QuayIT;
                        QuayIT = std::prev(this->Quays.end());
                    }
                }
                std::cout << "\t\t\t\t" << "zug. Quay IFOPT-Nr=" << NextStopPoint.IFOPT << std::endl;

                // Hinzufuegen des Quays am Ende der Quay-Sequenz dieser Route
                TmpRoute.QuaySeq.push_back(NextStopPoint);

            }
        // append new route to List of routes
        this->Routes.push_back(TmpRoute);
    }
}

int _NETWORK::CheckIfStopplaceExists(pugi::xml_node _SP)
{
    // Deklariere Variable zur Speicherung des R�ckgabewertes
    int ReturnValue = 0;

    /*
    Diese Funktion durchsucht die Liste this->Stopplaces auf Vorhandensein von SP
    Wenn SP schon gespeichert ist => gebe den Wert 1 zur�ck
    Wenn SP noch nicht gespeichert ist => gebe den Wert 0 zur�ck

    Idee: Vergleiche f�r jeden gespeichter Stopplace S S.ID mit _SP.ID
    */

    // deklariere den Iterator f�r die STopplace-Liste
    std::list<struct _STOPPLACE>::iterator S;

    // durchlaufe nacheinander alle gespeicherten Listen-Elemente
    for( S=this->StopPlaces.begin() ; S != this->StopPlaces.end() ; S++)
    {
        // falls S.ID identisch zu _SP.ID ist dann merke ich mir das
        if( S->ID == _SP.attribute("id").as_string() )
        {
            ReturnValue = 1;
        }
    }

    // Rueckgabe des Wertes
    return( ReturnValue );
}

void _NETWORK::PrintStopplaceListOnSCreen(void)
{
    cout << "Liste der gespeicherten Stopplaces" << endl;
    // deklariere den Iterator f�r die STopplace-Liste
    std::list<struct _STOPPLACE>::iterator S;

    // durchlaufe nacheinander alle gespeicherten Listen-Elemente
    for( S=this->StopPlaces.begin() ; S != this->StopPlaces.end() ; S++)
    {
        cout << S->ID << "\t" << S->NAME << "\t" << S->IFOPT << endl;
    }
}

int _NETWORK::CheckIfQuayExists(pugi::xml_node _Q)
{
    // Deklariere Variable zur Speicherung des R�ckgabewertes
    int ReturnValue = 0;

    /*
    Diese Funktion durchsucht die Liste this->Quays auf Vorhandensein von _Q
    Wenn _Q schon gespeichert ist => gebe den Wert 1 zur�ck
    Wenn _Q noch nicht gespeichert ist => gebe den Wert 0 zur�ck

    Idee: Vergleiche f�r jeden gespeichter Stopplace Q Q.ID mit _Q.ID
    */

    // deklariere den Iterator f�r die Quay-Liste
    std::list<struct _QUAY>::iterator Q;

    // durchlaufe nacheinander alle gespeicherten Listen-Elemente
    for( Q=this->Quays.begin() ; Q != this->Quays.end() ; Q++)
    {
        // falls Q.ID identisch zu _Q.ID ist dann merke ich mir das
        if( Q->ID == _Q.attribute("id").as_string() )
        {
            ReturnValue = 1;
        }
    }

    // Rueckgabe des Wertes
    return( ReturnValue );
}

struct _STOPPLACE _NETWORK::GetStopPlaceDataByIfopt(string _SPIFOPT)
{
    // Die Procedure durchsucht die Liste der gespeicherten StopPlace-Instanzen this->StopPlaces
    // Falls sie ein StopPlace findet deren IFOPT mit dem dem Beginn von _SPIFOPT �bereinstimmt, gibt Sie diesen
    // zur�ck.
    // Falls sie keinen solchen StopPlace findet, sowird das Attribut ID des zur�ckgegebenen struct auf "" gesetzt.

    // Deklarieren der zur�ckzugebenen Variablen
    struct _STOPPLACE AuxStopPlace;
    AuxStopPlace.ID = "";

    // Teste sukzessive alle gespeicherten StopPlaces auf �bereinstimmung des Beginns der IFOPT-Nummer
    // Deklaration eines passenden Iterators
    std::list<struct _STOPPLACE>::iterator SPIT;

    // for-schleife �ber alle gespeicherten StopPlaces
    for( SPIT=this->StopPlaces.begin() ; SPIT != this->StopPlaces.end() ; SPIT++ )
    {
        // gucken, ob die �bereinstimmung am Beginn startet
        if( _SPIFOPT.find(SPIT->IFOPT) == 0 )
        {
            AuxStopPlace = *SPIT;
        }
    }

    // R�ckgabe der gefundenen Information(en)
    return(AuxStopPlace);
};

void _NETWORK::ExportQuayInformation(string _FILENAME)
{
    // �ffnen der der Textdatei
    std::fstream MyFile;        // file handler
    MyFile.open(_FILENAME.c_str(),ios::out);

    // Deklariere einen Iterator, der �ber alle gespeicherten Quay-Instanzen l�uft
    std::list<struct _QUAY>::iterator QIT;

    // Laufe durch alle gespeicherten Quay-Instanzen
    for( QIT=this->Quays.begin() ; QIT != this->Quays.end() ; QIT++ )
    {
        // suche zugeh�rige StopPlace-Instanz in this->StopPlaces
        struct _STOPPLACE SP = this->GetStopPlaceDataByIfopt(QIT->IFOPT);

        // Ausgabe
        if( SP.ID != "")
        {
            MyFile << SP.ID
            << "\t"
            << SP.IFOPT
            << "\t"
            << SP.NAME
            << "\t"
            << SP.lon
            << "\t"
            << SP.lat
            << "\t"
            << SP.alt
            << "\t"
            << SP.PublicCode
            << "\t"
            << QIT->ID
            << "\t"
            << QIT->NAME
            << "\t"
            << QIT->IFOPT
            << "\t"
            << QIT->lon
            << "\t"
            << QIT->lat
            << "\t"
            << QIT->alt
            << std::endl;
        }
        else
        {
            MyFile << ""
            << "\t"
            << ""
            << "\t"
            << ""
            << "\t"
            << ""
            << "\t"
            << ""
            << "\t"
            << ""
            << "\t"
            << QIT->ID
            << "\t"
            << QIT->NAME
            << "\t"
            << QIT->IFOPT
            << "\t"
            << QIT->lon
            << "\t"
            << QIT->lat
            << "\t"
            << QIT->alt
            << std::endl;
        }
    }

    // Schlie�en der Datei
    MyFile.close();
}

void _NETWORK::print_routes_on_console(void)
{
    // Deklaration eines Iterators
    std::list<struct _ROUTE>::iterator RouteIT;

    // Durchlaufen aller Routen
    for( RouteIT = this->Routes.begin() ; RouteIT != this->Routes.end() ; RouteIT++ )
    {
        RouteIT->print_on_console();
    }
}


void _NETWORK::ExportStopPlacesForContour(string filename)
{
    ofstream file(filename);
    if(!file.is_open()) {
        cout << "Error opening file " << filename << "\n";
        return;
    }

    for (const auto& sp : this->StopPlaces) {
        file << sp.lon << " "
             << sp.lat  << " "
             << sp.alt  << "\n";
    }
    file.close();
}

void _NETWORK::ExportRoutesForContour(string filename)
{
    std::map<std::string, _ROUTE*> bestRoutes; // Linie -> längste Route
    std::list<struct _ROUTE>::iterator it;

    for (it = this->Routes.begin(); it != this->Routes.end(); ++it)
    {
        std::string lineNumber = it->Name; // Name enthält Liniennummer + Name

        if (bestRoutes.find(lineNumber) == bestRoutes.end() || 
            it->QuaySeq.size() > bestRoutes[lineNumber]->QuaySeq.size())
        {
            bestRoutes[lineNumber] = &(*it);
        }
    }

    std::ofstream file(filename);
    if(!file.is_open()) {
        cout << "Error opening file " << filename << "\n";
        return;
    }

    std::map<std::string, struct _ROUTE*>::iterator mapIt;
    for (mapIt = bestRoutes.begin(); mapIt != bestRoutes.end(); ++mapIt)
    {
        std::list<struct _QUAY>::iterator quayIt;
        for (quayIt = mapIt->second->QuaySeq.begin();
             quayIt != mapIt->second->QuaySeq.end();
             ++quayIt)
        {
            file << quayIt->lon << " "
                 << quayIt->lat  << " "
                 << quayIt->alt  << "\n";
        }
        file << "\n"; // Leerzeile trennt Linien
    }


    file.close();
    std::cout << "Exported routes to " << filename << std::endl;
}
