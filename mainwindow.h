#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "serial.h"
#ifdef Q_OS_WIN
#include "windowsserial.h"
#else
#include "posixserial.h"
#endif

#include "stk500v2.h"
#include "programmer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Serial* _serial;
    STK500v2* _stk500v2;
    Programmer* _programmer;

    static bool isRaspberryPi();

private slots:
    void deviceChanged(const QString& device);
    void flashClicked();
    void programmerProgress(float progress);
    void programmerDone();
    void programmerError(const QString& error);
    void programmerInfo(const QString& info);
};
#endif // MAINWINDOW_H
