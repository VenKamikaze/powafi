#include "mytype.h"

/*
 * Terrible, needs to be refactored something serious.
 * Too noob at C++ these days to refactor this out... I hate having to deal with pointers
 */

MyType::MyType(QObject *parent) :
    QObject(parent),
    m_message("")
{

}

void MyType::readUDP()
{

}

void MyType::convertToByteArray(QString finalCode, QByteArray &data)
{
    for(int i = 0; i < finalCode.size(); i=i+2)
    {
        QStringRef byteStr(&finalCode, i, 2);
        data.append((char) byteStr.toInt(NULL, 16));
    }
}

void MyType::subscribeUDP(const QString &ip, quint16 port, const QString &mac)
{
    const QString subscribeCode = QLatin1String("6864001e636c");
    const QString twenties = QLatin1String("202020202020");
    QStringList macList = mac.split(":");
    QString reversedMac = "";
    for (int i=macList.size() -1; i >= 0; i--)
    {
        reversedMac+=macList.at(i);
    }

    QString finalCode = subscribeCode + macList.join("") + twenties + reversedMac + twenties;
    QByteArray *data = new QByteArray();
    convertToByteArray(finalCode, *data);

    QUdpSocket *socket;
    socket = new QUdpSocket(this);

    const QHostAddress addr(ip);
    socket->bind(addr, port);
    connect(socket, SIGNAL(readUDP()), this, SLOT(readUDP()));

    socket->writeDatagram(*data, addr, port);
    socket->close();

    delete socket;
    delete data;
}

void MyType::switchOffUDP(const QString &ip, quint16 port, const QString &mac)
{
    const QString actionCode = QLatin1String("686400176463");
    const QString twenties = QLatin1String("202020202020");
    const QString powerCode = QLatin1String("0000000000");

    QStringList macList = mac.split(":");

    QString finalCode = actionCode + macList.join("") + twenties + powerCode;

    QByteArray *data = new QByteArray();
    convertToByteArray(finalCode, *data);

    QUdpSocket *socket;
    socket = new QUdpSocket(this);

    const QHostAddress addr(ip);
    socket->bind(addr, port);
    connect(socket, SIGNAL(readUDP()), this, SLOT(readUDP()));

    socket->writeDatagram(*data, addr, port);
    socket->close();

    delete socket;
    delete data;
}


void MyType::switchOnUDP(const QString &ip, quint16 port, const QString &mac)
{
    const QString actionCode = QLatin1String("686400176463");
    const QString twenties = QLatin1String("202020202020");
    const QString powerCode = QLatin1String("0000000001");

    QStringList macList = mac.split(":");

    QString finalCode = actionCode + macList.join("") + twenties + powerCode;

    QByteArray *data = new QByteArray();
    convertToByteArray(finalCode, *data);

    QUdpSocket *socket;
    socket = new QUdpSocket(this);

    const QHostAddress addr(ip);
    socket->bind(addr, port);
    connect(socket, SIGNAL(readUDP()), this, SLOT(readUDP()));

    socket->writeDatagram(*data, addr, port);
    socket->close();

    delete socket;
    delete data;
}


MyType::~MyType() {

}

