#ifndef SERVICES_H
#define SERVICES_H

#include <QDialog>

namespace Ui {
class services;
}

/*!
    \brief Класс описывает логику работы с окном услуг
*/

class services : public QDialog
{
    Q_OBJECT

public:

/*!
    Конструктор принимает родительский класс.
*/

    explicit services(QWidget *parent = nullptr);
    ~services();

/*!
    \brief Метод для считывания данных об услугах из базы данных и переноса их в таблицу.
*/

    void parse_serv();

/*!
    \brief Метод для выключения/включения кнопки удаления услуги.

    Параметры:

    pos - true (включить) / false (выключить).
 */

    void enabled_b_serv(bool pos);

private slots:
    void on_ok_b_clicked();

    void on_Del_serv_b_clicked();

    void on_Add_serv_b_clicked();

    void on_renew_serv_b_clicked();

    void on_serv_t_cellClicked();

    void on_serv_t_cellActivated();

private:
    Ui::services *ui;
};

#endif // SERVICES_H
