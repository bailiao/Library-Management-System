#include "user.h"
#include "ui_user.h"
#include "query.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDateTime>
#include <QMessageBox>

User::User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page_user_record);
    ui->lineEdit_updatepasswd_passwd1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_updatepasswd_passwd2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_updatepasswd_passwd3->setEchoMode(QLineEdit::Password);
    ui->tableView_userRecord->resize(800,500);
    ui->tableView_user_query->resize(800,500);
}

User::~User()
{
    delete ui;
}

void User::on_pushButton_Nav_user_exit_clicked()
{
    emit sendsignal();
    this->close();
}

void User::receive_uid(QString ID)
{
    this->uid = ID;
}

void User::PrintRecord()
{
    QString str;
    QSqlQueryModel *dbmodel = new QSqlQueryModel();
    ui->stackedWidget->setCurrentWidget(ui->page_user_record);
    ui->tableView_userRecord->setModel(dbmodel);
    ui->tableView_userRecord->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    str = "select Bid as 图书编号, BookName as 书名, Publisher as 出版社, "
          "Author as 作者, Year as 出版年份, Record.CardID as 卡号, BorrowTime as 借出时间, "
          "ReturnTime as 应还时间 from Book natural join Record inner join Card on "
          "(Card.CardID = Record.CardID) where Uid = " + this->uid;
    qDebug() << str;
    dbmodel->setQuery(str);
}

void User::on_pushButton_Nav_user_record_clicked()
{
    PrintRecord();
}

void User::on_pushButton_Nav_update_passwd_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_update_passwd);
}

void User::on_pushButton_register_clicked()
{
    ui->label_updatepasswd_infor->setText("");  //清空提示信息
    ui->label_updatepasswd_infor->setStyleSheet("color: rgb(255, 78, 25);");//用于输出注册信息

    QSqlQuery query;
    QString firstPassWd = ui->lineEdit_updatepasswd_passwd1->text();//原密码
    QString secondPassWd = ui->lineEdit_updatepasswd_passwd2->text();//新密码
    QString thirdPassWd = ui->lineEdit_updatepasswd_passwd2->text();//确认密码
    QString str;
    bool flag = true;

    if(0 == firstPassWd.size() || secondPassWd.size() > 20)
    {
        ui->label_updatepasswd_infor->setText("密码不符合规范");
        flag = false;
    }

    if(0 == secondPassWd.size())
    {
        ui->label_updatepasswd_infor->setText("密码不能为空!");
        flag = false;
    }
    if(secondPassWd.size() > 20)
    {
        ui->label_updatepasswd_infor->setText("密码长度不能超过20位!");
        flag = false;
    }

    if(0 != secondPassWd.compare(thirdPassWd))
    {
        ui->label_updatepasswd_infor->setText("两次输入的密码不一致!");
        flag = false;
    }

    if(flag) {
        secondPassWd = "'" + secondPassWd +"'";
        str = "update User set Password = " + secondPassWd + " where Uid = "
              + this->uid + ";" ;
        bool success = query.exec(str);
        if(success) {
            ui->label_updatepasswd_infor->setText("修改成功");
        }else {
            ui->label_updatepasswd_infor->setText("修改失败");
        }
    }
}

void User::on_pushButton_Nav_user_borrow_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_user_borrow);
}

void User::Update_RecordandBook(QString BID, QString CID)
{
    QSqlQuery query;
    QString str;
    //更新Record表
    QDateTime current_date_time = QDateTime::currentDateTime();
    QDateTime return_date_time = current_date_time.addMonths(1);
    QString current_date = current_date_time.toString("yyyy-MM-dd");
    QString return_date = return_date_time.toString("yyyy-MM-dd");
    current_date = "'" + current_date + "'";
    return_date = "'" + return_date + "'";
    str = "Insert Record values(" +  CID + ", " + BID + ", "
            + current_date + ", " + return_date + ");";
    bool success = query.exec(str);
    if(!success) {
        //qDebug() << query.record();

        ui->label_userBorrow_infor->setText("您已借阅过相同的书籍");
        return;
    }
    //

    //更新Book表
    str = "update Book set Storage = Storage - 1 where Bid = " + BID + ";" ;
    qDebug() << str;
    success = query.exec(str);
    //

    if(success) ui->label_userBorrow_infor->setText("借阅成功");
    return;
}


void User::on_pushButton_userBorrow_clicked()
{
    ui->label_userBorrow_infor->setText("");  //清空提示信息
    ui->label_userBorrow_infor->setStyleSheet("color: rgb(255, 78, 25);");//用于输出注册信息

    QSqlQuery query;
    QString BookID = ui->lineEdit_userBorrow_BID->text();//图书编号
    QString CardID = ui->lineEdit_userBorrow_CID->text();//借书卡卡号
    QString str;
    bool flag = true;

    if(0 == BookID.size())
    {
        ui->label_userBorrow_infor->setText("图书编号不能为空!");
        flag = false;
    }

    if(0 == CardID.size())
    {
        ui->label_userBorrow_infor->setText("卡号不能为空!");
        flag = false;
    }


    if(flag) {
        BookID = "'" + BookID + "'";
        CardID = "'" + CardID + "'";
        str = "select * from Book where Bid = " + BookID + ";" ;
        //qDebug() << str;
        bool successBook = query.exec(str);
        if(successBook) {       //查找图书语句是否能正确执行
            if(query.next()) {  //结构是否为空集
                str.insert(str.length()-1, " and Storage > 0");
                bool successStorage = query.exec(str);
                //qDebug() << str;
                if(successStorage) {        //查询余量语句是否能正确执行
                    if(query.next()) {
                        str = "select * from Card where CardID = " + CardID + " and "
                        "Uid = " + this->uid + ";" ;
                        //qDebug() << str;
                        bool successCard = query.exec(str);
                        if(successCard) {
                            if(query.exec()) {
                                Update_RecordandBook(BookID, CardID);
                            }else {
                                ui->label_userBorrow_infor->setText("此卡号不属于当前用户");
                            }
                        }else {
                            ui->label_userBorrow_infor->setText("查询卡号发生错误");
                        }
                    }else {
                        ui->label_userBorrow_infor->setText("此书已全部被借走");
                    }
                }else {
                    ui->label_userBorrow_infor->setText("查询余量发生错误");
                }
            }else {
                ui->label_userBorrow_infor->setText("馆内没有收录该书");
            }
        }else {
            ui->label_userBorrow_infor->setText("查找书目发生错误");
        }
    }
}

void User::on_tableView_userRecord_doubleClicked(const QModelIndex &index)
{
    QAbstractItemModel *Imodel = ui->tableView_userRecord->model();
    QModelIndex Iindex0 = Imodel->index(index.row(),0);//index.row()为算选择的行号,0为所选中行的第一列,即Bid
    QModelIndex Iindex5 = Imodel->index(index.row(),5);//CID
    QVariant datatemp0 = Imodel->data(Iindex0);
    QVariant datatemp5 = Imodel->data(Iindex5);
    QString BID = datatemp0.toString();//BID即为所选择行的第一列的值
    QString CID = datatemp5.toString();
    qDebug() << BID;
    connect( this, SIGNAL(send_return_BID(QString)), this, SLOT(receive_return_BID(QString)) );
    connect( this, SIGNAL(send_return_CID(QString)), this, SLOT(receive_return_CID(QString)) );
    emit send_return_BID(BID);	//传递得到的信息
    emit send_return_CID(CID);
}

static QString return_BID, return_CID;
void User::receive_return_BID(QString BID)
{
    return_BID = "'" + BID + "'";
}

void User::receive_return_CID(QString CID)
{
    return_CID = "'" + CID + "'";
}

void User::on_pushButton_Nav_user_return_clicked()
{
    QSqlQuery query;
    QString str;

    //从Record表中删除借阅记录
    str = "delete from Record where Bid =" + return_BID + " and CardID = "
            + return_CID + ";" ;
    bool success_del = query.exec(str);
    qDebug() << str;
    //更新Book表的图书余量Storage
    str = "update Book set Storage = Storage + 1 where Bid = " + return_BID + ";" ;
    bool success_upd = query.exec(str);

    if(success_del && success_upd) {
        QMessageBox::information(this, "信息", "还书成功", QMessageBox::Ok);
    }else {
        QMessageBox::critical(this, "错误", "还书失败", QMessageBox::Ok);
    }

    PrintRecord();
}


void User::receive_from_query(QString data)
{
    Condition_user = data;
    //qDebug() << Condition;
}

void User::PrintBook()
{
    QString str;
    QSqlQueryModel *dbmodel = new QSqlQueryModel();
    ui->tableView_user_query->setModel(dbmodel);
    ui->tableView_user_query->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if(0 != Condition_user.compare(""))
        str = "select Bid as 图书编号, BookName as 书名, Publisher as 出版社, "
              "Author as 作者, Year as 出版年份, Total as 总量, Storage as 余量, "
              "UpdateTime as 最新入库时间 from Book where " + Condition_user;
    else
        str = "select* from Book";
    qDebug() << str;
    dbmodel->setQuery(str);
}

void User::on_pushButton_Nav_user_query_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_user_query);
    Query *q = new Query(this);
    connect(q, SIGNAL(send_Bookquery_condition(QString)), this, SLOT(receive_from_query(QString)));
    connect(q, SIGNAL(sendsignal()), this, SLOT(PrintBook()));
    q->show();
}
