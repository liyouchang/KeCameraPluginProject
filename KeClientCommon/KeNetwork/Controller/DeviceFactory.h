#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include <QObject>
#include <QMutex>

class SocketHandler;
class AbstractController;
class DeviceFactory : public QObject
{
    Q_OBJECT
public:
    explicit DeviceFactory(QObject *parent = 0);
    static DeviceFactory & instance();
    AbstractController * currentControl;
    QMutex resultMutex;
    AbstractController * CreateInstanceController(int controlType);
    AbstractController * CreateConnectionController(AbstractController * parent);
    AbstractController * CreateConnectionController(SocketHandler *socket,AbstractController * parent);

//internal signals and slots
signals:
    void sbCreateInstanceController(int controlType);
    void sbCreateConnectionController(AbstractController * parent,SocketHandler *socket);

public slots:
    void sCreateInstanceController(int controlType);
    void sCreateConnectionController(AbstractController * parent,SocketHandler *socket);
//


};

#endif // DEVICEFACTORY_H
