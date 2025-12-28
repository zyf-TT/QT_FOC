#ifndef TUNINGPAGE_H
#define TUNINGPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "models/ControlMode.h"

class TuningPage : public QWidget
{
    Q_OBJECT

public:
    explicit TuningPage(QWidget *parent = nullptr);
    ~TuningPage() = default;
    
    ControlMode getCurrentMode() const;
    double getTargetValue() const;
    void setControlsEnabled(bool enabled);

signals:
    void sendCommandRequested(const QString &command);
    void startRequested();
    void stopRequested();
    void modeChanged(ControlMode mode);

private slots:
    void onModeChanged(int id);
    void onSendCommand();
    void onStart();
    void onStop();

private:
    QVBoxLayout *mainLayout;
    QLabel *label_Mode;
    QButtonGroup *buttonGroup_Mode;
    QRadioButton *radioButton_Speed;
    QRadioButton *radioButton_Current;
    QRadioButton *radioButton_Position;
    QRadioButton *radioButton_Torque;
    ControlMode currentMode;
    QLabel *label_TargetParam;
    QDoubleSpinBox *doubleSpinBox_TargetParam;
    QPushButton *pushButton_SendCommand;
    QPushButton *pushButton_Start;
    QPushButton *pushButton_Stop;
    
    void setupUI();
    void updateControlModeUI(ControlMode mode);
};

#endif // TUNINGPAGE_H

