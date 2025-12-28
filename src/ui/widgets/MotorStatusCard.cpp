#include "MotorStatusCard.h"

MotorStatusCard::MotorStatusCard(const QString &motorName, const QString &headerColor, QWidget *parent)
    : Card("", parent)
    , motorName(motorName)
    , headerColor(headerColor)
{
    // 设置标题栏颜色 - 移除负边距，使用正常布局
    titleLabel->setText(motorName);
    // 调整Card的margins以适应标题栏
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        mainLayout->setContentsMargins(0, 0, 0, 20);  // 顶部边距为0，让标题栏延伸到Card顶部
    }
    titleLabel->setStyleSheet(QString("font-size: 16px; font-weight: 600; color: %1; background-color: %2; padding: 10px 20px; border-radius: 12px 12px 0 0; margin: 0;").arg("#ffffff").arg(headerColor));
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    titleLabel->setMinimumHeight(40);
    titleLabel->setMinimumWidth(100);  // 确保标题有足够空间
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setWordWrap(false);  // 标题不应该换行
    
    setupContent();
}

void MotorStatusCard::setupContent()
{
    QVBoxLayout *contentLayout = getContentLayout();
    
    // 状态徽章
    statusBadge = new StatusBadge("STOPPED", headerColor, this);
    contentLayout->addWidget(statusBadge);
    
    // 模式标签
    modeLabel = new QLabel("> Speed Control", this);
    modeLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    modeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    modeLabel->setWordWrap(true);
    contentLayout->addWidget(modeLabel);
    
    // 值标签1 - 需要足够空间显示大数值（如 "RPM: 15000"）
    value1Label = new QLabel("> RPM: 0", this);
    value1Label->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    value1Label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    value1Label->setMinimumWidth(120);  // 确保可以显示大数值
    contentLayout->addWidget(value1Label);
    
    // 值标签2 - 需要足够空间显示温度值（如 "Temp: 125.5°C"）
    value2Label = new QLabel("> Temp: 0°C", this);
    value2Label->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    value2Label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    value2Label->setMinimumWidth(120);  // 确保可以显示带小数点的温度值
    contentLayout->addWidget(value2Label);
    
    contentLayout->addStretch();
}

void MotorStatusCard::updateStatus(const QString &mode, const QString &value1, const QString &value2, bool isRunning)
{
    if (isRunning) {
        statusBadge->setRunning();
    } else {
        statusBadge->setStopped();
    }
    
    modeLabel->setText("> " + mode);
    value1Label->setText("> " + value1);
    value2Label->setText("> " + value2);
}

