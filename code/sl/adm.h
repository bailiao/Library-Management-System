#ifndef ADM_H
#define ADM_H

#include <QDialog>

namespace Ui {
class Adm;
}

class Adm : public QDialog
{
    Q_OBJECT

public:
    explicit Adm(QWidget *parent = nullptr);
    ~Adm();

private slots:

    void on_pushButton_administration_exit_clicked();

    void on_pushButton_allRecord_clicked();
    void PrintAllRecord();

    void on_pushButton_adm_CardManagement_clicked();
    void PrintAllCard();

    void on_tableView_adm_all_record_doubleClicked(const QModelIndex &index);

    void on_pushButton_adm_DelRecord_clicked();
    void receive_return_BID(QString);
    void receive_return_CID(QString);

    void on_pushButton_apply_Card_clicked();

    void on_tableView_adm_allCard_doubleClicked(const QModelIndex &index);

    void on_pushButton_adm_DelCard_clicked();

    void on_pushButton_storage_clicked();

    void on_pushButton_adm_BookStorage_clicked();
    void InsertBook(QString, QString, QString, QString, QString, QString);

    void on_pushButton_adm_BookManagement_clicked();

    void on_pushButton_BulkImporting_clicked();

    void on_pushButton_adm_ManagementQuery_clicked();

    void PrintBook();       //打印查询图书

    void UpdateBook();      //更新图书

    void receive_from_query(QString data);  //接受条件信息

    void on_pushButton_adm_ManagementDelete_clicked();

    void on_tableView_adm_BookManagement_doubleClicked(const QModelIndex &index);

    void on_pushButton_adm_ManagementUpdate_clicked();

    void receive_from_update(QString);

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息
    void send_return_BID(QString);
    void send_return_CID(QString);

private:
    Ui::Adm *ui;
};

static QString Condition_adm_query;
static QString Condition_adm_update;

#endif // ADM_H
