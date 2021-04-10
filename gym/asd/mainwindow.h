/*!
  @mainpage Приложение для фитнес клуба
    ⚡Приложения для ведения базы данных фитнес клуба

    📄Приложение выполнено с помощью графической библиотеки Qt и программной библиотеки SQLite

    🔨Основные функции:

        - Добавление/редактирование/удаление клиентов

        - Добавление/редактирование/удаление тарифов

        - Добавление/удаление услуг

    🔧В папке "default db" лежит тестовая база данных (частично заполненная), чтобы активировать её,
        необходимо скопировать/перенести её в папку с билдом программы

*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "editwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
    \brief Класс описывает логику работы основного окна

    Основное окно содержит 2 вкладки: для работы с клиентами и для работы с услугами/тарифами.

*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

/*!
    Конструктор принимает родительский класс.
*/

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/*!
    \brief Метод для заполнения таблицы именами клиентов.
*/

    void parse_db();

/*!
    \brief Метод для очистки блока "Информация о тарифе".
*/

    void clear_info();

/*!
    \brief Метод для заполнения блока "Информация о тарифе".

    Параметры:

    row - номер строки с клиентом, для которого нужно вывести информацию.
*/

    void parse_info(int row);

/*!
    \brief Метод для выключения/включения кнопок удаления/изменения клиента.

    Параметры:

    pos - true (включить) / false (выключить).
*/

    void enabled_b(bool pos);

/*!
    \brief Метод для вывода всех тарифов в таблицу.
*/

    void parse_tariff();

/*!
    \brief Метод для заполнения блока "Информация о тарифе".

    Параметры:

    row - номер строки с тарифом, для которого нужно вывести информацию.
*/

    void parse_tariff(int row);

/*!
    \brief Метод для очистки блока "Информация о тарифе".
*/

    void clear_info_tariff();
/*!
    \brief Метод для выключения/включения кнопок удаления/изменения тарифа.

    Параметры:

    pos - true (включить) / false (выключить).
 */

    void enabled_b_tariff(bool pos);

/*!
    \brief Метод для контроля отображения блока смены тарифа.

    Параметры:

    pos - true (не отображать) / false (отображать).
 */

    void close_change_tariff(bool pos);

/*!
    \brief Метод для контроля отображения блока продления тарифа.

    Параметры:

    pos - true (не отображать) / false (отображать).
 */

    void close_change_date(bool pos);

/*!
    \brief Метод для проверки на возможность продления тарифа.
*/

    void isChanged();

/*!
    \brief Метод для контроля возможности редактирования полей "тел." и "ФИО".

    Параметры:

    pos - true (нельзя редактировать) / false (можно редактировать).
*/

    void FIO_phone_readonly(bool pos);

/*!
    \brief Метод для поиска по подстроке.

    Параметры:

    arg1 - подстрока, по которой осуществляется поиск.
*/

    void search(const QString &arg1);


signals:

/*!
    \brief Сигнал для передачи id о выбранном тарифе в окно класса "EditWindow".

    Параметры:

    id - primary key для идентификации конкретного тарифа.
 */

    void sentId(QString id);

private slots:

    void on_renew_b_clicked();

    void on_FIO_t_cellClicked(int row);

    void on_add_b_clicked();

    void on_del_b_clicked();

    void on_date_b_clicked();

    void on_FIO_t_cellActivated(int row);

    void on_change_b_clicked();

    void on_tariff_t_cellClicked(int row);

    void on_renew_tariff_clicked();

    void on_tariff_t_cellActivated(int row);

    void on_add_tariff_clicked();

    void on_delete_tariff_clicked();

    void on_edit_tariff_clicked();

    void on_serv_b_clicked();


    void on_ok_t_b_clicked();

    void on_no_t_b_clicked();

    void on_ok_d_b_clicked();

    void on_no_d_b_clicked();

    void on_edit_clicked();

    void on_Search_textChanged(const QString &arg1);

    void on_tabWidget_currentChanged();

private:
    Ui::MainWindow *ui;
    DataBase *db;
    EditWindow *window;
};
#endif // MAINWINDOW_H
