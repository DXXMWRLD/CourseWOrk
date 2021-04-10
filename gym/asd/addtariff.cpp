#include "addtariff.h"
#include "ui_addtariff.h"
#include <QCheckBox>
#include <QSqlQuery>
#include <QMessageBox>

AddTariff::AddTariff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTariff)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавление тарифа");

    ui->tariff_t->setColumnCount(3);
    ui->tariff_t->setColumnHidden(0,true);
    QStringList LIST;
    LIST << "ID" << "Выбрать" << "Услуги";
    ui->tariff_t->setHorizontalHeaderLabels(LIST);
    ui->tariff_t->horizontalHeader()->setStretchLastSection(true);
    ui->tariff_t->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tariff_t->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tariff_t->setEditTriggers(0);
    ui->addition_praise->setEnabled(false);
    parse_serv();
}

AddTariff::~AddTariff()
{
    delete ui;
}

void AddTariff::parse_serv()
{
    ui->tariff_t->clearContents();
    ui->tariff_t->setRowCount(0);
    QSqlQuery query;
    query.exec("SELECT * FROM services;");
    int i = 0;
    while(query.next())
    {
        //Подготовка CheckBox
        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
        layoutCheckBox->addWidget(checkBox);
        layoutCheckBox->setAlignment(Qt::AlignCenter);
        layoutCheckBox->setContentsMargins(0,0,0,0);

        QString id_serv = query.value(0).toString();
        QString name = query.value(1).toString();
        QTableWidgetItem* a1 =new QTableWidgetItem(id_serv);
        QTableWidgetItem* a2 =new QTableWidgetItem(name);
        ui->tariff_t->setRowCount(ui->tariff_t->rowCount()+1);
        ui->tariff_t->setItem(i,0, a1);
        ui->tariff_t->setCellWidget(i,1, checkBoxWidget);
        ui->tariff_t->setItem(i,2, a2);
        ++i;
    }
    query.finish();
}

void AddTariff::on_no_clicked()
{
    close();
}

void AddTariff::on_ok_clicked()
{
    if(!ui->name->text().isEmpty() && !ui->praise->text().isEmpty() && !(ui->addition->currentText() == "Да" && ui->addition_praise->text().isEmpty()))
    {
        if (ui->duration->currentText() == "Ежемесячно обновляемый")
        {
            QSqlQuery query;
            query.exec("INSERT INTO tariff (name, praise, duration) VALUES ('" + ui->name->text() + "', " + ui->praise->text() + ", -1);");

        }
        else
        {
            if(ui->addition->currentText() == "Нет")
            {
                QSqlQuery query;
                query.exec("INSERT INTO tariff (name, praise, duration, addition) VALUES "
                           "('" + ui->name->text() + "', " + ui->praise->text() + ", " + ui->duration->currentText() + ", 0);");
            }
            else
            {
                QSqlQuery query;
                query.exec("INSERT INTO tariff (name, praise, duration, addition, addition_praise) VALUES "
                           "('" + ui->name->text() + "', " + ui->praise->text() + ", "
                           + ui->duration->currentText() + ", 1, " + ui->addition_praise->text() + ");");
            }
        }
        QSqlQuery postquery;
        postquery.exec("SELECT id_tariff FROM tariff WHERE rowid=last_insert_rowid();");
        postquery.next();
        QString id_tariff = postquery.value(0).toString();
        for(int i = 0; i < ui->tariff_t->rowCount(); i++)
        {
            QWidget *item = ( ui->tariff_t->cellWidget(i,1));
            QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            if(checkB->isChecked())
            {
                QSqlQuery query;
                query.exec("INSERT INTO tariff_services (id_tariff, id_serv) VALUES ("
                           + id_tariff + ", " + ui->tariff_t->item(i, 0)->text() + ");");
            }
        }
        close();
    }
    else
    {
        QMessageBox::critical(this,"Ошибка ввода данных","Введены некорретные данные. \n Проверьте поля для ввода!");
    }
}

void AddTariff::on_addition_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Нет")
    {
        ui->addition_praise->setEnabled(false);
        ui->addition_praise->clear();
    }
    else
    {
        ui->addition_praise->setEnabled(true);
    }
}

void AddTariff::on_duration_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Ежемесячно обновляемый")
    {
        ui->addition->setCurrentIndex(0);
        ui->addition->setEnabled(false);
    }
    else
    {
        ui->addition->setEnabled(true);
    }
}
