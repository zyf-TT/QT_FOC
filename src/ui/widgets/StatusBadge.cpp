#include "StatusBadge.h"
#include "../styles/StyleSheet.h"

StatusBadge::StatusBadge(const QString &text, const QString &color, QWidget *parent)
    : QLabel(parent)
{
    setMinimumWidth(90);  // 增加宽度确保"RUNNING"文本完整显示
    setMinimumHeight(30);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setStatus(text, color);
}

void StatusBadge::setStatus(const QString &text, const QString &color)
{
    setText(text);
    updateStyle(color);
}

void StatusBadge::setRunning()
{
    setStatus("RUNNING", "#34c759");
}

void StatusBadge::setStopped()
{
    setStatus("STOPPED", "#ff9500");
}

void StatusBadge::updateStyle(const QString &color)
{
    QString style = StyleSheet::getStatusBadgeStyle();
    style += QString("QLabel { background-color: %1; padding: 6px 14px; }").arg(color);  // 增加padding确保文本有足够空间
    setStyleSheet(style);
    setAlignment(Qt::AlignCenter);
    setWordWrap(false);  // 状态文本不应该换行
}

