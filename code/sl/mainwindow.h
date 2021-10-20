#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Nav_query_clicked();

    void PrintBook();       //打印查询图书

    void on_pushButton_Nav_register_clicked();

    void on_pushButton_Nav_log_clicked();

    void on_pushButton_register_clicked();

    void on_pushButton_log_clicked();

    void receive_from_query(QString data);  //接受条件信息

    void on_pushButton_Nav_rootlog_clicked();

    void on_pushButton_rootlog_clicked();

signals:
    void send_uid(QString);

private:
    Ui::MainWindow *ui;
};

static QString Condition_main;

#endif // MAINWINDOW_H
