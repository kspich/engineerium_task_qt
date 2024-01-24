#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contactwidget.h"

#include <QMainWindow>
#include <QDir>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void updateActions(const QItemSelection &selection);

private:
    void createMenus();
    void loadFile();

    ContactWidget *contactWidget;
    QMenu *fileMenu;
    QMenu *toolMenu;
    QAction *loadAct;
    QAction *exitAct;
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;
};

#endif // MAINWINDOW_H
