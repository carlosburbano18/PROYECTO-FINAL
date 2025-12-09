/********************************************************************************
** Form generated from reading UI file 'CompForm.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPFORM_H
#define UI_COMPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ComponentDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelNombre;
    QLineEdit *lineEditNombre;
    QLabel *labelTipo;
    QComboBox *comboTipo;
    QLabel *labelCantidad;
    QSpinBox *spinCantidad;
    QLabel *labelUbicacion;
    QLineEdit *lineEditUbicacion;
    QLabel *labelFecha;
    QDateEdit *dateEdit;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnAceptar;
    QPushButton *btnCancelar;

    void setupUi(QDialog *ComponentDialog)
    {
        if (ComponentDialog->objectName().isEmpty())
            ComponentDialog->setObjectName("ComponentDialog");
        ComponentDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(ComponentDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelNombre = new QLabel(ComponentDialog);
        labelNombre->setObjectName("labelNombre");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelNombre);

        lineEditNombre = new QLineEdit(ComponentDialog);
        lineEditNombre->setObjectName("lineEditNombre");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditNombre);

        labelTipo = new QLabel(ComponentDialog);
        labelTipo->setObjectName("labelTipo");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelTipo);

        comboTipo = new QComboBox(ComponentDialog);
        comboTipo->setObjectName("comboTipo");

        formLayout->setWidget(1, QFormLayout::FieldRole, comboTipo);

        labelCantidad = new QLabel(ComponentDialog);
        labelCantidad->setObjectName("labelCantidad");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelCantidad);

        spinCantidad = new QSpinBox(ComponentDialog);
        spinCantidad->setObjectName("spinCantidad");
        spinCantidad->setMinimum(1);
        spinCantidad->setMaximum(9999);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinCantidad);

        labelUbicacion = new QLabel(ComponentDialog);
        labelUbicacion->setObjectName("labelUbicacion");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelUbicacion);

        lineEditUbicacion = new QLineEdit(ComponentDialog);
        lineEditUbicacion->setObjectName("lineEditUbicacion");

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEditUbicacion);

        labelFecha = new QLabel(ComponentDialog);
        labelFecha->setObjectName("labelFecha");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelFecha);

        dateEdit = new QDateEdit(ComponentDialog);
        dateEdit->setObjectName("dateEdit");

        formLayout->setWidget(4, QFormLayout::FieldRole, dateEdit);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnAceptar = new QPushButton(ComponentDialog);
        btnAceptar->setObjectName("btnAceptar");

        horizontalLayout->addWidget(btnAceptar);

        btnCancelar = new QPushButton(ComponentDialog);
        btnCancelar->setObjectName("btnCancelar");

        horizontalLayout->addWidget(btnCancelar);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ComponentDialog);
        QObject::connect(btnCancelar, &QPushButton::clicked, ComponentDialog, qOverload<>(&QDialog::reject));
        QObject::connect(btnAceptar, &QPushButton::clicked, ComponentDialog, qOverload<>(&QDialog::accept));

        QMetaObject::connectSlotsByName(ComponentDialog);
    } // setupUi

    void retranslateUi(QDialog *ComponentDialog)
    {
        labelNombre->setText(QCoreApplication::translate("ComponentDialog", "Nombre:", nullptr));
        labelTipo->setText(QCoreApplication::translate("ComponentDialog", "Tipo:", nullptr));
        labelCantidad->setText(QCoreApplication::translate("ComponentDialog", "Cantidad:", nullptr));
        labelUbicacion->setText(QCoreApplication::translate("ComponentDialog", "Ubicaci\303\263n:", nullptr));
        labelFecha->setText(QCoreApplication::translate("ComponentDialog", "Fecha:", nullptr));
        btnAceptar->setText(QCoreApplication::translate("ComponentDialog", "Aceptar", nullptr));
        btnCancelar->setText(QCoreApplication::translate("ComponentDialog", "Cancelar", nullptr));
        (void)ComponentDialog;
    } // retranslateUi

};

namespace Ui {
    class ComponentDialog: public Ui_ComponentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPFORM_H
