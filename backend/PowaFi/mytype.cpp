#include "mytype.h"

/*
 * Terrible, needs to be refactored something serious.
 * Too noob at C++ these days to refactor this out... I hate having to deal with pointers
 */

MyType::MyType(QObject *parent, uint standardPort) :
    QObject(parent)
{
    this->standardPort = standardPort;
    m_discoverSock = new QUdpSocket(this);
    m_deviceList = new QStringList();
    twenties = "202020202020";
}

void MyType::readUDP()
{

}

QStringList MyType::getDiscoveredDevices()
{
    qDebug() << QString("getDiscoveredDevices - returning list of discovered devices");
    return *m_deviceList;
}

void MyType::convertToByteArray(QString finalCode, QByteArray &data)
{
    for(int i = 0; i < finalCode.size(); i=i+2)
    {
        QStringRef byteStr(&finalCode, i, 2);
        data.append((char) byteStr.toInt(NULL, 16));
    }
}

QString MyType::buildHexPacket(const QString &actionCode, const QString &mac, const QString &endCode)
{
    const QString finalCode = actionCode + mac + twenties + endCode;
    return finalCode;
}

void MyType::startDiscover()
{
    qDebug() << QString("startDiscover - beginning");
    QByteArray *data = new QByteArray();
    convertToByteArray(QLatin1String("686400067161"), *data);

    m_discoverSock->bind(standardPort, QUdpSocket::ShareAddress);
    connect(m_discoverSock, SIGNAL(readyRead()), this, SLOT(processDiscoverPacket()), Qt::QueuedConnection);
    qDebug() << QString("startDiscover - sent discover packet");
    m_discoverSock->writeDatagram(*data, QHostAddress::Broadcast, standardPort);

    delete data;
}

void MyType::processDiscoverPacket()
{
    qDebug() << QString("processDiscoverPacket called");
    QByteArray *data = new QByteArray();
    if(m_discoverSock->pendingDatagramSize() <= 0)
        return;

    data->resize(m_discoverSock->pendingDatagramSize());

    QHostAddress sender;
    quint16 port;
    m_discoverSock->readDatagram(data->data(), data->size(), &sender, &port);

    if(data->length() > 12)
    {
        QString hexString = QString(data->toHex());
        QStringRef command(&hexString, 8, 4);
        qDebug() << "processDiscoverPacket - got code: " << command.toString();
        if(QString("7161") == command.toString())
        {
            QString startMac = "accf";
            if(hexString.indexOf(startMac,12) != -1)
            {
              QStringRef fullMac (&hexString, hexString.indexOf(startMac,12), 12);
              QString entry = fullMac.toString() + "|" + sender.toString();
              if( !m_deviceList->contains(entry) )
              {
                m_deviceList->append(fullMac.toString() + "|" + sender.toString());
                qDebug() << QString("processDiscoverPacket - adding: " + fullMac.toString() + "|" + sender.toString());
              }
              else
              {
                qDebug() << QString("processDiscoverPacket - already exists: " + fullMac.toString() + "|" + sender.toString());
              }
            }
        }
    }

    delete data;
}


void MyType::subscribeUDP(const QString &ip, quint16 port, const QString &mac)
{
    //const QString subscribeCode = QLatin1String("6864001e636c");
    //const QString twenties = QLatin1String("202020202020");
    QStringList macList = mac.split(":");
    QString reversedMac = "";
    for (int i=macList.size() -1; i >= 0; i--)
    {
        reversedMac+=macList.at(i);
    }

    //QString finalCode = subscribeCode + macList.join("") + twenties + reversedMac + twenties;
    QString finalCode = buildHexPacket("6864001e636c", macList.join(""), reversedMac + twenties);

    QByteArray *data = new QByteArray();
    convertToByteArray(finalCode, *data);

    QUdpSocket *socket;
    socket = new QUdpSocket(this);

    const QHostAddress addr(ip);
    socket->bind(addr, port);
    socket->waitForConnected(1200);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readUDP()));

    socket->writeDatagram(*data, addr, port);
    socket->close();

    delete socket;
    delete data;
}

void MyType::switchOffUDP(const QString &ip, quint16 port, const QString &mac)
{
    //const QString actionCode = QLatin1String("686400176463");
    //const QString twenties = QLatin1String("202020202020");
    //const QString powerCode = QLatin1String("0000000000");

    QStringList macList = mac.split(":");

    //QString finalCode = actionCode + macList.join("") + twenties + powerCode;
    QString finalCode = buildHexPacket("686400176463", macList.join(""), "0000000000");

    QByteArray *data = new QByteArray();
    convertToByteArray(finalCode, *data);

    QUdpSocket *socket;
    socket = new QUdpSocket(this);

    const QHostAddress addr(ip);
    socket->bind(addr, port);
    socket->waitForConnected(1200);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readUDP()));

    socket->writeDatagram(*data, addr, port);
    socket->close();

    delete socket;
    delete data;
}


void MyType::switchOnUDP(const QString &ip, quint16 port, const QString &mac)
{
    //const QString actionCode = QLatin1String("686400176463");
    //const QString twenties = QLatin1String("202020202020");
    //const QString powerCode = QLatin1String("0000000001");

    QStringList macList = mac.split(":");

    //QString finalCode = actionCode + macList.join("") + twenties + powerCode;
    QString finalCode = buildHexPacket("686400176463", macList.join(""), "0000000001");

    QByteArray *data = new QByteArray();
    convertToByteArray(finalCode, *data);

    QUdpSocket *socket;
    socket = new QUdpSocket(this);

    const QHostAddress addr(ip);
    socket->bind(addr, port);
    socket->waitForConnected(1200);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readUDP()));

    socket->writeDatagram(*data, addr, port);
    socket->close();

    delete socket;
    delete data;
}


MyType::~MyType() {
    qDebug() << QString("destroying MyType");
    if(m_discoverSock)
    {
        if (m_discoverSock->isOpen())
            m_discoverSock->close();
    }

    delete m_deviceList;
    delete m_discoverSock;
}

