#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include "hintwidget.h"
#include "tablemodel.h"

#include <QItemSelection>
#include <QtWidgets>
#include <Qdir>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
class QSqlDatabase;
QT_END_NAMESPACE

class ContactWidget : public QWidget
{
    Q_OBJECT

public:
    ContactWidget(QWidget *parent = 0);
    void loadFile(const QString &path);

public slots:
    void showAddEntryDialog();
    void addEntry(QString name, QString address, bool onlymodel);
    void editEntry();
    void removeEntry();

signals:
    void selectionChanged (const QItemSelection &selected);

private:
    QVBoxLayout* layout;
    QTableView* tableView;
    QString defaultPath;
    void setupTable();

    TableModel *table;
    HintWidget *hintWidget;
    QSortFilterProxyModel *proxyModel;
};

#endif // CONTACTWIDGET_H
