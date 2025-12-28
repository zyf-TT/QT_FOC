#include "TuningPage.h"
#include "../widgets/Card.h"
#include <QHBoxLayout>

TuningPage::TuningPage(QWidget *parent)
    : QWidget(parent)
    , currentMode(ControlMode::SpeedControl)
{
    setupUI();
}

void TuningPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    Card *controlCard = new Card("Control Commands", this);
    QVBoxLayout *cardLayout = controlCard->getContentLayout();
    
    // 运行模式选择
    label_Mode = new QLabel("运行模式:", this);
    label_Mode->setStyleSheet("font-weight: 600; font-size: 14px;");
    cardLayout->addWidget(label_Mode);
    
    buttonGroup_Mode = new QButtonGroup(this);
    radioButton_Speed = new QRadioButton("速度控制", this);
    radioButton_Current = new QRadioButton("电流控制", this);
    radioButton_Position = new QRadioButton("位置控制", this);
    radioButton_Torque = new QRadioButton("转矩控制", this);
    
    buttonGroup_Mode->addButton(radioButton_Speed, static_cast<int>(ControlMode::SpeedControl));
    buttonGroup_Mode->addButton(radioButton_Current, static_cast<int>(ControlMode::CurrentControl));
    buttonGroup_Mode->addButton(radioButton_Position, static_cast<int>(ControlMode::PositionControl));
    buttonGroup_Mode->addButton(radioButton_Torque, static_cast<int>(ControlMode::TorqueControl));
    
    QHBoxLayout *modeLayout = new QHBoxLayout();
    modeLayout->addWidget(radioButton_Speed);
    modeLayout->addWidget(radioButton_Current);
    modeLayout->addWidget(radioButton_Position);
    modeLayout->addWidget(radioButton_Torque);
    modeLayout->addStretch();
    cardLayout->addLayout(modeLayout);
    
    connect(buttonGroup_Mode, &QButtonGroup::idClicked, this, &TuningPage::onModeChanged);
    
    // 控制参数
    label_TargetParam = new QLabel("目标参数:", this);
    cardLayout->addWidget(label_TargetParam);
    
    doubleSpinBox_TargetParam = new QDoubleSpinBox(this);
    cardLayout->addWidget(doubleSpinBox_TargetParam);
    
    // 按钮
    pushButton_SendCommand = new QPushButton("发送命令", this);
    connect(pushButton_SendCommand, &QPushButton::clicked, this, &TuningPage::onSendCommand);
    cardLayout->addWidget(pushButton_SendCommand);
    
    pushButton_Start = new QPushButton("启动", this);
    connect(pushButton_Start, &QPushButton::clicked, this, &TuningPage::onStart);
    cardLayout->addWidget(pushButton_Start);
    
    pushButton_Stop = new QPushButton("停止", this);
    connect(pushButton_Stop, &QPushButton::clicked, this, &TuningPage::onStop);
    cardLayout->addWidget(pushButton_Stop);
    
    mainLayout->addWidget(controlCard);
    mainLayout->addStretch();
    
    // 默认选择速度控制模式
    radioButton_Speed->setChecked(true);
    updateControlModeUI(ControlMode::SpeedControl);
}

void TuningPage::onModeChanged(int id)
{
    currentMode = static_cast<ControlMode>(id);
    updateControlModeUI(currentMode);
    emit modeChanged(currentMode);
}

void TuningPage::updateControlModeUI(ControlMode mode)
{
    switch (mode) {
        case ControlMode::SpeedControl:
            label_TargetParam->setText("目标转速 (RPM):");
            doubleSpinBox_TargetParam->setDecimals(1);
            doubleSpinBox_TargetParam->setRange(-10000.0, 10000.0);
            doubleSpinBox_TargetParam->setValue(0.0);
            pushButton_SendCommand->setText("发送转速命令");
            break;
        case ControlMode::CurrentControl:
            label_TargetParam->setText("目标电流 (A):");
            doubleSpinBox_TargetParam->setDecimals(2);
            doubleSpinBox_TargetParam->setRange(-100.0, 100.0);
            doubleSpinBox_TargetParam->setValue(0.0);
            pushButton_SendCommand->setText("发送电流命令");
            break;
        case ControlMode::PositionControl:
            label_TargetParam->setText("目标角度 (°):");
            doubleSpinBox_TargetParam->setDecimals(2);
            doubleSpinBox_TargetParam->setRange(-360.0, 360.0);
            doubleSpinBox_TargetParam->setValue(0.0);
            pushButton_SendCommand->setText("发送位置命令");
            break;
        case ControlMode::TorqueControl:
            label_TargetParam->setText("目标转矩 (N·m):");
            doubleSpinBox_TargetParam->setDecimals(3);
            doubleSpinBox_TargetParam->setRange(-50.0, 50.0);
            doubleSpinBox_TargetParam->setValue(0.0);
            pushButton_SendCommand->setText("发送转矩命令");
            break;
    }
}

void TuningPage::onSendCommand()
{
    double targetValue = doubleSpinBox_TargetParam->value();
    QString command;
    
    switch (currentMode) {
        case ControlMode::SpeedControl:
            command = QString("CMD:SPEED=%1\n").arg(targetValue);
            break;
        case ControlMode::CurrentControl:
            command = QString("CMD:CURRENT=%1\n").arg(targetValue);
            break;
        case ControlMode::PositionControl:
            command = QString("CMD:POSITION=%1\n").arg(targetValue);
            break;
        case ControlMode::TorqueControl:
            command = QString("CMD:TORQUE=%1\n").arg(targetValue);
            break;
    }
    
    emit sendCommandRequested(command);
}

void TuningPage::onStart()
{
    emit startRequested();
}

void TuningPage::onStop()
{
    emit stopRequested();
}

ControlMode TuningPage::getCurrentMode() const
{
    return currentMode;
}

double TuningPage::getTargetValue() const
{
    return doubleSpinBox_TargetParam->value();
}

void TuningPage::setControlsEnabled(bool enabled)
{
    pushButton_SendCommand->setEnabled(enabled);
    pushButton_Start->setEnabled(enabled);
    pushButton_Stop->setEnabled(enabled);
}

