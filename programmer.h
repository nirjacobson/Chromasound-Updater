#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <QThread>
#include <QFileInfo>
#include <QtMath>
#include <QProcess>
#include <QDir>
#include <QDirIterator>

#include "chromasound.h"
#include "filedownloader.h"
#include "stk500v2.h"

#ifdef Q_OS_LINUX
#include <linux/module.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "gpio.h"

#define finit_module(fd, param_values, flags) syscall(__NR_finit_module, fd, param_values, flags)
#define delete_module(name, flags) syscall(__NR_delete_module, name, flags)
#endif

class Programmer : public QThread
{
    Q_OBJECT

public:
    Programmer(STK500v2* stk500v2);
    ~Programmer();

    void program(const Chromasound& chromasound, const bool latest);

signals:
    void error(const QString& string);
    void info(const QString& string);
    void progress(float progress);
    void done();

    // QThread interface
protected:
    void run();

private:
    STK500v2* _stk500v2;
    const Chromasound* _chromasound;
    QByteArray _firmware;
    FileDownloader* _downloader;
};

#endif // PROGRAMMER_H
