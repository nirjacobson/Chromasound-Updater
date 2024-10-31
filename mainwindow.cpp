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
    if (GPIO::available()) {
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

void MainWindow::deviceChanged(const QString& device)
{
    ui->instructionLabel->setVisible(!device.toLower().contains("direct"));
}

void MainWindow::flashClicked()
{
    ui->flashButton->setEnabled(false);

    _serial->setPort(ui->portComboBox->currentText());


#ifdef Q_OS_LINUX
    if (GPIO::available()) {
        switch (ui->deviceComboBox->currentIndex()) {
        case 0:
            _programmer->program(Chromasound::ChromasoundNova);
            break;
        case 1:
            _programmer->program(Chromasound::ChromasoundNovaDirect);
            break;
        case 2:
            _programmer->program(Chromasound::ChromasoundPro);
            break;
        case 3:
            _programmer->program(Chromasound::ChromasoundProDirect);
            break;
        case 4:
            _programmer->program(Chromasound::ChromasoundPro);
            break;
        case 5:
            _programmer->program(Chromasound::ChromasoundProDirect);
            break;
        }
    } else {
#endif

        switch (ui->deviceComboBox->currentIndex()) {
        case 0:
            _programmer->program(Chromasound::ChromasoundNova);
            break;
        case 1:
            _programmer->program(Chromasound::ChromasoundPro);
            break;
        case 2:
            _programmer->program(Chromasound::ChromasoundPro);
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
