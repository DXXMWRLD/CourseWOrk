#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>

namespace Ui {
class EditWindow;
}

/*!
    \brief Класс описывает логику работы с окном редактирования тарифа
*/

class EditWindow : public QDialog
{
    Q_OBJECT

public:

/*!
    Конструктор принимает родительский класс.
*/

    explicit EditWindow(QWidget *parent = nullptr);
    ~EditWindow();

/*!
    \brief Метод для считывания данных об услугах из базы данных и переноса их в таблицу.
*/

    void parse_serv();

public slots:

/*!
    \brief Слот для получения информации о выбранном тарифе.

    Параметры:

    id - primary key для идентификации конкретного тарифа.
*/

    void recieveId(QString id);

private slots:

    void on_no_clicked();

    void on_ok_clicked();

    void on_duration_currentTextChanged(const QString &arg1);

    void on_addition_currentTextChanged(const QString &arg1);

private:
    Ui::EditWindow *ui;
    QString id;
};

#endif // EDITWINDOW_H
