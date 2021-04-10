#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent){}


void DataBase::connectToDataBase()
{
    if(!QFile("coursework.db").exists())
    {
        this->restoreDataBase();
    }
    else
    {
        this->openDataBase();
    }
}

bool DataBase::restoreDataBase()
{
    if(this->openDataBase())
    {
        if(!this->createTables())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName("coursework.db");
    if(db.open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DataBase::closeDataBase()
{
    db.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

bool DataBase::createTableUsers()
{
    QSqlQuery query;
    if(
            !query.exec(
                "CREATE TABLE users ( "
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name VARCHAR(100), "
                    "phone VARCHAR(30), end_date DATE, "
                    "id_tariff INTEGER NOT NULL,"
                    "FOREIGN KEY (id_tariff) REFERENCES tariff(id_tariff));"))
    {
        qDebug() << "DataBase: error of create table 'users'";
        query.finish();
        return false;
    }
    else
    {
        query.finish();
        return true;
    }
    query.finish();
    return false;
}
bool DataBase::createTableTariff()
{
    QSqlQuery query;
    if(
            !query.exec(
                "CREATE TABLE tariff ( "
                    "id_tariff INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name VARCHAR(50), "
                    "duration INTEGER, "
                    "praise INTEGER, "
                    "addition INTEGER, "
                    "addition_praise INTEGER);"))
    {
        qDebug() << "DataBase: error of create table 'tariff'";
        query.finish();
        return false;
    }
    else
    {
        query.finish();
        return true;
    }
    query.finish();
    return false;
}
bool DataBase::createTableServices()
{
    QSqlQuery query;
    if(
            !query.exec(
                "CREATE TABLE services ( "
                    "id_serv INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name VARCHAR(50));"))
    {
        qDebug() << "DataBase: error of create table 'services'";
        query.finish();
        return false;
    }
    else
    {
        query.finish();
        return true;
    }
    query.finish();
    return false;
}
bool DataBase::createTableTariff_Services()
{
    QSqlQuery query;
    if(
            !query.exec(
                "CREATE TABLE tariff_services ( "
                    "id_serv INTEGER NOT NULL, "
                    "id_tariff INTEGER NOT NULL, "
                    "FOREIGN KEY (id_serv) REFERENCES services (id_serv), "
                    "FOREIGN KEY (id_tariff) REFERENCES tariff (id_tariff));"))
    {
        qDebug() << "DataBase: error of create table 'tariff_services'";
        query.finish();
        return false;
    }
    else
    {
        query.finish();
        return true;
    }
    query.finish();
    return false;
}
bool DataBase::createTables()
{
    if(createTableUsers() && createTableTariff() && createTableServices() && createTableTariff_Services())
    {
        return true;
    }
    else
    {
        qDebug() << "DataBase: error of create tables";
        return false;
    }
    return false;
}


