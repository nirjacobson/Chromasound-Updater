#include "linuxserial.h"

LinuxSerial::LinuxSerial()
    : _fd(-1)
{

}

LinuxSerial::~LinuxSerial()
{
    if (_fd != -1) {
        close(_fd);
    }
}

void LinuxSerial::setPort(const QString& port)
{
    _port = port;

    if (_fd != -1) {
        close(_fd);
    }

    _fd = open(_port.toStdString().c_str(), O_RDWR | O_NONBLOCK);

    setupPort();
}

void LinuxSerial::writeData(const void* data, size_t size)
{
    size_t written = 0;
    do {
        written += write(_fd, (const char*)data + written, size - written);
    } while (written < size);
}

void LinuxSerial::readData(void* data, size_t size)
{
    size_t didRead = 0;
    do {
        didRead += read(_fd, (char*)data + didRead, size - didRead);
    } while (didRead < size);
}

QStringList LinuxSerial::serialPorts()
{
    QDir devDir("/dev");

#ifdef Q_OS_MACOS
    QFileInfoList fileInfoList = devDir.entryInfoList(QStringList{"tty.*"}, QDir::Filters{QDir::System});
#else
    QFileInfoList fileInfoList = devDir.entryInfoList(QStringList{"ttyS*", "ttyUSB*", "ttyAMA*"}, QDir::Filters{QDir::System});
#endif


    QStringList result;

    for (const QFileInfo& fileInfo : fileInfoList) {
        result.append(fileInfo.absoluteFilePath());
    }

    return result;
}

void LinuxSerial::setupPort()
{
    struct termios tty;

    if (tcgetattr(_fd, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    cfsetispeed(&tty, B57600);
    cfsetospeed(&tty, B57600);

    if (tcsetattr(_fd, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
}
