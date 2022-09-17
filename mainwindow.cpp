#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    serial.setBaudRate(9600);
    avaliablePorts = QSerialPortInfo().availablePorts();

    connect(&serial, SIGNAL(readyRead()),
            this, SLOT(readSerialInput()));

    QList<QSerialPortInfo>::iterator it = avaliablePorts.begin();
    for (; it != avaliablePorts.end(); it++)
        ui->COMPortComboBox->addItem(it->portName());

    ui->TempLcdNumber->display(25.0);

    QString chekedStyle = "QPushButton:checked {background-color: rgba(0, 255, 0, 50%);}";
    ui->LEDButton_1->setStyleSheet(chekedStyle);
    ui->LEDButton_2->setStyleSheet(chekedStyle);
    ui->LEDButton_3->setStyleSheet(chekedStyle);
    ui->LEDButton_4->setStyleSheet(chekedStyle);
    ui->LEDButton_5->setStyleSheet(chekedStyle);
    ui->LEDButton_6->setStyleSheet(chekedStyle);
    ui->FanButton->setStyleSheet(chekedStyle);

    ui->PumpStatusLineEdit->setStyleSheet("QLineEdit {border: 1px solid gray; border-radius: 5px;background-color: rgba(255, 0, 0, 25%);}");
    ui->PumpStatusLineEdit->setText("Pump is stopped!");

    ui->InfusionRadioButton->setChecked(true);

    ui->DiameterSpinBox->setValue(15.5);
    ui->DiameterSpinBox->setRange(1, 40);
    ui->DiameterSpinBox->setSingleStep(0.1);

    ui->VolumeSpinBox->setValue(10.0);
    ui->VolumeSpinBox->setRange(0, 20);
    ui->VolumeSpinBox->setSingleStep(0.1);

    ui->RateSpinBox->setValue(10);
    ui->RateSpinBox->setRange(0, 3600);
    ui->RateSpinBox->setSingleStep(0.5);

    ui->DelaySpinBox->setValue(0);
    ui->DelaySpinBox->setRange(0, 3600);

    on_VolumeSpinBox_valueChanged(0);

    // plot configuration
    customPlot = new QCustomPlot(this);
    ui->gridLayout_2->addWidget(customPlot, 1, 0, 1, 1);
    customPlot->xAxis->setLabel("t, min");
    customPlot->yAxis->setLabel("T, °C");
    customPlot->setInteraction(QCP::iRangeDrag, true);
    customPlot->setInteraction(QCP::iRangeZoom, true);

    graph = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_COMPortComboBox_currentIndexChanged(int index)
{
    ui->lineEdit->clear();
    ui->lineEdit->setText(avaliablePorts.at(index).description());
}


void MainWindow::on_ConnectButton_clicked()
{
    serial.setPortName(ui->COMPortComboBox->currentText());
    if (serial.open(QIODevice::ReadWrite))
    {
        //serial.setReadBufferSize(50);
        ui->COMPortComboBox->setEnabled(false);
        ui->ConnectButton->setText("Connected!");
        serial.write("7,0;");
        ui->ConnectButton->setEnabled(false);
        serial.write("0");
    }
    else
        QMessageBox::critical(this, "Connection failed", "Connection failed: " + serial.errorString());
}

void MainWindow::on_LEDButton_1_clicked()
{
    if (ui->LEDButton_1->isChecked())
    {
        ui->LEDButton_1->setText("ON");
        serial.write("1,1;");
    }
    else
    {
        ui->LEDButton_1->setText("OFF");
        serial.write("1,0;");
    }
}


void MainWindow::on_LEDButton_2_clicked()
{
    if (ui->LEDButton_2->isChecked())
    {
        ui->LEDButton_2->setText("ON");
        serial.write("2,1;");
    }
    else
    {
        ui->LEDButton_2->setText("OFF");
        serial.write("2,0;");
    }
}


void MainWindow::on_LEDButton_3_clicked()
{
    if (ui->LEDButton_3->isChecked())
    {
        ui->LEDButton_3->setText("ON");
        serial.write("3,1;");
    }
    else
    {
        ui->LEDButton_3->setText("OFF");
        serial.write("3,0;");
    }
}


void MainWindow::on_LEDButton_4_clicked()
{
    if (ui->LEDButton_4->isChecked())
    {
        ui->LEDButton_4->setText("ON");
        serial.write("4,1;");
    }
    else
    {
        ui->LEDButton_4->setText("OFF");
        serial.write("4,0;");
    }
}


void MainWindow::on_LEDButton_5_clicked()
{
    if (ui->LEDButton_5->isChecked())
    {
        ui->LEDButton_5->setText("ON");
        serial.write("5,1;");
    }
    else
    {
        ui->LEDButton_5->setText("OFF");
        serial.write("5,0;");
    }
}


void MainWindow::on_LEDButton_6_clicked()
{
    if (ui->LEDButton_6->isChecked())
    {
        ui->LEDButton_6->setText("ON");
        serial.write("6,1;");
    }
    else
    {
        ui->LEDButton_6->setText("OFF");
        serial.write("6,0;");
    }
}


void MainWindow::on_FanButton_clicked()
{
    if (ui->FanButton->isChecked())
    {
        ui->FanButton->setText("ON");
        serial.write("8,1;");
    }
    else
    {
        ui->FanButton->setText("OFF");
        serial.write("8,0;");
    }
}

void MainWindow::processTemperature(QStringList data)
{
    temperature.append(data.at(1).toDouble());
    time.append(data.at(2).toDouble()); // перевести в минуты

    refreshGraph();
}

void MainWindow::refreshGraph()
{
    graph->addData(time, temperature);
    customPlot->xAxis->setRange(time.last() - 5, time.last());
    customPlot->yAxis->setRange(temperature.last() - 5, temperature.last() + 5);
    customPlot->replot();
}

void MainWindow::on_AllLEDButton_clicked()
{
    if (ui->LEDButton_1->isChecked() || ui->LEDButton_2->isChecked() || ui->LEDButton_3->isChecked() ||ui->LEDButton_4->isChecked() || ui->LEDButton_5->isChecked() || ui->LEDButton_6->isChecked())
    {
        ui->LEDButton_1->setText("OFF");
        ui->LEDButton_2->setText("OFF");
        ui->LEDButton_3->setText("OFF");
        ui->LEDButton_4->setText("OFF");
        ui->LEDButton_5->setText("OFF");
        ui->LEDButton_6->setText("OFF");

        ui->LEDButton_1->setChecked(false);
        ui->LEDButton_2->setChecked(false);
        ui->LEDButton_3->setChecked(false);
        ui->LEDButton_4->setChecked(false);
        ui->LEDButton_5->setChecked(false);
        ui->LEDButton_6->setChecked(false);

        serial.write("1,0;");
        serial.write("2,0;");
        serial.write("3,0;");
        serial.write("4,0;");
        serial.write("5,0;");
        serial.write("6,0;");
    }
    else
    {
        ui->LEDButton_1->setText("ON");
        ui->LEDButton_2->setText("ON");
        ui->LEDButton_3->setText("ON");
        ui->LEDButton_4->setText("ON");
        ui->LEDButton_5->setText("ON");
        ui->LEDButton_6->setText("ON");

        ui->LEDButton_1->setChecked(true);
        ui->LEDButton_2->setChecked(true);
        ui->LEDButton_3->setChecked(true);
        ui->LEDButton_4->setChecked(true);
        ui->LEDButton_5->setChecked(true);
        ui->LEDButton_6->setChecked(true);

        serial.write("1,1;");
        serial.write("2,1;");
        serial.write("3,1;");
        serial.write("4,1;");
        serial.write("5,1;");
        serial.write("6,1;");
    }
}

void MainWindow::readSerialInput()
{
    //QByteArray recievedData = serial.readLine();
    //ui->SerialInputPlainTextEdit->appendPlainText(QString(recievedData));

    data.append(serial.readAll());
    ui->SerialInputPlainTextEdit->setPlainText(QString(data));
    ui->SerialInputPlainTextEdit->moveCursor(QTextCursor().End);

    //QStringList strl = QString(recievedData).split(",");
    //if (strl.at(0) == "0") processTemperature(strl);
}


void MainWindow::on_StartPumpButton_clicked()
{
    ui->StartPumpButton->setEnabled(false);
    ui->PausePumpButton->setEnabled(true);
    ui->StopPumpButton->setEnabled(true);

    if (ui->PumpStatusLineEdit->text() == "Pump is stopped!")
    {
        QTimer::singleShot(1, this, SLOT(setDiameter()));
        QTimer::singleShot(100, this, SLOT(setVolume()));
        QTimer::singleShot(300, this, SLOT(setRate()));
        QTimer::singleShot(400, this, SLOT(setDelay()));
        QTimer::singleShot(550, this, SLOT(startPump()));
    }
    else if (ui->PumpStatusLineEdit->text() == "Pump is paused!")
    {
        serial.write("7,5;");
    }

    ui->PumpStatusLineEdit->setText("Pump is started!");
    ui->PumpStatusLineEdit->setStyleSheet("QLineEdit {border: 1px solid gray; border-radius: 5px;background-color: rgba(0, 255, 0, 25%);}");
}


void MainWindow::on_PausePumpButton_clicked()
{
    ui->StartPumpButton->setEnabled(true);
    ui->PausePumpButton->setEnabled(false);
    ui->StopPumpButton->setEnabled(false);

    serial.write("7,6;");

    ui->PumpStatusLineEdit->setStyleSheet("QLineEdit {border: 1px solid gray; border-radius: 5px;background-color: rgba(255, 255, 0, 25%);}");
    ui->PumpStatusLineEdit->setText("Pump is paused!");
}


void MainWindow::on_StopPumpButton_clicked()
{
    ui->StartPumpButton->setEnabled(true);
    ui->PausePumpButton->setEnabled(false);
    ui->StopPumpButton->setEnabled(false);

    serial.write("7,7;");

    ui->PumpStatusLineEdit->setStyleSheet("QLineEdit {border: 1px solid gray; border-radius: 5px;background-color: rgba(255, 0, 0, 25%);}");
    ui->PumpStatusLineEdit->setText("Pump is stopped!");
}


void MainWindow::on_VolumeSpinBox_valueChanged(double arg1)
{
    double time = ui->VolumeSpinBox->value() / ui->RateSpinBox->value();
    QString dim = "h";

    if (time < 1)
    {
        time *= 60;
        dim = "min";
    }

    ui->EstimatedTimeLabel->setText(QString("Estimated time: %1 ").arg(time) + dim);
}


void MainWindow::on_RateSpinBox_valueChanged(double arg1)
{
    on_VolumeSpinBox_valueChanged(0);
}

void MainWindow::setDiameter()
{
     serial.write(QString("7,1,%1;").arg(ui->DiameterSpinBox->value()).toLocal8Bit().data());
     //qDebug() << QString("7,1,%1;").arg(ui->DiameterSpinBox->value()).toLocal8Bit().data();
}

void MainWindow::setVolume()
{
    if (ui->InfusionRadioButton->isChecked())
        serial.write(QString("7,2,%1;").arg(ui->VolumeSpinBox->value()).toLocal8Bit().data());
    else
        serial.write(QString("7,2,-%1;").arg(ui->VolumeSpinBox->value()).toLocal8Bit().data());
}

void MainWindow::setRate()
{
    serial.write(QString("7,3,%1;").arg(ui->RateSpinBox->value()).toLocal8Bit().data());
    //qDebug() << QString("7,3,%1;").arg(ui->RateSpinBox->value()).toLocal8Bit().data();

}

void MainWindow::setDelay()
{
    serial.write(QString("7,4,%1;").arg(ui->DelaySpinBox->value()).toLocal8Bit().data());
    //qDebug() << QString("7,4,%1;").arg(ui->DelaySpinBox->value()).toLocal8Bit().data();
}

void MainWindow::startPump()
{
    serial.write("7,5;");
    //qDebug() << QString("7,5;");
}


