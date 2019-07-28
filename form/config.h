//#ifndef ONEFACEPASS_CONFIG
//#define ONEFACEPASS_CONFIG
#pragma once

#include <QSettings>

namespace Config {
    extern QSettings* config;

    inline void SetConfigFile(const QString &file) {
        if (!config) {
            config = new QSettings(file,  QSettings::IniFormat);
        }

    }

    inline QString GetPreloadPath() {
        return config->value("FaceDetect/preload").toString();
    }

    inline void checkConfig()
    {
        if (config->allKeys().size() == 0) {
            qDebug() << "配置文件初始化失败！";
        } else if (!config->contains("FaceDetect/preload")) {
            qDebug() << "配置参数preload错误！";
        }
#ifdef DEBUG_CONFIG
        qDebug() << "[CONFIG]";
        foreach (auto key, config->allKeys()) {
            qDebug() << key << "=" << config->value(key).toString();
        }
#endif
    }
}

//#endif
