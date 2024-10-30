#ifndef CHROMASOUND_H
#define CHROMASOUND_H

#include <QString>

class Chromasound
{
public:
    static const Chromasound ChromasoundNova;
    static const Chromasound ChromasoundNovaDirect;
    static const Chromasound ChromasoundPro;
    static const Chromasound ChromasoundProDirect;

    int pageSize() const;
    bool isDirect() const;
    const QString& firmwareFile() const;

private:
    Chromasound(int pageSize, bool isDirect, const QString& firmwareFile);

    int _pageSize;
    bool _isDirect;
    QString _firmwareFile;
};

#endif // CHROMASOUND_H
