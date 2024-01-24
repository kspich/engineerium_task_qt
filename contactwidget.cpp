#include "adddialog.h"
#include "contactwidget.h"

#include <QtWidgets>

ContactWidget::ContactWidget(QWidget *parent)
    : QWidget(parent)
{
    hintWidget = new HintWidget(this);
    connect(hintWidget, &HintWidget::sendDetails,
        this, &ContactWidget::addEntry);
    layout = new QVBoxLayout;
    tableView = new QTableView;

    QDir databasePath;
    defaultPath = databasePath.currentPath() + "/contacts.db";
    const QFileInfo databaseInfo(defaultPath);
    if ((!databaseInfo.exists()) || (databaseInfo.isDir())) {
        qWarning() << "contacts.db does not exist, or is a directory";
        setupTable();
        table->dbSetup(databaseInfo.absoluteFilePath());
        tableView->setVisible(false);
    }
    else 
        loadFile(databaseInfo.absoluteFilePath());

    layout->addWidget(hintWidget);
    layout->addWidget(tableView);
    setLayout(layout);
}

void ContactWidget::showAddEntryDialog()
{
    AddDialog aDialog;

    if (aDialog.exec()) {
        QString name = aDialog.nameText->text();
        QString number = aDialog.numberText->text();

        addEntry(name, number, false);
    }
}

void ContactWidget::addEntry(QString name, QString number, bool onlymodel)
{
    if (!table->getContacts().contains({ name, number })) {
        table->insertRows(0, 1, QModelIndex());
        if (!onlymodel)
            table->dbInsert(name, number);

        QModelIndex index = table->index(0, 0, QModelIndex());
        table->setData(index, name, Qt::EditRole);
        index = table->index(0, 1, QModelIndex());
        table->setData(index, number, Qt::EditRole);
        hintWidget->setVisible(false);
        tableView->setVisible(true);
    } else {
        QMessageBox::information(this, tr("Duplicate Name"),
            tr("The name \"%1\" already exists.").arg(name));
    }
}

void ContactWidget::editEntry()
{
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(tableView->model());
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QString name;
    QString number;
    int row = -1;

    foreach (QModelIndex index, indexes) {
        row = proxy->mapToSource(index).row();
        QModelIndex nameIndex = table->index(row, 0, QModelIndex());
        QVariant varName = table->data(nameIndex, Qt::DisplayRole);
        name = varName.toString();

        QModelIndex numberIndex = table->index(row, 1, QModelIndex());
        QVariant varNum = table->data(numberIndex, Qt::DisplayRole);
        number = varNum.toString();
    }

    AddDialog aDialog;
    aDialog.setWindowTitle(tr("Edit a Contact"));

    aDialog.nameText->setReadOnly(true);
    aDialog.nameText->setText(name);
    aDialog.numberText->setText(number);

    if (aDialog.exec()) {
        QString newNumber = aDialog.numberText->text();
        if (newNumber != number) {
            QModelIndex index = table->index(row, 1, QModelIndex());
            table->dbUpdate(name, newNumber);
            table->setData(index, newNumber, Qt::EditRole);
        }
    }
}

void ContactWidget::removeEntry()
{
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(tableView->model());
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();

    foreach (QModelIndex index, indexes) {
        int row = proxy->mapToSource(index).row();
        table->dbDelete(row);
        table->removeRows(row, 1, QModelIndex());
    }

    if (table->rowCount(QModelIndex()) == 0) {
        tableView->setVisible(false);
        hintWidget->setVisible(true);
    }
}

void ContactWidget::setupTable()
{
    table = new TableModel(this);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(table);
    proxyModel->setFilterKeyColumn(0);
    
    tableView->setModel(proxyModel);
    
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->hide();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    
    tableView->setSortingEnabled(true);
    
    connect(tableView->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this, &ContactWidget::selectionChanged);
}

void ContactWidget::loadFile(const QString &path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        return;
    }

    setupTable();
    table->dbSetup(path);

    QList<Contact> contacts = table->dbGetContacts();
    
    if (contacts.isEmpty()) {
        if (path != defaultPath)
            QMessageBox::information(this, tr("No contacts in file"),
                                 tr("The file you are attempting to open contains no contacts."));
        tableView->setVisible(false);
    } else {
        for (const auto &contact: qAsConst(contacts))
            addEntry(contact.name, contact.number, true);
    }
}