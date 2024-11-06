#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    _serial = new WindowsSerial;
#else
    _serial = new POSIXSerial;
#endif

    _stk500v2 = new STK500v2(_serial);

    _programmer = new Programmer(_stk500v2);

    ui->portComboBox->addItems(_serial->serialPorts());

#ifdef Q_OS_LINUX
    if (isRaspberryPi()) {
        ui->deviceComboBox->insertItem(1, "Chromasound Nova Direct");
        ui->deviceComboBox->insertItem(3, "Chromasound Pro Direct");
        ui->deviceComboBox->insertItem(5, "Chromasound Prodigy Direct");
    }
#endif

    connect(ui->deviceComboBox, &QComboBox::currentTextChanged, this, &MainWindow::deviceChanged);
    connect(ui->flashButton, &QPushButton::clicked, this, &MainWindow::flashClicked);
    connect(_programmer, &Programmer::progress, this, &MainWindow::programmerProgress);
    connect(_programmer, &Programmer::done, this, &MainWindow::programmerDone);
    connect(_programmer, &Programmer::error, this, &MainWindow::programmerError);
}

MainWindow::~MainWindow()
{
    delete _programmer;
    delete _stk500v2;
    delete _serial;

    delete ui;
}

bool MainWindow::isRaspberryPi()
{
    QFile cpuinfo("/proc/cpuinfo");

    if (cpuinfo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (cpuinfo.isReadable()) {
            QString contents = cpuinfo.readAll();
            QTextStream textStream(&contents);

            while (!textStream.atEnd()) {
                QString line = textStream.readLine();

                if (line.startsWith("Model")) {
                    cpuinfo.close();
                    return line.contains("Raspberry Pi");
                }
            }
        }

        cpuinfo.close();
    } else {
        qDebug() << cpuinfo.errorString();
    }

    return false;
}

void MainWindow::deviceChanged(const QString& device)
{
    ui->instructionLabel->setVisible(!device.toLower().contains("direct"));
}

void MainWindow::flashClicked()
{
    ui->flashButton->setEnabled(false);

    _serial->setPort(ui->portComboBox->currentText());


#ifdef Q_OS_LINUX
    if (isRaspberryPi()) {
        switch (ui->deviceComboBox->currentIndex()) {
        case 0:
            _programmer->program(Chromasound::ChromasoundNova, 1 - ui->firmwareComboBox->currentIndex());
            break;
        case 1:
            _programmer->program(Chromasound::ChromasoundNovaDirect, 1 - ui->firmwareComboBox->currentIndex());
            break;
        case 2:
            _programmer->program(Chromasound::ChromasoundPro, 1 - ui->firmwareComboBox->currentIndex());
            break;
        case 3:
            _programmer->program(Chromasound::ChromasoundProDirect, 1 - ui->firmwareComboBox->currentIndex());
            break;
        case 4:
            _programmer->program(Chromasound::ChromasoundPro, 1 - ui->firmwareComboBox->currentIndex());
            break;
        case 5:
            _programmer->program(Chromasound::ChromasoundProDirect, 1 - ui->firmwareComboBox->currentIndex());
            break;
        }
    } else {
#endif

        switch (ui->deviceComboBox->currentIndex()) {
        case 0:
            _programmer->program(Chromasound::ChromasoundNova, 1 - ui->firmwareComboBox->currentIndex());
            break;
        case 1:
            _programmer->program(Chromasound::ChromasoundPro, 1 - ui->firmwareComboBox->currentIndex());
            break;
        case 2:
            _programmer->program(Chromasound::ChromasoundPro, 1 - ui->firmwareComboBox->currentIndex());
            break;
        }


#ifdef Q_OS_LINUX
    }
#endif
}

void MainWindow::programmerProgress(float progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::programmerDone()
{
    ui->flashButton->setEnabled(true);
}

void MainWindow::programmerError(const QString& error)
{
    QMessageBox::critical(this, "Error", error);
    ui->flashButton->setEnabled(true);
}
