#include "update.h"
#include "ui_update.h"
#include <QSqlQuery>
#include <QDebug>

Update::Update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
}

Update::~Update()
{
    delete ui;
}

void Update::receive_updateBID(QString BID)
{
    updateBID = BID;
}

void Update::prepare()
{
    Placeholder();
}

void Update::Placeholder()
{
    QSqlQuery query;
    QString oldBID = updateBID;
    QString oldBookName;
    QString oldPublisher;
    QString oldAuthor;
    QString oldTotal;
    QString oldStorage;
    QString oldYear;
    QString str = "select * from Book where Bid = '" + oldBID + "';" ;
    query.exec(str);
    qDebug() << str;
    if(query.next()) {
        qDebug() << query.value(0).toString();
        oldBookName = query.value(1).toString();
        oldPublisher = query.value(2).toString();
        oldAuthor = query.value(3).toString();
        oldYear = query.value(4).toString();
        oldTotal = query.value(5).toString();
        oldStorage = query.value(6).toString();
    }
    qDebug() << oldBID << oldBookName << oldPublisher << oldAuthor << oldTotal
             << oldStorage << oldYear;
    //oldBID = oldBID.mid(1, oldBID.length()-2);
    ui->lineEdit_update_Bid->setPlaceholderText(oldBID);
    ui->lineEdit_update_BookName->setPlaceholderText(oldBookName);
    ui->lineEdit_update_publisher->setPlaceholderText(oldPublisher);
    ui->lineEdit_update_Author->setPlaceholderText(oldAuthor);
    ui->lineEdit_update_Total->setPlaceholderText(oldTotal);
    ui->lineEdit_update_Storage->setPlaceholderText(oldStorage);
    ui->lineEdit_update_Year->setPlaceholderText(oldYear);
}

void Update::on_pushButton_update_confirm_clicked()
{

    QString Condition;
    QString str, str1;

    str = ui->lineEdit_update_Bid->text();
    if(str != "") {      //非空则更新，否则保持原来的Bid
        str = "'" + str + "'";
        Condition += "Bid = " + str;
    }else {
        str = "'" + updateBID + "'";
        Condition += "Bid = " + str;
    }

    str = ui->lineEdit_update_BookName->text();
    if(str != "") {
        Condition += ", ";
        str = "'" + str + "'";
        Condition += "BookName = " + str;
    }

    str = ui->lineEdit_update_publisher->text();
    if(str != "") {
        Condition += ", ";
        str = "'" + str + "'";
        Condition += "Publisher = " + str;
    }

    str = ui->lineEdit_update_Author->text();
    if(str != "") {
        Condition += ", ";
        str = "'" + str + "'";
        Condition += "Author = " + str;
    }

    str = ui->lineEdit_update_Total->text();
    if(str != "") {
        Condition += ", ";
        Condition += "Total = " + str;
    }

    str = ui->lineEdit_update_Storage->text();
    if(str != "") {
        Condition += ", ";
        Condition += "Storage = " + str;
    }

    str = ui->lineEdit_update_Year->text();
    if(str != "") {
        Condition += ", ";
        Condition += "Year = " + str;
    }


    Condition += " " ;
    qDebug() << Condition;
    emit send_Bookupdate_condition(Condition);
    emit sendsignal();
    this->close();
}
