#include "SpectralData.h"

SpectralData::SpectralData() {
    stdObserverXbar = new QMap<int, double>();
    stdObserverYbar = new QMap<int, double>();
    stdObserverZbar = new QMap<int, double>();

    this->I = NULL;

    readStandardObserverFromFile("data/cie_standard_observer_2deg.in");
    readIlluminants("data/illuminants.in");

    current_illuminant = "D50";

}

// TODO: This is a great opportunity to bugs!
SpectralData::~SpectralData() {
    clearAllSpectralData();
}

void SpectralData::setCurrentIlluminant(string illuminant){
    this->current_illuminant = illuminant;
}

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

void SpectralData::readIlluminants(const char *filePath) {
    ifstream inFile;

    qDebug() << "Reading readIlluminant file: " << filePath;
    inFile.open(filePath);

    if (!inFile) {
        qDebug() << "Unable to open file " << filePath;
        exit(1); // call system to stop
    }

    illuminants.clear();

    string line, name;
    int wavelength, length;
    double data;
    int state = 0;

    while(getline(inFile, line)) {
        //TODO: Just comments in the begin of the line are supported
        if(line[0] == '#') continue; // ignore lines starting with #

        istringstream is(line);
        switch (state) {
        case 0: is >> name >> length;
            illuminants.insert(name, new QMap <int, double> );
            state = 1;
            break;
        case 1:
            is >> wavelength >> data;
            illuminants[name]->insert(wavelength, data);
            length--;
            if(!length) {
                // finish reading the current illuminant, must way for
                // another
                state = 0;
            }
            break;

        }

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
        QString qcolorname(colorname.c_str());
        this->color_name.push_back(qcolorname);

        for (int j = 0; j < ncols; j++)
            inFile >> this->I[i][j];
    }

    inFile.close();
}


void SpectralData::clearColorData() {
    //free spectrum data
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

void SpectralData::clearAllSpectralData() {
    qDebug() << "freeSpectralData ()";
    clearColorData();
    string key;
    foreach(key, illuminants.keys()) {
        illuminants[key]->clear();
        delete illuminants[key];
    }
    illuminants.clear();
    color_name.clear();
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

        N += (ybar) *
             this->illuminants[current_illuminant]->value(wavelength)/100 *
             (this->step);
    }

    double sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;

    for(uint i = 0; i < ncols; i++) {
        wavelength = this->startWaveLength + i * (this->step);

        double xbar = this->stdObserverXbar->value(wavelength);
        double ybar = this->stdObserverYbar->value(wavelength);
        double zbar = this->stdObserverZbar->value(wavelength);

        sum_x += this->I[colorIndex][i] * xbar * (this->step) *
                 this->illuminants[current_illuminant]->value(wavelength);
        sum_y += this->I[colorIndex][i] * ybar * (this->step) *
                 this->illuminants[current_illuminant]->value(wavelength);
        sum_z += this->I[colorIndex][i] * zbar * (this->step) *
                 this->illuminants[current_illuminant]->value(wavelength);
    }

    sum_x /= N;
    sum_y /= N;
    sum_z /= N;

    double x, y;
    x = sum_x/(sum_x+sum_y+sum_z);
    y = sum_y/(sum_x+sum_y+sum_z);

    //    Y = (sum_y>10.0)?sum_y:10.0;      /* Luminance with a floor in 10% */
     Y = sum_y;
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

    // Tristimulus Reference to D65
    double Xw, Yw, Zw;
    tristimulusFromCurrentIlluminant(Xw, Yw, Zw);

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

void SpectralData::convertToCIELab( int colorIndex,
                                    double &L, double &a, double &b) {
    double X, Y, Z;
    convertToCIEXYZ(colorIndex, X, Y, Z);

    double Xw, Yw, Zw; //tristimulus from current illuminant
    tristimulusFromCurrentIlluminant(Xw, Yw, Zw);

    double xn = X/Xw;
    double yn = Y/Yw;
    double zn = Z/Zw;

    double fx = (xn>0.008856451586)?pow(xn,1./3):7.787036979*xn+16.0/116;
    double fy = (yn>0.008856451586)?pow(yn,1./3):7.787036979*yn+16.0/116;
    double fz = (zn>0.008856451586)?pow(zn,1./3):7.787036979*zn+16.0/116;

    L = (yn>0.008856451586 )?116*pow(yn,1./3)-16: 903.2963058*yn;
    a = 500.*( fx - fy );
    b = 200.*( fy - fz );
}

void SpectralData::convertToCIELuv(int colorIndex,
                                   double &L, double &u, double &v) {

    double X, Y, Z;
    convertToCIEXYZ(colorIndex, X, Y, Z);

    double Xw, Yw, Zw;
    // Get tristimulus from current illuminant
    tristimulusFromCurrentIlluminant(Xw, Yw, Zw);

    // Lu'v'
    u = (4 * X) / (X + (15 * Y) + (3 * Z) );
    v = (6 * Y) / (X + (15 * Y) + (3 * Z) );

    // Lu'v'
    u = (4 * X) / (X+15*Y+3*Z);
    v = (9 * Y) / (X + 15 * Y+ 3 * Z);

    /* Gattas Implementation */
    double ref_X = Xw;
    double ref_Y = Yw;
    double ref_Z = Zw;

    double ref_U = ( 4 * ref_X ) / ( ref_X + ( 15 * ref_Y ) + ( 3 * ref_Z ) );
    double ref_V = ( 9 * ref_Y ) / ( ref_X + ( 15 * ref_Y ) + ( 3 * ref_Z ) );

    double var_U = ( 4 * X ) / ( X + ( 15 * Y ) + ( 3 * Z ) );
    double var_V = ( 9 * Y ) / ( X + ( 15 * Y ) + ( 3 * Z ) );
    double var_Y = Y / Yw;


    if ( var_Y > 0.008856 ) var_Y = pow(var_Y,( 1./3 ));
    else var_Y = (( 7.787 * var_Y ) + ( 16./ 116 ));


    L = ( 116 * var_Y ) - 16;
    u = 13 * L * ( var_U - ref_U );
    v = 13 * L * ( var_V - ref_V );
}

void SpectralData::tristimulusFromCurrentIlluminant(double &Xw, double &Yw, double &Zw){
    if(current_illuminant == "D50") {
        Xw = 95.047;
        Yw = 100.00;
        Zw = 108.883;

    } else if(current_illuminant == "D65") {
        Xw = 96.422;
        Yw = 100.000;
        Zw = 82.521;
    }
}
