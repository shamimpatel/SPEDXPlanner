#include "spectrum.h"


Spectrum::Spectrum()
{
    MinE = 3.0;
    MaxE = 9.0;
}

float Spectrum::GetMaxE()
{
    if(MaxE < MinE)
    {
        return MinE;
    }
    else
    {
        return MaxE;
    }
}

float Spectrum::GetMinE()
{
    return MinE;
}

void Spectrum::SetMinE(float MinE)
{
    if(MinE >= 0 && MaxE <= 100)
    {
        this->MinE = MinE;
    }
}

void Spectrum::SetMaxE(float MaxE)
{
    if(MaxE > 0 && MaxE <= 100)
    {
        this->MaxE = MaxE;
    }
}
