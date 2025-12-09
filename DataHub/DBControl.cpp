#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Nombre de la conexión para evitar duplicados en QSqlDatabase
static const QString CONNECTION_NAME = "main_connection";

// Constructor de la clase DatabaseManager
DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {}

// Inicializa la base de datos SQLite en la ruta especificada
bool DatabaseManager::initialize(const QString &databasePath) {
    // Usa una conexión nombrada para evitar duplicados
    if (QSqlDatabase::contains(CONNECTION_NAME)) {
        m_db = QSqlDatabase::database(CONNECTION_NAME);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    }
    m_db.setDatabaseName(databasePath);

    // Intenta abrir la base de datos
    if (!m_db.open()) {
        qCritical() << "Error al abrir DB:" << m_db.lastError();
        return false;
    }
    // Crea las tablas si no existen
    return createTables();
}

// Crea la tabla de componentes si no existe
bool DatabaseManager::createTables() {
    QSqlQuery query(m_db);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS components ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "type TEXT NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "location TEXT NOT NULL,"
        "purchase_date TEXT NOT NULL)"
    );
}

// Inserta un nuevo componente en la base de datos
bool DatabaseManager::addComponent(const QString &name, const QString &type,
                                   int quantity, const QString &location,
                                   const QDate &purchaseDate) {
    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO components (name, type, quantity, location, purchase_date) "
        "VALUES (:name, :type, :quantity, :location, :date)"
    );
    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":quantity", quantity);
    query.bindValue(":location", location);
    query.bindValue(":date", purchaseDate.toString(Qt::ISODate));

    // Ejecuta la consulta y verifica si fue exitosa
    if (!query.exec()) {
        qCritical() << "Error al insertar:" << query.lastError();
        return false;
    }
    return true;
}

// Actualiza un componente existente en la base de datos
bool DatabaseManager::actualizarComponente(int id, const QString& nombre,
                                           const QString& tipo, int cantidad,
                                           const QString& ubicacion,
                                           const QDate& fecha) {
    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE components SET "
        "name = :name, "
        "type = :type, "
        "quantity = :quantity, "
        "location = :location, "
        "purchase_date = :date "
        "WHERE id = :id"
    );

    query.bindValue(":id", id);
    query.bindValue(":name", nombre);
    query.bindValue(":type", tipo);
    query.bindValue(":quantity", cantidad);
    query.bindValue(":location", ubicacion);
    query.bindValue(":date", fecha.toString(Qt::ISODate));

    // Ejecuta la consulta de actualización
    return query.exec();
}

// Obtiene todos los componentes de la base de datos y los devuelve como una lista de listas de strings
QVector<QStringList> DatabaseManager::getAllComponents() const {
    QVector<QStringList> components;
    QSqlQuery query("SELECT * FROM components ORDER BY name", m_db);

    // Recorre los resultados y los agrega a la lista
    while (query.next()) {
        QStringList component;
        component << query.value("id").toString()
                  << query.value("name").toString()
                  << query.value("type").toString()
                  << query.value("quantity").toString()
                  << query.value("location").toString()
                  << query.value("purchase_date").toString();
        components.append(component);
    }
    return components;
}

// Elimina un componente de la base de datos por su ID
bool DatabaseManager::eliminarComponente(const QString &id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM components WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

// Devuelve el último error de la base de datos
QSqlError DatabaseManager::lastError() const {
    return m_db.lastError();
}

// Destructor: cierra la base de datos si está abierta
DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}