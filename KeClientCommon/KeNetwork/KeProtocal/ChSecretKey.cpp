#include "ChSecretKey.h"

ChSecretKey::ChSecretKey(Channel *parent) :
    Channel(parent)
{
}

void ChSecretKey::OnRespond(QByteArray &data)
{

}

int ChSecretKey::Request()
{
    return 0;
}
