#ifndef SYSTEMPAGE_H
#define SYSTEMPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>

enum class ConnectionState {
    Disconnected,
    Connecting,
    Connected,
    Error
};

class SystemPage : public QWidget
{
    Q_OBJECT

public:
    explicit SystemPage(QWidget *parent = nullptr);
    ~SystemPage();
    
    QString getSelectedPort() const;
    int getSelectedBaudRate() const;
    void setPortList(const QStringList &ports);
    void setBaudRateList(const QStringList &baudRates);
    void setPortEnabled(bool enabled);
    void setConnectionState(ConnectionState state);
    void appendLog(const QString &message, bool isReceived = true);

signals:
    void openPortRequested(const QString &portName, int baudRate);
    void closePortRequested();

private slots:
    void onOpenCloseClicked();

private:
    QVBoxLayout *mainLayout;
    QComboBox *comboBox_Port;
    QComboBox *comboBox_BaudRate;
    QPushButton *pushButton_OpenClose;
    QTextEdit *textEdit_DataLog;
    QLabel *statusLabel;
    bool isPortOpen;
    ConnectionState connectionState;
    QFile *logFile;
    QTextStream *logStream;
    
    void setupUI();
    void updateUI();
    QString getStateText(ConnectionState state) const;
    QString getStateColor(ConnectionState state) const;
    void initializeLogFile();
    void closeLogFile();
};

#endif // SYSTEMPAGE_H

