#ifndef STATUSBADGE_H
#define STATUSBADGE_H

#include <QLabel>

class StatusBadge : public QLabel
{
    Q_OBJECT

public:
    explicit StatusBadge(const QString &text, const QString &color, QWidget *parent = nullptr);
    ~StatusBadge() = default;
    
    // 设置状态文本和颜色
    void setStatus(const QString &text, const QString &color);
    
    // 预设状态
    void setRunning();
    void setStopped();

private:
    void updateStyle(const QString &color);
};

#endif // STATUSBADGE_H

