#include "mainwindow.h"
#include <QNetworkInterface>
#include <QHostInfo>
#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    timer = new QTimer;

    hWidget = new QWidget;
    hBoxLayout = new QHBoxLayout;
    pushButton[0] = new QPushButton("获取网络信息");
    pushButton[1] = new QPushButton("清除网络信息");
    hBoxLayout->addWidget(pushButton[0]);
    hBoxLayout->addWidget(pushButton[1]);
    hWidget->setLayout(hBoxLayout);

    textBrowser = new QTextBrowser;
    vBoxLayout = new QVBoxLayout;
    vWidget = new QWidget(this);
    vBoxLayout->addWidget(textBrowser);
    vBoxLayout->addWidget(hWidget);
    vWidget->setLayout(vBoxLayout);
    this->setCentralWidget(vWidget);

    connect(pushButton[0], &QPushButton::clicked, this, &MainWindow::timerStart);
    connect(pushButton[1], &QPushButton::clicked, this, &MainWindow::clearHostInfo);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerTimeOut);
}

void MainWindow::timerStart()       //启动定时器
{
    textBrowser->clear();
    timer->start(100);
}

void MainWindow::timerTimeOut()     //定时结束操作
{
    showHostInfo();
    timer->stop();
}

void MainWindow::showHostInfo()
{
    textBrowser->insertPlainText(getHostInfo());
}

QString MainWindow::getHostInfo()   //获取网络信息
{
    QString str = "主机名称:" + QHostInfo::localHostName() + "\n";           //主机信息
                                  //QNetworkInterface类提供主机的IP地址和网络接口的列表
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();     //所有网络接口
    foreach (QNetworkInterface interface, list) {           //遍历
        str += "网卡设备:" + interface.name() + "\n";
        str += "MAC地址:" + interface.hardwareAddress() + "\n";
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        foreach (QNetworkAddressEntry entry, entryList) {
            if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol){
                str += "IP地址:" + entry.ip().toString() + "\n";
                str += "子网掩玛:" + entry.netmask().toString() + "\n";
                str += "广播地址:" + entry.broadcast().toString() + "\n";
            }
        }
    }
    return str;
}

void MainWindow::clearHostInfo()        //清除网络信息
{
    if(!textBrowser->toPlainText().isEmpty())
        textBrowser->clear();
}

MainWindow::~MainWindow()
{

}
