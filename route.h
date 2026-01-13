#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

#include "network.h"

#include <list>
#include <string>

using namespace std;

struct _ROUTE {

    // Attribute
    std::string NeTEX_ID;
    std::string Name;

    std::list<struct _QUAY> QuaySeq;

    // Methoden

    // standard-Konstruktor
    _ROUTE();

    // sonstiges
    void print_on_console(void);
};

#endif // ROUTE_H_INCLUDED
