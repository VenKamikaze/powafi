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
    //Q_PROPERTY( QString helloWorld READ helloWorld WRITE setHelloWorld NOTIFY helloWorldChanged )

public:
    explicit MyType(QObject *parent = 0);
    Q_INVOKABLE void switchOnUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void switchOffUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void subscribeUDP(const QString &ip, quint16 port, const QString &mac);
    ~MyType();

    void convertToByteArray(QString finalCode, QByteArray &data);
Q_SIGNALS:
    void helloWorldChanged();
signals:

public slots:
    void readUDP();

protected:
    QString helloWorld() { return m_message; }
    void setHelloWorld(QString msg) { m_message = msg; Q_EMIT helloWorldChanged(); }

    QString m_message;
};

#endif // MYTYPE_H

