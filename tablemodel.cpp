#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

TableModel::TableModel(QList<Contact> contacts, QObject *parent)
    : QAbstractTableModel(parent)
    , contacts(contacts)
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return contacts.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= contacts.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &contact = contacts.at(index.row());

        if (index.column() == 0)
            return contact.name;
        else if (index.column() == 1)
            return contact.number;
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Name");

            case 1:
                return tr("Number");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

void TableModel::dbSetup(QString path) {
    this->path = path;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
    query = QSqlQuery(db);
    bool q = query.exec("CREATE TABLE IF NOT EXISTS contacts "
        "(name varchar(20) primary key, "
        "number int);");
    if (!q)
    {
        qDebug() << strerror(errno);
        qDebug() << query.lastError();
    }
}

bool TableModel::dbInsert(QString name, QString number)
{
    query.prepare("INSERT INTO contacts (name, number)"
                  "VALUES ( ?, ?)");
    query.addBindValue(name);
    query.addBindValue(number);
    bool q = query.exec();
    if (!q)
    {
        qDebug() << strerror(errno);
        qDebug() << query.lastError();
    }
    return q;
}

bool TableModel::dbDelete(int position)
{
    query.prepare("DELETE FROM contacts WHERE name = ?;");
    query.addBindValue(contacts[position].name);
    bool q = query.exec();
    if (!q)
    {
        qDebug() << strerror(errno);
        qDebug() << query.lastError();
    }
    return q;
}

bool TableModel::dbUpdate(QString name, QString number)
{
    query.prepare("UPDATE contacts SET number = ?"
                  "WHERE name = ?;");
    query.addBindValue(number);
    query.addBindValue(name);
    bool q = query.exec();
    if (!q)
    {
        qDebug() << strerror(errno);
        qDebug() << query.lastError();
    }
    return q;
}

QList<Contact> TableModel::dbGetContacts()
{
    bool q = query.exec("SELECT name, number FROM contacts;");
    if (!q)
    {
        qDebug() << strerror(errno);
        qDebug() << query.lastError();
    }
    QList<Contact> res;
    while (query.next()) {
        QString name = query.value(0).toString();
        QString number = query.value(1).toString();
        res.append({ name, number });
    }
    return res;
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        contacts.insert(position, { QString(), QString() });

    endInsertRows();
    return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        contacts.removeAt(position);
    }

    endRemoveRows();
    return true;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        auto contact = contacts.value(row);

        if (index.column() == 0)
            contact.name = value.toString();
        else if (index.column() == 1)
            contact.number = value.toString();
        else
            return false;

        contacts.replace(row, contact);
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QList<Contact> TableModel::getContacts() const
{
    return contacts;
}