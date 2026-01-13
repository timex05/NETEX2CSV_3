#include "route.h"
#include <iostream>
#include <list>

using namespace std;

_ROUTE::_ROUTE()
{
    this->Name = "undefined route";
    this->NeTEX_ID = "n.n.";

    this->QuaySeq.clear();
}

void _ROUTE::print_on_console(void)
{
    // Die Stammdaten
    std::cout << this->NeTEX_ID
        << "\t"
        << this->Name
        << "\t"
        << "besuchte Quays: "
        << this->QuaySeq.size()
        << std::endl;

    // die Quay-Sequenz
    std::list<struct _QUAY>::iterator QuayIT;
    for( QuayIT = this->QuaySeq.begin() ; QuayIT != this->QuaySeq.end() ; QuayIT++ )
    {
        std::cout << "\t" << QuayIT->IFOPT << std::endl;
    }
}
