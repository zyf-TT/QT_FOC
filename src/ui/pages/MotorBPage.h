#ifndef MOTORBPAGE_H
#define MOTORBPAGE_H

#include <QWidget>
#include "models/FOCParameters.h"

class MotorBPage : public QWidget
{
    Q_OBJECT

public:
    explicit MotorBPage(QWidget *parent = nullptr);
    ~MotorBPage() = default;
    
    void updateData(const FOCParameters &params);

private:
    void setupUI();
};

#endif // MOTORBPAGE_H

