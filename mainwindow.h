#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QPainter>
#include <QQueue>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QtMath>
#include "thread.h"
#include "chart.h"
#include "Gist.h"
#define DSIZE (128)
#define DBUFOR (100)
#define DSIZE2 (DSIZE/2)
#define AVGSIZE (64)
#define WINDOW 4
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
 Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    typedef struct  WAV_HEADER{
        char                RIFF[4];        // RIFF Header      Magic header
        unsigned long       ChunkSize;      // RIFF Chunk Size
        char                WAVE[4];        // WAVE Header
        char                fmt[4];         // FMT header
        unsigned long       Subchunk1Size;  // Size of the fmt chunk
        unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio
        unsigned long       SamplesPerSec;  // Sampling Frequency in Hz
        unsigned long       bytesPerSec;    // bytes per second
        unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
        unsigned short      bitsPerSample;  // Number of bits per sample
        char                Subchunk2ID[4]; // "data"  string
        unsigned long       Subchunk2Size;  // Sampled data length

    }wav_hdr;
    wav_hdr fileWav;
 ~MainWindow();
private slots:
 void externalThread_tick();
 void sendCommand();
 void on_actionOpen_triggered();
 void on_actionSave_triggered();
 void on_actionLine_triggered();
 void on_actionBar_triggered();
 void on_actionAverage_triggered();
 void on_actionOpenWAV_triggered();

private:
 Ui::MainWindow *ui;
 void paintEvent(QPaintEvent *event);
 Thread thread;
 QSerialPort serial;
 QByteArray senddata;
 QByteArray readdata;
 QByteArray data;
 unsigned int bitsPerSample;
 int16_t * dataPtr;
 QVector<double> timeData;
 QVector<double> meanData;
 QVector<double> avgData;
 Gist<double> * gist;
 Chart * chart;
 int position=0;
 int posData=0;
 int time = 0;
 int position2 = 0;
};
#endif // MAINWINDOW_H
