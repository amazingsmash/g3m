//
//  TextureMapping.hpp
//  G3MiOSSDK
//
//  Created by José Miguel S N on 12/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef G3MiOSSDK_TextureMapping_hpp
#define G3MiOSSDK_TextureMapping_hpp

//#include <vector>
#include "MutableVector2D.hpp"

class IGLTextureId;

class G3MRenderContext;
class IFloatBuffer;
class GLState;
class GPUProgramState;

class TextureMapping {
public:
  
  virtual ~TextureMapping() {
  }
  
  /**
   Returns a new GLState and changes the current program state
   */
  virtual GLState* bind(const G3MRenderContext* rc, const GLState& parentState, GPUProgramState& progState) const = 0;

  virtual bool isTransparent(const G3MRenderContext* rc) const = 0;
  
  virtual void modifyGLState(GLState* glState) const = 0;
  virtual void modifyGPUProgramState(GPUProgramState* progState) const= 0;
};



class SimpleTextureMapping : public TextureMapping {
private:
  const IGLTextureId* _glTextureId;
  
  IFloatBuffer* _texCoords;
  const bool    _ownedTexCoords;

  MutableVector2D _translation;
  MutableVector2D _scale;

  const bool _isTransparent;
  
public:
  
  SimpleTextureMapping(const IGLTextureId* glTextureId,
                       IFloatBuffer* texCoords,
                       bool ownedTexCoords,
                       bool isTransparent) :
  _glTextureId(glTextureId),
  _texCoords(texCoords),
  _translation(0, 0),
  _scale(1, 1),
  _ownedTexCoords(ownedTexCoords),
  _isTransparent(isTransparent)
  {
    
  }
  
  void setTranslationAndScale(const Vector2D& translation,
                              const Vector2D& scale) {
    _translation = translation.asMutableVector2D();
    _scale       = scale.asMutableVector2D();
  }
  
  virtual ~SimpleTextureMapping();
  
  const IGLTextureId* getGLTextureId() const {
    return _glTextureId;
  }

  IFloatBuffer* getTexCoords() const {
    return _texCoords;
  }
  
  GLState* bind(const G3MRenderContext* rc, const GLState& parentState, GPUProgramState& progState) const;

  bool isTransparent(const G3MRenderContext* rc) const {
    return _isTransparent;
  }
  
  void modifyGLState(GLState* glState) const;
  
  void modifyGPUProgramState(GPUProgramState* progState) const;

};
#endif
