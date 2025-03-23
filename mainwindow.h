#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>
#include <QSystemTrayIcon>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_StartButton_clicked();

    void on_cintime_valueChanged(int arg1);

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason); // 新增槽函数
    void updateCountdown(); // 更新倒计时

    void on_fullscreenCountdownButton_clicked();

    void startWorkInterval();

    void startRestPeriod();

    void onRestPeriodFinished();

    void on_startTimerButton_clicked();

protected:

    void closeEvent(QCloseEvent *event) override; // 重写closeEvent



private slots:
    void forceQuitApp();

private:
    QShortcut *exitShortcut;

private:
    QTimer *workIntervalTimer; // 工作间隔定时器

    int workIntervalSeconds;   // 用户自定义的工作间隔

    int restPeriodSeconds;     // 强制休息时长

    Ui::MainWindow *ui;

    QTimer *timer;

    QSystemTrayIcon *trayIcon;

    int totalTime;      // 用户设置的总时间（秒）
    int remainingTime;  // 剩余时间（秒）
    bool isRunning;
    void createTrayIcon();
    void updateDisplay(); // 更新界面显示

};
#endif // MAINWINDOW_H
