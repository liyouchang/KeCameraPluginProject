#ifndef UPDATEDEVMODEL_H
#define UPDATEDEVMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QStringList>
#include "DeviceItem.h"

class UpdateDevModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit UpdateDevModel(const QStringList &headers,QObject *parent = 0);
    ~UpdateDevModel();
    void testData();
signals:

public slots:
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool removeRows(int row, int count, const QModelIndex &parent);

private:
    QStringList headerTexts;
    QList<DeviceItem *> items;

public:
    int itemsCount();
    bool pushItem(DeviceItem *item);
    void changeItem(DeviceItem * item);
    bool changeItem(void * c,int updateStatus,int updatePercent);
    bool deleteItem(void * c);
    int getDevItemPos(void * c);

};

#endif // UPDATEDEVMODEL_H
