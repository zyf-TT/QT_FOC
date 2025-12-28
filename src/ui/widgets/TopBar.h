#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

class TopBar : public QWidget
{
    Q_OBJECT

public:
    explicit TopBar(QWidget *parent = nullptr);
    ~TopBar() = default;
    
    // 设置标题
    void setTitle(const QString &title);
    
    // 设置连接状态
    void setConnectionStatus(const QString &status);
    void setConnected(const QString &portName, int baudRate);
    void setDisconnected();

private:
    QHBoxLayout *layout;
    QLabel *titleLabel;
    QLabel *statusLabel;
    
    void setupUI();
};

#endif // TOPBAR_H

