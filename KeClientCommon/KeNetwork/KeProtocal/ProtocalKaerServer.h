#ifndef PROTOCALKAERSERVER_H
#define PROTOCALKAERSERVER_H

#include <QObject>
#include "ProtocalProcess.h"
class ProtocalKaerServer : public ProtocalProcess
{
    Q_OBJECT
public:
    explicit ProtocalKaerServer(QObject *parent = 0);

signals:

public slots:


};

#endif // PROTOCALKAERSERVER_H
