#ifndef ADDSERV_H
#define ADDSERV_H

#include <QDialog>

namespace Ui {
class AddServ;
}

/*!
    \brief Класс описывает логику работы с окном добавления услуг
*/

class AddServ : public QDialog
{
    Q_OBJECT

public:

/*!
    Конструктор принимает родительский класс.
*/

    explicit AddServ(QWidget *parent = nullptr);
    ~AddServ();

private slots:
    void on_ok_clicked();

    void on_no_clicked();

private:
    Ui::AddServ *ui;
};

#endif // ADDSERV_H
