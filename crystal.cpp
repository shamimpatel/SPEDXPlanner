#include "crystal.h"


Crystal::Crystal()
{
    LatticeConstant = 3.31f;
    CrystalSymmetry = Symmetry::CUBIC_BCC;
}

float Crystal::GetLatticeConstant()
{
    return LatticeConstant;
}

void  Crystal::SetLatticeConstant(const float LatticeConstant)
{
    this->LatticeConstant = LatticeConstant;
}

void Crystal::SetSymmetry(Crystal::Symmetry CrystalSymmetry)
{
    this->CrystalSymmetry = CrystalSymmetry;
}

Crystal::Symmetry Crystal::GetSymmetry()
{
    return CrystalSymmetry;
}
