#ifndef MYTYPE_H
#define MYTYPE_H

#include <QObject>
#include <QUdpSocket>

class MyType : public QObject
{
    Q_OBJECT
    //Q_PROPERTY( QString helloWorld READ helloWorld WRITE setHelloWorld NOTIFY helloWorldChanged )

public:
    explicit MyType(QObject *parent = 0, uint standardPort = 10000);
    Q_INVOKABLE void switchOnUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void switchOffUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void subscribeUDP(const QString &ip, quint16 port, const QString &mac);
    Q_INVOKABLE void startDiscover();
    Q_INVOKABLE QStringList getDiscoveredDevices();
    ~MyType();

    void convertToByteArray(QString finalCode, QByteArray &data);
//Q_SIGNALS:
//    void helloWorldChanged();
signals:

public slots:
    void readUDP();
    void processDiscoverPacket();

protected:
    QString buildHexPacket(const QString &actionCode, const QString &mac, const QString &endCode);
    //QString helloWorld() { return m_message; }
    //void setHelloWorld(QString msg) { m_message = msg; Q_EMIT helloWorldChanged(); }

    //QString m_message;

private:
    QString twenties;
    uint standardPort;
    QUdpSocket *m_discoverSock;
    QStringList *m_deviceList; // use a proper class
};

#endif // MYTYPE_H

