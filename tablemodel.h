#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <qsqlerror.h>
#include <qdebug.h>
#include <QDir>

struct Contact
{
    QString name;
    QString number;

    bool operator==(const Contact &other) const
    {
        return name == other.name && number == other.number;
    }
};

inline QDataStream &operator<<(QDataStream &stream, const Contact &contact)
{
    return stream << contact.name << contact.number;
}

inline QDataStream &operator>>(QDataStream &stream, Contact &contact)
{
    return stream >> contact.name >> contact.number;
}

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent = 0);
    TableModel(QList<Contact> contacts, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    void dbSetup(QString path);
    bool dbInsert(QString name, QString number);
    bool dbDelete(int position);
    bool dbUpdate(QString name, QString number);
    QList<Contact> dbGetContacts();
    QList<Contact> getContacts() const;
    QString path;

private:
    QSqlDatabase db;
    QSqlQuery query;
    QList<Contact> contacts;
};

#endif // TABLEMODEL_H
