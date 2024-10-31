#include "chromasound.h"

const Chromasound Chromasound::ChromasoundNova(128, false, ":/res/csnova.bin", QByteArray("\x1e\x95\x16", 3));
const Chromasound Chromasound::ChromasoundNovaDirect(128, true, ":/res/csnova-direct.bin", QByteArray("\x1e\x95\x16", 3));
const Chromasound Chromasound::ChromasoundPro(256, false, ":/res/cspro.bin", QByteArray("\x1e\x96\x0a", 3));
const Chromasound Chromasound::ChromasoundProDirect(256, true, ":/res/cspro-direct.bin", QByteArray("\x1e\x96\x0a", 3));

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

Chromasound::Chromasound(int pageSize, bool isDirect, const QString& firmwareFile, const QByteArray& signature)
    : _pageSize(pageSize)
    , _isDirect(isDirect)
    , _firmwareFile(firmwareFile)
    , _signature(signature)
{

}
