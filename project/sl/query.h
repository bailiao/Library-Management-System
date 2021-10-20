#ifndef QUERY_H
#define QUERY_H

#include <QDialog>

namespace Ui {
class Query;
}

class Query : public QDialog
{
    Q_OBJECT

public:
    explicit Query(QWidget *parent = nullptr);
    ~Query();


signals:
    void send_Bookquery_condition(QString);   //用来传递数据的信号
    void sendsignal();//这个函数用户向主界面通知关闭的消息

private slots:
    void on_pushButton_clicked();

private:
    Ui::Query *ui;
};

#endif // QUERY_H
