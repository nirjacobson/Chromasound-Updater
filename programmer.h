#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <QThread>
#include <QFileInfo>
#include <QtMath>
#include <QProcess>

#include "chromasound.h"
#include "stk500v2.h"
#include "gpio.h"

class Programmer : public QThread
{
    Q_OBJECT

public:
    Programmer(STK500v2* stk500v2);

    void program(const Chromasound& chromasound);

signals:
    void error(const QString& string);
    void progress(float progress);
    void done();

    // QThread interface
protected:
    void run();

private:
    STK500v2* _stk500v2;
    const Chromasound* _chromasound;
};

#endif // PROGRAMMER_H
