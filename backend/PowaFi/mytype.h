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

#ifndef MYTYPE_H
#define MYTYPE_H

#include <QObject>
#include <QUdpSocket>

class MyType : public QObject
{
    Q_OBJECT

public:
    explicit MyType(QObject *parent = 0, uint standardPort = 10000);
    Q_INVOKABLE void switchOnUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void switchOffUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void subscribeUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void startDiscover();
    Q_INVOKABLE QStringList getDiscoveredDevices(); // not used

    // Because I cannot figure out how to write to a QML ListModel from C++
    Q_INVOKABLE int getNumberDevicesFound();
    Q_INVOKABLE QString getDeviceMac(uint index);
    Q_INVOKABLE QString getDeviceIp(uint index);
    ~MyType();

    void convertToByteArray(QString finalCode, QByteArray &data);
signals:

public slots:
    void readUDP(); // delete me
    void processDiscoverPacket();

protected:
    QString buildHexPacket(const QString &actionCode, const QString &mac, const QString &endCode);

private:
    QString twenties;
    uint standardPort;
    QUdpSocket *m_discoverSock;
    QStringList *m_deviceList; // should probably use a proper class
};

#endif // MYTYPE_H

