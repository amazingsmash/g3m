//
//  WebMercatorPyramidDEMProvider.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/19/16.
//
//

#ifndef WebMercatorPyramidDEMProvider_hpp
#define WebMercatorPyramidDEMProvider_hpp

#include "PyramidDEMProvider.hpp"

class WebMercatorPyramidDEMProvider : public PyramidDEMProvider {
protected:

  WebMercatorPyramidDEMProvider(const double deltaHeight,
                             const Vector2S& tileExtent);

  virtual ~WebMercatorPyramidDEMProvider();

public:

  PyramidNode* createNode(PyramidNode* parent,
                          const size_t childID);

};

#endif
