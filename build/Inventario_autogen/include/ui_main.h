/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Inventario
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLineEdit *lineEditBuscar;
    QComboBox *comboFiltrarTipo;
    QTableView *tableView;
    QPushButton *btnAnadir;
    QPushButton *btnEditar;
    QPushButton *btnEliminar;
    QPushButton *btnReporte;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Inventario)
    {
        if (Inventario->objectName().isEmpty())
            Inventario->setObjectName("Inventario");
        Inventario->resize(800, 600);
        centralwidget = new QWidget(Inventario);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        lineEditBuscar = new QLineEdit(centralwidget);
        lineEditBuscar->setObjectName("lineEditBuscar");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditBuscar->sizePolicy().hasHeightForWidth());
        lineEditBuscar->setSizePolicy(sizePolicy);
        lineEditBuscar->setAutoFillBackground(true);
        lineEditBuscar->setEchoMode(QLineEdit::Normal);
        lineEditBuscar->setReadOnly(false);

        gridLayout->addWidget(lineEditBuscar, 0, 0, 1, 1);

        comboFiltrarTipo = new QComboBox(centralwidget);
        comboFiltrarTipo->setObjectName("comboFiltrarTipo");
        comboFiltrarTipo->setDuplicatesEnabled(false);

        gridLayout->addWidget(comboFiltrarTipo, 0, 1, 1, 1);

        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setSizeIncrement(QSize(1, 1));

        gridLayout->addWidget(tableView, 1, 0, 1, 2);

        btnAnadir = new QPushButton(centralwidget);
        btnAnadir->setObjectName("btnAnadir");
        btnAnadir->setAutoFillBackground(true);

        gridLayout->addWidget(btnAnadir, 2, 1, 1, 1);

        btnEditar = new QPushButton(centralwidget);
        btnEditar->setObjectName("btnEditar");

        gridLayout->addWidget(btnEditar, 3, 1, 1, 1);

        btnEliminar = new QPushButton(centralwidget);
        btnEliminar->setObjectName("btnEliminar");

        gridLayout->addWidget(btnEliminar, 4, 1, 1, 1);

        btnReporte = new QPushButton(centralwidget);
        btnReporte->setObjectName("btnReporte");

        gridLayout->addWidget(btnReporte, 5, 1, 1, 1);

        Inventario->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Inventario);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 20));
        Inventario->setMenuBar(menubar);
        statusbar = new QStatusBar(Inventario);
        statusbar->setObjectName("statusbar");
        Inventario->setStatusBar(statusbar);

        retranslateUi(Inventario);

        QMetaObject::connectSlotsByName(Inventario);
    } // setupUi

    void retranslateUi(QMainWindow *Inventario)
    {
        Inventario->setWindowTitle(QCoreApplication::translate("Inventario", "Inventario", nullptr));
        lineEditBuscar->setText(QString());
        lineEditBuscar->setPlaceholderText(QCoreApplication::translate("Inventario", "Buscar: ...", nullptr));
#if QT_CONFIG(tooltip)
        comboFiltrarTipo->setToolTip(QCoreApplication::translate("Inventario", "Seleccciona el modo de filtrado para los datos", nullptr));
#endif // QT_CONFIG(tooltip)
        comboFiltrarTipo->setCurrentText(QString());
        comboFiltrarTipo->setPlaceholderText(QCoreApplication::translate("Inventario", "Filtrar", nullptr));
        btnAnadir->setText(QCoreApplication::translate("Inventario", "A\303\261adir", nullptr));
        btnEditar->setText(QCoreApplication::translate("Inventario", "Editar", nullptr));
        btnEliminar->setText(QCoreApplication::translate("Inventario", "Eliminar", nullptr));
        btnReporte->setText(QCoreApplication::translate("Inventario", "Reporte", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Inventario: public Ui_Inventario {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
