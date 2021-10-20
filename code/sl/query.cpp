#include "query.h"
#include "ui_query.h"
#include <QDebug>
#include <QDateTime>

Query::Query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Query)
{
    ui->setupUi(this);
    ui->label_query_infor->setAlignment(Qt::AlignCenter);
    ui->query_dateEdit_lowerbound->setDisplayFormat("yyyy-MM-dd");   //设置输出格式;
    ui->query_dateEdit_lowerbound->setCalendarPopup(true);
    ui->query_dateEdit_upperbound->setDateTime(QDateTime::currentDateTime()); //设置当前时间为开始时间
    ui->query_dateEdit_upperbound->setDisplayFormat("yyyy-MM-dd");   //设置输出格式;
    ui->query_dateEdit_upperbound->setCalendarPopup(true);
}

Query::~Query()
{
    delete ui;
}

void Query::on_pushButton_clicked()
{
    QString Condition;
    QString str, str1;

    str = ui->lineEdit_query_Bid->text();
    if(str != "") {      //非空
        str = "'" + str + "'";
        Condition += "Bid = " + str;
    }

    str = ui->lineEdit_query_BookName->text();
    if(str != "") {
        if(Condition != "") Condition += " and ";
        str = "'" + str + "'";
        Condition += "BookName = " + str;
    }

    str = ui->lineEdit_query_publisher->text();
    if(str != "") {
        if(Condition != "") Condition += " and ";
        str = "'" + str + "'";
        Condition += "Publisher = " + str;
    }

    str = ui->lineEdit_query_Author->text();
    if(str != "") {
        if(Condition != "") Condition += " and ";
        str = "'" + str + "'";
        Condition += "Author = " + str;
    }

    str = ui->lineEdit_query_Year1->text();
    str1 = ui->lineEdit_query_Year2->text();
    if(str == "" && str1 != "") {
        if(Condition != "") Condition += " and ";
        str1 = "'" + str1 + "'";
        Condition += "Year <= " + str1;
    }else if(str != "" && str1 == "") {
        if(Condition != "") Condition += " and ";
        str = "'" + str + "'";
        Condition += "Year >= " + str;
    }else if(str != "" && str1 != "") {
        if(Condition != "") Condition += " and ";
        str = "'" + str + "'";
        str1 = "'" + str1 + "'";
        Condition += "(Year >= " + str + "and Year <= " + str1 + ")";
    }

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd");
    QString lowerbound = "2000-01-01";
    str = ui->query_dateEdit_lowerbound->text();
    str1 = ui->query_dateEdit_upperbound->text();
    if( !str.compare(lowerbound) && str1.compare(current_date)) {
        if(Condition != "") Condition += " and ";
        str1 = "'" + str1 + "'";
        Condition += "UpdateTime <= " + str1;
    }else if(str.compare(lowerbound) && !str1.compare(current_date)) {
        if(Condition != "") Condition += " and ";
        str = "'" + str + "'";
        Condition += "UpdateTime >= " + str;
    }else if(str1.compare(lowerbound) && str1.compare(current_date)) {
        if(Condition != "") Condition += " and ";
        str = "'" + str + "'";
        str1 = "'" + str1 + "'";
        Condition += "(UpdateTime >= " + str + "and UpdateTime <= " + str1 + ")";
    }

    //Condition += ";" ;
    qDebug() << Condition;
    emit send_Bookquery_condition(Condition);
    emit sendsignal();
    this->close();
}
