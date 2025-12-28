#ifndef CARD_H
#define CARD_H

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class Card : public QFrame
{
    Q_OBJECT

public:
    explicit Card(const QString &title = "", QWidget *parent = nullptr);
    ~Card() = default;
    
    // 设置标题
    void setTitle(const QString &title);
    QString getTitle() const;
    
    // 获取内容布局（用于添加子组件）
    QVBoxLayout* getContentLayout() const;
    QHBoxLayout* getHeaderLayout() const;

protected:
    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;

private:
    void setupUI();
};

#endif // CARD_H

