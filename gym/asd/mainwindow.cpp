#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QDate>
#include "addwindow.h"
#include "addtariff.h"
#include <QMessageBox>
#include <QModelIndex>
#include "services.h"
#include "editwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Управление данными");

    //Подключение БД

    db = new DataBase();
    db->connectToDataBase();

    window = new EditWindow();
    window->setModal(true);
    connect(this, &MainWindow::sentId, window, &EditWindow::recieveId);

    ui->tabWidget->setTabText(0,"Клиенты");
    ui->tabWidget->setTabText(1,"Тарифы");

    //Базовые настройки виджетов

    close_change_tariff(true);
    close_change_date(true);
    ui->Date->setReadOnly(true);
    FIO_phone_readonly(true);
    ui->edit->setText("Редактировать ФИО/тел.");
    ui->status->setReadOnly(true);
    ui->tariff->setReadOnly(true);
    enabled_b(false);

    //Найстройка таблицы клиентов

    QStringList LIST1;
    LIST1 << "ID" << "ФИО";
    ui->FIO_t->setColumnCount(2);
    ui->FIO_t->setColumnHidden(0,true);
    ui->FIO_t->setHorizontalHeaderLabels(LIST1);
    ui->FIO_t->horizontalHeader()->setStretchLastSection(true);
    ui->FIO_t->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->FIO_t->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->FIO_t->setEditTriggers(0);

    parse_db();

    //Найстройка таблицы тарифов

    ui->tariff_t->setColumnCount(2);
    ui->tariff_t->setColumnHidden(0,true);
    QStringList LIST2;
    LIST2 << "ID" << "Тариф";
    ui->tariff_t->setHorizontalHeaderLabels(LIST2);
    ui->tariff_t->horizontalHeader()->setStretchLastSection(true);
    ui->tariff_t->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tariff_t->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tariff_t->setEditTriggers(0);
    parse_tariff();

    //Найстройка таблицы включенных в тариф услуг

    ui->services_t->setColumnCount(2);
    ui->services_t->setColumnHidden(0,true);
    QStringList LIST3;
    LIST3 << "ID" << "Включенные услуги";
    ui->services_t->setHorizontalHeaderLabels(LIST3);
    ui->services_t->horizontalHeader()->setStretchLastSection(true);
    ui->services_t->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->services_t->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->services_t->setEditTriggers(0);
    ui->addition->setReadOnly(true);
    ui->addition_praise->setReadOnly(true);
    ui->tariff_name->setReadOnly(true);
    ui->duration->setReadOnly(true);
    ui->praise->setReadOnly(true);
    enabled_b_tariff(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

const QString NoLimited = "-1";
const QString Yes = "1";

void MainWindow::close_change_tariff(bool pos)
{
    ui->change_t->clear();
    ui->change_t->setHidden(pos);
    ui->ok_t_b->setHidden(pos);
    ui->no_t_b->setHidden(pos);
}

void MainWindow::on_change_b_clicked()
{
    parse_info(ui->FIO_t->currentRow());
    close_change_date(true);
    close_change_tariff(false);
    FIO_phone_readonly(true);
    ui->edit->setText("Редактировать ФИО/тел.");
    QSqlQuery query;
    QString text = ui->FIO_t->item(ui->FIO_t->currentRow(),0)->text();
    query.exec("SELECT id_tariff FROM users where id = " + text + ";");
    query.next();
    text = query.value(0).toString();
    query.clear();
    query.exec("SELECT name FROM tariff where id_tariff != " + text + ";");
    while(query.next())
    {
        ui->change_t->addItem(query.value(0).toString());
    }
    query.finish();
}

void MainWindow::on_ok_t_b_clicked()
{
    QSqlQuery query;
    query.exec("SELECT id_tariff FROM tariff WHERE name = '" + ui->change_t->currentText() + "';");
    query.next();
    QString id_tariff = query.value(0).toString();
    QString id = ui->FIO_t->item(ui->FIO_t->currentRow(), 0)->text();
    query.clear();
    query.exec("UPDATE users SET id_tariff = " + id_tariff + " where id = " + id + ";");
    close_change_tariff(true);
    int RowIndex =ui->FIO_t->currentRow();
    parse_db();
    parse_info(RowIndex);
    QModelIndex index = ui->FIO_t->model()->index(RowIndex, 0);
    ui->FIO_t->selectionModel()->select(index,QItemSelectionModel::Select);
    ui->FIO_t->setCurrentIndex(index);
    isChanged();
}

void MainWindow::on_no_t_b_clicked()
{
    close_change_tariff(true);
}

void MainWindow::close_change_date(bool pos)
{
    if (pos)
    {
        ui->add_t->clear();
    }
    else
    {
        QString text;
        for(int i = 1; i <= 12; ++i )
        {
            ui->add_t->addItem(text.number(i));
        }
    }
    ui->add_t->setHidden(pos);
    ui->ok_d_b->setHidden(pos);
    ui->no_d_b->setHidden(pos);
}

void MainWindow::on_date_b_clicked()
{
    parse_info(ui->FIO_t->currentRow());
    close_change_date(false);
    close_change_tariff(true);
    FIO_phone_readonly(true);
    ui->edit->setText("Редактировать ФИО/тел.");
}

void MainWindow::on_ok_d_b_clicked()
{
    QDate end_date = QDate::fromString(ui->Date->text(),"dd.MM.yy");
    if (end_date.toString("yy.MM.dd") < QDate::currentDate().toString("yy.MM.dd"))
    {
        end_date = QDate::currentDate().addMonths(ui->add_t->currentText().toInt());
    }
    else
    {
        end_date = end_date.addMonths(ui->add_t->currentText().toInt());
    }
    QString separator = (QString)'"';
    QSqlQuery query;
    query.exec("UPDATE users SET end_date = " + separator + end_date.toString("dd.MM.yy") + separator
                                          + " WHERE id = " + ui->FIO_t->item(ui->FIO_t->currentRow(), 0)->text() + ";");
    close_change_date(true);
    int RowIndex =ui->FIO_t->currentRow();
    parse_db();
    parse_info(RowIndex);
    QModelIndex index = ui->FIO_t->model()->index(RowIndex, 0);
    ui->FIO_t->selectionModel()->select(index,QItemSelectionModel::Select);
    ui->FIO_t->setCurrentIndex(index);
}

void MainWindow::on_no_d_b_clicked()
{
    close_change_date(true);
}

void MainWindow::isChanged()
{
    QSqlQuery query;
    query.exec("SELECT duration, addition FROM tariff where name = '" + ui->tariff->text() + "';");
    query.next();
    QString duration = query.value(0).toString();
    QString addition = query.value(1).toString();
    if ((duration == NoLimited) || (addition == Yes))
    {
        ui->date_b->setEnabled(true);
    }
    else
    {
        ui->date_b->setEnabled(false);
    }
    query.finish();
}

void MainWindow::FIO_phone_readonly(bool pos)
{
    ui->phone->setReadOnly(pos);
    ui->FIO->setReadOnly(pos);
}

void MainWindow::on_edit_clicked()
{
    if(ui->edit->text() == "Редактировать ФИО/тел.")
    {
        close_change_date(true);
        close_change_tariff(true);
        FIO_phone_readonly(false);
        ui->edit->setText("Сохранить изменения");
    }
    else
    {
        FIO_phone_readonly(true);
        ui->edit->setText("Редактировать ФИО/тел.");
        QSqlQuery query;
        query.exec("UPDATE users SET name = '" + ui->FIO->text() + "' WHERE id = " + ui->FIO_t->item(ui->FIO_t->currentRow(),0)->text()+ ";");
        query.clear();
        query.exec("UPDATE users SET phone = '" + ui->phone->text() + "' WHERE id = " + ui->FIO_t->item(ui->FIO_t->currentRow(),0)->text()+ ";");
        query.finish();
        QModelIndex index = ui->FIO_t->model()->index(ui->FIO_t->currentRow(), 0);
        parse_db();
        ui->FIO_t->selectionModel()->select(index,QItemSelectionModel::Select);
        ui->FIO_t->setCurrentIndex(index);
    }
}

void MainWindow::enabled_b(bool pos)
{
    ui->del_b->setEnabled(pos);
    ui->date_b->setEnabled(pos);
    ui->change_b->setEnabled(pos);
    ui->edit->setEnabled(pos);
}

void MainWindow::on_add_b_clicked()
{
    QSqlQuery query;
    query.exec("SELECT * FROM tariff;");
    if (query.next())
    {
        close_change_date(true);
        close_change_tariff(true);
        FIO_phone_readonly(true);
        ui->edit->setText("Редактировать ФИО/тел.");

        AddWindow window;
        window.setModal(true);
        window.exec();
        QString searchText = ui->Search->text();
        int searchIndex = ui->FIO_t->currentRow();
        ui->Search->clear();
        int count = ui->FIO_t->rowCount();
        parse_db();
        if (count != ui->FIO_t->rowCount()){
            ui->FIO_t->scrollToBottom();
            QModelIndex index = ui->FIO_t->model()->index(ui->FIO_t->rowCount() - 1, 0);
            ui->FIO_t->selectionModel()->select(index,QItemSelectionModel::Select);
            ui->FIO_t->setCurrentIndex(index);
            parse_info(ui->FIO_t->rowCount() - 1);
            enabled_b(true);
            isChanged();
        }
        else
        {
            ui->Search->setText(searchText);
            QModelIndex index = ui->FIO_t->model()->index(searchIndex, 0);
            ui->FIO_t->selectionModel()->select(index,QItemSelectionModel::Select);
            ui->FIO_t->setCurrentIndex(index);
            parse_info(searchIndex);
        }
    }
    else
    {
        QMessageBox::critical(this,"Ошибка добавления","Нет доступных тарифов!");
    }
}

void MainWindow::on_del_b_clicked()
{
    QMessageBox but (QMessageBox::Question, "Удаление","Удалить клиента?", QMessageBox::Yes | QMessageBox::No, this);
    but.setButtonText(QMessageBox::Yes, "Да");
    but.setButtonText(QMessageBox::No, "Нет");
    if (but.exec() == QMessageBox::Yes)
    {
        close_change_date(true);
        close_change_tariff(true);
        FIO_phone_readonly(true);
        ui->edit->setText("Редактировать ФИО/тел.");
        if (ui->FIO_t->currentRow() != -1)
        {
        QSqlQuery query;
        QString id = ui->FIO_t->item(ui->FIO_t->currentRow(),0)->text();
        query.exec("DELETE FROM users where id = " + id + ";");
        clear_info();
        parse_db();
        enabled_b(false);
        query.finish();
        }
        else
        {
            QMessageBox::critical(this,"Ошибка удаления","Выберите объект удаления!");
        }
    }
}

void MainWindow::on_Search_textChanged(const QString &arg1)
{
    close_change_date(true);
    close_change_tariff(true);
    enabled_b(false);
    ui->Search->setClearButtonEnabled(true);

    QModelIndex index = ui->FIO_t->model()->index(-1, 0);
    ui->FIO_t->selectionModel()->select(index,QItemSelectionModel::Select);
    ui->FIO_t->setCurrentIndex(index);
    clear_info();
    search(arg1);
}

void MainWindow::search(const QString &arg1)
{
    for (int i = 0; i < ui->FIO_t->rowCount(); ++i)
    {
        if(ui->FIO_t->item(i,1)->text().contains(arg1,Qt::CaseInsensitive))
        {
            ui->FIO_t->showRow(i);
        }
        else
        {
            ui->FIO_t->hideRow(i);
        }
    }
}

void MainWindow::parse_db()
{
    ui->FIO_t->clearContents();
    ui->FIO_t->setRowCount(0);
    QSqlQuery query;
    query.exec("SELECT id, name FROM users;");
    int i = 0;
    while (query.next())
    {
    QString id = query.value(0).toString();
    QString name = query.value(1).toString();
    QTableWidgetItem* a1 =new QTableWidgetItem(id);
    QTableWidgetItem* a2 =new QTableWidgetItem(name);
    ui->FIO_t->setRowCount(ui->FIO_t->rowCount()+1);
    ui->FIO_t->setItem(i,0, a1);
    ui->FIO_t->setItem(i,1, a2);
    ++i;
    }
    query.finish();
    search(ui->Search->text());
}

void MainWindow::parse_info(int row)
{
    if(row != -1)
    {
        QString text = ui->FIO_t->item(row, 0)->text();
        QSqlQuery query;
        query.exec("SELECT * FROM users where id = " + text + ";");
        query.next();
        ui->FIO->setText(query.value(1).toString());
        ui->Date->setText(query.value(3).toString());
        ui->phone->setText(query.value(2).toString());
        QDate end_date = QDate::fromString(ui->Date->text(), "dd.MM.yy").addYears(100);
        if (end_date >= QDate::currentDate())
            ui->status->setText("Активен");
        else
            ui->status->setText("Неактивен");
        text = query.value(4).toString();
        query.clear();
        query.exec("SELECT name FROM tariff where id_tariff = " + text + ";");
        query.next();
        if (!query.value(0).toString().isEmpty())
            ui->tariff->setText(query.value(0).toString());
        else
            ui->tariff->setText("Нет тарифа");
        query.finish();
    }
}

void MainWindow::clear_info()
{
    ui->FIO->clear();
    ui->Date->clear();
    ui->phone->clear();
    ui->tariff->clear();
    ui->status->clear();
}

void MainWindow::on_renew_b_clicked()
{
    parse_db();
    clear_info();
    enabled_b(false);
    ui->FIO_t->scrollToTop();
    close_change_tariff(true);
    close_change_date(true);
    FIO_phone_readonly(true);
    ui->edit->setText("Редактировать ФИО/тел.");
    ui->Search->clear();
}

void MainWindow::on_FIO_t_cellClicked(int row)
{
    parse_info(row);
    enabled_b(true);
    close_change_tariff(true);
    close_change_date(true);
    isChanged();
    FIO_phone_readonly(true);
    ui->edit->setText("Редактировать ФИО/тел.");
}

void MainWindow::on_FIO_t_cellActivated(int row)
{
    parse_info(row);
    enabled_b(true);
    close_change_tariff(true);
    close_change_date(true);
    isChanged();
    FIO_phone_readonly(true);
    ui->edit->setText("Редактировать ФИО/тел.");
}

void MainWindow::parse_tariff()
{
    ui->tariff_t->clearContents();
    ui->tariff_t->setRowCount(0);
    QSqlQuery query;
    query.exec("SELECT id_tariff, name FROM tariff;");
    int i = 0;
    while (query.next())
    {
    QString id_tariff = query.value(0).toString();
    QString name = query.value(1).toString();
    QTableWidgetItem* a1 =new QTableWidgetItem(id_tariff);
    QTableWidgetItem* a2 =new QTableWidgetItem(name);
    ui->tariff_t->setRowCount(ui->tariff_t->rowCount()+1);
    ui->tariff_t->setItem(i,0, a1);
    ui->tariff_t->setItem(i,1, a2);
    ++i;
    }
    query.finish();
}

void MainWindow::enabled_b_tariff(bool pos)
{
    ui->edit_tariff->setEnabled(pos);
    ui->delete_tariff->setEnabled(pos);
}

void MainWindow::parse_tariff(int row)
{
    ui->services_t->clearContents();
    ui->services_t->setRowCount(0);
    QString text = ui->tariff_t->item(row, 0)->text();
    QSqlQuery query;
    query.exec("SELECT id_serv FROM tariff_services where id_tariff = " + text + ";");
    int i = 0;
    QSqlQuery queryIn;
    while(query.next())
    {
        QString id_serv;
        id_serv = query.value(0).toString();
        queryIn.exec("SELECT name FROM services where id_serv = " + id_serv + ";");
        queryIn.next();
        QString name = queryIn.value(0).toString();
        QTableWidgetItem* a1 =new QTableWidgetItem(id_serv);
        QTableWidgetItem* a2 =new QTableWidgetItem(name);
        ui->services_t->setRowCount(ui->services_t->rowCount()+1);
        ui->services_t->setItem(i,0, a1);
        ui->services_t->setItem(i,1, a2);
        ++i;
        queryIn.finish();
    }
    ui->services_t->scrollToTop();
    query.clear();
    query.exec("SELECT * FROM tariff where id_tariff = " + text + ";");
    query.next();
    ui->tariff_name->setText(query.value(1).toString());
    if (query.value(2).toString() != NoLimited)
    {
        ui->duration->setText(query.value(2).toString() + " мес.");
        if (query.value(4).toString() == Yes)
        {
            ui->addition->setText("Можно пополнять");
            ui->addition_praise->setText(query.value(5).toString());
            ui->addition_praise->setHidden(false);
            ui->addition->setHidden(false);
            ui->l_duration->setHidden(false);
            ui->l_duration_praise->setHidden(false);
        }
        else
        {
            ui->addition->setText("Нельзя пополнять");
            ui->addition->setHidden(false);
            ui->addition_praise->setHidden(true);
            ui->l_duration_praise->setHidden(true);
            ui->l_duration->setHidden(false);
        }
    }
    else
    {
        ui->duration->setText("Ежемесячно обновляемый");
        ui->addition->setHidden(true);
        ui->addition_praise->setHidden(true);
        ui->l_duration->setHidden(true);
        ui->l_duration_praise->setHidden(true);

    }
    ui->praise->setText(query.value(3).toString());
    query.finish();
}

void MainWindow::on_tariff_t_cellClicked(int row)
{
    parse_tariff(row);
    enabled_b_tariff(true);
}

void MainWindow::on_tariff_t_cellActivated(int row)
{
    parse_tariff(row);
    enabled_b_tariff(true);
}

void MainWindow::on_renew_tariff_clicked()
{
    clear_info_tariff();
    parse_tariff();
    clear_info();
    enabled_b_tariff(false);
    ui->tariff_t->scrollToTop();
}

void MainWindow::clear_info_tariff()
{
    ui->addition->clear();
    ui->addition_praise->clear();
    ui->duration->clear();
    ui->praise->clear();
    ui->tariff_name->clear();
    ui->services_t->clearContents();
    ui->services_t->setRowCount(0);
    ui->addition_praise->setHidden(false);
    ui->addition->setHidden(false);
    ui->l_duration->setHidden(false);
    ui->l_duration_praise->setHidden(false);
}

void MainWindow::on_add_tariff_clicked()
{
    QSqlQuery query;
    query.exec("SELECT * FROM services;");
    if (query.next())
    {
        AddTariff window;
        window.setModal(true);
        int count = ui->tariff_t->rowCount();
        int preindex = ui->tariff_t->currentRow();
        window.exec();
        parse_tariff();
        clear_info_tariff();
        if (count != ui->tariff_t->rowCount()){
            ui->tariff_t->scrollToBottom();
            QModelIndex index = ui->tariff_t->model()->index(ui->tariff_t->rowCount() - 1, 0);
            ui->tariff_t->selectionModel()->select(index,QItemSelectionModel::Select);
            ui->tariff_t->setCurrentIndex(index);
            parse_tariff(ui->tariff_t->rowCount() - 1);
            enabled_b(true);
        }
        else
        {
            if (preindex != -1)
            {
                QModelIndex index = ui->tariff_t->model()->index(preindex, 0);
                ui->tariff_t->selectionModel()->select(index,QItemSelectionModel::Select);
                ui->tariff_t->setCurrentIndex(index);
                parse_tariff(preindex);
            }
        }
    }
    else
    {
        QMessageBox::critical(this,"Ошибка добавления","Нет доступных услуг!");
    }
}

void MainWindow::on_delete_tariff_clicked()
{
    QSqlQuery query;
    query.exec("SELECT * FROM users WHERE id_tariff = " + ui->tariff_t->item(ui->tariff_t->currentRow(),0)->text() + ";");
    if (!query.next())
    {
        QMessageBox but (QMessageBox::Question, "Удаление","Удалить тариф?", QMessageBox::Yes | QMessageBox::No, this);
        but.setButtonText(QMessageBox::Yes, "Да");
        but.setButtonText(QMessageBox::No, "Нет");
        if (but.exec() == QMessageBox::Yes)
        {
            if (ui->tariff_t->currentRow() != -1)
            {
            QSqlQuery query;
            QString id = ui->tariff_t->item(ui->tariff_t->currentRow(),0)->text();
            query.exec("DELETE FROM tariff where id_tariff = " + id + ";");
            query.clear();
            query.exec("DELETE FROM tariff_services where id_tariff = " + id + ";");
            clear_info_tariff();
            parse_tariff();
            enabled_b_tariff(false);
            query.finish();
            }
            else
            {
                QMessageBox::critical(this,"Ошибка удаления","Выберите объект удаления!");
            }
        }
    }
    else
    {
        QMessageBox::critical(this,"Ошибка удаления","Вы не можете удалить тариф, так как он используется!");
    }
}

void MainWindow::on_edit_tariff_clicked()
{
    int preindex = ui->tariff_t->currentRow();
    emit sentId(ui->tariff_t->item(ui->tariff_t->currentRow(),0)->text());
    window->exec();
    parse_tariff();
    clear_info_tariff();
    ui->tariff_t->scrollToBottom();
    QModelIndex index = ui->tariff_t->model()->index(preindex, 0);
    ui->tariff_t->selectionModel()->select(index,QItemSelectionModel::Select);
    ui->tariff_t->setCurrentIndex(index);
    parse_tariff(preindex);
    enabled_b(true);
}

void MainWindow::on_serv_b_clicked()
{
    services window;
    window.setModal(true);
    window.exec();

    clear_info_tariff();
    parse_tariff();
    clear_info();
    enabled_b_tariff(false);
    ui->tariff_t->scrollToTop();
}

void MainWindow::on_tabWidget_currentChanged()
{
    //Обновление 1 листа
    parse_db();
    clear_info();
    enabled_b(false);
    ui->FIO_t->scrollToTop();
    close_change_tariff(true);
    close_change_date(true);
    FIO_phone_readonly(true);
    ui->edit->setText("Редактировать ФИО/тел.");
    ui->Search->clear();

    //Обновление 2 листа
    clear_info_tariff();
    parse_tariff();
    clear_info();
    enabled_b_tariff(false);
    ui->tariff_t->scrollToTop();
}
