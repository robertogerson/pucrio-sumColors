#ifndef SPECTRALDATA_H
#define SPECTRALDATA_H

#include <math.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#include <QDebug>

class SpectralData
{
public:
    int step;
    int startWaveLength, endWaveLength;
    double **I;
    vector<string> color_name;
    QMap <int, double> *stdObserverXbar;
    QMap <int, double> *stdObserverYbar;
    QMap <int, double> *stdObserverZbar;
    QMap <int, double> *illuminant, *illuminant2;

    SpectralData();
//    virtual ~SpectralData();

    void readFromFile(const char *fileName);
    void readStandardObserverFromFile(const char* fileName);
    void readIlluminant(const char* fileName);
    void readIlluminant2(const char* fileName);

    void freeSpectralData();

    void convertToCIEXYZ( int colorIndex, double &X, double &Y, double &Z);
    void convertToCIExyY( int colorIndex, double &x, double &y, double &Y);
    void convertTosRGB( int colorIndex, int &R, int &G, int &B);
    void convertToRGB( int colorIndex, int &R, int &G, int &B);
};

#endif // SPECTRALDATA_H
