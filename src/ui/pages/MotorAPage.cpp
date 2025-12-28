#include "MotorAPage.h"
#include "../widgets/Card.h"
#include <QVBoxLayout>
#include <QLabel>

MotorAPage::MotorAPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void MotorAPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    
    Card *card = new Card("Motor A Details", this);
    QVBoxLayout *cardLayout = card->getContentLayout();
    
    QLabel *label = new QLabel("Motor A detailed parameters will be displayed here.", this);
    cardLayout->addWidget(label);
    
    layout->addWidget(card);
    layout->addStretch();
}

void MotorAPage::updateData(const FOCParameters &params)
{
    // TODO: 实现Motor A详细参数显示
}

