#ifndef FOCDEBUGCARD_H
#define FOCDEBUGCARD_H

#include "Card.h"
#include "models/FOCParameters.h"
#include <QLabel>
#include <QComboBox>

class FOCDebugCard : public Card
{
    Q_OBJECT

public:
    explicit FOCDebugCard(QWidget *parent = nullptr);
    ~FOCDebugCard() = default;
    
    void updateData(const FOCParameters &params);

private:
    QLabel *clarkeLabel;
    QLabel *parkLabel;
    QLabel *encoderLabel;
    
    void setupContent();
};

#endif // FOCDEBUGCARD_H

