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
#include <QtWebKit/QWebView>
#include <About.h>
#include "SpectralData.h"
#include "ColorInformationDock.h"
#include "ColorChooseDock.h"
#include "ViewConfiguration.h"
#include "qt.h"

class PlotWindow : public QMainWindow
{
    Q_OBJECT
private:
    QWebView view;
    About aboutWidget;
    bool normalizeResultSpectrum;

    int colorIndex1, colorIndex2;
    SpectralData *spectralIn;
    SpectralData resultSpectrum;

    ColorChooseDock *colorChooseDock;
    ViewConfiguration *viewConfiguration;

    void createDocks();
    QDockWidget *leftDock;
    ColorInformationDock *colorInfoDock;

    void createMenus();
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    /* Private function related to plot */
    void plotSpectralData(int color1, int color2);
    void plotStdObserverFunction();
    void plotIlluminantSpectrum();
    void plotXY(); void plotxy(); void plotab(); void plotuv(); void plotRG();
    void drawLegends();
    void updateResultColor();

    /* Visualization control */
    //showing spectrum User Control
    bool showStdObserverSpectrum, showColorSpectrum, showIlluminantSpectrum;

    //showing diagram User Control
    bool showXY, showxy, showab, showuv, showRG;

    /// set to default configuration, i.e., just spectrum of colors.
    void defaultViewConfiguration();

public slots:
    void setStdObserverVisibility(int v){ showStdObserverSpectrum=v; updatePlot(); }
    void setColorSpectrumVisibility(int v) { showColorSpectrum = v; updatePlot(); }
    void setIlluminantSpectrumVisibility(int v) { showIlluminantSpectrum = v; updatePlot(); }
    void setXYVisibility(int v) { showXY = v; updatePlot(); }
    void setxyVisibility(int v) { showxy = v; updatePlot(); }
    void setabVisibility(int v) { showab = v; updatePlot(); }
    void setuvVisibility(int v) { showuv = v; updatePlot(); }
    void setRGVisibility(int v) { showRG = v; updatePlot(); }

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
    void helpContents();
    void switchNormalizeResultSpectrum();

protected:
    PLINT       strm;
    QtExtWidget *plotColorSpectrum;

};

#endif
