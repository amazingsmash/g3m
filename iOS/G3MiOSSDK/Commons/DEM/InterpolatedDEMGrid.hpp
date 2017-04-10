//
//  InterpolatedDEMGrid.hpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 10/04/2017.
//
//

#ifndef InterpolatedDEMGrid_hpp
#define InterpolatedDEMGrid_hpp

#include <stdio.h>

#include "DecoratorDEMGrid.hpp"
#include "Interpolator.hpp"

class InterpolatedDEMGrid: public DecoratorDEMGrid{
  Interpolator* _interpolator;
public:
  InterpolatedDEMGrid(DEMGrid* grid,
                      const Sector& sector,
                      const Vector2I& extent,
                      Interpolator* interpolator);
  ~InterpolatedDEMGrid();
  
#warning this class could have its own projection
  double getElevation(int x, int y) const;
};

#endif /* InterpolatedDEMGrid_hpp */
