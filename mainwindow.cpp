#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>

MainWindow::MainWindow()
{
    contactWidget = new ContactWidget;
    setCentralWidget(contactWidget);
    createMenus();
    setWindowTitle(tr("Phonebook"));
}

void MainWindow::createMenus()
{
    toolMenu = menuBar()->addMenu(tr("&Tools"));

    addAct = new QAction(tr("&Add contact"), this);
    toolMenu->addAction(addAct);
    connect(addAct, &QAction::triggered, contactWidget, &ContactWidget::showAddEntryDialog);

    editAct = new QAction(tr("&Edit contact"), this);
    editAct->setEnabled(false);
    toolMenu->addAction(editAct);
    connect(editAct, &QAction::triggered, contactWidget, &ContactWidget::editEntry);

    removeAct = new QAction(tr("&Remove contact"), this);
    removeAct->setEnabled(false);
    toolMenu->addAction(removeAct);
    connect(removeAct, &QAction::triggered, contactWidget, &ContactWidget::removeEntry);

    toolMenu->addSeparator();

    loadAct = new QAction(tr("&Load from file"), this);
    toolMenu->addAction(loadAct);
    connect(loadAct, &QAction::triggered, this, &MainWindow::loadFile);

    connect(contactWidget, &ContactWidget::selectionChanged,
        this, &MainWindow::updateActions);
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        contactWidget->loadFile(fileName);
}

void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

    if (!indexes.isEmpty()) {
        removeAct->setEnabled(true);
        editAct->setEnabled(true);
    } else {
        removeAct->setEnabled(false);
        editAct->setEnabled(false);
    }
}