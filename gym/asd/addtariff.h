#ifndef ADDTARIFF_H
#define ADDTARIFF_H

#include <QDialog>

namespace Ui {
class AddTariff;
}

/*!
    \brief Класс описывает логику работы с окном добавления тарифа
*/

class AddTariff : public QDialog
{
    Q_OBJECT

public:

/*!
    Конструктор принимает родительский класс.
*/

    explicit AddTariff(QWidget *parent = nullptr);
    ~AddTariff();

/*!
    \brief Метод для считывания данных об услугах из базы данных и переноса их в таблицу.
*/

    void parse_serv();


private slots:
    void on_no_clicked();

    void on_ok_clicked();

    void on_addition_currentTextChanged(const QString &arg1);

    void on_duration_currentTextChanged(const QString &arg1);

private:
    Ui::AddTariff *ui;
};

#endif // ADDTARIFF_H
