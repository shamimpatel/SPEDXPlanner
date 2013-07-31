#ifndef SPECTRUM_H
#define SPECTRUM_H

class Spectrum
{
public:
    Spectrum();
    float GetMaxE();
    float GetMinE();
    void SetMinE(float MinE);
    void SetMaxE(float MaxE);
private:
    float MinE;
    float MaxE;
};

#endif // SPECTRUM_H
