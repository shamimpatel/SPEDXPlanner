#ifndef VECTORHELPERS_H
#define VECTORHELPERS_H


#include <QVector>
#include <iostream>
#include <cmath>

#ifndef PI
#define PI (3.14159265)
#endif


double Deg2Rad( double Degrees )
{
    return Degrees * (PI/180.0);
}

double Rad2Deg( double Radians )
{
    return Radians * (180.0/PI);
}


void PrintVector(QVector3D V, bool bNewline = true)
{
    std::cout << V.x() << " " << V.y() << " " << V.z();
    if( bNewline )
    {
        std::cout << std::endl;
    }
}

QVector3D CreateSphericalVector( double r, double theta_Radians, double phi_Radians)
{
    return QVector3D( r*sin(theta_Radians)*cos(phi_Radians),
                      r*sin(theta_Radians)*sin(phi_Radians),
                      r*cos(theta_Radians) );
}


#endif // VECTORHELPERS_H
