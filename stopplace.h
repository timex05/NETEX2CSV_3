#ifndef STOPPLACE_H_INCLUDED
#define STOPPLACE_H_INCLUDED

#include <string>

using namespace std;

struct _STOPPLACE {

    // Attribute
    string ID;
    string NAME;
    string IFOPT;
    string PublicCode;
    double lon;
    double lat;

    double alt;

    // Methoden
    void print(void);
};

#endif // STOPPLACE_H_INCLUDED
