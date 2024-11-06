#include "chromasound.h"

const QUrl Chromasound::FirmwareURL("https://github.com/nirjacobson/releases/raw/refs/heads/master/Chromasound/firmware/");

const Chromasound Chromasound::ChromasoundNova(128, false, "csnova.bin", QByteArray("\x1e\x95\x16", 3), 0xEE, 0xD8, 0xF7);
const Chromasound Chromasound::ChromasoundNovaDirect(128, true, "csnova-direct.bin", QByteArray("\x1e\x95\x16", 3), 0xEE, 0xDA, 0xF7);
const Chromasound Chromasound::ChromasoundPro(256, false, "cspro.bin", QByteArray("\x1e\x96\x0a", 3), 0xE6, 0xDA, 0xFF);
const Chromasound Chromasound::ChromasoundProDirect(256, true, "cspro-direct.bin", QByteArray("\x1e\x96\x0a", 3), 0xE6, 0xDC, 0xFF);

int Chromasound::pageSize() const
{
    return _pageSize;
}

bool Chromasound::isDirect() const
{
    return _isDirect;
}

const QString& Chromasound::firmwareFile() const
{
    return _firmwareFile;
}

const QByteArray& Chromasound::signature() const
{
    return _signature;
}

quint8 Chromasound::lowFuse() const
{
    return _lowFuse;
}

quint8 Chromasound::highFuse() const
{
    return _highFuse;
}

quint8 Chromasound::extendedFuse() const
{
    return _extendedFuse;
}

Chromasound::Chromasound(int pageSize, bool isDirect, const QString& firmwareFile, const QByteArray& signature, const quint8 lowFuse, const quint8 highFuse, const quint8 extendedFuse)
    : _pageSize(pageSize)
    , _isDirect(isDirect)
    , _firmwareFile(firmwareFile)
    , _signature(signature)
    , _lowFuse(lowFuse)
    , _highFuse(highFuse)
    , _extendedFuse(extendedFuse)
{

}
