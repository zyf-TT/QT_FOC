#include "ElectricalStatusCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

ElectricalStatusCard::ElectricalStatusCard(QWidget *parent)
    : Card("Electrical Status", parent)
{
    setupContent();
}

void ElectricalStatusCard::setupContent()
{
    QVBoxLayout *contentLayout = getContentLayout();
    
    // 下拉菜单（占位）
    QComboBox *combo = new QComboBox(this);
    combo->addItem("D-Q Currents");
    combo->setMinimumWidth(150);
    combo->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    getHeaderLayout()->addWidget(combo);
    
    // Id和Iq显示 - 需要足够空间显示大电流值（如 "Id: 99.99 A"）
    QHBoxLayout *currentLayout = new QHBoxLayout();
    idLabel = new QLabel("Id: 0.00 A", this);
    idLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #007aff;");
    idLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    idLabel->setMinimumWidth(90);  // 增加宽度以显示大电流值
    iqLabel = new QLabel("Iq: 0.00 A", this);
    iqLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #007aff;");
    iqLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    iqLabel->setMinimumWidth(90);  // 增加宽度以显示大电流值
    currentLayout->addWidget(idLabel);
    currentLayout->addWidget(iqLabel);
    currentLayout->addStretch();
    contentLayout->addLayout(currentLayout);
    
    // 电流进度条
    currentBar = new QProgressBar(this);
    currentBar->setRange(0, 100);
    currentBar->setValue(0);
    currentBar->setTextVisible(false);
    contentLayout->addWidget(currentBar);
    
    // DC Bus - 需要足够空间显示大电压值（如 "DC Bus: 999.9 V"）
    dcBusLabel = new QLabel("DC Bus: 0.0 V", this);
    dcBusLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    dcBusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    dcBusLabel->setMinimumWidth(130);  // 增加宽度以显示大电压值
    contentLayout->addWidget(dcBusLabel);
    
    // 温度 - 需要足够空间显示大温度值（如 "Motor Temp: 125.5°C"）
    motorTempLabel = new QLabel("Motor Temp: 0°C", this);
    motorTempLabel->setStyleSheet("font-size: 13px; color: #ff9500;");
    motorTempLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    motorTempLabel->setMinimumWidth(150);  // 增加宽度以显示带小数点的温度值
    contentLayout->addWidget(motorTempLabel);
    
    driverTempLabel = new QLabel("Driver Temp: 0°C", this);
    driverTempLabel->setStyleSheet("font-size: 13px; color: #ff9500;");
    driverTempLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    driverTempLabel->setMinimumWidth(150);  // 增加宽度以显示带小数点的温度值
    contentLayout->addWidget(driverTempLabel);
    
    contentLayout->addStretch();
}

void ElectricalStatusCard::updateData(const FOCParameters &params)
{
    idLabel->setText(QString("Id: %1 A").arg(params.id, 0, 'f', 2));
    iqLabel->setText(QString("Iq: %1 A").arg(params.iq, 0, 'f', 2));
    
    // 计算电流比例（简化）
    double maxCurrent = qMax(qAbs(params.id), qAbs(params.iq));
    int barValue = qMin(100, static_cast<int>(maxCurrent * 10));
    currentBar->setValue(barValue);
    
    dcBusLabel->setText(QString("DC Bus: %1 V").arg(params.dcBus, 0, 'f', 1));
    motorTempLabel->setText(QString("Motor Temp: %1°C").arg(params.motorTemp, 0, 'f', 1));
    driverTempLabel->setText(QString("Driver Temp: %1°C").arg(params.driverTemp, 0, 'f', 1));
}

