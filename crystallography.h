#ifndef CRYSTALLOGRAPHY_H
#define CRYSTALLOGRAPHY_H
#include <QTextEdit>

class Crystal;
class Backlighter;
class Detector;
struct Spectrum;
class Geometry;

struct LatticePlane
{
    LatticePlane(int h, int k, int l, int M)
    {
        this->h = h;
        this->k = k;
        this->l = l;
        this->M = M;
    }

    int h,k,l,M;

    static bool LatticePlaneCompare(const LatticePlane &P1, const LatticePlane &P2)
    {
        float hkl1 = P1.h*P1.h + P1.k*P1.k + P1.l*P1.l;
        float hkl2 = P2.h*P2.h + P2.k*P2.k + P2.l*P2.l;
        return hkl1 < hkl2;
    }

};

class Crystallography
{
public:
    Crystallography();

    void SetTarget(Crystal* Target);
    void SetSpectrum(Spectrum* BacklighterSpectrum);
    void SetGeometry(Geometry* ExptGeometry);

    void CalculateDiffractionSpectrum(QTextEdit* TempOutput);
    bool CheckSymmetry(LatticePlane *P);
    bool isPlaneBCC( LatticePlane* Plane);

private:
    bool isPlaneWithinEnergyRange(LatticePlane* Plane);
    void GetWavelengthRangeForPlane(LatticePlane *Plane, float &MinWavelength, float &MaxWavelength);

private:
    Crystal* Target;
    Spectrum* BacklighterSpectrum;
    Geometry* ExptGeometry;
};

#endif // CRYSTALLOGRAPHY_H
