#include "SpectralData.h"

SpectralData::SpectralData() {
    stdObserverXbar = new QMap<int, double>();
    stdObserverYbar = new QMap<int, double>();
    stdObserverZbar = new QMap<int, double>();

    illuminant = new QMap<int, double>();
    illuminant2 = new QMap<int, double>();
    this->I = NULL;

    readStandardObserverFromFile("data/cie_standard_observer_2deg.in");
    readIlluminant("data/illuminant_d50.in");
//    readIlluminant2("data/illuminant_d50.in");

}

//TODO: This is a great opportunity to bugs!
//SpectralData::~SpectralData() {

//}

void SpectralData::readStandardObserverFromFile(const char *filePath) {
    ifstream inFile;

    qDebug() << "Reading StandardObserver file: " << filePath;
    inFile.open(filePath);
    if (!inFile) {
        qDebug() << "Unable to open file " << filePath;
        exit(1); //call system to stop
    }

    stdObserverXbar->clear();
    stdObserverYbar->clear();
    stdObserverZbar->clear();

    int wavelength;
    double xbar, ybar, zbar;
    while (inFile >> wavelength ) {
        inFile >> xbar >> ybar >> zbar;
        stdObserverXbar->insert(wavelength, xbar);
        stdObserverYbar->insert(wavelength, ybar);
        stdObserverZbar->insert(wavelength, zbar);
    }

    inFile.close();

}

void SpectralData::readIlluminant(const char *filePath) {
    ifstream inFile;

    qDebug() << "Reading readIlluminant file: " << filePath;
    inFile.open(filePath);

    if (!inFile) {
        qDebug() << "Unable to open file " << filePath;
        exit(1); //call system to stop
    }

    illuminant->clear();

    int wavelength;
    double data;
    while (inFile >> wavelength ) {
        inFile >> data;
        illuminant->insert(wavelength, data);
    }

    inFile.close();
}

void SpectralData::readIlluminant2(const char *filePath) {
    ifstream inFile;

    qDebug() << "Reading readIlluminant file: " << filePath;
    inFile.open(filePath);

    if (!inFile) {
        qDebug() << "Unable to open file " << filePath;
        exit(1); //call system to stop
    }

    illuminant2->clear();

    int wavelength;
    double data;
    while (inFile >> wavelength ) {
        inFile >> data;
        qDebug() << wavelength << data;
        illuminant2->insert(wavelength, data);
        qDebug() << illuminant2->value(wavelength);
    }

    inFile.close();
}

void SpectralData::readFromFile(const char *filePath) {
    ifstream inFile;
    inFile.open(filePath);

    if (!inFile) {
        qDebug() << "Unable to open file " << filePath;
        exit(1); //call system to stop
    }

    string cmd;
    int ncolors;

    qDebug() << "Reading input spectral file: '" << filePath << "'";
    //colorRange
    inFile >> cmd;
    inFile >> this->startWaveLength >> this->endWaveLength;
    //        qDebug() << this->startWaveLength << " " << this->endWaveLength
    //                        << endl;

    //step
    inFile >> cmd;
    inFile >> this->step;

    //n_colors
    inFile >> cmd;
    inFile >> ncolors;

    int ncols = (this->endWaveLength - this->startWaveLength)
                / this->step + 1;

    this->I = new double*[ncolors];
    for (int i = 0; i < ncolors; i++) {
        this->I[i] = new double[ncols];
    }

    for (int i = 0; i < ncolors; i++) {
        string colorname;
        inFile >> colorname;
        this->color_name.push_back(colorname);

        for (int j = 0; j < ncols; j++)
            inFile >> this->I[i][j];
    }

    inFile.close();
}

void SpectralData::freeSpectralData() {
    if(this->I != NULL) {
        for (uint i = 0; i < this->color_name.size(); i++) {
            if(this->I != NULL){
                delete this->I[i];
                this->I[i] = NULL;
            }
            delete this->I;
            this->I = NULL;
        }
    }
}

void SpectralData::convertToCIEXYZ( int colorIndex,
                                    double &X, double &Y, double &Z) {
    uint ncols = (this->endWaveLength - this->startWaveLength)
                 / this->step + 1;

    int wavelength = this->startWaveLength;

    X = Y = Z = 0.0;
    double N = 0.0;
    for (uint i = 0; i < ncols; i++) {
        wavelength = this->startWaveLength + i * (this->step);
        double ybar = this->stdObserverYbar->value(wavelength);

        N += (ybar) * this->illuminant->value(wavelength)/100 * (this->step);
    }

    double sum_x = 0.0, sum_y = 0.0, sum_z =0.0;

    for(uint i = 0; i < ncols; i++) {
        wavelength = this->startWaveLength + i * (this->step);

        double xbar = this->stdObserverXbar->value(wavelength);
        double ybar = this->stdObserverYbar->value(wavelength);
        double zbar = this->stdObserverZbar->value(wavelength);

        sum_x += this->I[colorIndex][i] * xbar * (this->step) * this->illuminant->value(wavelength);
        sum_y += this->I[colorIndex][i] * ybar * (this->step) * this->illuminant->value(wavelength);
        sum_z += this->I[colorIndex][i] * zbar * (this->step) * this->illuminant->value(wavelength);
    }

    sum_x /= N;
    sum_y /= N;
    sum_z /= N;

    double x, y;
    x = sum_x/(sum_x+sum_y+sum_z);
    y = sum_y/(sum_x+sum_y+sum_z);

    Y = (sum_y>10.0)?sum_y:10.0;      /* Luminance with a floor in 10% */
    X = (x/y)*(Y);
    Z = ((1-x-y)/y)*(Y);
}

void SpectralData::convertToCIExyY( int colorIndex,
                                    double &x, double &y, double &Y) {
    double X, Z;
    convertToCIEXYZ (colorIndex, X, Y, Z);

    x = X/(X+Y+Z);
    y = Y/(X+Y+Z);
}

void SpectralData::convertTosRGB( int colorIndex,
                                  int &R, int &G, int &B) {
    double X, Y, Z;
    convertToCIEXYZ(colorIndex, X, Y, Z);

    //normalizing X, Y, Z
    double r, g, b;
    r = (3.240 * X) - (1.537 * Y) - (0.499 * Z);
    g = (-0.969 * X) + (1.876 * Y) + (0.042 * Z);
    b = (0.056 * X) - (0.204 * Y) + (1.057 * Z);

    R = (r/100)*255;
    G = (g/100)*255;
    B = (b/100)*255;

    // X /= 100; Y /= 100; Z /= 100;
    double Xw = 95.047;
    double Yw = 100;
    double Zw = 108.883;

    double gamma = 1/2.4;
    //livro de gattas implementation
    //ps 1.0570 estava com 0.0570
    r = 3.2410*(X/Xw)-1.5374*(Y/Yw)-0.4986*(Z/Zw);
    g =-0.9692*(X/Xw)+1.8760*(Y/Yw)+0.0416*(Z/Zw);
    b = 0.0556*(X/Xw)-0.2040*(Y/Yw)+1.0570*(Z/Zw);

    /* r = 3.2406*X - 1.5372*Y - 0.4986*Z;
    g = -0.9689*X + 1.8758*Y - 0.0415*Z;
    b = 0.0557*X - 0.2040*Y + 1.0570*Z;*/

    //correcao gama
    r = (r<0.00304) ? 12.92*r : 1.055*pow(r, gamma)-0.055;
    g = (g<0.00304) ? 12.92*g : 1.055*pow(g, gamma)-0.055;
    b = (b<0.00304) ? 12.92*b : 1.055*pow(b, gamma)-0.055;

    R = r*255;
    if(R > 255) R = 255;
    else if (R < 0) R = 0;

    G = g*255;
    if(G > 255) G= 255;
    else if (G < 0) G = 0;

    B = b*255;
    if(B > 255) B = 255;
    else if (B < 0) B = 0;
}

void SpectralData::convertToRGB( int colorIndex,
                                 int &R, int &G, int &B) {
    double X, Y, Z;
    convertToCIEXYZ(colorIndex, X, Y, Z);

    double var_X = X / 100;        //X = From 0 to ref_X
    double var_Y = Y / 100;        //Y = From 0 to ref_Y
    double var_Z = Z / 100;        //Z = From 0 to ref_Y

    double r = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
    double g = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
    double b = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

    double gamma = 1/2.4;
    r = (r<0.00304) ? 12.92*r : 1.055*pow(r, gamma)-0.055;
    g = (g<0.00304) ? 12.92*g : 1.055*pow(g, gamma)-0.055;
    b = (b<0.00304) ? 12.92*b : 1.055*pow(b, gamma)-0.055;

    R = r*255;
    if(R > 255) R = 255;
    else if (R < 0) R = 0;

    G = g*255;
    if(G > 255) G = 255;
    else if (G < 0) R = 0;

    B = b*255;
    if(B > 255) B = 255;
    else if (B < 0) B = 0;
}
