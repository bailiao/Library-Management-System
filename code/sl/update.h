#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>

namespace Ui {
class Update;
}

class Update : public QDialog
{
    Q_OBJECT

public:
    explicit Update(QWidget *parent = nullptr);
    ~Update();
    void prepare();

signals:
    void send_Bookupdate_condition(QString);   //用来传递数据的信号
    void sendsignal();//这个函数用户向主界面通知关闭的消息

private slots:
    void on_pushButton_update_confirm_clicked();

    void receive_updateBID(QString);

private:
    Ui::Update *ui;
    void Placeholder();
};

static QString updateBID;

#endif // UPDATE_H
