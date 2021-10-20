#ifndef USER_H
#define USER_H

#include <QDialog>

namespace Ui {
class User;
}

class User : public QDialog
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);
    ~User();

private slots:
    void on_pushButton_Nav_user_exit_clicked();

    void on_pushButton_Nav_user_record_clicked();

    void receive_uid(QString);
    void on_pushButton_Nav_update_passwd_clicked();

    void on_pushButton_register_clicked();

    void on_pushButton_Nav_user_borrow_clicked();

    void on_pushButton_userBorrow_clicked();

    void on_pushButton_Nav_user_return_clicked();
    void receive_return_BID(QString);
    void receive_return_CID(QString);

    void PrintRecord();
    void PrintBook();

    void on_tableView_userRecord_doubleClicked(const QModelIndex &index);

    void on_pushButton_Nav_user_query_clicked();
    void receive_from_query(QString data);  //接受条件信息

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息
    void send_return_BID(QString);
    void send_return_CID(QString);

private:
    Ui::User *ui;
    QString uid;
    void Update_RecordandBook(QString BID, QString CID);
};

static QString Condition_user;

#endif // USER_H
