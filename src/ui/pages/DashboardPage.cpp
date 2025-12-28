#include "DashboardPage.h"
#include "../widgets/Card.h"
#include <QScrollArea>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void DashboardPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    // 第一行：Motor Status卡片
    QHBoxLayout *motorRow = new QHBoxLayout();
    motorRow->setSpacing(15);
    motorRow->setContentsMargins(0, 0, 0, 0);  // 确保没有额外的边距
    
    motorACard = new MotorStatusCard("Motor A", "#34c759", this);
    motorBCard = new MotorStatusCard("Motor B", "#ff9500", this);
    
    motorRow->addWidget(motorACard, 1);
    motorRow->addWidget(motorBCard, 1);
    
    mainLayout->addLayout(motorRow);
    
    // 第二行：参数显示卡片
    QHBoxLayout *paramRow = new QHBoxLayout();
    paramRow->setSpacing(15);
    paramRow->setContentsMargins(0, 0, 0, 0);  // 确保没有额外的边距
    
    electricalCard = new ElectricalStatusCard(this);
    controlCard = new ControlOutputCard(this);
    debugCard = new FOCDebugCard(this);
    
    paramRow->addWidget(electricalCard, 1);
    paramRow->addWidget(controlCard, 1);
    paramRow->addWidget(debugCard, 1);
    
    mainLayout->addLayout(paramRow);
    
    // 第三行：波形图卡片
    Card *graphCard = new Card("Realtime Graphs", this);
    QVBoxLayout *graphLayout = graphCard->getContentLayout();
    
    // 图例
    QLabel *legend = new QLabel("• Speed (orange)  • D-Q Currents (blue)  • Torque vs Position (purple)", this);
    legend->setStyleSheet("font-size: 12px; color: #8e8e93;");
    legend->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    legend->setWordWrap(true);
    legend->setMinimumWidth(300);
    graphLayout->addWidget(legend);
    
    // 波形图（临时用TextEdit占位）
    graphTextEdit = new QTextEdit(this);
    graphTextEdit->setReadOnly(true);
    graphTextEdit->setFixedHeight(200);
    graphTextEdit->setPlainText("Waveform display area\n(To be implemented with QCustomPlot or QChart)");
    graphLayout->addWidget(graphTextEdit);
    
    mainLayout->addWidget(graphCard, 1);
    
    mainLayout->addStretch();
}

void DashboardPage::updateData(const FOCParameters &params)
{
    // 更新Motor A
    QString modeA = params.isRunning ? "Speed Control" : "Stopped";
    motorACard->updateStatus(modeA, 
                            QString("RPM: %1").arg(params.speed, 0, 'f', 0),
                            QString("Temp: %1°C").arg(params.motorTemp, 0, 'f', 1),
                            params.isRunning);
    
    // 更新Motor B（使用转矩数据）
    QString modeB = params.isRunning ? "Torque Control" : "Stopped";
    motorBCard->updateStatus(modeB,
                            QString("Torque: %1 Nm").arg(params.torque, 0, 'f', 2),
                            QString("Temp: %1°C").arg(params.driverTemp, 0, 'f', 1),
                            false);
    
    // 更新其他卡片
    electricalCard->updateData(params);
    controlCard->updateData(params);
    debugCard->updateData(params);
}

