#include "adddialog.h"
#include "hintwidget.h"

#include <QtWidgets>

HintWidget::HintWidget(QWidget *parent)
{
    Q_UNUSED(parent);

    descriptionLabel = new QLabel(tr("Your phonebook is empty. Click Add"
                                     "\nto add new contact or use Tools > Load"
                                     "\nto load contacts from existing database."
    ));

    setMinimumWidth(300);

    addButton = new QPushButton(tr("Add"));

    connect(addButton, &QAbstractButton::clicked, this, &HintWidget::addEntry);

    layout = new QVBoxLayout;
    layout->addWidget(descriptionLabel);
    layout->addWidget(addButton, 0, Qt::AlignCenter);
    //layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setAlignment(Qt::AlignCenter);

    setLayout(layout);
}

void HintWidget::addEntry()
{
    AddDialog aDialog;

    if (aDialog.exec()) {
        QString name = aDialog.nameText->text();
        QString address = aDialog.numberText->text();

        emit sendDetails(name, address, false);
    }
}