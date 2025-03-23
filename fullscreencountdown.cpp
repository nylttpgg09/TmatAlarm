#include "fullscreencountdown.h"
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QDebug>
FullscreenCountdown::FullscreenCountdown(int countdownSeconds, QWidget *parent)
    : QWidget(parent), remainingSeconds(countdownSeconds)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("background-color:black;");
    showFullScreen();


    // 使用明确的父对象创建标签
    countdownLabel = new QLabel(QString::number(remainingSeconds), this);
    countdownLabel->setAlignment(Qt::AlignCenter);
    countdownLabel->setStyleSheet("color: white; font-weight: bold;");

    // 显式创建布局并设置到窗口
    auto *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(countdownLabel.data()); // 使用.data()安全取指针
    setLayout(layout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FullscreenCountdown::updateCountdown);
    timer->start(1000);
}


void FullscreenCountdown::adjustFontSize()
{
    if (!countdownLabel)
        return;

    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
        return;

    QRect screenGeometry = screen->geometry();
    int shortestSide = qMin(screenGeometry.width(), screenGeometry.height());

    int fontSize = shortestSide / 2; // 明显调整字体大小

    QFont font("Microsoft YaHei", fontSize, QFont::Bold);
    countdownLabel->setFont(font);
    countdownLabel->setStyleSheet(R"(
        color: #00FF00; /* 醒目的绿色字体 */
        font-weight: bold;
        text-shadow: 4px 4px #222222;
    )");
    countdownLabel->adjustSize();
    countdownLabel->updateGeometry();
    this->layout()->update();
}


void FullscreenCountdown::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (!initialized) {
        adjustFontSize();    // 窗口完全显示后再调整字体
        initialized = true;
    }
}

void FullscreenCountdown::updateCountdown()
{
    if (remainingSeconds > 0) {
        remainingSeconds--;
        countdownLabel->setText(QString::number(remainingSeconds));
    } else {
        timer->stop();
        close();
    }
}

void FullscreenCountdown::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        close();
}

void FullscreenCountdown::closeEvent(QCloseEvent *event)
{
    if (timer->isActive())
        timer->stop();
    QWidget::closeEvent(event);
}
