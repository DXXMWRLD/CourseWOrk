#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/*!
    \brief Класс описывает логику работы с базой данных

    Если база данных coursework.db существует, то подключение происходит к ней, если данный файл отсутствует, то он создается
*/

class DataBase : public QObject
{
    Q_OBJECT
public:

/*!
    Конструктор принимает родительский класс.
*/

    explicit DataBase(QObject *parent = 0);
    ~DataBase()= default;

/*!
    \brief Метод для подключения базы данных.
*/

    void connectToDataBase();

/*!
    \brief Метод для прекращения работы с базой данных.
*/

    void closeDataBase();

private:
    QSqlDatabase db;

private:
    bool openDataBase();
    bool restoreDataBase();
    bool createTables();
    bool createTableUsers();
    bool createTableTariff();
    bool createTableServices();
    bool createTableTariff_Services();
};

#endif // DATABASE_H
