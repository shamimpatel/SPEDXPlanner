#include "crystallography.h"
#include "crystal.h"
#include "Utils.h"
#include <cmath>
#include <iostream>
#include <vector>
#include "spectrum.h"
#include "geometry.h"
using namespace std;

Crystallography::Crystallography()
{
    this->Target = NULL;
}

void Crystallography::SetTarget(Crystal *Target)
{
    this->Target = Target;
}

void Crystallography::SetSpectrum(Spectrum *BacklighterSpectrum)
{
    this->BacklighterSpectrum = BacklighterSpectrum;
}

void Crystallography::SetGeometry(Geometry *ExptGeometry)
{
    this->ExptGeometry = ExptGeometry;
}

bool isPlaneInList( LatticePlane* Plane, std::vector<LatticePlane> *List)
{
    for( int i=0; i < int(List->size()); i++ )
    {
        if( Plane->h == ((*List)[i]).h && Plane->k == ((*List)[i]).k && Plane->l == ((*List)[i]).l)
        {
            return true;
        }
    }

    return false;
}

bool Crystallography::isPlaneBCC( LatticePlane* Plane)
{
    if( (Plane->h + Plane->k + Plane->l)%2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Crystallography::isPlaneWithinEnergyRange(LatticePlane* Plane)
{
    if(!Target)
    {
        cout << "Error: No Target setup." << endl;
        return false;
    }

    float MinDiffractWavelength,MaxDiffractWavelength;
    GetWavelengthRangeForPlane(Plane, MinDiffractWavelength, MaxDiffractWavelength);

    float MinE = BacklighterSpectrum->MinE;
    float MaxE = BacklighterSpectrum->MaxE;

    float MinSourceWavelength = EnergyToWavelength(MaxE);
    float MaxSourceWavelength = EnergyToWavelength(MinE);

    if( MinSourceWavelength > MaxSourceWavelength)
    {
        float temp = MinSourceWavelength;
        MinSourceWavelength = MaxSourceWavelength;
        MaxSourceWavelength = temp;
    }
/*
    cout << "MinSource: " << MinSourceWavelength << endl;
    cout << "MaxSource: " << MaxSourceWavelength << endl;

    cout << "MinDiffract: " << MinDiffractWavelength << endl;
    cout << "MaxDiffract: " << MaxDiffractWavelength << endl;
*/

    //if neither diffracting wavelengths are within the range
    if( (MinDiffractWavelength < MinSourceWavelength || MinDiffractWavelength > MaxSourceWavelength) &&
        (MaxDiffractWavelength < MinSourceWavelength || MaxDiffractWavelength > MaxSourceWavelength)   )
    {
        //if the range of bragg angles/diffracting wavelengths is larger than the backlighter
        if(MinDiffractWavelength < MinSourceWavelength && MaxDiffractWavelength > MaxDiffractWavelength)
        {
            return true;
        }

        return false;
    }
    else
    {
        //if either wavelength is within the backlighter spectrum
        return true;
    }
}

void Crystallography::GetWavelengthRangeForPlane(LatticePlane *Plane, float &MinWavelength, float &MaxWavelength)
{
    int h,k,l;
    h = Plane->h;
    k = Plane->k;
    l = Plane->l;
    float d = Target->GetLatticeConstant()/sqrt(h*h + k*k + l*l);

    float MinBraggTheta = Deg2Rad( ExptGeometry->GetMinBraggAngle() );
    float MaxBraggTheta = Deg2Rad( ExptGeometry->GetMaxBraggAngle() );

    MinWavelength = 2.0f*d*sin(MinBraggTheta);
    MaxWavelength = 2.0f*d*sin(MaxBraggTheta);
}

void Crystallography::CalculateDiffractionSpectrum(QTextEdit* TempOutput)
{
    if(!Target)
    {
        cout << "Error: No Target setup." << endl;
        return;
    }

    float MinE = BacklighterSpectrum->MinE;
    float MaxE = BacklighterSpectrum->MaxE;

    float MaxBraggTheta = Deg2Rad( ExptGeometry->GetMaxBraggAngle() );
    float SinBraggTheta = sin(MaxBraggTheta);

    float MinWavelength = EnergyToWavelength(MaxE);
    float MaxWavelength = EnergyToWavelength(MinE);

    if( MinWavelength > MaxWavelength)
    {
        float temp = MinWavelength;
        MinWavelength = MaxWavelength;
        MaxWavelength = temp;
    }

    float fMaxIndex = (2.0f*Target->GetLatticeConstant()*SinBraggTheta)/(MinWavelength);
    int MaxIndex = int(fMaxIndex);
    cout << "Max Index: " << MaxIndex << endl;

    std::vector<LatticePlane> hklPlanes;
    LatticePlane P(0,0,0,0);

    for( int n1 = 0; n1 <= MaxIndex; n1++)
    {
        if(n1 != 0)
        {
            P = LatticePlane(0 ,0 ,n1, 6);
            if( !isPlaneInList(&P, &hklPlanes) && CheckSymmetry(&P) && isPlaneWithinEnergyRange(&P))
            {
                hklPlanes.push_back(P);
            }
            P = LatticePlane(0 ,n1,n1,12);
            if( !isPlaneInList(&P, &hklPlanes) && CheckSymmetry(&P) && isPlaneWithinEnergyRange(&P))
            {
                hklPlanes.push_back(P);
            }
            P = LatticePlane(n1,n1,n1, 8);
            if( !isPlaneInList(&P, &hklPlanes) && CheckSymmetry(&P) && isPlaneWithinEnergyRange(&P))
            {
                hklPlanes.push_back(P);
            }
        }

        for( int n2 = n1; n2 <= MaxIndex; n2++)
        {
            if( n1 != 0 && n2 !=0)
            {
                P = LatticePlane(0 ,n1,n2,24);
                if( !isPlaneInList(&P, &hklPlanes) && CheckSymmetry(&P) && isPlaneWithinEnergyRange(&P))
                {
                    hklPlanes.push_back(P);
                }
                P = LatticePlane(n1,n1,n2,24);
                if( !isPlaneInList(&P, &hklPlanes) && CheckSymmetry(&P) && isPlaneWithinEnergyRange(&P))
                {
                    hklPlanes.push_back(P);
                }
            }

            for( int n3 = n2; n3 <= MaxIndex; n3++)
            {
                if( n1 != 0 && n2 !=0 && n3 != 0)
                {
                    P = LatticePlane(n1,n2,n3,48);
                    if( !isPlaneInList(&P, &hklPlanes) && CheckSymmetry(&P) && isPlaneWithinEnergyRange(&P))
                    {
                        hklPlanes.push_back(P);
                    }
                }
            }
        }
    }


    TempOutput->clear();
    cout << "Valid Planes:" << endl;
    TempOutput->append("Valid Planes:");

    std::sort(hklPlanes.begin(), hklPlanes.end(), LatticePlane::LatticePlaneCompare);

    for( auto it = hklPlanes.begin(); it != hklPlanes.end(); ++it)
    {
        float MinWavelength, MaxWavelength, MinE, MaxE;
        LatticePlane P = *it;
        GetWavelengthRangeForPlane(&P, MinWavelength, MaxWavelength);
        MinE = EnergyToWavelength(MaxWavelength);
        MaxE = EnergyToWavelength(MinWavelength);
        QString OutputLine;
        OutputLine += QString::number(P.h); OutputLine += " ";
        OutputLine += QString::number(P.k); OutputLine += " ";
        OutputLine += QString::number(P.l); OutputLine += "\t";
        OutputLine += QString::number(MinE); OutputLine += " --> ";
        OutputLine += QString::number(MaxE);
        //cout << P.h << " " << P.k << " " << P.l << " " << MinE << " --> " << MaxE << endl;
        cout << OutputLine.toStdString(); cout << endl;
        TempOutput->append(OutputLine);
    }
}

bool Crystallography::CheckSymmetry(LatticePlane* P)
{
    if(!Target)
    {
        cout << "Error: No Target setup." << endl;
        return false;
    }

    Crystal::Symmetry CrystalSymmetry = Target->GetSymmetry();

    switch (CrystalSymmetry)
    case Crystal::Symmetry::CUBIC_BCC:
        return isPlaneBCC(P);

    return false;
}
