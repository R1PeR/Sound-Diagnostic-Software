#ifndef CHART_H
#define CHART_H

#include <QPainter>
#include <math.h>
#include <vector>

#define MX      40
#define MY      40
#define LOGMAX  50
//#define M_PI    3.14159

enum ChartMode {
    LinearChart,
    LogChart,
    SemiLogChart
};

class Chart
{
public:
    Chart(int specH);
    void drawLogGrid(QPainter &painter, QRect geometry);
    void drawLogData(QPainter &painter, QVector<double> &data);
    void drawSemiLogData(QPainter &painter, QVector<double> &data);
    void drawLinearGrid(QPainter &painter, QRect geometry);
    void drawLinearData(QPainter &painter, QVector<double> &data);
    void drawBarsData(QPainter &painter, QVector<double> &data);
    void drawSpectrumData(QPainter &painter);
    void showMarkers(QPainter &painter, QVector<double> &data);
    void showTimeWindow(QPainter &painter, QVector<double> &data);
    void getMousePosition(int x, int y);
    void fillSpect(int x, std::vector<double> arr);
    double interpolate( double val, double y0, double x0, double y1, double x1 );
    double base( double val );
    double red( double val );
    double green( double val );
    double blue( double val );
    QColor JetColor(float v,float vmin,float vmax);
    int gridNumX=10, gridNumY=10;
    int minValueX=0, maxValueX=100;
    int minValueY=-100, maxValueY=100;
    int chartMode=0;
    double markerX=1, markerY=0;
    int dataSize;
    int graphPos = 0;
    int maxSpect = 200;
    QImage spect;

    QColor backgroundColor=Qt::black;
    QColor gridColor=Qt::gray;
    QColor textColor=Qt::white;
    QColor plotColor=Qt::cyan;
    QColor markersColorA=Qt::red;
    QColor markersColorB=Qt::yellow;

private:
    double dx, dy, dvx, dvy, dmkx, dmky;
    int gx, gy, gw, gh, gmy;
    double logTable[LOGMAX];
    bool repaint = false;

};

#endif // CHART_H
