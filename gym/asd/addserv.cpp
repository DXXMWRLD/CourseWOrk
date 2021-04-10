#include "addserv.h"
#include "ui_addserv.h"
#include <QSqlQuery>
#include <QMessageBox>

AddServ::AddServ(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddServ)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавление услуги");
}

AddServ::~AddServ()
{
    delete ui;
}

void AddServ::on_ok_clicked()
{
    if (!ui->name->text().isEmpty())
    {
        QSqlQuery query;
        query.exec("SELECT * FROM services WHERE name = '" + ui->name->text() + "';");
        if (query.next())
        {
            QMessageBox::critical(this,"Ошибка добавления","Данная услуга уже существует.");
        }
        else
        {
            query.clear();
            query.exec("INSERT INTO services (name) VALUES ('" + ui->name->text() + "');" );
            query.finish();
            close();
        }
    }
    else
    {
        QMessageBox::critical(this,"Ошибка добавления","Введены некорретные данные. \n Проверьте поля для ввода!");
    }
}

void AddServ::on_no_clicked()
{
    close();
}
