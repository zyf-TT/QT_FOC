#include "TopBar.h"
#include <QApplication>

TopBar::TopBar(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setTitle("FOC Tuning - Project XYZ");
    setDisconnected();
}

void TopBar::setupUI()
{
    setFixedHeight(50);
    setStyleSheet("background-color: #ffffff; border-bottom: 1px solid #d2d2d7;");
    
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 0, 20, 0);
    layout->setSpacing(0);
    
    // 标题（居中）
    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: #1d1d1f;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
    // 状态标签（右侧）
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("font-size: 13px; color: #8e8e93;");
    statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    statusLabel->setMinimumWidth(200);  // 增加宽度以容纳长连接信息（如 "Connected: COM3 @ 115200 bps"）
    statusLabel->setWordWrap(false);  // 状态文本不应该换行
    
    layout->addWidget(titleLabel, 1);
    layout->addWidget(statusLabel, 0);
}

void TopBar::setTitle(const QString &title)
{
    if (titleLabel) {
        titleLabel->setText(title);
    }
}

void TopBar::setConnectionStatus(const QString &status)
{
    if (statusLabel) {
        statusLabel->setText(status);
    }
}

void TopBar::setConnected(const QString &portName, int baudRate)
{
    QString status = QString("Connected: %1 @ %2 bps").arg(portName).arg(baudRate);
    statusLabel->setText(status);
    statusLabel->setStyleSheet("font-size: 13px; color: #34c759; font-weight: 500;");
}

void TopBar::setDisconnected()
{
    statusLabel->setText("Disconnected");
    statusLabel->setStyleSheet("font-size: 13px; color: #8e8e93;");
}

