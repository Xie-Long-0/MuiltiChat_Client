#pragma once

#include <QString>

class EncryptionManager
{
public:
    static QString hashPassword(const QString &password);
};
