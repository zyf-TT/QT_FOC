#include "ControlOutputCard.h"
#include <QVBoxLayout>

ControlOutputCard::ControlOutputCard(QWidget *parent)
    : Card("Control & Output", parent)
{
    setupContent();
}

void ControlOutputCard::setupContent()
{
    QVBoxLayout *contentLayout = getContentLayout();
    
    // 速度 - 需要足够空间显示大转速值（如 "Setpoint: 15000 RPM"）
    speedSetpointLabel = new QLabel("> Speed\nSetpoint: 0 RPM\nFeedback: 0 RPM", this);
    speedSetpointLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    speedSetpointLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    speedSetpointLabel->setWordWrap(true);
    speedSetpointLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    speedSetpointLabel->setMinimumWidth(170);  // 增加宽度以显示大转速值
    contentLayout->addWidget(speedSetpointLabel);
    
    // 转矩 - 需要足够空间显示转矩值（如 "Setpoint: 99.99 Nm"）
    torqueSetpointLabel = new QLabel("> Torque\nSetpoint: 0.00 Nm\nFeedback: 0.00 Nm", this);
    torqueSetpointLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    torqueSetpointLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    torqueSetpointLabel->setWordWrap(true);
    torqueSetpointLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    torqueSetpointLabel->setMinimumWidth(170);  // 增加宽度以显示转矩值
    contentLayout->addWidget(torqueSetpointLabel);
    
    // 位置 - 需要足够空间显示位置值（如 "Setpoint: 999999 cnt"）
    positionSetpointLabel = new QLabel("> Position\nSetpoint: 0 cnt\nFeedback: 0 cnt", this);
    positionSetpointLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    positionSetpointLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    positionSetpointLabel->setWordWrap(true);
    positionSetpointLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    positionSetpointLabel->setMinimumWidth(170);  // 增加宽度以显示大位置值
    contentLayout->addWidget(positionSetpointLabel);
    
    // PWM占空比
    pwmBar = new QProgressBar(this);
    pwmBar->setRange(0, 100);
    pwmBar->setValue(0);
    pwmBar->setFormat("0% PWM Duty");
    pwmBar->setStyleSheet("QProgressBar::chunk { background-color: #34c759; }");
    contentLayout->addWidget(pwmBar);
    
    // Vd和Vq - 需要足够空间显示大电压值（如 "Vd: 999.9 V"）
    QHBoxLayout *voltageLayout = new QHBoxLayout();
    vdLabel = new QLabel("Vd: 0.0 V", this);
    vdLabel->setStyleSheet("font-size: 13px; color: #34c759; font-weight: 600;");
    vdLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    vdLabel->setMinimumWidth(90);  // 增加宽度以显示大电压值
    vqLabel = new QLabel("Vq: 0.0 V", this);
    vqLabel->setStyleSheet("font-size: 13px; color: #34c759; font-weight: 600;");
    vqLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    vqLabel->setMinimumWidth(90);  // 增加宽度以显示大电压值
    voltageLayout->addWidget(vdLabel);
    voltageLayout->addWidget(vqLabel);
    voltageLayout->addStretch();
    contentLayout->addLayout(voltageLayout);
    
    contentLayout->addStretch();
}

void ControlOutputCard::updateData(const FOCParameters &params)
{
    speedSetpointLabel->setText(QString("> Speed\nSetpoint: %1 RPM\nFeedback: %2 RPM")
                                .arg(1500.0, 0, 'f', 0).arg(params.speed, 0, 'f', 0));
    torqueSetpointLabel->setText(QString("> Torque\nSetpoint: %1 Nm\nFeedback: %2 Nm")
                                 .arg(1.50, 0, 'f', 2).arg(params.torque, 0, 'f', 2));
    positionSetpointLabel->setText(QString("> Position\nSetpoint: %1 cnt\nFeedback: %2 cnt")
                                   .arg(10240).arg(static_cast<int>(params.angle * 100)));
    
    pwmBar->setValue(static_cast<int>(params.pwmDuty));
    pwmBar->setFormat(QString("%1% PWM Duty").arg(params.pwmDuty, 0, 'f', 0));
    
    vdLabel->setText(QString("Vd: %1 V").arg(params.vd, 0, 'f', 1));
    vqLabel->setText(QString("Vq: %1 V").arg(params.vq, 0, 'f', 1));
}

