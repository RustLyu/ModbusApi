#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "common.h"
#include "Includes/ModbusApi.h"
#include "modbus/modbus.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Insert2DB(RawData data);
    void Insert2DB(QVector<RawData>& data);

private slots:
    void on_m_conDBBtn_clicked();

    void on_m_conPlcBtn_clicked();

    void on_m_modeCombox_currentIndexChanged(int index);

    void on_m_startTestBtn_clicked();

private:
    Ui::MainWindow *ui;
    Modbus*                 m_pCtx;
    modbus_t*                ctx;
private:
    void initUI();
signals:
    void signal_showMessage(QString &text, int timeout = 0);
};

#endif // MAINWINDOW_H
