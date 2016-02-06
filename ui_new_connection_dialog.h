/********************************************************************************
** Form generated from reading ui file 'new_connection_dialog.ui'
**
** Created: Sat Jul 26 21:47:29 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_NEW_CONNECTION_DIALOG_H
#define UI_NEW_CONNECTION_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_NewConnectionDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *addressLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewConnectionDialog)
    {
    if (NewConnectionDialog->objectName().isEmpty())
        NewConnectionDialog->setObjectName(QString::fromUtf8("NewConnectionDialog"));
    NewConnectionDialog->resize(391, 127);
    gridLayout = new QGridLayout(NewConnectionDialog);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(NewConnectionDialog);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    addressLineEdit = new QLineEdit(NewConnectionDialog);
    addressLineEdit->setObjectName(QString::fromUtf8("addressLineEdit"));

    horizontalLayout->addWidget(addressLineEdit);


    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

    buttonBox = new QDialogButtonBox(NewConnectionDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


    retranslateUi(NewConnectionDialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), NewConnectionDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), NewConnectionDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(NewConnectionDialog);
    } // setupUi

    void retranslateUi(QDialog *NewConnectionDialog)
    {
    NewConnectionDialog->setWindowTitle(QApplication::translate("NewConnectionDialog", "New Connection", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("NewConnectionDialog", "Address", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(NewConnectionDialog);
    } // retranslateUi

};

namespace Ui {
    class NewConnectionDialog: public Ui_NewConnectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEW_CONNECTION_DIALOG_H
