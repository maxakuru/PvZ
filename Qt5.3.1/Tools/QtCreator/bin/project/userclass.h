#ifndef USERCLASS_H
#define USERCLASS_H

#include <qstring.h>

class UserClass
{
public:
    UserClass();
    UserClass(unsigned int Timestamp, int Level, QString Name);
    QString name;
    unsigned int timestamp;
    int level;

};

#endif // USERCLASS_H
