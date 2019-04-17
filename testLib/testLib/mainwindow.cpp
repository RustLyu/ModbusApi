#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recordmanager.h"
#include "calprointerval.h"
#include "Log.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCtx(nullptr),
    ctx(nullptr)
{
    ui->setupUi(this);
    initUI();
    on_m_conDBBtn_clicked();
    on_m_conPlcBtn_clicked();
    connect(this, &MainWindow::signal_showMessage, this->statusBar(), &QStatusBar::showMessage);
}

MainWindow::~MainWindow()
{
    closeConnect(m_pCtx);
    freeContext(m_pCtx);
    m_pCtx = nullptr;
    delete ui;
}

void MainWindow::on_m_conDBBtn_clicked()
{
    if(RecordManager::Instance()->connect())
    {
        ui->m_conDBBtn->setEnabled(false);
    }
}

void MainWindow::on_m_conPlcBtn_clicked()
{
//    ctx = modbus_new_tcp("192.168.1.121", 502);
//    if (ctx == nullptr)                //使用UART1,对应的设备描述符为ttySP1
//    {
//        fprintf(stderr, "Unable to allocate libmodbus contex\n");
//        return;
//    }

//    modbus_set_debug(ctx, 1);      //设置1可看到调试信息
//    modbus_set_slave(ctx, 1);      //设置slave ID

//    if (modbus_connect(ctx) == -1) //等待连接设备
//    {
//        fprintf(stderr, "Connection failed:%s\n", modbus_strerror(errno));
//    }
//    else
//    {
//        ui->m_conPlcBtn->setEnabled(false);
//    }
    if (m_pCtx == nullptr)
    {
        m_pCtx = const_cast<Modbus*>(createModbusContext("192.168.1.121", 502));
    }
    int ret = connect2Server(m_pCtx);
    if(ret != 0)
    {
        freeContext (m_pCtx);
        m_pCtx = nullptr;
    }
    else
    {
        setMaxRequestTimeOut(m_pCtx, 0, 10000);
        setMaxResponseTimeOut(m_pCtx, 0, 10000);
        ui->m_conPlcBtn->setEnabled(false);
    }
}

void MainWindow::on_m_modeCombox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}

void MainWindow::on_m_startTestBtn_clicked()
{
    ui->m_progressBar->setValue(0);
    uint16_t data[101] = {1};
    int size = ui->m_frameSizeEdit->text().toInt();
    // size 合法性判断
    int count = ui->m_testCount->text().toInt();
    QTime start = QTime::currentTime();
    ui->m_progressBar->setRange(0, count);
    QVector<double>		time;
    QVector<double>		value;
    ui->m_chartView->xAxis->setRange(0, count + 1);
    int maxY = 20;
    QVector<RawData> rawVec;

    double alluse = 0;
    for(int i = 1; i <= count; ++i)
    {
        CalProInterval::Instance->updateStartTime();
        int ret = writeMulRegister(m_pCtx, 0, size, data);
//        int ret = modbus_write_registers(ctx,0,size, data);
        if(ret < 0)
        {
            LOG("recv error");
        }
        ui->m_progressBar->setValue(i);
        double use = CalProInterval::Instance->getDiffTime();
        time.push_back(i);
        value.push_back(use);
        ui->m_chartView->graph(0)->setData(time, value);
        ui->m_chartView->replot();
        RawData raw;
        raw.size = size;
        raw.time = use;
        raw.count = i;
        if(i == count)
        {
            QTime end = QTime::currentTime();
            alluse = CalProInterval::Instance->getDiffTime(start, end);
        }
        rawVec.push_back(raw);
        if(use > maxY)
            maxY = use;
        ui->m_chartView->yAxis->setRange(0, maxY);
    }
//    Insert2DB(rawVec);
    ui->m_useTime->setText(QString::number(alluse));
    ui->m_averageTime->setText(QString::number(alluse/count));
}

void MainWindow::initUI()
{
    ui->m_chartView->legend->setVisible(true);
    ui->m_chartView->legend->setFont(QFont("Helvetica", 9));
    ui->m_chartView->plotLayout()->insertRow(0);
    ui->m_chartView->plotLayout()->addElement(0, 0, new QCPTextElement(ui->m_chartView, "ModbusTcp 读写耗时曲线", QFont("sans", 12, QFont::Bold)));

    ui->m_chartView->addGraph(ui->m_chartView->xAxis, ui->m_chartView->yAxis);
    ui->m_chartView->graph(0)->setPen(QPen(Qt::green));
    ui->m_chartView->graph(0)->setName("耗时-序号");
    ui->m_chartView->xAxis->setLabel("次数(次)");
    ui->m_chartView->yAxis->setLabel("耗时(mm)");

    ui->m_chartView->xAxis->setRange(0, 100);
    ui->m_chartView->yAxis->setRange(0, 20);
    ui->m_chartView->graph(0)->rescaleAxes();
    ui->m_chartView->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::Insert2DB(RawData data)
{
    QString sql = QString("insert into rawdata (framesize, usetime, count)\
                          values('%1','%2', '%3')").arg(
                          QString::number(data.size),
                          QString::number(data.time),
                          QString::number(data.count));
    RecordManager::insert(sql);
}

void MainWindow::Insert2DB(QVector<RawData> &data)
{
    QString str = "正在存档...";
    emit signal_showMessage(str, 1000000);
    for(auto cell : data)
    {
        Insert2DB(cell);
    }
    str = "存档结束";
    emit signal_showMessage(str, 10000);
}
