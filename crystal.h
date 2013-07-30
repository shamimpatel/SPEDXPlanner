#ifndef CRYSTAL_H
#define CRYSTAL_H

class Crystal
{
public:
    enum Symmetry
    {
        CUBIC_BCC,
        CUBIC_FCC,
        NONE
    };


public:
    Crystal();
    
public:
    float GetLatticeConstant();
    void  SetLatticeConstant(const float LatticeConstant);
    void  SetSymmetry( Symmetry CrystalSymmetry );
    Symmetry  GetSymmetry();

private:
    float LatticeConstant;
    Symmetry CrystalSymmetry;
};



#endif // CRYSTAL_H
