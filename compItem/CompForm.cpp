#include "ComponentDialog.h"
#include "ui_ComponentDialog.h"
#include <QDebug>

// Constructor del diálogo para añadir/editar componentes
ComponentDialog::ComponentDialog(QWidget *parent,
                                 const QString& nombre,
                                 const QString& tipo,
                                 int cantidad,
                                 const QString& ubicacion,
                                 const QDate& fecha)
    : QDialog(parent), ui(new Ui::ComponentDialog)
{
    ui->setupUi(this); // Inicializa la interfaz gráfica generada por Qt Designer

    // Configuración inicial del UI
    ui->comboTipo->addItems({"Electrónico", "Mecánico", "Herramienta", "Consumible"}); // Llena el combo de tipos
    ui->spinCantidad->setMinimum(1);      // Valor mínimo para cantidad
    ui->spinCantidad->setMaximum(9999);   // Valor máximo para cantidad
    ui->dateEdit->setCalendarPopup(true); // Permite seleccionar fecha con calendario

    // Establecer valores (para edición o valores por defecto)
    ui->lineEditNombre->setText(nombre);           // Nombre del componente
    ui->comboTipo->setCurrentText(tipo);           // Tipo seleccionado
    ui->spinCantidad->setValue(cantidad);          // Cantidad
    ui->lineEditUbicacion->setText(ubicacion);     // Ubicación
    ui->dateEdit->setDate(fecha);                  // Fecha de adquisición

    // Título dinámico según el modo (añadir o editar)
    setWindowTitle(nombre.isEmpty() ? "Añadir Componente" : "Editar Componente");
}

// Destructor: libera la memoria de la interfaz gráfica
ComponentDialog::~ComponentDialog()
{
    delete ui;
}

// Implementación de los getters para obtener los valores ingresados por el usuario

// Devuelve el nombre ingresado, sin espacios al inicio/final
QString ComponentDialog::nombre() const {
    return ui->lineEditNombre->text().trimmed();
}

// Devuelve el tipo seleccionado en el combo
QString ComponentDialog::tipo() const {
    return ui->comboTipo->currentText();
}

// Devuelve la cantidad seleccionada
int ComponentDialog::cantidad() const {
    return ui->spinCantidad->value();
}

// Devuelve la ubicación ingresada, sin espacios al inicio/final
QString ComponentDialog::ubicacion() const {
    return ui->lineEditUbicacion->text().trimmed();
}

// Devuelve la fecha de adquisición seleccionada
QDate ComponentDialog::fechaAdquisicion() const {
    return ui->dateEdit->date();
}