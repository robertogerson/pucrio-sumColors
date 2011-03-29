#include "MainWindow.h"

PlotWindow::PlotWindow( int argc, char** argv, QWidget* parent ) :
        QMainWindow( parent ) {

    setAttribute( Qt::WA_DeleteOnClose );
    setWindowTitle("RGColorConverter");
    createMenus();
    createDocks();

    plotColorSpectrum = new QtExtWidget( QT_DEFAULT_X, QT_DEFAULT_Y, this );
    setCentralWidget( plotColorSpectrum );

    // One window = One plot widget = one stream
    plmkstrm( &strm );
    plsdev( "extqt" );

    // Get all the application arguments as argc, argv
    plsetqtdev( plotColorSpectrum );

    plinit();

    resize( 600, 600 );
    pladv( 0 );
    stdObserverSpectrumActived = false;
    normalizeResultSpectrum = false;

    setStyleSheet(QString("font-size: 11px;"));
    colorIndex1 = colorIndex2 = 0;
    resultSpectrum.color_name.push_back("result");
}

void PlotWindow::createDocks() {
    colorChooseDock = new ColorChooseDock(this);
    this->addDockWidget(Qt::LeftDockWidgetArea, colorChooseDock);
    connect( colorChooseDock->getListWidgetColor1(),
             SIGNAL(currentRowChanged(int)),
             this,
             SLOT(changeColor1(int)));

    connect( colorChooseDock->getListWidgetColor2(),
             SIGNAL(currentRowChanged(int)),
             this,
             SLOT(changeColor2(int)));

    colorInfoDock = new ColorInformationDock(this);
    this->addDockWidget(Qt::LeftDockWidgetArea, colorInfoDock);
    colorInfoDock->setMinimumWidth(350);
}

void PlotWindow::createMenus() {
    /* File Menu */
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Load Colours Spectrum...", this, SLOT(quit()));
    fileMenu->addAction("Load Illuminants Spectrum...", this, SLOT(quit()) );
    fileMenu->addAction("Load Observer Spectrum...", this, SLOT(quit()));
    fileMenu->addSeparator();
    fileMenu->addAction("Quit", this, SLOT(quit()) );

    /* View Menu */
    viewMenu = menuBar()->addMenu( "View");
    QAction *checkableAction =
            viewMenu->addAction( "Standard Observer Function",
                                 this, SLOT(switchStdObsFuncVisibility()));
    checkableAction->setCheckable(true);

    editMenu = menuBar()->addMenu("Preferences");
    checkableAction =
            editMenu->addAction( "Normalize Color Sum Spectrum",
                                 this, SLOT(switchNormalizeResultSpectrum()));
    checkableAction->setCheckable(true);

    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("Contents", this, SLOT(helpContents()));
    helpMenu->addAction("About", this, SLOT(helpAbout()));

}

PlotWindow::~PlotWindow() {
    PLINT cur_strm;
    plgstrm( &cur_strm );
    plsstrm( strm );
    plfreeqtdev(); //also deletes the device ("plot" variable) !
    plsstrm( cur_strm );

}

void PlotWindow::interactive() {
    PLFLT x, y;
    plotColorSpectrum->captureMousePlotCoords( &x, &y );

    char buf[100];
    sprintf( buf, "Selection: (%f, %f)", x, y );

    QMessageBox msgBox;
    msgBox.setText( buf );
    msgBox.exec();

}

void PlotWindow::changeColor1(int color) {
    colorIndex1 = color;
    updatePlot();
    double X, Y, Z;
    spectralIn.convertToCIEXYZ(color, X, Y, Z);
    colorInfoDock->setCIEXYZ(X, Y, Z);

    double x, y;
    spectralIn.convertToCIExyY(color, x, y, Y);
    colorInfoDock->setCIExyY(x, y, Y);

    int R, G, B;
    spectralIn.convertTosRGB(color, R, G, B);
    colorInfoDock->setsRGB(R, G, B);

    spectralIn.convertToRGB(color, R, G, B);
    colorInfoDock->setSampleColor(R, G, B);

    updateResultColor();
}

void PlotWindow::changeColor2(int color) {
    colorIndex2 = color;
    updatePlot();

    double X, Y, Z;
    spectralIn.convertToCIEXYZ(color, X, Y, Z);
    colorInfoDock->setCIEXYZ_2(X, Y, Z);

    double x, y;
    spectralIn.convertToCIExyY(color, x, y, Y);
    colorInfoDock->setCIExyY_2(x, y, Y);

    int R, G, B;
    spectralIn.convertTosRGB(color, R, G, B);
    colorInfoDock->setsRGB_2(R, G, B);

    spectralIn.convertToRGB(color, R, G, B);
    colorInfoDock->setSampleColor_2(R, G, B);

    updateResultColor();

}

void PlotWindow::updateResultColor(){
    int ncols = (spectralIn.endWaveLength - spectralIn.startWaveLength)
                / spectralIn.step + 1;

    for (uint i = 0; i < ncols; i++) {
        resultSpectrum.I[0][i] = spectralIn.I[colorIndex1][i] +
                                 spectralIn.I[colorIndex2][i];
    }

    // If the user selected to normalize the spectrum, divide the sum above by 2
    if(normalizeResultSpectrum) {
        for (uint i = 0; i < ncols; i++) {
            resultSpectrum.I[0][i] /= 2;
        }
    }

    double X, Y, Z;
    resultSpectrum.convertToCIEXYZ(0, X, Y, Z);
    colorInfoDock->setCIEXYZ_Res(X, Y, Z);

    double x, y;
    resultSpectrum.convertToCIExyY(0, x, y, Y);
    colorInfoDock->setCIExyY_Res(x, y, Y);

    int R, G, B;
    resultSpectrum.convertTosRGB(0, R, G, B);
    colorInfoDock->setsRGB_Res(R, G, B);

    resultSpectrum.convertToRGB(0, R, G, B);
    colorInfoDock->setSampleColor_Res(R, G, B);

    updatePlot();

}

void PlotWindow::switchStdObsFuncVisibility() {
    stdObserverSpectrumActived = !stdObserverSpectrumActived;
    updatePlot();
}

void PlotWindow::switchNormalizeResultSpectrum() {
    normalizeResultSpectrum = !normalizeResultSpectrum;
    updateResultColor();
}

void PlotWindow::updatePlot() {
    plotColorSpectrum->clearWidget();
    if(stdObserverSpectrumActived) {
        plssub(1, 2);
        plotStdObserverFunction();
    }
    else
        plssub(1, 1);
    plotSpectralData(colorIndex1, colorIndex2);

}

void PlotWindow::plotStdObserverFunction() {
    PLFLT xmin, xmax, ymin, ymax;
    int ncols = spectralIn.stdObserverXbar->keys().size();
    double x[ncols];
    double xbar[ncols], ybar[ncols], zbar[ncols];

    int key;
    int i = 0;
    foreach ( key, spectralIn.stdObserverXbar->keys() ) {
        x[i] = key;
        xbar[i] = spectralIn.stdObserverXbar->value(key);
        ybar[i] = spectralIn.stdObserverYbar->value(key);
        zbar[i] = spectralIn.stdObserverZbar->value(key);
        i++;
    }

    xmin = x[0];
    xmax = x[ncols-1];
    ymin = 0.0;
    ymax = 2.0;

    plcol0(1);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    plcol0(2);

    QString title("");
    title += "Standard Observer Function";
    pllab("(Wavelength)", "(Intensity)", title.toStdString().c_str());

    plcol0(15);
    // draw grid
    plbox( "g", 15.0, 0, "g", 0.1, 0 );

    /* Draw the line through the data */
    plcol0(1);
    plline(ncols, x, xbar);
    plcol0(3);
    plline(ncols, x, ybar);
    plcol0(9);
    plline(ncols, x, zbar);

}

void PlotWindow::plotSpectralData(int color1, int color2) {
    // Set up the data
    int i;
    PLFLT xmin, xmax, ymin, ymax;

    int ncols = (spectralIn.endWaveLength - spectralIn.startWaveLength)
                / spectralIn.step + 1;
    double x1[ncols], x2[ncols];
    double y1[ncols], y2[ncols];
    double xRes[ncols], yRes[ncols];

    //Get color for index color1
    if(color1 >= 0) {
        //  cout << "ncols = " << ncols << endl;
        for (i = 0; i < ncols; i++) {
            x1[i] = spectralIn.startWaveLength + i*(spectralIn.step);
            y1[i] = spectralIn.I[color1][i];
        }
        xmin = x1[0];
        xmax = x1[ncols-1]+1;
        ymin = 0.0;
        ymax = 2.0;
    }

    //Get color for index color2
    if(color2 >= 0) {
        for (i = 0; i < ncols; i++) {
            x2[i] = spectralIn.startWaveLength + i*(spectralIn.step);
            y2[i] = spectralIn.I[color2][i];
        }
        xmin = x2[0];
        xmax = x2[ncols-1]+1;
        ymin = 0.0;
        ymax = 2.0;
    }

    /* Set up the viewport and window using PLENV. The range in X is
     * 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
     * scaled separately (just = 0), and we just draw a labelled
     * box (axis = 0).
     */
    plcol0(1);
    plenv(xmin, xmax, ymin, ymax, 0, 0);

    QString title("");
    // title += spectralIn.color_name[color1].c_str();
    title += " WaveLenght Spectrum";
    pllab("(Wavelength)", "(Intensity)", title.toStdString().c_str());

    // draw grid
    plcol0( 15 );
    plbox( "g", 15.0, 0, "g", 0.1, 0 );

    /* Draw the line1, line2 and resulting spectrum through the data */
    if(color1 >= 0) {
        plcol0(2);
        plline(ncols, x1, y1);
    }

    if(color2 >= 0) {
        plcol0(3);
        plline(ncols, x2, y2);
    }

    if(color1 >= 0 && color2 >= 0){
        for (i = 0; i < ncols; i++) {
            xRes[i] = spectralIn.startWaveLength + i*(spectralIn.step);
            yRes[i] = resultSpectrum.I[0][i];
        }
        plcol0(1);
        plline(ncols, xRes, yRes);
    }

    drawLegends();
}

void PlotWindow::drawLegends(){
    /*** Draw Legends **/
    PLINT        nlegend = 3;
    PLINT        opt_array[3];
    PLINT        text_colors[3];
    PLINT        line_colors[3];
    PLINT        line_styles[3];
    PLINT        line_widths[3];
    PLINT        symbol_numbers[3], symbol_colors[3];
    PLFLT        symbol_scales[3];
    char         *symbols[3];
    PLFLT        legend_width, legend_height;

    opt_array[0]   = PL_LEGEND_LINE;
    text_colors[0] = 2;
    line_colors[0] = 2;
    line_styles[0] = 1;
    line_widths[0] = 1;
    // note from the above opt_array the first symbol (and box) indices
    // do not have to be specified

    // Second legend entry.
    opt_array[1]   = PL_LEGEND_LINE;
    text_colors[1] = 3;
    line_colors[1] = 3;
    line_styles[1] = 1;
    line_widths[1] = 1;

    // Third legend entry.
    opt_array[2]   = PL_LEGEND_LINE;
    text_colors[2] = 1;
    line_colors[2] = 1;
    line_styles[2] = 1;
    line_widths[2] = 1;

    // from the above opt_arrays we can completely ignore everything
    // to do with boxes.
    const char *legend_text[3];
    legend_text[0] = spectralIn.color_name[colorIndex1].c_str();

    legend_text[1] = spectralIn.color_name[colorIndex2].c_str();

    string color_sum = spectralIn.color_name[colorIndex1];
    color_sum += "+";
    color_sum += spectralIn.color_name[colorIndex2];
    legend_text[2] = color_sum.c_str();

    plscol0a( 15, 32, 32, 32, 0.70 );
    pllegend( &legend_width, &legend_height,
              0, PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX,
              0.0, 0.0, 0.10, 15,
              1, 1, 0, 0,
              nlegend, opt_array,
              1.0, 1.0, 2.0,
              1., text_colors, (const char **) legend_text,
              NULL, NULL, NULL, NULL,
              line_colors, line_styles, line_widths,
              symbol_colors, symbol_scales, symbol_numbers,
              (const char **) symbols);

}

void PlotWindow::setSpectralData(SpectralData in) {
    this->spectralIn  = in;
    colorChooseDock->clearColours();

    for(uint i = 0 ; i < this->spectralIn.color_name.size(); i++) {
        colorChooseDock->addColor(i, QString(
                this->spectralIn.color_name[i].c_str()
                ));
    }

    resultSpectrum.freeSpectralData();
    resultSpectrum.step = spectralIn.step;
    resultSpectrum.startWaveLength = spectralIn.startWaveLength;
    resultSpectrum.endWaveLength = spectralIn.endWaveLength;
    int ncols = (spectralIn.endWaveLength - spectralIn.startWaveLength)
                / spectralIn.step + 1;

    resultSpectrum.I = new double*[1];
    resultSpectrum.I[0] = new double[ncols];

    colorChooseDock->getListWidgetColor1()->setCurrentRow(0);
    colorChooseDock->getListWidgetColor2()->setCurrentRow(0);
}


void PlotWindow::helpAbout(){
    QApplication::aboutQt();
}

void PlotWindow::quit() {
    resultSpectrum.freeSpectralData();
    QApplication::quit();

}

