#ifndef CUSTOMFILTERPROXYMODEL_H
#define CUSTOMFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QString>

// Clase proxy personalizada para filtrar la tabla por texto y por tipo
class CustomFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    // Constructor explícito, permite pasar un QObject padre
    explicit CustomFilterProxyModel(QObject *parent = nullptr);

    // Método para establecer el filtro por tipo (por ejemplo: "Electrónico")
    void setFilterTipo(const QString &tipo);

protected:
    // Método principal de filtrado: decide si una fila debe mostrarse o no
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_filterTipo; // Almacena el tipo de filtro activo (vacío = sin filtro)
};

#endif // CUSTOMFILTERPROXYMODEL_H