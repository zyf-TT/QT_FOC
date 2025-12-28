#ifndef ELECTRICALSTATUSCARD_H
#define ELECTRICALSTATUSCARD_H

#include "Card.h"
#include "models/FOCParameters.h"
#include <QLabel>
#include <QProgressBar>
#include <QComboBox>

class ElectricalStatusCard : public Card
{
    Q_OBJECT

public:
    explicit ElectricalStatusCard(QWidget *parent = nullptr);
    ~ElectricalStatusCard() = default;
    
    void updateData(const FOCParameters &params);

private:
    QLabel *idLabel;
    QLabel *iqLabel;
    QProgressBar *currentBar;
    QLabel *dcBusLabel;
    QLabel *motorTempLabel;
    QLabel *driverTempLabel;
    
    void setupContent();
};

#endif // ELECTRICALSTATUSCARD_H

