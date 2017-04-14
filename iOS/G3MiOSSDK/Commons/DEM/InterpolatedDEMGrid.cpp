//
//  InterpolatedDEMGrid.cpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 10/04/2017.
//
//

#include "InterpolatedDEMGrid.hpp"
#include "Projection.hpp"
#include "Sector.hpp"
#include "ILogger.hpp"
#include "IMathUtils.hpp"
#include "Vector2D.hpp"


InterpolatedDEMGrid::InterpolatedDEMGrid(DEMGrid* grid,
                                         const Sector& sector,
                                         const Vector2I& extent,
                                         const Projection* projection,
                                         const Interpolator* interpolator):
DecoratorDEMGrid(grid->getBaseData(),sector,extent),
_interpolator(interpolator),
_projection(projection)
{
  if (!_grid->getSector().fullContains(sector)){
    ILogger::instance()->logInfo("Creating InterpolatedDEMGrid with incomplete data. Base Grid: %s. Interpolated Grid: %s", grid->getBaseData()->getSector().description().c_str(), sector.description().c_str());
  }
}

InterpolatedDEMGrid::~InterpolatedDEMGrid(){
  delete _interpolator;
}

double InterpolatedDEMGrid::getElevation(int x, int y) const{
  
  //Calculating position in Interpolated
  Geodetic2D p = getInnerPoint(x, y);
  
  if (!_grid->getSector().contains(p)){
    ILogger::instance()->logInfo("Requesting point outside of InterpolatedDEMGrid.");
    return NAND;
  }
  
  const Vector2D uv = _grid->getProjection()->getUV(_grid->getSector(), p);
  const Vector2I ge = _grid->getExtent();
  const double gridX = uv._x * (ge._x-1);
  const double gridY = (1.0 - uv._y) * (ge._y-1);
  
  const int fx = (int)IMathUtils::instance()->floor(gridX);
  const int fy = (int)IMathUtils::instance()->floor(gridY);
  
  const double rx = gridX - (double)fx;
  const double ry = gridY - (double)fy;
  
  if (rx == 0.0 && ry == 0.0){
    return _grid->getElevation((int) uv._x, (int) uv._y);
  }
  
  const int fx1 = fx == ge._x-1? fx: fx+1;
  const int fy1 = fy == ge._y-1? fy: fy+1;
  
  const double valueNW = _grid->getElevation(fx, fy);
  const double valueSW = _grid->getElevation(fx, fy1);
  const double valueNE = _grid->getElevation(fx1, fy);
  const double valueSE = _grid->getElevation(fx1, fy1);
  
  double h = _interpolator->interpolation(valueSW, valueSE, valueNE, valueNW, rx, ry);
  
  //    printf("%s -> %d, %d -> %s (%f)\n", p.description().c_str(), fx, fy,
  //           _grid->getInnerPoint(fx, fy).description().c_str(), h);
  
  return h;
}

