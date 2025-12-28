#ifndef MOTORAPAGE_H
#define MOTORAPAGE_H

#include <QWidget>
#include "models/FOCParameters.h"

class MotorAPage : public QWidget
{
    Q_OBJECT

public:
    explicit MotorAPage(QWidget *parent = nullptr);
    ~MotorAPage() = default;
    
    void updateData(const FOCParameters &params);

private:
    void setupUI();
};

#endif // MOTORAPAGE_H

