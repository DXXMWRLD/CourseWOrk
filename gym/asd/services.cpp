#include "services.h"
#include "ui_services.h"
#include <QSqlQuery>
#include "addserv.h"
#include <QMessageBox>

services::services(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::services)
{
    ui->setupUi(this);
    this->setWindowTitle("Услуги");
    QStringList LIST1;
    LIST1 << "ID" << "Название услуги";
    ui->serv_t->setColumnCount(2);
    ui->serv_t->setColumnHidden(0,true);
    ui->serv_t->setHorizontalHeaderLabels(LIST1);
    ui->serv_t->horizontalHeader()->setStretchLastSection(true);
    ui->serv_t->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->serv_t->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->serv_t->setEditTriggers(0);
    parse_serv();
    enabled_b_serv(false);
}

services::~services()
{
    delete ui;
}

void services::on_ok_b_clicked()
{
    close();
}

void services::on_Del_serv_b_clicked()
{
    QMessageBox but (QMessageBox::Question, "Удаление","Удалить услугу?", QMessageBox::Yes | QMessageBox::No, this);
    but.setButtonText(QMessageBox::Yes, "Да");
    but.setButtonText(QMessageBox::No, "Нет");
    if (but.exec() == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.exec("DELETE FROM services where id_serv = " + (ui->serv_t->item(ui->serv_t->currentRow(),0)->text()) + ";");
        query.clear();
        query.exec("DELETE FROM tariff_services where id_serv = " + (ui->serv_t->item(ui->serv_t->currentRow(),0)->text()) + ";");
        parse_serv();
        enabled_b_serv(false);
        query.finish();
    }
}

void services::on_Add_serv_b_clicked()
{
    AddServ window;
    window.setModal(true);
    window.exec();
    parse_serv();
    ui->serv_t->scrollToBottom();
    QModelIndex index = ui->serv_t->model()->index(ui->serv_t->rowCount() - 1, 0);
    ui->serv_t->selectionModel()->select(index,QItemSelectionModel::Select);
    ui->serv_t->setCurrentIndex(index);
}

void services::parse_serv()
{
    ui->serv_t->clearContents();
    ui->serv_t->setRowCount(0);
    QSqlQuery query;
    query.exec("SELECT * FROM services");
    int i = 0;
    while(query.next())
    {
        QString id_serv = query.value(0).toString();
        QString name = query.value(1).toString();
        QTableWidgetItem* a1 =new QTableWidgetItem(id_serv);
        QTableWidgetItem* a2 =new QTableWidgetItem(name);
        ui->serv_t->setRowCount(ui->serv_t->rowCount()+1);
        ui->serv_t->setItem(i,0, a1);
        ui->serv_t->setItem(i,1, a2);
        ++i;
    }
    query.finish();
}
void services::enabled_b_serv(bool pos)
{
    ui->Del_serv_b->setEnabled(pos);
}
void services::on_renew_serv_b_clicked()
{
    parse_serv();
    enabled_b_serv(false);
}

void services::on_serv_t_cellClicked()
{
    enabled_b_serv(true);
}

void services::on_serv_t_cellActivated()
{
    enabled_b_serv(true);
}
