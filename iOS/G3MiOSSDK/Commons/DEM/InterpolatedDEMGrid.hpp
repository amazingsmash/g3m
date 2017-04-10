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
  const Interpolator* _interpolator;
  const Projection* _projection;
public:
  InterpolatedDEMGrid(DEMGrid* grid,
                      const Sector& sector,
                      const Vector2I& extent,
                      const Projection* projection,
                      const Interpolator* interpolator);
  
  ~InterpolatedDEMGrid();
  
  double getElevation(int x, int y) const;
  
  const Projection* getProjection() const{
    return _projection;
  }
};

#endif /* InterpolatedDEMGrid_hpp */
