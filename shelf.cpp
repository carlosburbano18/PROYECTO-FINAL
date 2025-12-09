#include "inventario.h"
#include "ui_inventario.h"
#include "./Component/ComponentDialog.h"
#include "./Models/CustomFilterProxyModel.h"
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

    // 8.conexiones:
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
    // Libera la memoria de la interfaz gráfica
    delete ui;
}

void Inventario::configurarBusqueda()
{
    // Configura el proxy model para que la búsqueda no distinga mayúsculas/minúsculas
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    // Configura el proxy model para buscar en todas las columnas
    m_proxyModel->setFilterKeyColumn(-1); // -1 significa todas las columnas

    // Conecta el cambio de texto en la caja de búsqueda con el slot correspondiente
    connect(ui->lineEditBuscar, &QLineEdit::textChanged,
            this, &Inventario::on_buscarTextoCambiado);
}

// Implementación del slot para búsqueda en tiempo real
void Inventario::on_buscarTextoCambiado(const QString &texto)
{
    // Si los modelos no están inicializados, salir
    if (!m_proxyModel || !m_componentModel) return;

    // Crea una expresión regular para filtrar (no distingue mayúsculas/minúsculas)
    QRegExp regex(texto, Qt::CaseInsensitive, QRegExp::FixedString);
    m_proxyModel->setFilterRegExp(QRegExp(texto, Qt::CaseInsensitive));

    // Actualiza la vista de la tabla para reflejar los cambios
    ui->tableView->viewport()->update();
}

// Implementación del filtrado por tipo de componente
void Inventario::on_filtrarPorTipo(int index)
{
    QString tipoFiltro = "";

    // Determina el tipo de filtro según el índice seleccionado en el combo
    switch(index) {
        case 1: tipoFiltro = "Electrónico"; break;
        case 2: tipoFiltro = "Mecánico"; break;
        case 3: tipoFiltro = "Herramienta"; break;
        case 4: tipoFiltro = "Consumible"; break;
        default: tipoFiltro = "";
    }

    // Aplica el filtro a la columna de tipo (columna 2)
    m_proxyModel->setFilterKeyColumn(2); // Columna de tipo
    m_proxyModel->setFilterFixedString(tipoFiltro);

    // Si el filtro es "Todos", muestra todos los registros
    if (tipoFiltro.isEmpty()) {
        m_proxyModel->setFilterWildcard("*");
    }
}

// Slot para añadir un nuevo componente
void Inventario::on_anadirClicked() {
    // Abre el diálogo para añadir componente (con valores por defecto)
    ComponentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Si el usuario acepta, intenta añadir el componente a la base de datos
        if (m_dbManager->addComponent(
                dialog.nombre(),
                dialog.tipo(),
                dialog.cantidad(),
                dialog.ubicacion(),
                dialog.fechaAdquisicion()))
        {
            // Si se añadió correctamente, refresca el modelo y muestra mensaje de éxito
            m_componentModel->refresh();
            QMessageBox::information(this, "Éxito", "Componente añadido correctamente");
        } else {
            // Si falla, muestra mensaje de error
            QMessageBox::warning(this, "Error", "No se pudo añadir el componente");
        }
    }
}

// Slot para editar un componente existente
void Inventario::on_editarClicked()
{
    // Obtiene el índice seleccionado en la tabla (proxy model)
    QModelIndex proxyIndex = ui->tableView->currentIndex();

    // Si no hay selección, muestra advertencia y sale
    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "Error", "Selecciona un componente para editar");
        return;
    }

    // Mapea el índice del proxy al modelo fuente
    QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);

    // Obtiene los datos del componente seleccionado
    int id = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 0)).toInt();
    QString nombre = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 1)).toString();
    QString tipo = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 2)).toString();
    int cantidad = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 3)).toInt();
    QString ubicacion = m_componentModel->data(m_componentModel->index(sourceIndex.row(), 4)).toString();
    QDate fecha = QDate::fromString(
        m_componentModel->data(m_componentModel->index(sourceIndex.row(), 5)).toString(),
        Qt::ISODate
        );

    // Abre el diálogo de edición con los datos actuales del componente
    ComponentDialog dialog(this, nombre, tipo, cantidad, ubicacion, fecha);

    if (dialog.exec() == QDialog::Accepted) {
        // Si el usuario acepta, intenta actualizar el componente en la base de datos
        if (m_dbManager->actualizarComponente(
                id,
                dialog.nombre(),
                dialog.tipo(),
                dialog.cantidad(),
                dialog.ubicacion(),
                dialog.fechaAdquisicion()))
        {
            // Si se actualizó correctamente, refresca el modelo y muestra mensaje de éxito
            m_componentModel->refresh();
            QMessageBox::information(this, "Éxito", "Componente actualizado correctamente");
        } else {
            // Si falla, muestra mensaje de error
            QMessageBox::warning(this, "Error", "No se pudo actualizar el componente");
        }
    }
}

// Slot para eliminar un componente seleccionado
void Inventario::on_eliminarClicked() {
    // Obtiene el índice seleccionado en la tabla
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        // Obtiene el ID del componente a eliminar
        QString id = m_componentModel->data(m_componentModel->index(index.row(), 0)).toString();
        // Intenta eliminar el componente de la base de datos
        if (m_dbManager->eliminarComponente(id)) {
            // Si se eliminó correctamente, refresca el modelo y muestra mensaje de éxito
            m_componentModel->refresh();
            QMessageBox::information(this, "Éxito", "Componente eliminado correctamente");
        } else {
            // Si falla, muestra mensaje de error
            QMessageBox::warning(this, "Error", "No se pudo eliminar el componente");
        }
    }
}

void Inventario::on_reporteClicked()
{
    // Obtiene todos los componentes de la base de datos como una lista de listas de strings
    QVector<QStringList> componentes = m_dbManager->getAllComponents();

    // Obtiene la ruta por defecto para guardar archivos (carpeta Documentos del usuario)
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    // Abre un diálogo para que el usuario elija dónde guardar el archivo CSV
    QString csvPath = QFileDialog::getSaveFileName(this, "Guardar reporte CSV", defaultPath + "/reporte.csv", "CSV (*.csv)");
    if (csvPath.isEmpty()) return; // Si el usuario cancela, salir

    // Crea y abre el archivo CSV para escritura en modo texto
    QFile csvFile(csvPath);
    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&csvFile);
        out.setCodec("UTF-8");      // Usa codificación UTF-8 para soportar caracteres especiales
        out << QChar(0xFEFF);       // Escribe el BOM para que Excel reconozca UTF-8
        out << "ID,Nombre,Tipo,Cantidad,Ubicacion,Fecha de compra\n"; // Escribe los encabezados

        // Escribe cada fila de datos en el CSV
        for (const QStringList &row : componentes) {
            QStringList escapedRow;
            for (const QString &field : row) {
                QString escaped = field;
                escaped.replace("\"", "\"\""); // Escapa comillas dobles
                escapedRow << "\"" + escaped + "\""; // Cada campo entre comillas
            }
            out << escapedRow.join(",") << "\n"; // Escribe la fila
        }
        csvFile.close(); // Cierra el archivo CSV
    } else {
        // Si no se pudo abrir el archivo, muestra un mensaje de error
        QMessageBox::warning(this, "Error", "No se pudo guardar el archivo CSV");
        return;
    }   

    // Abre un diálogo para que el usuario elija dónde guardar el archivo PDF
    QString pdfPath = QFileDialog::getSaveFileName(this, "Guardar reporte PDF", defaultPath + "/reporte.pdf", "PDF (*.pdf)");
    if (pdfPath.isEmpty()) return; // Si el usuario cancela, salir

    // Configura el escritor de PDF y el pintor
    QPdfWriter pdfWriter(pdfPath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfWriter);

    // Configura la fuente en negrita para los encabezados
    QFont font;
    font.setBold(true);
    painter.setFont(font);

    // Define los encabezados de la tabla
    QStringList headers = {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha de compra"};

    // Calcula el ancho máximo de cada columna (considerando encabezados y datos)
    QFontMetrics metrics(painter.font());
    QVector<int> colWidths(headers.size(), 0);

    // Calcula el ancho de los encabezados
    for (int i = 0; i < headers.size(); ++i)
        colWidths[i] = metrics.horizontalAdvance(headers[i]);

    // Calcula el ancho de los datos
    font.setBold(false);
    painter.setFont(font);
    QFontMetrics dataMetrics(painter.font());
    for (const QStringList &row : componentes) {
        for (int i = 0; i < row.size() && i < colWidths.size(); ++i) {
            int w = dataMetrics.horizontalAdvance(row[i]);
            if (w > colWidths[i]) colWidths[i] = w;
        }
    }

    // Suma un margen extra a cada columna para que no se vea pegado
    for (int &w : colWidths) w += 100;

    // Calcula la posición X de cada columna (empezando en 100)
    QVector<int> colX(headers.size());
    colX[0] = 100;
    for (int i = 1; i < colX.size(); ++i)
        colX[i] = colX[i-1] + colWidths[i-1];

    // Imprime encabezados y líneas de columna
    font.setBold(true);
    painter.setFont(font);
    int y = 100; // Margen superior
    int rowHeight = metrics.height() + 20; // Altura de cada fila
    int tableTop = y - metrics.ascent(); // Arriba de la tabla
    int tableBottom = y + (componentes.size() + 1) * rowHeight; // Abajo de la tabla

    // Dibuja líneas verticales de columna
    for (int i = 0; i <= headers.size(); ++i) {
        int xLine = (i < colX.size()) ? colX[i] : colX.last() + colWidths.last();
        painter.drawLine(xLine, tableTop, xLine, tableBottom);
    }

    // Dibuja los encabezados de la tabla
    for (int i = 0; i < headers.size(); ++i)
        painter.drawText(colX[i] + 5, y, headers[i]);

    // Dibuja una línea horizontal debajo del encabezado
    painter.drawLine(colX[0], y + 50, colX.last() + colWidths.last(), y + 50);

    // Imprime los datos de la tabla
    font.setBold(false);
    painter.setFont(font);
    y += rowHeight;
    for (const QStringList &row : componentes) {
        for (int i = 0; i < row.size() && i < colX.size(); ++i)
            painter.drawText(colX[i] + 5, y, row[i]);
        y += rowHeight;
        // Si se acaba la página, crea una nueva y reimprime encabezados y líneas
        if (y > pdfWriter.height() - 100) {
            pdfWriter.newPage();
            y = 100 + rowHeight;
            // Reimprime encabezados y líneas en la nueva página
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
    painter.end(); // Finaliza el pintado del PDF

    // Muestra mensaje de éxito al usuario
    QMessageBox::information(this, "Reporte", "Reportes CSV y PDF generados correctamente.");
}
