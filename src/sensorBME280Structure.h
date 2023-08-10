
#ifndef SENSORBME280STRUCTURE_H
#define SENSORBME280STRUCTURE_H

struct SensorBMe280Structure
{
    char temperature[10] = {'\0'};
    char humidity[10] = {'\0'};
    char pressure[10] = {'\0'};
    char altitudeMeter[10] = {'\0'};
    char altitudeFeet[10] = {'\0'};
};

#endif
