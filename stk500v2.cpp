#include "stk500v2.h"

STK500v2::STK500v2(Serial* serial)
    : _serial(serial)
{

}

void STK500v2::sendMessage(STK500V2_Message* message, STK500V2_Message* response)
{
    message->checksum = message->getChecksum();

    int writeSize = message->size + 6;
    char* writeData = new char[writeSize];

    writeData[0] = message->start;
    writeData[1] = message->sequenceNumber;
    writeData[2] = message->size >> 8;
    writeData[3] = message->size &  0xFF;
    writeData[4] = message->token;
    memcpy(&writeData[5], message->body, message->size);
    writeData[writeSize - 1] = message->checksum;

    _serial->writeData(writeData, writeSize);

    delete [] writeData;

    char readData[512];
    _serial->readData(readData, 4);

    if (readData[0] != STK500V2_START && readData[1] == STK500V2_START) {
        _serial->readData(readData + 4, 1);
        memcpy(readData, readData + 1, 4);
    }

    size_t restSize = readData[2];
    restSize &= 0xFF;
    restSize <<= 8;
    restSize |= readData[3] & 0xFF;
    restSize += 2;

    _serial->readData(readData + 4, restSize);

    response->init();
    response->start = readData[0];
    response->sequenceNumber = readData[1];
    response->size = readData[2];
    response->size &= 0xFF;
    response->size <<= 8;
    response->size |= readData[3] & 0xFF;
    response->token = readData[4];
    memcpy(response->body, &readData[5], response->size);
    response->checksum = readData[response->size + 5];

    if (response->checksum != response->getChecksum()) {
        // error
    }
}

void STK500V2_Message::init()
{
    start = STK500V2_START;
    sequenceNumber = 0;
    size = 0;
    token = STK500V2_TOKEN;
    memset(body, '0', sizeof(body));
    checksum = 0;
}

uint8_t STK500V2_Message::getChecksum()
{
    uint8_t checksum = start;
    checksum ^= sequenceNumber;
    checksum ^= (size >> 8);
    checksum ^= (size & 0xFF);
    checksum ^= token;
    for (uint16_t i = 0; i < size; i++) {
        checksum ^= body[i];
    }

    return checksum;
}
