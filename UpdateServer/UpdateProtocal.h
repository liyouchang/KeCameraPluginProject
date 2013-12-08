#ifndef UPDATEPROTOCAL_H
#define UPDATEPROTOCAL_H

#include <QObject>
#include "KeProtocal/ProtocalProcess.h"
class UpdateProtocal : public ProtocalProcess
{
    Q_OBJECT
public:
    explicit UpdateProtocal();

signals:

public slots:
    void ExtractMessage(QByteArray &allBytes, Channel *parser);
};

#endif // UPDATEPROTOCAL_H
