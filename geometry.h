#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector3D>

class Geometry
{
public:
    Geometry();

    float GetMaxBraggAngle();
    float GetMinBraggAngle();

    void OverrideMinBraggAngle(float Angle);
    void OverrideMaxBraggAngle(float Angle);

private:
    float MinBraggAngle;
    float MaxBraggAngle;

};

#endif // GEOMETRY_H
