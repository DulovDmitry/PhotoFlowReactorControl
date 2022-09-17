#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_COMPortComboBox_currentIndexChanged(int index);

    void on_ConnectButton_clicked();

    void on_LEDButton_1_clicked();

    void on_LEDButton_2_clicked();

    void on_LEDButton_3_clicked();

    void on_LEDButton_4_clicked();

    void on_LEDButton_5_clicked();

    void on_LEDButton_6_clicked();

    void on_AllLEDButton_clicked();

    void readSerialInput();

    void on_StartPumpButton_clicked();

    void on_PausePumpButton_clicked();

    void on_StopPumpButton_clicked();

    void on_VolumeSpinBox_valueChanged(double arg1);

    void on_RateSpinBox_valueChanged(double arg1);

    void setDiameter();
    void setVolume();
    void setRate();
    void setDelay();
    void startPump();

    void on_FanButton_clicked();

    void processTemperature(QStringList data);

    void refreshGraph();

private:
    QSerialPort serial;
    QList<QSerialPortInfo> avaliablePorts;
    QByteArray data;

    QCustomPlot *customPlot;
    QCPGraph *graph;

    QVector<double> time;
    QVector<double> temperature;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
