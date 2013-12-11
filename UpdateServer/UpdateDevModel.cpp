#include "UpdateDevModel.h"
#include <QTime>
#include <QDebug>
#include <QTimer>
UpdateDevModel::UpdateDevModel(const QStringList &headers, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->headerTexts = headers;
    QTimer * refreshTimer = new QTimer();
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshDataChanged()));
    refreshTimer->start(500);
    //testData();
}

UpdateDevModel::~UpdateDevModel()
{
    qDeleteAll(items);
}

void UpdateDevModel::refreshDataChanged()
{
    if(topLeftList.empty() || bottomRightList.empty()){
        topLeftList.clear();
        bottomRightList.clear();
        return;
    }
    QModelIndex topLeft = topLeftList.at(0);
    QModelIndex bottomRight = bottomRightList.at(0);
    int minLeft = topLeft.column();
    int minTop = topLeft.row();
    int maxRight = bottomRight.column();
    int maxBottom = bottomRight.row();

    for(int i=0;i< topLeftList.size();++i){
        topLeft  = topLeftList.at(i);
        if(minLeft > topLeft.column()){
            minLeft = topLeft.column();
        }
        if(minTop > topLeft.row()){
            minTop = topLeft.row();
        }
    }
    for(int i=0;i< bottomRightList.size();++i){
        bottomRight  = bottomRightList.at(i);
        if(maxRight < bottomRight.column()){
            maxRight = bottomRight.column();
        }
        if(maxBottom < bottomRight.row()){
            maxBottom = bottomRight.row();
        }
    }
    topLeft = this->createIndex(minTop,minLeft);
    bottomRight = this->createIndex(maxBottom,maxRight);
    emit dataChanged(topLeft, bottomRight);

    topLeftList.clear();
    bottomRightList.clear();
}


Qt::ItemFlags UpdateDevModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

//    if ( index.column() == 0 )
//        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

bool UpdateDevModel::pushItem(DeviceItem *item)
{

    int pos = items.size();
    if (!this->insertRow(pos, QModelIndex()))
        return false;

    DeviceItem * newItem = items.value(pos);
    bool result = newItem->setData(item);
    if (result){
        QModelIndex left = this->createIndex(pos,0);
        QModelIndex right = this->createIndex(pos,headerTexts.size()-1);
        topLeftList.append(left);
        bottomRightList.append(right);
        //emit dataChanged(left, right);
    }
    return result;
}

void UpdateDevModel::changeItem(DeviceItem *item)
{

}

bool UpdateDevModel::changeItem(void *c, int updateStatus, int updatePercent)
{
    int pos = this->getDevItemPos(c);
    if(pos == -1)
        return false;
    DeviceItem * item = this->items.at(pos);

    bool toSet = item->setUpdateStatus(updateStatus,updatePercent);
    if(toSet){
        QModelIndex left = this->createIndex(pos,3);
        QModelIndex right = this->createIndex(pos,3);
        topLeftList.append(left);
        bottomRightList.append(right);
    }
    return true;
}

bool UpdateDevModel::deleteItem(void *c)
{
    int pos = getDevItemPos(c);
    if(pos == -1)
        return false;
    int ret = this->removeRow(pos);
    return ret;
}


int UpdateDevModel::getDevItemPos(void *c)
{
    int pos = 0;
    for(pos = 0;pos< items.size();++pos){
        if(items.at(pos)->connect == c){
            break;
        }
    }
    if(pos == items.size())
        return -1;
    return pos;
}

int UpdateDevModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return items.size();
    else
        return 0;
}

int UpdateDevModel::columnCount(const QModelIndex &parent) const
{
    return headerTexts.size();
}

QModelIndex UpdateDevModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();


    if (!parent.isValid()){
        DeviceItem * item = items.value(row);
        return createIndex(row, column,item);
    }
}

QModelIndex UpdateDevModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant UpdateDevModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

//    DeviceItem *item = static_cast<DeviceItem*>(index.internalPointer());

//    if ( role == Qt::CheckStateRole && index.column() == 0 )
//        return static_cast< int >( item->isChecked() ? Qt::Checked : Qt::Unchecked );

    if(role == Qt::UserRole && index.row() < items.size()){
        DeviceItem * item = items[index.row()];
        return createIndex(index.row(), index.column(),item);
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    if(index.row() < items.size()){
        DeviceItem * item = items[index.row()];
        return item->data(index.column());
    }

    return QVariant();
}

QVariant UpdateDevModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headerTexts[section];

    return QVariant();
}

bool UpdateDevModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool success = true;

    beginInsertRows(parent, row, row + count - 1);

    if (row < 0 || row > items.size())
        success = false;
    else
        for (int i = 0; i < count; ++i) {
            DeviceItem *item = new DeviceItem();
            items.insert(row+i, item);
        }

    endInsertRows();

    return success;
}

bool UpdateDevModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

//    if(role == Qt::CheckStateRole && index.column() == 0){
//        DeviceItem *item = static_cast<DeviceItem*>(index.internalPointer());
//        if (value==Qt::Unchecked)
//        {
//            item->setCheckState(false);
//            emit dataChanged(index, index);
//            //m_checkedList.removeOne(item);
//            //emit selectedRows(m_checkedList.size());
//        }
//        else if(value==Qt::Checked)
//        {
//            item->setCheckState(true);
//            emit dataChanged(index, index);
//            if(!m_checkedList.contains(item)){
//                m_checkedList.append(item);
//                emit selectedRows(m_checkedList.size());
//            }
//        }
//        return true;
//    }

    if(role == Qt::UserRole){
        return false;
    }
    if (role != Qt::EditRole)
        return false;

    DeviceItem *item = items.value(index.row());
    bool result = item->setData(index.column(), value);

    if (result){
        topLeftList.append(index);
        bottomRightList.append(index);
    //    emit dataChanged(index, index);
    }
    return result;
}

bool UpdateDevModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool success = true;

    beginRemoveRows(parent, row, row + count - 1);
    if (row < 0 || row + count > items.size())
        success =  false;
    for (int i = 0; i < count; ++i){
        DeviceItem * item =  items.takeAt(row+i);
        m_checkedList.removeOne(item);
        delete item;
    }

    endRemoveRows();

    return success;
}

DeviceItem *UpdateDevModel::getItem(const QModelIndex &index)
{
    return static_cast<DeviceItem*>(index.internalPointer());
}

int UpdateDevModel::itemsCount()
{
    return this->items.size();
}


