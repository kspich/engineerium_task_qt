#ifndef HINTWIDGET_H
#define HINTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

class HintWidget : public QWidget
{
    Q_OBJECT

public:
    HintWidget(QWidget *parent = 0);

public slots:
    void addEntry();

signals:
    void sendDetails(QString name, QString address, bool onlymodel);

private:
    QLabel *descriptionLabel;
    QPushButton *addButton;
    QVBoxLayout *layout;

};

#endif // HINTWIDGET_H