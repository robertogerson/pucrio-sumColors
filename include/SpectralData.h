#ifndef SPECTRALDATA_H
#define SPECTRALDATA_H

#include <math.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <QDebug>

class SpectralData
{
public:
    int step;
    int startWaveLength, endWaveLength;
    double **I;
    QVector <QString> color_name;
    QMap <int, double> *stdObserverXbar;
    QMap <int, double> *stdObserverYbar;
    QMap <int, double> *stdObserverZbar;

    //Map Illuminant Name -> Spectrum
    QMap < string, QMap <int, double> * > illuminants;
    string current_illuminant;

    SpectralData();
    virtual ~SpectralData();

    void readFromFile(const char *fileName);
    void readStandardObserverFromFile(const char* fileName);
    void readIlluminants(const char* fileName);

    void clearColorData();
    void clearAllSpectralData();

    // Functions to convertions among color spaces
    void convertToCIEXYZ( int colorIndex, double &X, double &Y, double &Z);
    void convertToCIExyY( int colorIndex, double &x, double &y, double &Y);
    void convertTosRGB( int colorIndex, int &R, int &G, int &B);
    void convertToRGB( int colorIndex, int &R, int &G, int &B);
    void convertToLab( int colorIndex, int &L, int &a, int &b);
    void convertToLuv( int colorIndex, int &L, int &u, int &v);
};

#endif // SPECTRALDATA_H
