#ifndef NAVBAR_H
#define NAVBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

class NavBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavBar(QWidget *parent = nullptr);
    ~NavBar() = default;
    
    // 设置活动标签
    void setActiveTab(int index);
    int getActiveTab() const;

signals:
    void tabClicked(int index);

private slots:
    void onButtonClicked(int id);

private:
    QHBoxLayout *layout;
    QButtonGroup *buttonGroup;
    QPushButton *btn_Dashboard;
    QPushButton *btn_MotorA;
    QPushButton *btn_MotorB;
    QPushButton *btn_Waveform;
    QPushButton *btn_Tuning;
    QPushButton *btn_System;
    int activeTabIndex;
    
    void setupUI();
    void updateButtonStyle();
};

#endif // NAVBAR_H

