#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <QDialog>

namespace Ui {
class Administrator;
}

class Administrator : public QDialog
{
    Q_OBJECT

public:
    explicit Administrator(QWidget *parent = nullptr);
    ~Administrator();

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息

private slots:
    void on_pushButton_administration_exit_clicked();

    void on_pushButton_allRecord_clicked();

    void PrintAllRecord();
private:
    Ui::Administrator *ui;
};

#endif // ADMINISTRATOR_H
