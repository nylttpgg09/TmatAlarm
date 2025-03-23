#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "fullscreencountdown.h"
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , totalTime(25 * 60) // 默认25分钟
    , remainingTime(totalTime)
    , isRunning(false)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCountdown);

    ui->cintime->setValue(25); // 设置默认时间值
    ui->Blacktime->setValue(30); // 设置默认时间值
    updateDisplay();

    createTrayIcon();

    // 初始化时长（可以从界面获取，下面以默认30分钟工作，5分钟休息）
    workIntervalSeconds = 30 * 60; // 30分钟

    restPeriodSeconds = 5 * 60;    // 5分钟
    // 初始化快捷键 Ctrl+Alt+Q
    exitShortcut = new QShortcut(QKeySequence("Ctrl+Alt+Q"), this);
    connect(exitShortcut, &QShortcut::activated, this, &MainWindow::forceQuitApp);

    // 创建定时器
    workIntervalTimer = new QTimer(this);

    workIntervalTimer->setSingleShot(true); // 单次触发

    connect(workIntervalTimer, &QTimer::timeout, this, &MainWindow::startRestPeriod);

    // 启动第一个工作周期
    startWorkInterval();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 开始工作间隔
void MainWindow::startWorkInterval()
{
    workIntervalTimer->start(workIntervalSeconds * 1000);
}

// 工作间隔到期后自动进入休息

void MainWindow::startRestPeriod()
{
    auto *restScreen = new FullscreenCountdown(restPeriodSeconds);
    restScreen->setAttribute(Qt::WA_DeleteOnClose);
    restScreen->show();

    // 当休息屏幕关闭后，自动重新启动下一个工作周期
    connect(restScreen, &FullscreenCountdown::destroyed,
            this, &MainWindow::onRestPeriodFinished);
}

// 休息结束后的处理
void MainWindow::onRestPeriodFinished()
{
    startWorkInterval(); // 重启工作计时
}

void MainWindow::on_StartButton_clicked()//用于开始
{
    if(isRunning) {
        timer->stop();
        ui->StartButton->setText("开始");
        isRunning = false;
    } else {
        timer->start(1000);
        ui->StartButton->setText("暂停");
        isRunning = true;
    }
}
void MainWindow::forceQuitApp()
{
    QApplication::quit(); // 立即强制退出程序
}


void MainWindow::on_cintime_valueChanged(int arg1)//用于调整一次专注的时间
{
    totalTime = arg1 * 60;
    remainingTime = totalTime;
    updateDisplay();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick || reason == QSystemTrayIcon::Trigger) {
        if(this->isVisible())
            hide();
        else
            showNormal();
    }
}

// 界面显示逻辑
void MainWindow::updateDisplay()
{
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    ui->showtime->setText(QString::asprintf("%02d:%02d", minutes, seconds));
}

void MainWindow::updateCountdown()
{
    if(remainingTime > 0) {
        remainingTime--;
        updateDisplay();
    } else {
        timer->stop();
        isRunning = false;
        ui->StartButton->setText("开始");
        trayIcon->showMessage("提示", "时间到，休息一下吧！",
        QSystemTrayIcon::Information, 5000);
        QMessageBox::information(this, "番茄钟", "时间到，休息一下吧！");
        remainingTime = totalTime; // 重置倒计时
        updateDisplay();
    }
}


void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/prices/1.png"), this);
    trayIcon->setToolTip("番茄钟");

    QMenu *trayMenu = new QMenu(this);
    trayMenu->addAction("打开主窗口", this, &MainWindow::showNormal);
    trayMenu->addSeparator();
    trayMenu->addAction("退出", qApp, &QApplication::quit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated,
            this, &MainWindow::trayIconActivated);
}

void MainWindow::on_fullscreenCountdownButton_clicked()
{
    int countdownSeconds = ui->Blacktime->value() * 60;

    // 延迟调用确保主窗口完全启动
    QTimer::singleShot(0, [countdownSeconds](){
        auto *fsCountdown = new FullscreenCountdown(countdownSeconds);
        fsCountdown->show();
    });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();  // 隐藏主窗口
        trayIcon->showMessage("提示", "程序已最小化到托盘，双击托盘图标显示窗口。",
                              QSystemTrayIcon::Information, 3000);
        event->ignore();  // 忽略关闭事件
    } else {
        event->accept();  // 否则正常关闭
    }
}


void MainWindow::on_startTimerButton_clicked()
{
    workIntervalSeconds = ui->starttumer->value() * 60;
    restPeriodSeconds = ui->relaxtime->value() * 60;

    startWorkInterval();
}

