#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user.h"
#include "query.h"
#include "adm.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page_index);
    ui->lineEdit_register_passwd1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_register_passwd2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_rootlog_passwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_log_passwd->setEchoMode(QLineEdit::Password);
    QFont ft;
    ft.setPointSize(20);
    ui->label_4->setFont(ft);
    ui->tableView->resize(800,500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receive_from_query(QString data)
{
    Condition_main = data;
    //qDebug() << Condition;
}

void MainWindow::PrintBook()
{
    QString str;
    QSqlQueryModel *dbmodel = new QSqlQueryModel();
    ui->tableView->setModel(dbmodel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if(0 != Condition_main.compare(""))
        str = "select Bid as 图书编号, BookName as 书名, Publisher as 出版社, "
              "Author as 作者, Year as 出版年份, Total as 总量, Storage as 余量, "
              "UpdateTime as 最新入库时间 from Book where " + Condition_main + " order by Bid;";
    else
        str = "select* from Book order by Bid;";
    qDebug() << str;
    dbmodel->setQuery(str);
}

void MainWindow::on_pushButton_Nav_query_clicked()
{
    Query *q = new Query(this);
    connect(q, SIGNAL(send_Bookquery_condition(QString)), this, SLOT(receive_from_query(QString)));
    connect(q, SIGNAL(sendsignal()), this, SLOT(PrintBook()));
    q->show();
    ui->stackedWidget->setCurrentWidget(ui->page_querybooks);
}


void MainWindow::on_pushButton_Nav_register_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_register);
}


void MainWindow::on_pushButton_Nav_log_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_log);
}

void MainWindow::on_pushButton_register_clicked()
{
    ui->label_register_infor->setText("");  //清空提示信息
    ui->label_register_infor->setStyleSheet("color: rgb(255, 78, 25);");//用于输出注册信息

    QSqlQuery query;
    QString uid = ui->lineEdit_register_id->text();//用户名
    QString firstPassWd = ui->lineEdit_register_passwd1->text();//用户密码
    QString secondPassWd = ui->lineEdit_register_passwd2->text();//确认密码
    QString str;
    bool flag = true;

    if(0 == uid.size())
    {
        ui->label_register_infor->setText("用户名不能为空!");
        flag = false;
    }
    if(uid.size() > 20)
    {
        ui->label_register_infor->setText("用户名长度不能超过20位!");
        flag = false;
    }

    if(0 == firstPassWd.size())
    {
        ui->label_register_infor->setText("密码不能为空!");
        flag = false;
    }
    if(firstPassWd.size() > 20)
    {
        ui->label_register_infor->setText("密码长度不能超过20位!");
        flag = false;
    }

    if(0 != firstPassWd.compare(secondPassWd))
    {
        ui->label_register_infor->setText("两次输入的密码不一致!");
        flag = false;
    }


    if(flag) {
        uid = "'" + uid + "'";
        firstPassWd = "'" + firstPassWd +"'";
        str = "insert User values(" + uid + "," + firstPassWd + ",'User');" ;
        //qDebug() << str;
        bool success = query.exec(str);
        if(success) {
            ui->label_register_infor->setText("注册成功");
        }else {
            ui->label_register_infor->setText("注册失败");
        }
    }
}

void MainWindow::on_pushButton_log_clicked()
{
    ui->label_log_infor->setText("");  //清空提示信息
    ui->label_log_infor->setStyleSheet("color: rgb(255, 78, 25);");//用于输出登录信息

    QSqlQuery query;
    QString uid = ui->lineEdit_log_uid->text();//用户名
    QString PassWd = ui->lineEdit_log_passwd->text();//用户密码
    QString str;
    bool flag = true;
    qDebug() << uid << PassWd;
    if(0 == uid.size())
    {
        ui->label_log_infor->setText("用户名不能为空!");
        flag = false;
    }
    if(uid.size() > 20)
    {
        ui->label_log_infor->setText("用户名长度不能超过20位!");
        flag = false;
    }

    if(0 == PassWd.size())
    {
        ui->label_log_infor->setText("密码不能为空!");
        flag = false;
    }
    if(PassWd.size() > 20)
    {
        ui->label_log_infor->setText("密码长度不能超过20位!");
        flag = false;
    }


    if(flag) {
        uid = "'" + uid + "'";
        PassWd = "'" + PassWd +"'";
        str = "select * from User where Uid = " + uid + " and Password = " + PassWd +
               " and Type = 'User'"  + ";" ;
        qDebug() << str;
        bool success = query.exec(str);
        if(success) {
            if(query.next()) {  //select结果集不为空，即存在这个用户
                ui->label_log_infor->setText("登录成功");
                this->hide();
                User *u = new User(this);
                u->setFixedSize(1000, 600);
                connect(u, SIGNAL(sendsignal()), this, SLOT(show()));
                connect(this, SIGNAL(send_uid(QString)), u, SLOT(receive_uid(QString)));
                emit send_uid(uid);
                u->show();
                ui->label_log_infor->setText("");  //清空提示信息
            }else {
                ui->label_log_infor->setText("账号或密码错误");
            }
        }
    }
}


void MainWindow::on_pushButton_Nav_rootlog_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_rootlog);
}

void MainWindow::on_pushButton_rootlog_clicked()
{
    ui->label_rootlog_infor->setText("");  //清空提示信息
    ui->label_rootlog_infor->setStyleSheet("color: rgb(255, 78, 25);");//用于输出登录信息

    QSqlQuery query;
    QString rootid = ui->lineEdit_rootlog_rootid->text();//用户名
    QString PassWd = ui->lineEdit_rootlog_passwd->text();//用户密码
    QString str;
    bool flag = true;

    if(0 == rootid.size())
    {
        ui->label_rootlog_infor->setText("用户名不能为空!");
        flag = false;
    }
    if(rootid.size() > 20)
    {
        ui->label_rootlog_infor->setText("用户名长度不能超过20位!");
        flag = false;
    }

    if(0 == PassWd.size())
    {
        ui->label_rootlog_infor->setText("密码不能为空!");
        flag = false;
    }
    if(PassWd.size() > 20)
    {
        ui->label_rootlog_infor->setText("密码长度不能超过20位!");
        flag = false;
    }


    if(flag) {
        rootid = "'" + rootid + "'";
        PassWd = "'" + PassWd +"'";
        str = "select * from User where Uid = " + rootid + " and Password = " + PassWd +
               " and Type = 'Root'" + ";" ;
        qDebug() << str;
        bool success = query.exec(str);
        if(query.next()) {	//判断结果集是否为空，即判断用户名、密码是否正确
            ui->label_rootlog_infor->setText("登录成功");
            this->hide();
            Adm *A = new Adm(this);
            A->setFixedSize(1000, 600);
            connect(A, SIGNAL(sendsignal()), this, SLOT(show()));
            connect(this, SIGNAL(send_uid(QString)), A, SLOT(receive_uid(QString)));
            emit send_uid(rootid);
            A->show();	//转到用户界面
            ui->label_rootlog_infor->setText("");  //清空提示信息
        }else {
            ui->label_rootlog_infor->setText("登录失败");
        }
    }
}
