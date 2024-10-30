#include "chromasound.h"

const Chromasound Chromasound::ChromasoundNova(128, false, ":/res/csnova.bin");
const Chromasound Chromasound::ChromasoundNovaDirect(128, true, ":/res/csnova-direct.bin");
const Chromasound Chromasound::ChromasoundPro(256, false, ":/res/cspro.bin");
const Chromasound Chromasound::ChromasoundProDirect(256, true, ":/res/cspro-direct.bin");

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

Chromasound::Chromasound(int pageSize, bool isDirect, const QString& firmwareFile)
    : _pageSize(pageSize)
    , _isDirect(isDirect)
    , _firmwareFile(firmwareFile)
{

}
