#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "../Database/DatabaseManager.h"

// Modelo de tabla para representar los componentes en la vista
class ComponentModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    // Constructor: recibe el gestor de base de datos y el padre opcional
    explicit ComponentModel(DatabaseManager* dbManager, QObject* parent = nullptr);

    // Métodos requeridos por QAbstractTableModel

    // Devuelve el número de filas (componentes)
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    // Devuelve el número de columnas (campos por componente)
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    // Devuelve los datos para una celda específica
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Devuelve los encabezados de columna o fila
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // Devuelve los datos de un componente específico (por fila)
    QStringList getComponentData(int row) const {
        if (row >= 0 && row < m_components.size())
            return m_components[row];
        return QStringList();
    }

    // Recarga los datos desde la base de datos
    void refresh();

private:
    DatabaseManager* m_dbManager;         // Puntero al gestor de base de datos
    QVector<QStringList> m_components;    // Almacena los datos de los componentes
};

#endif // COMPONENTMODEL_H
