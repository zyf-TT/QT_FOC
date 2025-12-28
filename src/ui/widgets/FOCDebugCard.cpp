#include "FOCDebugCard.h"
#include <QVBoxLayout>

FOCDebugCard::FOCDebugCard(QWidget *parent)
    : Card("FOC Debug Info", parent)
{
    setupContent();
}

void FOCDebugCard::setupContent()
{
    QVBoxLayout *contentLayout = getContentLayout();
    
    // Advanced下拉菜单
    QComboBox *combo = new QComboBox(this);
    combo->addItem("Advanced");
    combo->setMinimumWidth(120);
    combo->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    getHeaderLayout()->addWidget(combo);
    
    // Clarke变换 - 需要足够空间显示特殊字符和大数值（如 "Iα: 99.99 / Iβ: 99.99"）
    clarkeLabel = new QLabel("> Clarke: Iα: 0.00 / Iβ: 0.00", this);
    clarkeLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    clarkeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    clarkeLabel->setWordWrap(true);
    clarkeLabel->setMinimumWidth(220);  // 增加宽度以确保特殊字符和大数值完整显示
    contentLayout->addWidget(clarkeLabel);
    
    // Park变换 - 需要足够空间显示特殊字符和大数值（如 "θ: 360.0° / ώ: 9999 RPM"）
    parkLabel = new QLabel("> Park: θ: 0.0° / ώ: 0 RPM", this);
    parkLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    parkLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    parkLabel->setWordWrap(true);
    parkLabel->setMinimumWidth(220);  // 增加宽度以确保特殊字符和大数值完整显示
    contentLayout->addWidget(parkLabel);
    
    // 编码器 - 需要足够空间显示角度值（如 "Angle: 360.0°"）
    encoderLabel = new QLabel("> Encoder: Angle: 0.0° / PLL: Loc", this);
    encoderLabel->setStyleSheet("font-size: 13px; color: #1d1d1f;");
    encoderLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    encoderLabel->setWordWrap(true);
    encoderLabel->setMinimumWidth(220);  // 增加宽度以确保角度值完整显示
    contentLayout->addWidget(encoderLabel);
    
    contentLayout->addStretch();
}

void FOCDebugCard::updateData(const FOCParameters &params)
{
    clarkeLabel->setText(QString("> Clarke: Iα: %1 / Iβ: %2")
                        .arg(params.ia, 0, 'f', 2).arg(params.ib, 0, 'f', 2));
    parkLabel->setText(QString("> Park: θ: %1° / ώ: %2 RPM")
                      .arg(params.theta, 0, 'f', 1).arg(params.omega, 0, 'f', 0));
    encoderLabel->setText(QString("> Encoder: Angle: %1° / PLL: Loc")
                         .arg(params.encoderAngle, 0, 'f', 1));
}

