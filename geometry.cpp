#include "geometry.h"

Geometry::Geometry()
{
    MinBraggAngle = 40.5;
    MaxBraggAngle = 41.5;
}

float Geometry::GetMaxBraggAngle()
{
    return MaxBraggAngle;
}

float Geometry::GetMinBraggAngle()
{
    return MinBraggAngle;
}

void Geometry::OverrideMinBraggAngle(float Angle)
{
    MinBraggAngle = Angle;
}

void Geometry::OverrideMaxBraggAngle(float Angle)
{
    MaxBraggAngle = Angle;
}
