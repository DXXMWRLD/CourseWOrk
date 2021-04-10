#include "addwindow.h"
#include "ui_addwindow.h"
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>

AddWindow::AddWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавление клиента");

    QSqlQuery query;
    query.exec("SELECT name FROM tariff;");
    while(query.next())
    {
        QString tariff = query.value(0).toString();
        ui->comboBox_t->addItem(tariff);
    }
    set_duration_comboBox(ui->comboBox_t->itemText(0));
    query.finish();
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::on_pushButton_2_clicked()
{
    close();
}

void AddWindow::on_pushButton_clicked()
{
    if(!ui->name->text().isEmpty() && !ui->phone->text().isEmpty())
    {
    QString tariffname = ui->comboBox_t->currentText();
    QSqlQuery query1;
    query1.exec("SELECT id_tariff FROM tariff where name = '" + tariffname + "';");
    query1.next();
    tariffname = (query1.value(0).toString());
    QString FIO = ui->name->text();
    QString phone = ui->phone->text();
    int Months = ui->comboBox_m->currentText().toInt();
    QDate end_date = QDate::currentDate().addYears(100).addMonths(Months);
    QSqlQuery query2;
    query2.exec("INSERT INTO users (name, id_tariff, phone, end_date) VALUES "
               "('" + FIO + "'," + tariffname + ",'" + phone + "'," + '"' + end_date.toString("dd.MM.yy") + '"' + ");");
    query1.finish();
    query2.finish();
    close();
    }
    else
    {
        QMessageBox::critical(this,"Ошибка ввода данных","Введены некорретные данные. \n Проверьте поля для ввода!");
    }
}
void AddWindow::set_duration_comboBox(const QString &arg1)
{
    ui->comboBox_m->clear();
    QSqlQuery query;
    query.exec("SELECT duration FROM tariff where name = '" + arg1 + "';");
    query.next();
    QString duration = query.value(0).toString();
    if (duration != "-1")
    {
        ui->comboBox_m->addItem(duration);
    }
    else
    {
        QString month;
        for (int i = 1; i <= 12; ++i)
        {
            ui->comboBox_m->addItem(month.number(i));
        }
    }
}
void AddWindow::on_comboBox_t_textActivated(const QString &arg1)
{
    set_duration_comboBox(arg1);
}
