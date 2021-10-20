#include "adm.h"
#include "ui_adm.h"
#include "query.h"
#include "update.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

Adm::Adm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adm)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page_adm_index);
    ui->tableView_adm_all_record->resize(800,500);
}

Adm::~Adm()
{
    delete ui;
}

void Adm::on_pushButton_administration_exit_clicked()
{
    emit sendsignal();
    this->close();
}

static QString return_BID, return_CID;
void Adm::receive_return_BID(QString BID)
{
    return_BID = "'" + BID + "'";
    qDebug() << "cao: " << BID;
    qDebug() << "cao: " << return_BID;
}

void Adm::receive_return_CID(QString CID)
{
    return_CID = "'" + CID + "'";
}

void Adm::PrintAllRecord()
{
    QString str;
    QSqlQueryModel *dbmodel = new QSqlQueryModel();
    ui->tableView_adm_all_record->setModel(dbmodel);
    ui->tableView_adm_all_record->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    str = "select Bid as 图书编号, BookName as 书名, Publisher as 出版社, "
          "Author as 作者, Record.CardID as 卡号, Uid as 用户名, Name as 姓名, Phone as 联系方式, BorrowTime as 借出时间, "
          "ReturnTime as 应还时间 from (Book natural join Record) inner join Card on "
          "(Card.CardID = Record.CardID);" ;
    //qDebug() << str;
    dbmodel->setQuery(str);
}

void Adm::on_pushButton_allRecord_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_all_Record);
    PrintAllRecord();
}

void Adm::PrintAllCard()
{
    QString str;
    QSqlQueryModel *dbmodel = new QSqlQueryModel();
    ui->tableView_adm_allCard->setModel(dbmodel);
    ui->tableView_adm_allCard->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    str = "select CardID as 卡号, Uid as 用户名, Name as 姓名, Department as 所在学院, "
          "Phone as 联系方式, UpdateTime as 办卡时间 from Card;" ;
    qDebug() << str;
    dbmodel->setQuery(str);
}

void Adm::on_pushButton_adm_CardManagement_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_adm_card_management);
    PrintAllCard();
}

void Adm::on_tableView_adm_all_record_doubleClicked(const QModelIndex &index)
{
    QAbstractItemModel *Imodel = ui->tableView_adm_all_record->model();
    QModelIndex Iindex0 = Imodel->index(index.row(),0);
    QModelIndex Iindex4 = Imodel->index(index.row(),4);//index.row()为算选择的行号,1为所选中行的第一列,即Bid
    QVariant datatemp0 = Imodel->data(Iindex0);
    QVariant datatemp4 = Imodel->data(Iindex4);
    QString BID = datatemp0.toString();//BID即为所选择行的第一列的值
    QString CID = datatemp4.toString();
    qDebug() << BID;
    connect( this, SIGNAL(send_return_BID(QString)), this, SLOT(receive_return_BID(QString)) );
    connect( this, SIGNAL(send_return_CID(QString)), this, SLOT(receive_return_CID(QString)) );
    emit send_return_BID(BID);
    emit send_return_CID(CID);
}

void Adm::on_pushButton_adm_DelRecord_clicked()
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
        QMessageBox::information(this, "信息", "删除成功", QMessageBox::Ok);
    }else {
        QMessageBox::critical(this, "错误", "删除失败", QMessageBox::Ok);
    }

    PrintAllRecord();
}

void Adm::on_pushButton_apply_Card_clicked()
{
    ui->label_applyCard_infor->setText("");  //清空提示信息
    ui->label_applyCard_infor->setStyleSheet("color: rgb(255, 78, 25);");//用于输出注册信息

    QSqlQuery query;
    QString CID = ui->lineEdit_applyCard_CardID->text();
    QString UID = ui->lineEdit_applyCard_Uid->text();
    QString Name = ui->lineEdit_applyCard_name->text();
    QString Dept = ui->lineEdit_applyCard_dept->text();
    QString Phone = ui->lineEdit_applyCard_phone->text();
    QString str;
    bool flag = true;

    if(!CID.size() || !UID.size() || !Name.size() || !Dept.size() || !Phone.size() ) {
        flag = false;
        ui->label_applyCard_infor->setText("以上信息不能为空");
    }

    if(CID.size() > 20) {
        flag = false;
        ui->label_applyCard_infor->setText("卡号长度不能超过20位");
    }

    if(flag) {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd");
        current_date = "'" + current_date + "'";

        CID = "'" + CID + "'";
        UID = "'" + UID + "'";
        Name = "'" + Name + "'";
        Dept = "'" + Dept + "'";
        Phone = "'" + Phone + "'";
        str = "Insert Card values(" + CID + ", " + UID +", " + Name + ", " + Dept
                + ", " + Phone + ", " + current_date + ");" ;
        qDebug() << str;
        bool success = query.exec(str);
        if(success) {
            ui->label_applyCard_infor->setText("办卡成功");
        }else {
            ui->label_applyCard_infor->setText("此卡号已存在或用户不存在");
        }
    }

    PrintAllCard();
}

void Adm::on_tableView_adm_allCard_doubleClicked(const QModelIndex &index)
{
    QAbstractItemModel *Imodel = ui->tableView_adm_allCard->model();
    QModelIndex Iindex0 = Imodel->index(index.row(),0);
    QVariant datatemp0 = Imodel->data(Iindex0);
    QString CID = datatemp0.toString();
    qDebug() << CID;
    connect( this, SIGNAL(send_return_CID(QString)), this, SLOT(receive_return_CID(QString)) );
    emit send_return_CID(CID);
}

void Adm::on_pushButton_adm_DelCard_clicked()
{
    QSqlQuery query;
    QString str;

    //从Record表中删除借阅记录
    str = "delete from Card where CardID =" + return_CID + ";" ;
    bool success_del = query.exec(str);
    qDebug() << str;

    if(success_del) {
        QMessageBox::information(this, "信息", "删除成功", QMessageBox::Ok);
    }else {
        QMessageBox::critical(this, "错误", "删除失败", QMessageBox::Ok);
    }

    PrintAllCard();
}

void Adm::InsertBook(QString BID, QString BookName, QString Publisher,
                     QString Author, QString Year, QString Amount)
{
    QFont ft;
    ft.setPointSize(15);
    ui->label_storage_infor->setFont(ft);
    ui->label_storage_infor->setText("");  //清空提示信息
    ui->label_storage_infor->setStyleSheet("color: rgb(255, 78, 25);");//用于输出注册信息

    QSqlQuery query;
    QString str;
    bool flag = true, success;

    if(0 == BID.size()) {
        flag = false;
        ui->label_storage_infor->setText("图书编号不能为空\n");
    }
    if(0 == Amount.size()) {
        flag = false;
        ui->label_storage_infor->setText("添加册数不能为空\n");
    }

    if(flag) {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd");
        current_date = "'" + current_date + "'";

        BID = "'" + BID + "'";
        BookName = "'" + BookName + "'";
        Publisher = "'" + Publisher + "'";
        Author = "'" + Author + "'";
        Year = "'" + Year + "'";

        str = "select Bid from Book where Bid = " + BID + ";" ;
        qDebug() << str;
        query.exec(str);
        if(query.next()) {
            str = "update Book set Storage = Storage + " + Amount + ", "
                    + "Total = Total + " + Amount + ", "
                    + "UpdateTime = " + current_date + " where Bid = " + BID + ";" ;
            success = query.exec(str);
            qDebug() << str;
            if(success) ui->label_storage_infor->setText("该书已存在，增添册数成功");
        }else {
            str = "Insert Book values(" + BID + ", " + BookName + ", " + Publisher
                    + ", " + Author + ", " + Year + ", " + Amount + ", " + Amount
                    + ", " + current_date + ");" ;
            success = query.exec(str);
            if(success) ui->label_storage_infor->setText("新图书入库成功");
        }
    }
}

void Adm::on_pushButton_storage_clicked()
{
    QString BID = ui->lineEdit_storage_Bid->text();
    QString BookName = ui->lineEdit_storage_Name->text();
    QString Publisher = ui->lineEdit_storage_publisher->text();
    QString Author = ui->lineEdit_storage_Author->text();
    QString Year = ui->lineEdit_storage_Year->text();
    QString Amount = ui->lineEdit_storage_amount->text();
    InsertBook(BID, BookName, Publisher, Author, Year, Amount);
}

void Adm::on_pushButton_adm_BookStorage_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_adm_BookStrorage);
}

void Adm::on_pushButton_adm_BookManagement_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_adm_BookManagement);
}

void Adm::on_pushButton_BulkImporting_clicked()
{
    QString  fileName = QFileDialog::getOpenFileName(this,"choose file","C:\\",tr("*.txt"));
    if(!fileName.isEmpty()) ui->label_adm_file->setText("文件路径："+fileName);

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen()) {
        qDebug() << "打开成功";
        QTextStream in(&file);
        in.setCodec("utf-8");
        QString line = in.readLine();
        QList<QString>linelist;
        while(!line.isNull()) {
            qDebug() << line;
            //line = line.mid(0, line.length());
            linelist = line.split(",");

            if(linelist.count() < 1) {
                break;
            }
            for(int i = 0; i < linelist.count(); i++) {
                linelist[i] = linelist[i].simplified();
                if(i == 3) {
                    if(0 == linelist[i].size()) linelist[i] = "";
                }else if(i == 4 || i == 5) {
                    if(0 == linelist[i].size())linelist[i] = "0";
                }
                qDebug() << linelist[i];
            }
            InsertBook(linelist[0], linelist[1], linelist[2], linelist[3],
                    linelist[4], linelist[5]);
            line = in.readLine();
        }
    }else {
        qDebug() << "打开失败";
    }
}

void Adm::receive_from_query(QString data)
{
    Condition_adm_query = data;
    //qDebug() << Condition;
}

void Adm::PrintBook()
{
    QString str;
    QSqlQueryModel *dbmodel = new QSqlQueryModel();
    ui->tableView_adm_BookManagement->setModel(dbmodel);
    ui->tableView_adm_BookManagement->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if(0 != Condition_adm_query.compare(""))
        str = "select Bid as 图书编号, BookName as 书名, Publisher as 出版社, "
              "Author as 作者, Year as 出版年份, Total as 总量, Storage as 余量, "
              "UpdateTime as 最新入库时间 from Book where " + Condition_adm_query;
    else
        str = "select* from Book";
    qDebug() << str;
    dbmodel->setQuery(str);
}

void Adm::on_pushButton_adm_ManagementQuery_clicked()
{
    Query *q = new Query(this);
    connect(q, SIGNAL(send_Bookquery_condition(QString)), this, SLOT(receive_from_query(QString)));
    connect(q, SIGNAL(sendsignal()), this, SLOT(PrintBook()));
    q->show();
    ui->stackedWidget->setCurrentWidget(ui->page_adm_BookManagement);
}

void Adm::on_pushButton_adm_ManagementDelete_clicked()
{
    QSqlQuery query;
    QString str;

    //从Record表中删除借阅记录
    str = "delete from Book where Bid =" + return_BID + ";" ;
    bool success_del = query.exec(str);
    qDebug() << str;

    if(success_del) {
        QMessageBox::information(this, "信息", "删除图书成功", QMessageBox::Ok);
    }else {
        QMessageBox::critical(this, "错误", "删除失败，有读者正在借阅此书", QMessageBox::Ok);
    }

    PrintBook();
}

void Adm::on_tableView_adm_BookManagement_doubleClicked(const QModelIndex &index)
{
    QAbstractItemModel *Imodel = ui->tableView_adm_BookManagement->model();
    QModelIndex Iindex0 = Imodel->index(index.row(),0);
    QVariant datatemp0 = Imodel->data(Iindex0);
    QString BID = datatemp0.toString();
    qDebug() << BID;
    connect( this, SIGNAL(send_return_BID(QString)), this, SLOT(receive_return_BID(QString)) );
    qDebug() << "guanli:" << BID;
    emit send_return_BID(BID);
    qDebug() << "guanli:" << BID;
}

void Adm::receive_from_update(QString data)
{
    Condition_adm_update = data;
    qDebug() <<Condition_adm_update;
}


void Adm::on_pushButton_adm_ManagementUpdate_clicked()
{
    Update *up = new Update(this);
    connect(this, SIGNAL(send_return_BID(QString)), up, SLOT(receive_updateBID(QString)));
    connect(up, SIGNAL(send_Bookupdate_condition(QString)), this, SLOT(receive_from_update(QString)));
    connect(up, SIGNAL(sendsignal()), this, SLOT(UpdateBook()));
    emit send_return_BID(return_BID.mid(1,return_BID.length()-2));       //要发BID，否则会导致后面的所有return_BID都多一对单引号
    up->prepare();
    up->show();
}

void Adm::UpdateBook()
{
    QSqlQuery query;
    QString str;
    if(0 != Condition_adm_update.compare(" "))
        str = "update Book set " + Condition_adm_update + "where Bid = " + return_BID + ";" ;
    qDebug() << str;
    bool success = query.exec(str);
    if(success) {
        QMessageBox::information(this, "信息", "更新图书成功", QMessageBox::Ok);
    }else {
        QMessageBox::critical(this, "错误", "更新失败，请检查输入信息是否符合要求", QMessageBox::Ok);
    }

    PrintBook();
}
