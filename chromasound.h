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
    const QByteArray& signature() const;
    quint8 lowFuse() const;
    quint8 highFuse() const;
    quint8 extendedFuse() const;

private:
    Chromasound(int pageSize, bool isDirect, const QString& firmwareFile, const QByteArray& signature, const quint8 lowFuse, const quint8 highFuse, const quint8 extendedFuse);

    int _pageSize;
    bool _isDirect;
    QString _firmwareFile;
    QByteArray _signature;
    quint8 _lowFuse;
    quint8 _highFuse;
    quint8 _extendedFuse;
};

#endif // CHROMASOUND_H
