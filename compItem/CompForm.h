#ifndef COMPFORM_H
#define COMPFORM_H

#include <QDialog>
#include <QDate>

// Espacio de nombres generado por Qt Designer para la UI
namespace Ui {
class ComponentDialog;
}

// Clase para el diálogo de añadir/editar componentes
class ComponentDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor único para añadir o editar un componente.
    // Permite inicializar los campos con valores por defecto o existentes.
    explicit ComponentDialog(QWidget *parent = nullptr,
                             const QString& nombre = "",
                             const QString& tipo = "",
                             int cantidad = 1,
                             const QString& ubicacion = "",
                             const QDate& fecha = QDate::currentDate());
    // Destructor
    ~ComponentDialog();

    // Getters para obtener los datos ingresados en el formulario
    QString nombre() const;           // Devuelve el nombre del componente
    QString tipo() const;             // Devuelve el tipo seleccionado
    int cantidad() const;             // Devuelve la cantidad
    QString ubicacion() const;        // Devuelve la ubicación
    QDate fechaAdquisicion() const;   // Devuelve la fecha de adquisición

private:
    Ui::ComponentDialog *ui; // Puntero a la interfaz gráfica generada por Qt Designer
};

#endif // COMPONENTDIALOG_H