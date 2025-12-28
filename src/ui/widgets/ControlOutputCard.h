#ifndef CONTROLOUTPUTCARD_H
#define CONTROLOUTPUTCARD_H

#include "Card.h"
#include "models/FOCParameters.h"
#include <QLabel>
#include <QProgressBar>

class ControlOutputCard : public Card
{
    Q_OBJECT

public:
    explicit ControlOutputCard(QWidget *parent = nullptr);
    ~ControlOutputCard() = default;
    
    void updateData(const FOCParameters &params);

private:
    QLabel *speedSetpointLabel;
    QLabel *speedFeedbackLabel;
    QLabel *torqueSetpointLabel;
    QLabel *torqueFeedbackLabel;
    QLabel *positionSetpointLabel;
    QLabel *positionFeedbackLabel;
    QProgressBar *pwmBar;
    QLabel *vdLabel;
    QLabel *vqLabel;
    
    void setupContent();
};

#endif // CONTROLOUTPUTCARD_H

