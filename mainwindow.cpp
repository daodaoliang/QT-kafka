#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qprocess.h"
#include "qdebug.h"
#include "qtextcodec.h"
#include "qstringlistmodel.h"
#include "qprogressdialog.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    disPlay();
    getTopic();

    /*
    progressDialog = new QProgressDialog(this);
    QFont font("Times",10,QFont::Bold);
    progressDialog->setFont(font);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setWindowTitle("plese wait");
    progressDialog->setLabelText(tr("Waiting..."));
    progressDialog->setRange(0,100);

    progressDialog->setValue(60);
    progressDialog->show();
//    progressDialog->show();
    */
}

MainWindow::~MainWindow()
{
    delete ui;
    delete progressDialog;
}

bool MainWindow::ExecDosCmd(QString cmd)
{
    QProcess p(0);
    p.start(cmd);
    p.waitForStarted();
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    return true;
}

void MainWindow::disPlay()
{
    QProcess p(0);
    p.start("ps -aux");
    p.waitForStarted();
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    if(result.contains("zookeeper.properties"))
    {
        ui->zkStart->setDisabled(true);
        ui->zkMsg->setText("Starting.....");
    }
    if(result.contains("zookeeper-1.properties"))
    {
        ui->zkStart2->setDisabled(true);
        ui->zkMsg2->setText("Starting.....");
    }
    if(result.contains("zookeeper-2.properties"))
    {
        ui->zkStart3->setDisabled(true);
        ui->zkMsg3->setText("Starting.....");
    }
    if(result.contains("server.properties"))
    {
        ui->kfStart->setDisabled(true);
        ui->kfMsg->setText("Starting.....");
    }
    if(result.contains("server-1.properties"))
    {
        ui->kfStart2->setDisabled(true);
        ui->kfMsg2->setText("Starting.....");
    }
    if(result.contains("server-2.properties"))
    {
        ui->kfStart3->setDisabled(true);
        ui->kfMsg3->setText("Starting.....");
    }
}

void MainWindow::getTopic()
{
    QProcess p(0);
    QString cmd ="sh /home/yudesong/Download/kafka_2.11-0.8.2.1/bin/kafka-topics.sh --list --zookeeper localhost:2181";
    p.start(cmd);
//  p.execute(cmd);
    p.waitForStarted();
    p.waitForFinished();
    QString str = QString::fromLocal8Bit(p.readAllStandardOutput());
    topics = str.split("\n");
//    qDebug()<<qsl[0];
//    qDebug()<<qsl[1];
 //   ui->topicList->addItem(str);
    //qDebug()<<str.length();
    //str[i];
    QStringList info;
    for(int i= 0;i<topics.length();i++)
    {
        ui->topicList->addItem(new QListWidgetItem(topics[i],ui->topicList));
        QProcess p(0);
        QString cmd = "sh /home/yudesong/Download/kafka_2.11-0.8.2.1/bin/kafka-topics.sh --describe --zookeeper localhost:2181 --topic "+topics[i];
        p.start(cmd);
        p.waitForStarted();
        p.waitForFinished();
        QString msg = QString::fromLocal8Bit(p.readAllStandardOutput());
        info.append(msg);
    }
    ui->info->setModel(new QStringListModel(info));
}

void MainWindow::on_zkStart_clicked()
{
//ExecDosCmd("ps -aux");
 //   progressDialog->close();
}

void MainWindow::on_pushButton_13_clicked()
{
    QString topic = ui->cTopic->text();
    QString parition = ui->cParition->text();
    QString replication = ui->cReplication->text();

    QProgressDialog  *progressDialog = new QProgressDialog(this);
    QFont font("Times",10,QFont::Bold);
    progressDialog->setFont(font);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->show();


    QProcess p(0);
    QString cmd = "sh /home/yudesong/Download/kafka_2.11-0.8.2.1/bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor "+replication+" --partitions "+parition+"  --topic "+topic;
    p.start(cmd);
    p.waitForStarted();
    p.waitForFinished();
    QString msg = QString::fromLocal8Bit(p.readAllStandardOutput());
//    qDebug()<<msg<<endl;
//    qDebug()<<cmd<<endl;

    if(!msg.isEmpty())
    {
        progressDialog->close();
        delete progressDialog;
        ui->cMsg->setText("Topic create success..");
    }
    else
    {
        progressDialog->close();
        delete progressDialog;
        ui->cMsg->setText("Topic create failed");
    }
    ui->topicList->clear();
    getTopic();
}

void MainWindow::on_consumer_clicked()
{
    QString topic = ui->conTopic->text();
    QProcess p(0);
    QString cmd = "sh /home/yudesong/Download/kafka_2.11-0.8.2.1/bin/kafka-console-consumer.sh --zookeeper localhost:2181 --topic "+topic+" --from-beginning";
    p.start(cmd);
    p.waitForStarted();
    p.waitForFinished();
   QString msg = QString::fromLocal8Bit(p.readAllStandardOutput());
    ui->consumerMsg->setText(msg);
    p.close();
}

void MainWindow::on_pushButton_14_clicked()
{
    QString topic = ui->proTopic->text();
    QString message = ui->proMessage->toPlainText();
    QProcess p(0);
    QString cmd = "sh /home/yudesong/Download/kafka_2.11-0.8.2.1/bin/kafka-console-producer.sh --broker-list localhost:9092 --topic "+topic;
    p.start(cmd);
    p.inputChannelMode();
    p.waitForStarted();
    p.write("message");
    p.closeWriteChannel();
    p.waitForFinished();
    QString msg = QString::fromLocal8Bit(p.readAllStandardError());
    qDebug()<<msg<<endl;
    p.close();
}
