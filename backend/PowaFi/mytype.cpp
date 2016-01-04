/**
    PowaFi - Controls power state of WiFi controlled power switches on your local network
    Copyright (C) 2015  Michael Saunders

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    This program comes with ABSOLUTELY NO WARRANTY
    This is free software, and you are welcome to redistribute it
    under certain conditions
*/

#include "mytype.h"

#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include "deviceobject.h"

/*
 * This should be refactored
 * However, it works as is, so SHIP IT, haha.
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

    QList<QObject*> dataList;
    for(int i = 0; i < m_deviceList->size(); i++) {
        QStringRef fullMac (&m_deviceList->at(i), 0, 12);
        QStringRef address (&m_deviceList->at(i), 13, m_deviceList->at(i).length() - 13);
        qDebug() << QString("getDiscoveredDevices - mac: " + fullMac.toString() + " address: "+address.toString());

        dataList.append(new DeviceObject(fullMac.toString(), address.toString()));
    }

    QQuickView view;
    //view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext *ctxt = view.rootContext();
    ctxt->setContextProperty("listDevices", QVariant::fromValue(dataList));
    //view.show();
//    view.setSource(QUrl("qrc:view.qml"));
//    view.show();

    // TODO remove the following, this can just be a void function
    return *m_deviceList;
}

int MyType::getNumberDevicesFound()
{
    return m_deviceList->size();
}

QString MyType::getDeviceMac(uint index)
{
    if(index < m_deviceList->size())
    {
        QStringRef fullMac (&m_deviceList->at(index), 0, 12);
        QString formattedMac = "";
        for(int i=0; i < 12; i=i+2) {
            if(i>0)
                formattedMac += ":";
            formattedMac += fullMac.mid(i, 2);
        }
        return formattedMac;
    }
    return QString("");
}

QString MyType::getDeviceIp(uint index)
{
    if(index < m_deviceList->size())
    {
        QStringRef address (&m_deviceList->at(index), 13, m_deviceList->at(index).length() - 13);
        return address.toString();
    }
    return QString("");
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
    QStringList macList = mac.split(":");
    QString reversedMac = "";
    for (int i=macList.size() -1; i >= 0; i--)
    {
        reversedMac+=macList.at(i);
    }

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
    QStringList macList = mac.split(":");

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
    QStringList macList = mac.split(":");

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

