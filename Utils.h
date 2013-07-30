#ifndef UTILS_H
#define UTILS_H


#ifndef PI
#define PI (3.14159265f)
#endif



float Deg2Rad( float Degrees )
{
    return Degrees * (PI/180.0f);
}

float Rad2Deg( float Radians )
{
    return Radians * (180.0f/PI);
}


float EnergyToWavelength( float Energy )
{
  //Energy in keV. Wavelength in Angstroms
  return 12.39842f/Energy;
}


float WavelengthToEnergy( float Wavelength )
{
  return 12.39842f/Wavelength;
}


#endif // UTILS_H
