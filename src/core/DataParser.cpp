#include "DataParser.h"
#include <QStringList>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

bool DataParser::parseData(const QByteArray &data, FOCParameters &params)
{
    QString dataString = QString::fromUtf8(data).trimmed();
    return parseData(dataString, params);
}

bool DataParser::parseData(const QString &data, FOCParameters &params)
{
    if (data.isEmpty()) {
        return false;
    }
    
    // 判断数据格式并解析
    if (data.startsWith("{")) {
        // JSON格式
        return parseJSONFormat(data, params);
    } else if (data.contains(",") && !data.contains("=")) {
        // CSV格式
        return parseCSVFormat(data, params);
    } else if (data.startsWith("FOC:")) {
        // 自定义格式
        return parseCustomFormat(data, params);
    }
    
    return false;
}

bool DataParser::parseJSONFormat(const QString &data, FOCParameters &params)
{
    // 使用QJsonDocument进行标准JSON解析
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON解析错误:" << error.errorString();
        return false;
    }
    
    if (!doc.isObject()) {
        qDebug() << "JSON数据不是对象格式";
        return false;
    }
    
    QJsonObject obj = doc.object();
    
    // 解析所有字段
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString key = it.key().toLower();
        QJsonValue value = it.value();
        
        if (value.isDouble()) {
            updateParam(key, value.toDouble(), params);
        } else if (value.isBool() && key == "isrunning") {
            params.isRunning = value.toBool();
        }
    }
    
    return true;
}

bool DataParser::parseCSVFormat(const QString &data, FOCParameters &params)
{
    // CSV格式: speed,current,voltage,torque,temperature,angle,id,iq
    QStringList values = data.split(",");
    if (values.size() >= 8) {
        params.speed = values[0].toDouble();
        params.current = values[1].toDouble();
        params.voltage = values[2].toDouble();
        params.torque = values[3].toDouble();
        params.temperature = values[4].toDouble();
        params.angle = values[5].toDouble();
        params.id = values[6].toDouble();
        params.iq = values[7].toDouble();
        return true;
    }
    return false;
}

bool DataParser::parseCustomFormat(const QString &data, FOCParameters &params)
{
    // 自定义格式: FOC:SPEED=1000.0,CURRENT=2.5,VOLTAGE=24.0,...
    QString paramsStr = data.mid(4); // 去掉"FOC:"
    QStringList pairs = paramsStr.split(",");
    
    for (const QString &pair : pairs) {
        QStringList keyValue = pair.split("=");
        if (keyValue.size() == 2) {
            QString key = keyValue[0].trimmed().toUpper();
            double value = keyValue[1].toDouble();
            updateParam(key, value, params);
        }
    }
    return true;
}

void DataParser::updateParam(const QString &key, double value, FOCParameters &params)
{
    QString keyLower = key.toLower();
    
    if (keyLower == "speed") params.speed = value;
    else if (keyLower == "current") params.current = value;
    else if (keyLower == "voltage") params.voltage = value;
    else if (keyLower == "torque") params.torque = value;
    else if (keyLower == "temperature") params.temperature = value;
    else if (keyLower == "angle") params.angle = value;
    else if (keyLower == "id") params.id = value;
    else if (keyLower == "iq") params.iq = value;
    else if (keyLower == "dcbus" || keyLower == "dc_bus") params.dcBus = value;
    else if (keyLower == "motortemp" || keyLower == "motor_temp") params.motorTemp = value;
    else if (keyLower == "drivertemp" || keyLower == "driver_temp") params.driverTemp = value;
    else if (keyLower == "pwmduty" || keyLower == "pwm_duty") params.pwmDuty = value;
    else if (keyLower == "vd") params.vd = value;
    else if (keyLower == "vq") params.vq = value;
    else if (keyLower == "ia" || keyLower == "ialpha") params.ia = value;
    else if (keyLower == "ib" || keyLower == "ibeta") params.ib = value;
    else if (keyLower == "theta") params.theta = value;
    else if (keyLower == "omega") params.omega = value;
    else if (keyLower == "encoderangle" || keyLower == "encoder_angle") params.encoderAngle = value;
}

