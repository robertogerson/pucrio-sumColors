/*
 ============================================================================
 Name        : trabCor.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */
#include <math.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include <plplot/plplot.h>
#include "SpectralData.h"
#include "gui/MainWindow.h"

int main(int argc, char **argv) {
        if (argc <= 1) {
		cout << "Usage: ./trabCor inputFile" << endl;
		return 1;
	}

        SpectralData spectralIn;
        spectralIn.readFromFile(argv[1]);
        argc = 1;

        int res;

        QApplication a( argc, argv );
        PlotWindow   *win = new PlotWindow(argc, argv);
        win->setSpectralData(spectralIn);
        a.setActiveWindow( win );
        win->setVisible( true );
        res = a.exec();

        spectralIn.freeSpectralData();

	return 0;
}
