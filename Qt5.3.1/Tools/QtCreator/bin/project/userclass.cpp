#include "userclass.h"

UserClass::UserClass()
{
}

UserClass::UserClass(unsigned int Timestamp, int Level, QString Name)
{
    timestamp = Timestamp;
    level = Level;
    name = Name;
}
