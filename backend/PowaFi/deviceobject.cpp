
#include <QDebug>
#include "deviceobject.h"

DeviceObject::DeviceObject(QObject *parent) : QObject(parent)
{
}

DeviceObject::DeviceObject(const QString &mac, const QString &ip, QObject *parent)
    : QObject(parent), m_mac(mac), m_ip(ip)
{
}

QString DeviceObject::mac() const
{
    return m_mac;
}

void DeviceObject::setMac(const QString &mac)
{
    if (mac != m_mac) {
        m_mac = mac;
        emit macChanged();
    }
}

QString DeviceObject::ip() const
{
    return m_ip;
}

void DeviceObject::setIp(const QString &ip)
{
    if (ip != m_ip) {
        m_ip = ip;
        emit ipChanged();
    }
}
