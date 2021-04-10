#include "editwindow.h"
#include "ui_editwindow.h"
#include <QDebug>
#include <QCheckBox>
#include <QSqlQuery>
#include <QMessageBox>

EditWindow::EditWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Изменение тарифа");

    ui->tariff_t->setColumnCount(3);
    ui->tariff_t->setColumnHidden(0,true);
    QStringList LIST;
    LIST << "ID" << "Выбрать" << "Услуги";
    ui->tariff_t->setHorizontalHeaderLabels(LIST);
    ui->tariff_t->horizontalHeader()->setStretchLastSection(true);
    ui->tariff_t->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tariff_t->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tariff_t->setEditTriggers(0);
}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::recieveId(QString id)
{
    this->id = id;
    parse_serv();
}

void EditWindow::parse_serv()
{
    ui->tariff_t->clearContents();
    ui->tariff_t->setRowCount(0);
    QSqlQuery query;
    query.exec("SELECT * FROM tariff WHERE id_tariff = " + id + ";");
    query.next();
    ui->name->setText(query.value(1).toString());
    ui->praise->setText(query.value(3).toString());
    if (query.value(2).toString() != "-1")
    {
        ui->duration->setCurrentText(query.value(2).toString());
        ui->addition->setCurrentIndex(query.value(4).toInt());
        if(ui->addition->currentText() == "Да")
        {
            ui->addition_praise->setText(query.value(5).toString());
        }
        else
        {
            ui->addition_praise->setEnabled(false);
        }
    }
    else
    {
        ui->duration->setCurrentText("Ежемесячно обновляемый");
        ui->addition->setEnabled(false);
        ui->addition_praise->setEnabled(false);
    }

    QSqlQuery servQuery;
    QVector<QString> services;
    servQuery.exec("SELECT id_serv from tariff_services WHERE id_tariff = " + id + ";");
    while (servQuery.next())
    {
        services.push_back(servQuery.value(0).toString());
    }

    servQuery.finish();

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
        if (services.contains(id_serv))
        {
            checkBox->setChecked(true);
        }
        else
        {
             checkBox->setChecked(false);
        }
        ++i;
    }

    query.finish();
}
void EditWindow::on_no_clicked()
{
    close();
}

void EditWindow::on_ok_clicked()
{
    if(!ui->name->text().isEmpty() && !ui->praise->text().isEmpty() && !(ui->addition->currentText() == "Да" && ui->addition_praise->text().isEmpty()))
    {
        QString name;
        QString praise;
        QString addition;
        QString duration;
        QString addition_praise;
        if (ui->duration->currentText() == "Ежемесячно обновляемый")
        {
            duration = "-1";
            addition = "null";
            addition_praise = "null";

        }
        else
        {
            if(ui->addition->currentText() == "Нет")
            {
                duration = ui->duration->currentText();
                addition = "0";
                addition_praise = "null";
            }
            else
            {
                duration = ui->duration->currentText();
                addition = "1";
                addition_praise = ui->addition_praise->text();
            }
        }
        name = "'" + ui->name->text() + "'";
        praise = ui->praise->text();
        QSqlQuery query;
        query.exec("UPDATE tariff SET name = " + name + ", duration = " + duration + ", praise = " + praise +
                   ", addition = " + addition + ", addition_praise = " + addition_praise + " WHERE id_tariff = " + id + ";");
        query.clear();
        query.exec("DELETE FROM tariff_services where id_tariff = " + id + ";");
        query.clear();
        for(int i = 0; i < ui->tariff_t->rowCount(); i++)
        {
            QWidget *item = ( ui->tariff_t->cellWidget(i,1));
            QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            if(checkB->isChecked())
            {
                QSqlQuery query;
                query.exec("INSERT INTO tariff_services (id_tariff, id_serv) VALUES ("
                           + id + ", " + ui->tariff_t->item(i, 0)->text() + ");");
            }
        }
        close();
    }
    else
    {
        QMessageBox::critical(this,"Ошибка ввода данных","Введены некорретные данные. \n Проверьте поля для ввода!");
    }
}

void EditWindow::on_duration_currentTextChanged(const QString &arg1)
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


void EditWindow::on_addition_currentTextChanged(const QString &arg1)
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
