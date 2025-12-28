#ifndef MOTORSTATUSCARD_H
#define MOTORSTATUSCARD_H

#include "Card.h"
#include "StatusBadge.h"
#include <QLabel>
#include <QVBoxLayout>

class MotorStatusCard : public Card
{
    Q_OBJECT

public:
    explicit MotorStatusCard(const QString &motorName, const QString &headerColor, QWidget *parent = nullptr);
    ~MotorStatusCard() = default;
    
    // 更新电机状态
    void updateStatus(const QString &mode, const QString &value1, const QString &value2, bool isRunning);

private:
    QString motorName;
    QString headerColor;
    StatusBadge *statusBadge;
    QLabel *modeLabel;
    QLabel *value1Label;
    QLabel *value2Label;
    
    void setupContent();
};

#endif // MOTORSTATUSCARD_H

