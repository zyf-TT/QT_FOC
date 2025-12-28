#include "NavBar.h"
#include "../styles/StyleSheet.h"
#include <QDebug>

NavBar::NavBar(QWidget *parent)
    : QWidget(parent)
    , activeTabIndex(0)
{
    setupUI();
}

void NavBar::setupUI()
{
    setFixedHeight(45);
    setStyleSheet("background-color: #ffffff; border-bottom: 1px solid #d2d2d7;");
    
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 0, 20, 0);
    layout->setSpacing(0);
    
    buttonGroup = new QButtonGroup(this);
    
    // 创建标签按钮
    btn_Dashboard = new QPushButton("Dashboard", this);
    btn_MotorA = new QPushButton("Motor A", this);
    btn_MotorB = new QPushButton("Motor B", this);
    btn_Waveform = new QPushButton("Waveform", this);
    btn_Tuning = new QPushButton("Tuning", this);
    btn_System = new QPushButton("System", this);
    
    // 添加到按钮组
    buttonGroup->addButton(btn_Dashboard, 0);
    buttonGroup->addButton(btn_MotorA, 1);
    buttonGroup->addButton(btn_MotorB, 2);
    buttonGroup->addButton(btn_Waveform, 3);
    buttonGroup->addButton(btn_Tuning, 4);
    buttonGroup->addButton(btn_System, 5);
    
    // 设置按钮样式
    QString buttonStyle = StyleSheet::getNavBarButtonStyle();
    btn_Dashboard->setStyleSheet(buttonStyle);
    btn_MotorA->setStyleSheet(buttonStyle);
    btn_MotorB->setStyleSheet(buttonStyle);
    btn_Waveform->setStyleSheet(buttonStyle);
    btn_Tuning->setStyleSheet(buttonStyle);
    btn_System->setStyleSheet(buttonStyle);
    
    // 添加到布局
    layout->addWidget(btn_Dashboard);
    layout->addWidget(btn_MotorA);
    layout->addWidget(btn_MotorB);
    layout->addWidget(btn_Waveform);
    layout->addWidget(btn_Tuning);
    layout->addWidget(btn_System);
    layout->addStretch();
    
    // 连接信号
    connect(buttonGroup, &QButtonGroup::idClicked, this, &NavBar::onButtonClicked);
    
    // 默认选中Dashboard
    setActiveTab(0);
}

void NavBar::setActiveTab(int index)
{
    if (index >= 0 && index < 6) {
        activeTabIndex = index;
        updateButtonStyle();
    }
}

int NavBar::getActiveTab() const
{
    return activeTabIndex;
}

void NavBar::onButtonClicked(int id)
{
    setActiveTab(id);
    emit tabClicked(id);
}

void NavBar::updateButtonStyle()
{
    QString baseStyle = StyleSheet::getNavBarButtonStyle();
    
    QList<QPushButton*> buttons = {
        btn_Dashboard, btn_MotorA, btn_MotorB,
        btn_Waveform, btn_Tuning, btn_System
    };
    
    for (int i = 0; i < buttons.size(); ++i) {
        if (i == activeTabIndex) {
            buttons[i]->setProperty("active", true);
            buttons[i]->setStyleSheet(baseStyle);
        } else {
            buttons[i]->setProperty("active", false);
            buttons[i]->setStyleSheet(baseStyle);
        }
    }
}

