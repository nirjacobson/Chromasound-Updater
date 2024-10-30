#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "serial.h"
#if Q_OS_WIN
#include "windowsserial.h"
#else
#include "linuxserial.h"
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

private slots:
    void flashClicked();
    void programmerProgress(float progress);
    void programmerDone();
    void programmerError(const QString& error);
};
#endif // MAINWINDOW_H
