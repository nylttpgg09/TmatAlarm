#ifndef FULLSCREEN_COUNTDOWN_H
#define FULLSCREEN_COUNTDOWN_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPointer>  // <-- 新增
#include <QShortcut>

class FullscreenCountdown : public QWidget
{
    Q_OBJECT

public:
    explicit FullscreenCountdown(int countdownSeconds, QWidget *parent = nullptr);

private slots:
    void updateCountdown();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QTimer *timer;
    QPointer<QLabel> countdownLabel;
    int remainingSeconds;
    bool initialized = false;
    void adjustFontSize();
};

#endif // FULLSCREEN_COUNTDOWN_H
