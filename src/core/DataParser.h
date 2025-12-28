#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "models/FOCParameters.h"
#include <QByteArray>
#include <QString>

class DataParser
{
public:
    DataParser() = default;
    ~DataParser() = default;
    
    // 解析接收到的数据
    bool parseData(const QByteArray &data, FOCParameters &params);
    bool parseData(const QString &data, FOCParameters &params);
    
private:
    // 解析不同格式的数据
    bool parseJSONFormat(const QString &data, FOCParameters &params);
    bool parseCSVFormat(const QString &data, FOCParameters &params);
    bool parseCustomFormat(const QString &data, FOCParameters &params);
    
    // 辅助函数
    void updateParam(const QString &key, double value, FOCParameters &params);
};

#endif // DATAPARSER_H

