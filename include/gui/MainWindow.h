#ifndef QT01_PLOTWINDOW_H
#define QT01_PLOTWINDOW_H

#define PLD_qtwidget

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QFileDialog>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrinter>
#include <QtGui/QMessageBox>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include "SpectralData.h"
#include "ColorInformationDock.h"
#include "ColorChooseDock.h"
#include "qt.h"

class PlotWindow : public QMainWindow
{
    Q_OBJECT
private:
    bool stdObserverSpectrumActived, normalizeResultSpectrum;
    int colorIndex1, colorIndex2;

    SpectralData *spectralIn;
    SpectralData resultSpectrum;

    ColorChooseDock *colorChooseDock;

    void createDocks();
    QDockWidget *leftDock;
    ColorInformationDock *colorInfoDock;

    void createMenus();
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    /* Private function related to plot */
    void plotSpectralData(int color1, int color2);
    void plotStdObserverFunction();
    void drawLegends();
    void plotColoursSystems();
    void updateResultColor();

public:
    /// Constructor for the window, holding the widget parameters
    /// \param width Window width (independant of the plot size)
    /// \param height Window height
    /// \param saveWidth Plot actual width. Used to compute the aspect ratio on screen and the size in PS and SVG outputs \see resolution
    /// \param saveHeight Plot actual height.
    /// \param resolution Print resolution
    /// \param pageSize Print size
    /// \param orientation Print orientation
    /// \param parent Parent widget, if any.
    PlotWindow( int argc = 0, char** argv = NULL, QWidget* parent = NULL );
    ~PlotWindow();

    /// Set the input SpectralData object
    /// \param spectralIn SpectralData object
    void setSpectralData(SpectralData *spectralIn);

public slots:
    void interactive();
    void changeColor1(int color);
    void changeColor2(int color);
    void changeIlluminant(QString new_illuminant);
    void updatePlot();
    void switchStdObsFuncVisibility();
    void quit();
    void helpAbout();
    void switchNormalizeResultSpectrum();

protected:
    PLINT       strm;
    QtExtWidget *plotColorSpectrum;
    QtExtWidget *plotStdObserverFunctionWidget;

};

#endif
