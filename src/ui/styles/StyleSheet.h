#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>

class StyleSheet
{
public:
    // 获取完整的应用程序样式表
    static QString getApplicationStyleSheet();
    
    // 获取特定组件的样式
    static QString getCardStyle();
    static QString getNavBarButtonStyle();
    static QString getStatusBadgeStyle();
    static QString getProgressBarStyle();
};

#endif // STYLESHEET_H

