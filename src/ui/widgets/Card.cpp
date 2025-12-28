#include "Card.h"
#include "../styles/StyleSheet.h"

Card::Card(const QString &title, QWidget *parent)
    : QFrame(parent)
{
    setProperty("card", true);
    setStyleSheet(StyleSheet::getCardStyle());
    setupUI();
    setTitle(title);
}

void Card::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);
    
    // 设置Card的最小宽度，确保内容有足够空间
    setMinimumWidth(250);  // 增加最小宽度，确保内容有足够空间显示
    
    // 标题栏
    headerLayout = new QHBoxLayout();
    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: #1d1d1f;");
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    titleLabel->setWordWrap(true);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    mainLayout->addLayout(headerLayout);
}

void Card::setTitle(const QString &title)
{
    if (titleLabel) {
        titleLabel->setText(title);
        titleLabel->setVisible(!title.isEmpty());
    }
}

QString Card::getTitle() const
{
    return titleLabel ? titleLabel->text() : "";
}

QVBoxLayout* Card::getContentLayout() const
{
    return mainLayout;
}

QHBoxLayout* Card::getHeaderLayout() const
{
    return headerLayout;
}

