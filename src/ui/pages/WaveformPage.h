#ifndef WAVEFORMPAGE_H
#define WAVEFORMPAGE_H

#include <QWidget>

class WaveformPage : public QWidget
{
    Q_OBJECT

public:
    explicit WaveformPage(QWidget *parent = nullptr);
    ~WaveformPage() = default;

private:
    void setupUI();
};

#endif // WAVEFORMPAGE_H

