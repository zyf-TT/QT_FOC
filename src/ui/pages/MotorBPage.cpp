#include "MotorBPage.h"
#include "../widgets/Card.h"
#include <QVBoxLayout>
#include <QLabel>

MotorBPage::MotorBPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void MotorBPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    
    Card *card = new Card("Motor B Details", this);
    QVBoxLayout *cardLayout = card->getContentLayout();
    
    QLabel *label = new QLabel("Motor B detailed parameters will be displayed here.", this);
    cardLayout->addWidget(label);
    
    layout->addWidget(card);
    layout->addStretch();
}

void MotorBPage::updateData(const FOCParameters &params)
{
    // TODO: 实现Motor B详细参数显示
}

