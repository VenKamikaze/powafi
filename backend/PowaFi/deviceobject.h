#ifndef DEVICEOBJECT_H
#define DEVICEOBJECT_H

#include <QObject>

class DeviceObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mac READ mac WRITE setMac NOTIFY macChanged)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)

public:
    DeviceObject(QObject *parent=0);
    DeviceObject(const QString &mac, const QString &ip, QObject *parent=0);

    QString mac() const;
    void setMac(const QString &mac);

    QString ip() const;
    void setIp(const QString &ip);

signals:
    void macChanged();
    void ipChanged();

private:
    QString m_mac;
    QString m_ip;
};

#endif // DEVICEOBJECT_H
