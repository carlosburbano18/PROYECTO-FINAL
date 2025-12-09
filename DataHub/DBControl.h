#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDate>

// Clase que gestiona la conexión y operaciones con la base de datos
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    // Constructor explícito, permite pasar un QObject padre
    explicit DatabaseManager(QObject *parent = nullptr);

    // Destructor
    ~DatabaseManager();

    // Inicializa la base de datos en la ruta especificada (por defecto "inventario.db")
    bool initialize(const QString &databasePath = "inventario.db");

    // Agrega un nuevo componente a la base de datos
    bool addComponent(const QString &name, const QString &type,
                      int quantity, const QString &location,
                      const QDate &purchaseDate);

    // Actualiza un componente existente por su ID
    bool actualizarComponente(int id, const QString& nombre, const QString& tipo,
                              int cantidad, const QString& ubicacion,
                              const QDate& fecha);

    // Elimina un componente por su ID
    bool eliminarComponente(const QString &id);

    // Obtiene todos los componentes como una lista de listas de strings
    QVector<QStringList> getAllComponents() const;

    // Devuelve el último error de la base de datos
    QSqlError lastError() const;

private:
    QSqlDatabase m_db;      // Objeto de conexión a la base de datos

    // Crea las tablas necesarias si no existen
    bool createTables();
};

#endif // DATABASEMANAGER_H