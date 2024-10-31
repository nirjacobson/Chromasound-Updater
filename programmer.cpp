#include "programmer.h"

Programmer::Programmer(STK500v2* stk500v2)
    : _stk500v2(stk500v2)
    , _chromasound(nullptr)
{

}

void Programmer::program(const Chromasound& chromasound)
{
    _chromasound = &chromasound;

    start();
}

void Programmer::run()
{
#ifdef Q_OS_LINUX
    if (_chromasound->isDirect()) {
        QProcess rmmod;
        rmmod.start("rmmod", QStringList() << "spi_bcm2835");
        rmmod.waitForFinished(-1);

        GPIO* gpio = new GPIO();

        gpio->write(8, 0);
        QThread::msleep(250);
        gpio->write(2, 0);
        QThread::msleep(250);
        gpio->write(2, 1);
        QThread::msleep(250);
        gpio->write(8, 1);

        delete gpio;

        QProcess modprobe;
        modprobe.start("modprobe", QStringList() << "spi_bcm2835");
        modprobe.waitForFinished(-1);
    }
#endif

    STK500V2_Message message;
    STK500V2_Message response;

    message.init();
    message.body[0] = STK500V2_CMD_SIGN_ON;
    message.size = 1;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_SIGN_ON &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Could not sign on.");
        return;
    }

    message.init();
    message.body[0] = STK500V2_CMD_GET_PARAMETER;
    message.body[1] = STK500V2_PARAM_HW_VER;
    message.size = 2;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_GET_PARAMETER &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Could not get hardware version.");
        return;
    }

    // Get values

    message.init();
    message.body[0] = STK500V2_CMD_GET_PARAMETER;
    message.body[1] = STK500V2_PARAM_SW_MAJOR;
    message.size = 2;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_GET_PARAMETER &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Could not get software major version.");
        return;
    }

    // Get values

    message.init();
    message.body[0] = STK500V2_CMD_GET_PARAMETER;
    message.body[1] = STK500V2_PARAM_SW_MINOR;
    message.size = 2;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_GET_PARAMETER &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Could not get software minor version.");
        return;
    }

    // Get values

    message.init();
    message.body[0] = STK500V2_CMD_GET_PARAMETER;
    message.body[1] = STK500V2_PARAM_VTARGET;
    message.size = 2;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_GET_PARAMETER &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Could not get voltage target.");
        return;
    }

    // Get values

    message.init();
    message.body[0] = STK500V2_CMD_ENTER_PROGMODE_ISP;
    message.size = 12;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_ENTER_PROGMODE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error entering programming mode.");
        return;
    }

    message.init();
    message.body[0] = STK500V2_CMD_READ_SIGNATURE_ISP;
    message.body[4] = 0;
    message.size = 6;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_READ_SIGNATURE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK &&
          response.body[3] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error reading device signature.");
        return;
    }

    QByteArray signature;

    signature.append(response.body[2]);

    message.init();
    message.body[0] = STK500V2_CMD_READ_SIGNATURE_ISP;
    message.body[4] = 1;
    message.size = 6;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_READ_SIGNATURE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK &&
          response.body[3] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error reading device signature.");
        return;
    }

    signature.append(response.body[2]);

    message.init();
    message.body[0] = STK500V2_CMD_READ_SIGNATURE_ISP;
    message.body[4] = 2;
    message.size = 6;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_READ_SIGNATURE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK &&
          response.body[3] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error reading device signature.");
        return;
    }

    signature.append(response.body[2]);

    if (signature != _chromasound->signature()) {
        emit error("Invalid device signature.");
        return;
    }

    message.init();
    message.body[0] = STK500V2_CMD_READ_FUSE_ISP;
    message.body[2] = 0x50;
    message.body[3] = 0x00;
    message.size = 6;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_READ_FUSE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK &&
          response.body[3] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error reading low fuse byte.");
        return;
    }

    // Get low fuse byte

    message.init();
    message.body[0] = STK500V2_CMD_READ_FUSE_ISP;
    message.body[2] = 0x58;
    message.body[3] = 0x08;
    message.size = 6;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_READ_FUSE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK &&
          response.body[3] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error reading high fuse byte.");
        return;
    }

    // Get high fuse byte

    message.init();
    message.body[0] = STK500V2_CMD_READ_FUSE_ISP;
    message.body[2] = 0x50;
    message.body[3] = 0x08;
    message.size = 6;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_READ_FUSE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK &&
          response.body[3] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error reading extended fuse byte.");
        return;
    }

    // Get extended fuse byte

    message.init();
    message.body[0] = STK500V2_CMD_CHIP_ERASE_ISP;
    message.size = 7;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_CHIP_ERASE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error performing chip erase.");
        return;
    }

    /* Write the file */

    QFileInfo fileInfo(_chromasound->firmwareFile());
    QFile file(_chromasound->firmwareFile());
    file.open(QIODevice::ReadOnly);

    int numPages = qCeil((float)fileInfo.size() / _chromasound->pageSize());

    int processed = 0;
    for (int i = 0; i < numPages; i++) {
        message.init();
        message.body[0] = STK500V2_CMD_LOAD_ADDRESS;
        uint32_t addr = _chromasound->pageSize() / 2 * i;
        message.body[1] = (addr >> 24) & 0xFF;
        message.body[2] = (addr >> 16) & 0xFF;
        message.body[3] = (addr >>  8) & 0xFF;
        message.body[4] = (addr >>  0) & 0xFF;
        message.size = 5;

        _stk500v2->sendMessage(&message, &response);

        if (!(response.body[0] == STK500V2_CMD_LOAD_ADDRESS &&
              response.body[1] == STK500V2_STATUS_CMD_OK)) {
            emit error("Error setting the load address.");
            return;
        }

        file.seek(i * _chromasound->pageSize());
        QByteArray page = file.read(_chromasound->pageSize());

        message.init();
        message.body[0] = STK500V2_CMD_PROGRAM_FLASH_ISP;
        message.body[1] = page.size() >> 8;
        message.body[2] = page.size() &  0xFF;

        memcpy(&message.body[10], page.constData(), page.size());

        message.size = 10 + page.size();

        _stk500v2->sendMessage(&message, &response);

        if (!(response.body[0] == STK500V2_CMD_PROGRAM_FLASH_ISP &&
              response.body[1] == STK500V2_STATUS_CMD_OK)) {
            emit error("Error sending firmware data.");
            return;
        }

        processed += page.size();

        emit progress( (float)processed / fileInfo.size() / 2.0f * 100.0f );
    }

    emit progress(50);

    /* Read back the file */

    processed = 0;
    for (int i = 0; i < numPages; i++) {
        message.init();
        message.body[0] = STK500V2_CMD_LOAD_ADDRESS;
        uint32_t addr = _chromasound->pageSize() / 2 * i;
        message.body[1] = (addr >> 24) & 0xFF;
        message.body[2] = (addr >> 16) & 0xFF;
        message.body[3] = (addr >>  8) & 0xFF;
        message.body[4] = (addr >>  0) & 0xFF;
        message.size = 5;

        _stk500v2->sendMessage(&message, &response);

        if (!(response.body[0] == STK500V2_CMD_LOAD_ADDRESS &&
              response.body[1] == STK500V2_STATUS_CMD_OK)) {
            emit error("Error setting the load address.");
            return;
        }

        file.seek(i * _chromasound->pageSize());
        QByteArray page = file.read(_chromasound->pageSize());

        message.init();
        message.body[0] = STK500V2_CMD_READ_FLASH_ISP;
        message.body[1] = page.size() >> 8;
        message.body[2] = page.size() &  0xFF;
        message.size = 4;

        _stk500v2->sendMessage(&message, &response);

        if (!(response.body[0] == STK500V2_CMD_READ_FLASH_ISP &&
              response.body[1] == STK500V2_STATUS_CMD_OK)) {
            emit error("Error reading back firmware data.");
            return;
        }

        for (int j = 0; j < page.size(); j++) {
            if ((uint8_t)page[j] != response.body[2 + j]) {
                emit error("Firmware data mismatch.");
                return;
            }
        }

        processed += page.size();

        emit progress( 50 + (((float)processed / fileInfo.size() / 2.0f) * 100.0f) );
    }

    file.close();

    emit progress(100);

    message.init();
    message.body[0] = STK500V2_CMD_LEAVE_PROGMODE_ISP;
    message.size = 3;

    _stk500v2->sendMessage(&message, &response);

    if (!(response.body[0] == STK500V2_CMD_LEAVE_PROGMODE_ISP &&
          response.body[1] == STK500V2_STATUS_CMD_OK)) {
        emit error("Error leaving programming mode.");
        return;
    }

    emit done();
}
