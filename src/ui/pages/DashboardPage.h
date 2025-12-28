#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "../widgets/MotorStatusCard.h"
#include "../widgets/ElectricalStatusCard.h"
#include "../widgets/ControlOutputCard.h"
#include "../widgets/FOCDebugCard.h"
#include "models/FOCParameters.h"
#include <QTextEdit>

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage() = default;
    
    void updateData(const FOCParameters &params);

private:
    QVBoxLayout *mainLayout;
    
    // Motor Status卡片
    MotorStatusCard *motorACard;
    MotorStatusCard *motorBCard;
    
    // 参数显示卡片
    ElectricalStatusCard *electricalCard;
    ControlOutputCard *controlCard;
    FOCDebugCard *debugCard;
    
    // 波形图卡片
    QTextEdit *graphTextEdit;
    
    void setupUI();
};

#endif // DASHBOARDPAGE_H

