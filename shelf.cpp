#include "panel.h"
#include "ui_main.h"
#include "compItem/CompForm.h" 
#include "model/FiltProxy.h"
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QRegularExpression>  // Qt6: para reemplazar QRegExp

Inventario::Inventario(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Inventario)
{
    ui->setupUi(this);

    // 1. Inicializa primero el DatabaseManager
    m_dbManager = new DatabaseManager(this);

    // 2. Abre la base de datos ANTES de crear los modelos
    QString dbPath = QDir::current().absoluteFilePath("inventario.db");
    if (!m_dbManager->initialize(dbPath)) {
        QMessageBox::critical(this, "Error", "No se pudo iniciar la base de datos");
        qApp->exit(1);
        return;
    }

    // 3. Crea los modelos
    m_componentModel = new ComponentModel(m_dbManager, this);
    m_proxyModel = new CustomFilterProxyModel(this);

    // 4. Configuración del modelo y proxy
    m_proxyModel->setSourceModel(m_componentModel);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(-1); // Buscar en todas las columnas
    ui->tableView->setModel(m_proxyModel);

    // 5. Cargar datos iniciales
    m_componentModel->refresh();

    // 6. Configuración de la tabla
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // 7. Implementación de la ventana refresh
    ui->comboFiltrarTipo->addItems({"Todos", "Electrónico", "Mecánico", "Herramienta", "Consumible"});

    // 8. conexiones:
    connect(ui->lineEditBuscar, &QLineEdit::textChanged,
            this, &Inventario::on_buscarTextoCambiado);

    connect(ui->btnAnadir, &QPushButton::clicked,
            this, &Inventario::on_anadirClicked);

    connect(ui->btnEditar, &QPushButton::clicked,
            this, &Inventario::on_editarClicked);

    connect(ui->btnEliminar, &QPushButton::clicked,
            this, &Inventario::on_eliminarClicked);

    connect(ui->btnReporte, &QPushButton::clicked,
            this, &Inventario::on_reporteClicked);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &Inventario::on_editarClicked);

    connect(ui->comboFiltrarTipo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Inventario::on_filtrarPorTipo);
}

Inventario::~Inventario()
{
    delete ui;
}

void Inventario::configurarBusqueda()
{
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(-1);

    connect(ui->lineEditBuscar, &QLineEdit::textChanged,
            this, &Inventario::on_buscarTextoCambiado);
}

// Qt6: reemplazo QRegExp por QRegularExpression
void Inventario::on_buscarTextoCambiado(const QString &texto)
{
    if (!m_proxyModel || !m_componentModel) return;

    QRegularExpression regex(texto, QRegularExpression::CaseInsensitiveOption);
    m_proxyModel->setFilterRegularExpression(regex);

    ui->tableView->viewport()->update();
}

void Inventario::on_filtrarPorTipo(int index)
{
    QString tipoFiltro = "";

    switch(index) {
        case 1: tipoFiltro = "Electrónico"; break;
        case 2: tipoFiltro = "Mecánico"; break;
        case 3: tipoFiltro = "Herramienta"; break;
        case 4: tipoFiltro = "Consumible"; break;
        default: tipoFiltro = "";
    }

    m_proxyModel->setFilterKeyColumn(2); // Columna de tipo
    m_proxyModel->setFilterFixedString(tipoFiltro);

    if (tipoFiltro.isEmpty()) {
        m_proxyModel->setFilterWildcard("*");
    }
}

void Inventario::on_anadirClicked() {
    ComponentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (m_dbManager->addComponent(
                dialog.nombre(),
                dialog.tipo(),
                dialog.cantidad(),
                dialog.ubicacion(),
                dialog.fechaAdquisicion()))
        {
            m_componentModel->refresh();
            QMessageBox::information(this, "Éxito", "Componente añadido correctamente");
        } else {
            QMessageBox::warning(this, "Error", "No se pudo añadir el componente");
        }
    }
}

void Inventario::on_editarClicked()
{
    QModelIndex proxyIndex = ui->tableView->currentIndex();

    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "Error", "Selecciona un componente para editar");
        return;
    }

    QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);

    int id = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 0)).toInt();
    QString nombre = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 1)).toString();
    QString tipo = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 2)).toString();
    int cantidad = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 3)).toInt();
    QString ubicacion = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 4)).toString();
    QDate fecha = QDate::fromString(
        m_componentModel->data(m_componentModel->index(sourceIndex.row(), 5)).toString(),
        Qt::ISODate
    );

    ComponentDialog dialog(this, nombre, tipo, cantidad, ubicacion, fecha);

    if (dialog.exec() == QDialog::Accepted) {
        if (m_dbManager->actualizarComponente(
                id,
                dialog.nombre(),
                dialog.tipo(),
                dialog.cantidad(),
                dialog.ubicacion(),
                dialog.fechaAdquisicion()))
        {
            m_componentModel->refresh();
            QMessageBox::information(this, "Éxito", "Componente actualizado correctamente");
        } else {
            QMessageBox::warning(this, "Error", "No se pudo actualizar el componente");
        }
    }
}

void Inventario::on_eliminarClicked() {
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        QString id = m_componentModel->data(m_componentModel->index(index.row(), 0)).toString();
        if (m_dbManager->eliminarComponente(id)) {
            m_componentModel->refresh();
            QMessageBox::information(this, "Éxito", "Componente eliminado correctamente");
        } else {
            QMessageBox::warning(this, "Error", "No se pudo eliminar el componente");
        }
    }
}

// Qt6: QTextStream::setCodec eliminado
void Inventario::on_reporteClicked()
{
    QVector<QStringList> componentes = m_dbManager->getAllComponents();

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QString csvPath = QFileDialog::getSaveFileName(this, "Guardar reporte CSV",
                                                   defaultPath + "/reporte.csv", "CSV (*.csv)");
    if (csvPath.isEmpty()) return;

    QFile csvFile(csvPath);
    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&csvFile);
        out << QChar(0xFEFF);
        out << "ID,Nombre,Tipo,Cantidad,Ubicacion,Fecha de compra\n";

        for (const QStringList &row : componentes) {
            QStringList escapedRow;
            for (const QString &field : row) {
                QString escaped = field;
                escaped.replace("\"", "\"\"");
                escapedRow << "\"" + escaped + "\"";
            }
            out << escapedRow.join(",") << "\n";
        }
        csvFile.close();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo guardar el archivo CSV");
        return;
    }

    QString pdfPath = QFileDialog::getSaveFileName(this, "Guardar reporte PDF",
                                                   defaultPath + "/reporte.pdf", "PDF (*.pdf)");
    if (pdfPath.isEmpty()) return;

    QPdfWriter pdfWriter(pdfPath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfWriter);

    QFont font;
    font.setBold(true);
    painter.setFont(font);

    QStringList headers = {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha de compra"};

    QFontMetrics metrics(painter.font());
    QVector<int> colWidths(headers.size(), 0);
    for (int i = 0; i < headers.size(); ++i)
        colWidths[i] = metrics.horizontalAdvance(headers[i]);

    font.setBold(false);
    painter.setFont(font);
    QFontMetrics dataMetrics(painter.font());
    for (const QStringList &row : componentes) {
        for (int i = 0; i < row.size() && i < colWidths.size(); ++i) {
            int w = dataMetrics.horizontalAdvance(row[i]);
            if (w > colWidths[i]) colWidths[i] = w;
        }
    }

    for (int &w : colWidths) w += 100;

    QVector<int> colX(headers.size());
    colX[0] = 100;
    for (int i = 1; i < colX.size(); ++i)
        colX[i] = colX[i-1] + colWidths[i-1];

    font.setBold(true);
    painter.setFont(font);
    int y = 100;
    int rowHeight = metrics.height() + 20;
    int tableTop = y - metrics.ascent();
    int tableBottom = y + (componentes.size() + 1) * rowHeight;

    for (int i = 0; i <= headers.size(); ++i) {
        int xLine = (i < colX.size()) ? colX[i] : colX.last() + colWidths.last();
        painter.drawLine(xLine, tableTop, xLine, tableBottom);
    }

    for (int i = 0; i < headers.size(); ++i)
        painter.drawText(colX[i] + 5, y, headers[i]);

    painter.drawLine(colX[0], y + 50, colX.last() + colWidths.last(), y + 50);

    font.setBold(false);
    painter.setFont(font);
    y += rowHeight;

    for (const QStringList &row : componentes) {
        for (int i = 0; i < row.size() && i < colX.size(); ++i)
            painter.drawText(colX[i] + 5, y, row[i]);
        y += rowHeight;
        if (y > pdfWriter.height() - 100) {
            pdfWriter.newPage();
            y = 100 + rowHeight;
            font.setBold(true);
            painter.setFont(font);
            for (int i = 0; i < headers.size(); ++i)
                painter.drawText(colX[i] + 5, 100, headers[i]);
            painter.drawLine(colX[0], 100 + 5, colX.last() + colWidths.last(), 100 + 5);
            for (int i = 0; i <= headers.size(); ++i) {
                int xLine = (i < colX.size()) ? colX[i] : colX.last() + colWidths.last();
                painter.drawLine(xLine, 100 - metrics.ascent(), xLine, pdfWriter.height() - 100);
            }
            font.setBold(false);
            painter.setFont(font);
        }
    }
    painter.end();

    QMessageBox::information(this, "Reporte", "Reportes CSV y PDF generados correctamente.");
}