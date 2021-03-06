//
//  NearFrustumRenderer.hpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 24/03/2017.
//
//

#ifndef NearFrustumRenderer_hpp
#define NearFrustumRenderer_hpp

#include "Renderer.hpp"

class Camera;


class NearFrustumRenderer : public Renderer {

public:

  virtual void render(const G3MRenderContext* rc,
                      GLState* glState) = 0;

  virtual void render(Camera* currentCamera,
                      const G3MRenderContext* rc,
                      GLState* glState) = 0;

  SurfaceElevationProvider* getSurfaceElevationProvider();

  PlanetRenderer* getPlanetRenderer();

  bool isPlanetRenderer();

};

#endif
