#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QDialog>

namespace Ui {
class AddWindow;
}

/*!
    \brief Класс описывает логику работы с окном добавления клиента
*/

class AddWindow : public QDialog
{
    Q_OBJECT

public:

/*!
    Конструктор принимает родительский класс.
*/

    explicit AddWindow(QWidget *parent = nullptr);
    ~AddWindow();

/*!
    \brief Метод для заполнения выпадающего списка "Оплачено месяцев" возможными вариантами для текущего тарифа.

    Параметры:

    arg1 - название выбранного тарифа.
*/

    void set_duration_comboBox(const QString &arg1);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_comboBox_t_textActivated(const QString &arg1);

private:
    Ui::AddWindow *ui;
};

#endif // ADDWINDOW_H
