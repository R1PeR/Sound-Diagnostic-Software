#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&thread, SIGNAL(tick()), this, SLOT(externalThread_tick()));
    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(sendCommand()));
    ui->actionRun->setEnabled(false);
    /*ui->statusBar->showMessage("No device");
    QString portname;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {

        if(info.serialNumber()=="NXP-77"){

            portname=info.portName();
            serial.setPortName(portname);
            if (serial.open(QIODevice::ReadWrite)) {
                serial.setBaudRate( serial.Baud115200,  serial.AllDirections);
                ui->statusBar->showMessage(tr("Device: %1").arg(info.serialNumber()));
                serial.clear();
                thread.start(thread.HighestPriority);
            }
            else
            {
                ui->statusBar->showMessage(tr("Can't open %1, error code %2") .arg(serial.portName()).arg(serial.error()));
                return;
            }
            break;
        }
    }*/
    chart = new Chart(DSIZE2*DBUFOR);
    chart->gridNumX=10;
    chart->gridNumY=10;
    chart->minValueX= 1.0;
    chart->maxValueX= DSIZE2*DBUFOR;
    chart->minValueY=-1.0;
    chart->maxValueY= 1.0;
    chart->chartMode=LinearChart;
    chart->dataSize=DSIZE2*DBUFOR;
    meanData.resize(DSIZE2*DBUFOR);
    meanData.fill(0.0);
    avgData.resize(DSIZE2*DBUFOR);
    avgData.fill(0.0);
    timeData.resize(DSIZE2*DBUFOR);
    timeData.fill(0.0);
    gist = new Gist<double>(DSIZE2*DBUFOR,44100,WindowType::HammingWindow);
    thread.start(thread.NormalPriority);
}

MainWindow::~MainWindow()
{
    thread.terminate();
    thread.wait();
    serial.close();
    delete ui;
    delete chart;
    delete gist;
}
void MainWindow::externalThread_tick()
{
    //if(!ui->actionRun->isChecked())
        //serial.clear();
    if(ui->actionRun->isChecked()){
        gist->processAudioFrame(timeData.toStdVector());
        chart->fillSpect(position2,gist->getMagnitudeSpectrum());
        position2++;
        if(position2 > DSIZE2*DBUFOR-1){
            position2 = 0;
        }
        timeData.remove(0,DSIZE2/4);
        for(int j=0; j<DSIZE2/4;j++){
            //if(position<DSIZE2*DBUFOR){
                //printf("%d %f\n",position,(*dataPtr)/65535.0);
                int16_t * sample = reinterpret_cast<int16_t *>((dataPtr + posData));
                //position++;
                //if(position > DSIZE2*DBUFOR-1){
                //    position = 0;
                //}


                timeData.append((*sample)/-32768.0);
                //timeData[position]=(*sample)/-32768.0;
                //printf("%f ",timeData[position]);
                posData++;
                if(posData > (fileWav.Subchunk2Size/2)-1){
                    posData = 0;
                }
                //printf("%d, %d\n",posData,fileWav.Subchunk2Size);
            /*}else{

            }*/
        }
        update();
    }
}

void MainWindow::sendCommand()
{
    //senddata.clear();
    //senddata[0]=static_cast<char>(128+(ui->actionTrigger->isChecked()<<6));
    //serial.write(senddata);
}
double clamp(double x, double upper, double lower){
    return fmin(upper, fmax(x, lower));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    chart->drawLinearGrid(painter, centralWidget()->geometry());
    if(ui->actionSpectro->isChecked()){
        chart->drawSpectrumData(painter);
    }
    if(ui->actionLine->isChecked()){
        chart->plotColor=Qt::red;    
        chart->drawLinearData(painter, timeData);

    }

    if(ui->actionBar->isChecked()){
        chart->plotColor=Qt::cyan;
        for(int i = 0; i < timeData.size()-AVGSIZE;i++){

            double sum = 0.0;
            for(int j = 0; j < AVGSIZE; j++){
                sum += timeData[i+j];
            }
            sum/=AVGSIZE;
            meanData[i+AVGSIZE/2] = sum;
        }

        chart->drawLinearData(painter, meanData);
    }
    if(ui->actionAverage->isChecked()){
        chart->plotColor=Qt::green;
        for(int i=0; i < timeData.size()-WINDOW;i++){
            double sum = 0.0;
            for(int j =0; j < WINDOW; j++ )
            {
                sum += (timeData[i+j]*timeData[i+j])/(double)(1.0/WINDOW);
            }
            avgData[i+WINDOW/2] = clamp(sqrt(sum),1.0,0.0);
        }
        chart->drawBarsData(painter, avgData);
    }
    //if(position >= DSIZE2*DBUFOR){
        //chart->minValueX= position1-DSIZE2*DBUFOR;
        //chart->maxValueX= position1;
    //}
}

void MainWindow::on_actionOpen_triggered()
{
     QString FileName;
     FileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home", tr("Data (*.dat)"));
     QFile file(FileName);
     if (!file.open(QIODevice::ReadOnly))
     return;
     file.read(reinterpret_cast<char*>(timeData.data()), static_cast<uint>(timeData.size())*sizeof(double));
     file.close();
     ui->actionRun->setChecked(false);
     ui->actionTrigger->setChecked(false);
     repaint();
}
void MainWindow::on_actionOpenWAV_triggered()
{
     QString FileName;
     FileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home", tr("WAV (*.wav)"));
     QFile file(FileName);
     if (!file.open(QIODevice::ReadOnly))
     return;
     file.read(reinterpret_cast<char*>(&fileWav),sizeof(wav_hdr));
     dataPtr = (short*)malloc(fileWav.Subchunk2Size);
     file.seek(sizeof(wav_hdr));
     file.read(reinterpret_cast<char*>(dataPtr),fileWav.Subchunk2Size);
     file.close();
     ui->actionRun->setEnabled(true);
     ui->actionRun->setChecked(false);
     ui->actionTrigger->setChecked(false);
     repaint();
}
void MainWindow::on_actionSave_triggered()
{
     QString FileName;
     FileName = QFileDialog::getSaveFileName(this, tr("Save File"),"/home", tr("Data (*.dat)"));
     QFile file(FileName);
     if (!file.open(QIODevice::WriteOnly))
     return;
     file.write(reinterpret_cast<char*>(timeData.data()), static_cast<uint>(timeData.size())*sizeof(double));
     file.close();
}

void MainWindow::on_actionLine_triggered()
{
    repaint();
}

void MainWindow::on_actionBar_triggered()
{
    repaint();
}

void MainWindow::on_actionAverage_triggered()
{
    repaint();
}
