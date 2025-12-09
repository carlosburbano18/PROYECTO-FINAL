#include "ComponentModel.h"
#include <QColor>

// Constructor del modelo, recibe el gestor de base de datos y el padre opcional
ComponentModel::ComponentModel(DatabaseManager* dbManager, QObject* parent)
    : QAbstractTableModel(parent), m_dbManager(dbManager)
{
    refresh(); // Carga los datos iniciales desde la base de datos
}

// Devuelve el número de filas (componentes) en el modelo
int ComponentModel::rowCount(const QModelIndex&) const {
    return m_components.size();
}

// Devuelve el número de columnas (campos por componente)
int ComponentModel::columnCount(const QModelIndex&) const {
    return 6; // ID, Nombre, Tipo, Cantidad, Ubicación, Fecha
}

// Devuelve los datos para una celda específica, según el rol solicitado
QVariant ComponentModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant(); // Si el índice no es válido, retorna vacío

    // Muestra el dato para la celda (modo visualización o edición)
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return m_components[index.row()][index.column()];
    }

    // Resalta en rojo la celda de cantidad si el stock es bajo (<5)
    if (role == Qt::BackgroundRole && index.column() == 3) { // Columna de cantidad
        int cantidad = m_components[index.row()][3].toInt();
        if (cantidad < 5) return QColor(Qt::red);
    }

    return QVariant(); // Para otros roles, retorna vacío
}

// Devuelve los encabezados de columna para la vista de tabla
QVariant ComponentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        static QStringList headers = {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha"};
        return headers[section];
    }
    return QVariant();
}

// Recarga los datos desde la base de datos y notifica a la vista
void ComponentModel::refresh()
{
    beginResetModel(); // Notifica a la vista que el modelo va a cambiar
    m_components = m_dbManager->getAllComponents(); // Obtiene los datos actualizados
    endResetModel(); // Notifica que el cambio terminó

    // Opcional: emitir señal de datos cambiados para actualizar la vista
    emit dataChanged(createIndex(0, 0),
                     createIndex(rowCount()-1, columnCount()-1));
}