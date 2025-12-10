#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "DataHub/DBControl.h"
#include "model/CompList.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Inventario; }
QT_END_NAMESPACE

class Inventario : public QMainWindow
{
    Q_OBJECT

public:
    Inventario(QWidget *parent = nullptr);
    ~Inventario();

private slots:
    void on_filtrarPorTipo(int index);
    void on_buscarTextoCambiado(const QString &texto);
    void on_anadirClicked();
    void on_editarClicked();
    void on_eliminarClicked();
    void on_reporteClicked();

private:
    void configurarBusqueda();
    Ui::Inventario *ui;
    DatabaseManager *m_dbManager;
    ComponentModel* m_componentModel;
    QSortFilterProxyModel* m_proxyModel;
};
#endif // INVENTARIO_H
