#include "CustomFilterProxyModel.h"
#include <QModelIndex>

// Constructor: inicializa el proxy model, llama al constructor base
CustomFilterProxyModel::CustomFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

// Método principal de filtrado: decide si una fila debe mostrarse o no
bool CustomFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    // Obtiene el valor de la columna "Tipo" para esta fila
    QModelIndex typeIndex = sourceModel()->index(sourceRow, 2, sourceParent); // Columna de tipo
    QString type = sourceModel()->data(typeIndex).toString();

    // Si hay un filtro de texto activo (búsqueda)
    if (!filterRegExp().pattern().isEmpty()) {
        bool matchesSearch = false;
        // Recorre todas las columnas de la fila
        for (int col = 0; col < sourceModel()->columnCount(); ++col) {
            QModelIndex idx = sourceModel()->index(sourceRow, col, sourceParent);
            // Si alguna columna contiene el texto buscado, acepta la fila
            if (idx.data().toString().contains(filterRegExp())) {
                matchesSearch = true;
                break;
            }
        }
        // Si ninguna columna coincide con la búsqueda, rechaza la fila
        if (!matchesSearch) return false;
    }

    // Si hay un filtro de tipo activo y no coincide, rechaza la fila
    if (!m_filterTipo.isEmpty() && type != m_filterTipo) {
        return false;
    }

    // Si pasa todos los filtros, acepta la fila
    return true;
}