//
//  BasicShadersGL2.hpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 15/11/13.
//
//

#ifndef G3MiOSSDK_BasicShadersGL2_h
#define G3MiOSSDK_BasicShadersGL2_h

#include "GPUProgramFactory.hpp"

class BasicShadersGL2: public GPUProgramFactory{

public:
  BasicShadersGL2(){
#ifdef C_CODE
    std::string emptyString = "";
#endif
#ifdef JAVA_CODE
    String emptyString = "";
#endif

    GPUProgramSources sourcesBillboard("Billboard",
 emptyString +  
"attribute vec2 aTextureCoord;\n" +
"uniform mat4 uModelview;\n" +
"uniform vec4 uBillboardPosition;\n" +
"uniform vec2 uBillboardAnchor; //Anchor in UV (texture-like) coordinates\n" +
"uniform vec2 uTextureExtent;\n" +
"uniform vec2 uViewPortExtent;\n" +
"varying vec2 TextureCoordOut;\n" +
"void main() {\n" +
"gl_Position = uModelview * uBillboardPosition;\n" +
"float fx = 2.0 * uTextureExtent.x / uViewPortExtent.x * gl_Position.w;\n" +
"float fy = 2.0 * uTextureExtent.y / uViewPortExtent.y * gl_Position.w;\n" +
"gl_Position.x += ((aTextureCoord.x - 0.5) - (uBillboardAnchor.x - 0.5)) * fx;\n" +
"gl_Position.y -= ((aTextureCoord.y - 0.5) - (uBillboardAnchor.y - 0.5)) * fy;\n" +
"TextureCoordOut = aTextureCoord;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"uniform sampler2D Sampler;\n" +
"void main() {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"}\n");
    this->add(sourcesBillboard);

    GPUProgramSources sourcesBillboard_TransformedTexCoor("Billboard_TransformedTexCoor",
 emptyString +  
"attribute vec2 aTextureCoord;\n" +
"uniform mat4 uModelview;\n" +
"uniform vec4 uBillboardPosition;\n" +
"uniform vec2 uBillboardAnchor; //Anchor in UV (texture-like) coordinates\n" +
"uniform vec2 uTextureExtent;\n" +
"uniform vec2 uViewPortExtent;\n" +
"uniform mediump vec2 uTranslationTexCoord;\n" +
"uniform mediump vec2 uScaleTexCoord;\n" +
"varying vec2 TextureCoordOut;\n" +
"void main() {\n" +
"gl_Position = uModelview * uBillboardPosition;\n" +
"float fx = 2.0 * uTextureExtent.x / uViewPortExtent.x * gl_Position.w;\n" +
"float fy = 2.0 * uTextureExtent.y / uViewPortExtent.y * gl_Position.w;\n" +
"gl_Position.x += ((aTextureCoord.x - 0.5) - (uBillboardAnchor.x - 0.5)) * fx;\n" +
"gl_Position.y -= ((aTextureCoord.y - 0.5) - (uBillboardAnchor.y - 0.5)) * fy;\n" +
"TextureCoordOut = (aTextureCoord * uScaleTexCoord) + uTranslationTexCoord;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"uniform sampler2D Sampler;\n" +
"void main() {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"}\n");
    this->add(sourcesBillboard_TransformedTexCoor);

    GPUProgramSources sourcesColorMesh("ColorMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec4 aColor;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"varying vec4 VertexColor;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"VertexColor = aColor;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec4 VertexColor;\n" +
"void main() {\n" +
"gl_FragColor = VertexColor;\n" +
"}\n");
    this->add(sourcesColorMesh);

    GPUProgramSources sourcesDefault("Default",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"attribute vec4 aColor;\n" +
"uniform mediump vec2 uTranslationTexCoord;\n" +
"uniform mediump vec2 uScaleTexCoord;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"varying vec4 VertexColor;\n" +
"varying vec2 TextureCoordOut;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"TextureCoordOut = (aTextureCoord * uScaleTexCoord) + uTranslationTexCoord;\n" +
"VertexColor = aColor;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"varying mediump vec4 VertexColor;\n" +
"uniform sampler2D Sampler;\n" +
"uniform bool EnableTexture;\n" +
"uniform lowp vec4 uFlatColor;\n" +
"uniform bool EnableColorPerVertex;\n" +
"uniform bool EnableFlatColor;\n" +
"uniform mediump float FlatColorIntensity;\n" +
"uniform mediump float ColorPerVertexIntensity;\n" +
"void main() {\n" +
"if (EnableTexture) {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"if (EnableFlatColor || EnableColorPerVertex) {\n" +
"lowp vec4 color;\n" +
"if (EnableFlatColor) {\n" +
"color = uFlatColor;\n" +
"if (EnableColorPerVertex) {\n" +
"color = color * VertexColor;\n" +
"}\n" +
"}\n" +
"else {\n" +
"color = VertexColor;\n" +
"}\n" +
"lowp float intensity = (FlatColorIntensity + ColorPerVertexIntensity) / 2.0;\n" +
"gl_FragColor = mix(gl_FragColor,\n" +
"VertexColor,\n" +
"intensity);\n" +
"}\n" +
"}\n" +
"else {\n" +
"if (EnableColorPerVertex) {\n" +
"gl_FragColor = VertexColor;\n" +
"if (EnableFlatColor) {\n" +
"gl_FragColor = gl_FragColor * uFlatColor;\n" +
"}\n" +
"}\n" +
"else {\n" +
"gl_FragColor = uFlatColor;\n" +
"}\n" +
"}\n" +
"}\n");
    this->add(sourcesDefault);

    GPUProgramSources sourcesFlatColor2DMesh("FlatColor2DMesh",
 emptyString +  
"attribute vec2 aPosition2D;\n" +
"uniform float uPointSize;\n" +
"uniform vec2 uTranslation2D;\n" +
"uniform vec2 uViewPortExtent;\n" +
"void main() {\n" +
"vec2 pixel = aPosition2D;\n" +
"pixel.x -= uViewPortExtent.x / 2.0;\n" +
"pixel.y += uViewPortExtent.y / 2.0;\n" +
"gl_Position = vec4((pixel.x + uTranslation2D.x) / (uViewPortExtent.x / 2.0),\n" +
"(pixel.y - uTranslation2D.y) / (uViewPortExtent.y / 2.0),\n" +
"0, 1);\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"uniform lowp vec4 uFlatColor;\n" +
"void main() {\n" +
"gl_FragColor = uFlatColor;\n" +
"}\n");
    this->add(sourcesFlatColor2DMesh);

    GPUProgramSources sourcesFlatColorMesh("FlatColorMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"uniform lowp vec4 uFlatColor;\n" +
"void main() {\n" +
"gl_FragColor = uFlatColor;\n" +
"}\n");
    this->add(sourcesFlatColorMesh);

    GPUProgramSources sourcesFlatColorMesh_DirectionLight("FlatColorMesh_DirectionLight",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec3 aNormal;\n" +
"uniform mat4 uModelview;\n" +
"uniform mat4 uModel;\n" +
"uniform float uPointSize;\n" +
"uniform vec3 uAmbientLightColor;\n" +
"uniform vec3 uDiffuseLightColor;\n" +
"uniform vec3 uDiffuseLightDirection; //We must normalize\n" +
"varying vec3 lightColor;\n" +
"void main() {\n" +
"vec3 normalInModel = normalize( vec3(uModel * vec4(aNormal, 0.0) ));\n" +
"vec3 lightDirNormalized = normalize( uDiffuseLightDirection );\n" +
"float diffuseLightIntensity = max(dot(normalInModel, lightDirNormalized), 0.0);\n" +
"gl_Position = uModelview * aPosition;\n" +
"gl_PointSize = uPointSize;\n" +
"lightColor = uAmbientLightColor + uDiffuseLightColor * diffuseLightIntensity;\n" +
"lightColor.x = min(lightColor.x, 1.0);\n" +
"lightColor.y = min(lightColor.y, 1.0);\n" +
"lightColor.z = min(lightColor.z, 1.0);\n" +
"}\n",
 emptyString +  
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n" +
"precision highp float;\n" +
"#else\n" +
"precision mediump float;\n" +
"#endif\n" +
"uniform lowp vec4 uFlatColor;\n" +
"varying vec3 lightColor;\n" +
"void main() {\n" +
"gl_FragColor.r = uFlatColor.r * lightColor.r;\n" +
"gl_FragColor.g = uFlatColor.g * lightColor.r;\n" +
"gl_FragColor.b = uFlatColor.b * lightColor.r;\n" +
"gl_FragColor.a = uFlatColor.a;\n" +
"}\n");
    this->add(sourcesFlatColorMesh_DirectionLight);

    GPUProgramSources sourcesFullTransformedTexCoorMultiTexturedMesh("FullTransformedTexCoorMultiTexturedMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"attribute vec2 aTextureCoord2;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"varying vec2 TextureCoordOut;\n" +
"varying vec2 TextureCoordOut2;\n" +
"uniform mediump vec2 uTranslationTexCoord;\n" +
"uniform mediump vec2 uScaleTexCoord;\n" +
"uniform float uRotationAngleTexCoord;\n" +
"uniform vec2 uRotationCenterTexCoord;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"float s = sin( uRotationAngleTexCoord );\n" +
"float c = cos( uRotationAngleTexCoord );\n" +
"TextureCoordOut = (aTextureCoord * uScaleTexCoord) + uTranslationTexCoord;\n" +
"TextureCoordOut = TextureCoordOut - uRotationCenterTexCoord;\n" +
"TextureCoordOut = vec2((TextureCoordOut.x * c) + (TextureCoordOut.y * s),\n" +
"(-TextureCoordOut.x * s) + (TextureCoordOut.y * c));\n" +
"TextureCoordOut += uRotationCenterTexCoord;\n" +
"TextureCoordOut2 = aTextureCoord2;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"varying mediump vec2 TextureCoordOut2;\n" +
"uniform sampler2D Sampler;\n" +
"uniform sampler2D Sampler2;\n" +
"void main() {\n" +
"mediump vec4 tex1 = texture2D(Sampler, TextureCoordOut);\n" +
"mediump vec4 tex2 = texture2D(Sampler2, TextureCoordOut2);\n" +
"gl_FragColor = tex1 * tex2;\n" +
"}\n");
    this->add(sourcesFullTransformedTexCoorMultiTexturedMesh);

    GPUProgramSources sourcesFullTransformedTexCoorTexturedMesh("FullTransformedTexCoorTexturedMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"uniform mediump vec2 uTranslationTexCoord;\n" +
"uniform mediump vec2 uScaleTexCoord;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"uniform float uRotationAngleTexCoord;\n" +
"uniform vec2 uRotationCenterTexCoord;\n" +
"varying vec4 VertexColor;\n" +
"varying vec2 TextureCoordOut;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"float s = sin( uRotationAngleTexCoord );\n" +
"float c = cos( uRotationAngleTexCoord );\n" +
"TextureCoordOut = (aTextureCoord * uScaleTexCoord) + uTranslationTexCoord;\n" +
"TextureCoordOut = TextureCoordOut - uRotationCenterTexCoord;\n" +
"TextureCoordOut = vec2((TextureCoordOut.x * c) + (TextureCoordOut.y * s),\n" +
"(-TextureCoordOut.x * s) + (TextureCoordOut.y * c));\n" +
"TextureCoordOut += uRotationCenterTexCoord;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"varying mediump vec4 VertexColor;\n" +
"uniform sampler2D Sampler;\n" +
"void main() {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"}\n");
    this->add(sourcesFullTransformedTexCoorTexturedMesh);

    GPUProgramSources sourcesMultiTexturedMesh("MultiTexturedMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"attribute vec2 aTextureCoord2;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"varying vec2 TextureCoordOut;\n" +
"varying vec2 TextureCoordOut2;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"TextureCoordOut = aTextureCoord;\n" +
"TextureCoordOut2 = aTextureCoord2;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"varying mediump vec2 TextureCoordOut2;\n" +
"uniform sampler2D Sampler;\n" +
"uniform sampler2D Sampler2;\n" +
"void main() {\n" +
"mediump vec4 tex1 = texture2D(Sampler, TextureCoordOut);\n" +
"mediump vec4 tex2 = texture2D(Sampler2, TextureCoordOut2);\n" +
"gl_FragColor = tex1 * tex2;\n" +
"}\n");
    this->add(sourcesMultiTexturedMesh);

    GPUProgramSources sourcesNoColorMesh("NoColorMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"void main() {\n" +
"gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); //RED\n" +
"}\n");
    this->add(sourcesNoColorMesh);

    GPUProgramSources sourcesShader("Shader",
 emptyString +  
"attribute vec4 Position;\n" +
"attribute vec2 TextureCoord;\n" +
"attribute vec4 Color;\n" +
"uniform mediump vec2 TranslationTexCoord;\n" +
"uniform mediump vec2 ScaleTexCoord;\n" +
"uniform mat4 Projection;\n" +
"uniform mat4 Modelview;\n" +
"uniform bool BillBoard;\n" +
"uniform vec2 TextureExtent;\n" +
"uniform vec2 ViewPortExtent;\n" +
"uniform float PointSize;\n" +
"varying vec4 VertexColor;\n" +
"varying vec2 TextureCoordOut;\n" +
"void main() {\n" +
"gl_Position = Projection * Modelview * Position;\n" +
"if (BillBoard) {\n" +
"gl_Position.x += ((TextureCoord.x - 0.5) * 2.0 * TextureExtent.x / ViewPortExtent.x) * gl_Position.w;\n" +
"gl_Position.y -= ((TextureCoord.y - 0.5) * 2.0 * TextureExtent.y / ViewPortExtent.y) * gl_Position.w;\n" +
"}\n" +
"TextureCoordOut = (TextureCoord * ScaleTexCoord) + TranslationTexCoord;\n" +
"VertexColor = Color;\n" +
"gl_PointSize = PointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"varying mediump vec4 VertexColor;\n" +
"uniform sampler2D Sampler;\n" +
"uniform bool EnableTexture;\n" +
"uniform lowp vec4 FlatColor;\n" +
"uniform bool EnableColorPerVertex;\n" +
"uniform bool EnableFlatColor;\n" +
"uniform mediump float FlatColorIntensity;\n" +
"uniform mediump float ColorPerVertexIntensity;\n" +
"void main() {\n" +
"if (EnableTexture) {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"if (EnableFlatColor || EnableColorPerVertex) {\n" +
"lowp vec4 color;\n" +
"if (EnableFlatColor) {\n" +
"color = FlatColor;\n" +
"if (EnableColorPerVertex) {\n" +
"color = color * VertexColor;\n" +
"}\n" +
"}\n" +
"else {\n" +
"color = VertexColor;\n" +
"}\n" +
"lowp float intensity = (FlatColorIntensity + ColorPerVertexIntensity) / 2.0;\n" +
"gl_FragColor = mix(gl_FragColor,\n" +
"VertexColor,\n" +
"intensity);\n" +
"}\n" +
"}\n" +
"else {\n" +
"if (EnableColorPerVertex) {\n" +
"gl_FragColor = VertexColor;\n" +
"if (EnableFlatColor) {\n" +
"gl_FragColor = gl_FragColor * FlatColor;\n" +
"}\n" +
"}\n" +
"else {\n" +
"gl_FragColor = FlatColor;\n" +
"}\n" +
"}\n" +
"}\n");
    this->add(sourcesShader);

    GPUProgramSources sourcesSphericalAtmosphere("SphericalAtmosphere",
 emptyString +  
"attribute vec4 aPosition; //Position of ZNear Frame corners in world-space\n" +
"uniform mat4 uModelview; //Model + Projection\n" +
"uniform float uPointSize;\n" +
"uniform highp vec3 uCameraPosition;\n" +
"varying highp vec3 rayDirection;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"gl_Position.z = 0.0;\n" +
"gl_PointSize = uPointSize;\n" +
"highp vec3 planePos = aPosition.xyz;\n" +
"rayDirection = planePos - uCameraPosition;\n" +
"}\n",
 emptyString +  
"uniform highp vec3 uCameraPosition;\n" +
"varying highp vec3 rayDirection;\n" +
"void main() {\n" +
"gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n" +
"}\n");
    this->add(sourcesSphericalAtmosphere);

    GPUProgramSources sourcesSphericalAtmosphere2("SphericalAtmosphere2",
 emptyString +  
"attribute vec4 aPosition; //Position of ZNear Frame corners in world-space\n" +
"uniform mat4 uModelview; //Model + Projection\n" +
"uniform float uPointSize;\n" +
"uniform highp vec3 uCameraPosition;\n" +
"varying highp vec3 rayDirirection;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"gl_Position.z = 0.0;\n" +
"gl_PointSize = uPointSize;\n" +
"highp vec3 planePos = aPosition.xyz;\n" +
"rayDirirection = planePos - uCameraPosition;\n" +
"}\n",
 emptyString +  
"precision highp float;\n" +
"uniform highp vec3 uCameraPosition;\n" +
"varying highp vec3 rayDirirection;\n" +
"highp vec3 currentSunDir = vec3(1.0, 0.0, 0.0);\n" +
"const highp float earthRadius = 6.36744e6;\n" +
"const highp float atmUndergroundOffset = 100e3;\n" +
"const highp float atmosphereScale = 3.0; //Atm. scale\n" +
"const highp float stratoHeight = 50e3 * atmosphereScale;\n" +
"const highp float atmRadius = earthRadius + stratoHeight;\n" +
"const highp float earth2AtmRatio = earthRadius / atmRadius;\n" +
"const highp mat3 CIE2RGB = mat3(3.2405,   -0.9693,    0.0556,\n" +
"-1.5371,    1.8760,   -0.2040,\n" +
"-0.4985,    0.0416,    1.0572);\n" +
"const highp mat3 RGB2CIE = mat3(0.4125,    0.2127,    0.0193,\n" +
"0.3576,    0.7152,    0.1192,\n" +
"0.1804,    0.0722,    0.9503);\n" +
"const float NaN = sqrt(-1.0);\n" +
"highp mat3 rayTo2D(in highp vec3 pa, in  highp vec3 pb){\n" +
"highp mat3 m;\n" +
"if (abs(pa.z) + distance(pa.y, pb.y) < 1e-7){ //Checking\n" +
"m = mat3(1.,0.,0.,\n" +
"0.,1.,0.,\n" +
"0.,0.,1.);\n" +
"} else{\n" +
"highp vec3 d = normalize(pb - pa);\n" +
"highp vec3 c = normalize(pa + (dot(-pa,d)*d));\n" +
"m[0] = d;\n" +
"m[1] = c;\n" +
"m[2] = cross(c,d);\n" +
"m = mat3(m[0][0], m[1][0], m[2][0],\n" +
"m[0][1], m[1][1], m[2][1],\n" +
"m[0][2], m[1][2], m[2][2]);\n" +
"}\n" +
"return m;\n" +
"}\n" +
"vec2 earthShadow2D(highp float Y0, highp mat3 m, vec3 sunDir){\n" +
"sunDir = normalize(sunDir);\n" +
"vec3 p2 = normalize(m * sunDir); //Sun Pos\n" +
"highp float x2 = p2.x;\n" +
"highp float y2 = p2.y;\n" +
"highp float z2 = p2.z;\n" +
"highp float R = earth2AtmRatio;\n" +
"highp float s = -(pow(x2,2.0)*pow(Y0,2.0)*pow(z2,2.0)) +\n" +
"pow(R,2.0)*(pow(y2,2.0) + pow(z2,2.0)) -\n" +
"(pow(y2,2.0) + pow(z2,2.0))*pow(Y0*z2,2.0);\n" +
"if (s >= 0.0){\n" +
"highp float n = x2*Y0*y2;\n" +
"highp float d = (pow(y2,2.0) + pow(z2,2.0));\n" +
"s = sqrt(s);\n" +
"highp float s1 = (n - s)/ d;\n" +
"highp float s2 = (n + s)/ d;\n" +
"if (distance(vec2(s1,Y0), p2.xy) <  distance(vec2(s1,Y0), -p2.xy)){\n" +
"return vec2(s1,s2);\n" +
"}\n" +
"}\n" +
"return vec2(NaN, NaN);\n" +
"}\n" +
"highp vec4 getRay2DEnds(vec2 p1, vec2 p2,\n" +
"mat3 m, vec3 sunDir,\n" +
"out bool atm,\n" +
"out bool earth,\n" +
"out bool shadow){\n" +
"highp float atm1 = -sqrt(1.0 - p1.y*p1.y);\n" +
"highp float atm2 = -atm1;\n" +
"atm1 = clamp(atm1, p1.x, p2.x);\n" +
"atm2 = clamp(atm2, p1.x, p2.x);\n" +
"atm = (atm1 != atm2);\n" +
"highp float earth1 = -sqrt(earth2AtmRatio*earth2AtmRatio - p1.y*p1.y);\n" +
"highp float earth2 = -earth1;\n" +
"earth1 = clamp(earth1, p1.x, p2.x);\n" +
"earth2 = clamp(earth2, p1.x, p2.x);\n" +
"earth = (earth1 != earth2);\n" +
"highp vec2 shadows = earthShadow2D(p1.y, m, sunDir);\n" +
"highp float rayEnd = earth? earth1 : atm2;\n" +
"highp float shadow1 = clamp(shadows.x, atm1, rayEnd);\n" +
"highp float shadow2 = clamp(shadows.y, atm1, rayEnd);\n" +
"shadow = (shadows.x == shadows.x) && (shadow1 != shadow2);\n" +
"if (!atm){\n" +
"return vec4(0.,0.,0.,0.);\n" +
"}\n" +
"if (!earth && !shadow){\n" +
"return vec4(atm1, atm2, 0., 0.);\n" +
"}\n" +
"if (earth && !shadow){\n" +
"return vec4(atm1, earth1, 0., 0.);\n" +
"}\n" +
"if (earth && shadow){\n" +
"return vec4(atm1, shadow1, 0., 0.);\n" +
"}\n" +
"if (!earth && shadow){\n" +
"return vec4(atm1, shadow1, shadow2, atm2);\n" +
"}\n" +
"}\n" +
"const int nSamplesPrimaryRay = 21;\n" +
"highp float sampledXFor21Samples(float x0, float x1, int n){\n" +
"if (x0*x1 < 0.0){\n" +
"if (n == 0) return x0 * 1.000000;\n" +
"if (n == 1) return x0 * 0.899549;\n" +
"if (n == 2) return x0 * 0.799199;\n" +
"if (n == 3) return x0 * 0.698949;\n" +
"if (n == 4) return x0 * 0.598800;\n" +
"if (n == 5) return x0 * 0.498750;\n" +
"if (n == 6) return x0 * 0.398800;\n" +
"if (n == 7) return x0 * 0.298951;\n" +
"if (n == 8) return x0 * 0.199201;\n" +
"if (n == 9) return x0 * 0.099551;\n" +
"if (n == 10) return 0.0;\n" +
"if (n == 0) return x1 * 0.099551;\n" +
"if (n == 1) return x1 * 0.199201;\n" +
"if (n == 2) return x1 * 0.298951;\n" +
"if (n == 3) return x1 * 0.398800;\n" +
"if (n == 4) return x1 * 0.498750;\n" +
"if (n == 5) return x1 * 0.598800;\n" +
"if (n == 6) return x1 * 0.698949;\n" +
"if (n == 7) return x1 * 0.799199;\n" +
"if (n == 8) return x1 * 0.899549;\n" +
"if (n == 9) return x1 * 1.000000;\n" +
"}\n" +
"highp float d = x1-x0;\n" +
"if (n == 0) return x0 + d * 0.000000;\n" +
"if (n == 1) return x0 + d * 0.184651;\n" +
"if (n == 2) return x0 + d * 0.335831;\n" +
"if (n == 3) return x0 + d * 0.459606;\n" +
"if (n == 4) return x0 + d * 0.560945;\n" +
"if (n == 5) return x0 + d * 0.643914;\n" +
"if (n == 6) return x0 + d * 0.711844;\n" +
"if (n == 7) return x0 + d * 0.767460;\n" +
"if (n == 8) return x0 + d * 0.812994;\n" +
"if (n == 9) return x0 + d * 0.850274;\n" +
"if (n == 10) return x0 + d * 0.880797;\n" +
"if (n == 11) return x0 + d * 0.905787;\n" +
"if (n == 12) return x0 + d * 0.926247;\n" +
"if (n == 13) return x0 + d * 0.942998;\n" +
"if (n == 14) return x0 + d * 0.956713;\n" +
"if (n == 15) return x0 + d * 0.967941;\n" +
"if (n == 16) return x0 + d * 0.977135;\n" +
"if (n == 17) return x0 + d * 0.984661;\n" +
"if (n == 18) return x0 + d * 0.990824;\n" +
"if (n == 19) return x0 + d * 0.995869;\n" +
"if (n == 20) return x0 + d * 1.000000;\n" +
"}\n" +
"float gFun(highp float a, highp float r){\n" +
"if (a > 1.6) return 4.2214e-04; //Mean value beyond a = 1.6\n" +
"r = (r - earth2AtmRatio) / (1.0 - earth2AtmRatio); //Normalizing [0..1]\n" +
"highp float x = r;\n" +
"highp float y = a;\n" +
"highp float x2 = x*x;\n" +
"highp float y2 = y*y;\n" +
"highp float x3 = x*x*x;\n" +
"highp float y3 = y*y*y;\n" +
"highp float x4 = x*x*x*x;\n" +
"highp float y4 = y*y*y*y;\n" +
"highp float x5 = x*x*x*x*x;\n" +
"highp float y5 = y*y*y*y*y;\n" +
"highp float g = 1787776115.6965 + -51039518.5128*x + -5614573368.3089*y +\n" +
"865443.705*x2 + 128015133.8374*x*y + 7053069042.9223*y2 +\n" +
"-10398.8322*x3 + -1624280.2711*x2*y + -120405648.8163*x*y2 +\n" +
"-4430036061.6336*y3 + 93.8694*x4 + 12958.7149*x3*y + 1016173.9827*x2*y2 +\n" +
"50332603.1065*x*y3 + 1391246665.5755*y4 + -0.64185*x5 + -57.8523*x4*y +\n" +
"-4037.6155*x3*y2 + -211914.0726*x2*y3 + -7890102.7191*x*y4 + -174766723.1861*y5;\n" +
"g = clamp(g, 0.0, 1.0);\n" +
"return g;\n" +
"}\n" +
"float getOpticalDepthFromInfinity(vec2 p){\n" +
"highp float a = abs(atan(p.y, p.x)); // Atan->[\u2212Pi,Pi]\n" +
"highp float r = length(p);\n" +
"highp float f;\n" +
"if (a > 0.0 && a < 1.57){ //0 < Pi/2\n" +
"f = 2.0 * gFun(1.5708, abs(p.y)) - gFun(3.1416 - a, r);\n" +
"f = clamp(f, 0.0, 2.0);\n" +
"} else{\n" +
"f = gFun(a, r);\n" +
"}\n" +
"return f;\n" +
"}\n" +
"highp float getSolarOutScattering(vec3 p, vec3 sunDir){\n" +
"highp vec3 pc = p - normalize(sunDir);\n" +
"highp mat3 m = rayTo2D(pc, p);\n" +
"pc = m * pc;\n" +
"p = m * p;\n" +
"if (pc.x * p.x < 0.0  && pc.y < earth2AtmRatio){ //Earth shadow\n" +
"return -1.0; //Infinity\n" +
"} else{\n" +
"return getOpticalDepthFromInfinity(p.xy);\n" +
"}\n" +
"}\n" +
"highp float getInnerOutScattering(float x0, float x1, float y0){\n" +
"highp float i = getOpticalDepthFromInfinity(vec2(x0, y0));\n" +
"highp float f = getOpticalDepthFromInfinity(vec2(x1, y0));\n" +
"return (f - i);\n" +
"}\n" +
"highp float getDensityCoefficient(float x, float y){\n" +
"highp vec2 p = vec2(x, y);\n" +
"highp float h = (length(p) - earth2AtmRatio) / ((atmRadius - earthRadius) / atmRadius);\n" +
"highp float d = exp(-h*6.2547); //Density Coefficient\n" +
"return d;\n" +
"}\n" +
"highp float Xs[nSamplesPrimaryRay];\n" +
"highp float scatteringFactors[nSamplesPrimaryRay];\n" +
"highp float outScatteringFactors[nSamplesPrimaryRay];\n" +
"void calculateScatteringFactorsAndXs(vec3 sunDir,\n" +
"vec3 sp1,\n" +
"vec3 sp2,\n" +
"highp float x0,\n" +
"highp float x1,\n" +
"highp float y0){\n" +
"highp vec3 ray3DDir = normalize(sp2-sp1);\n" +
"for (int i = 0; i < nSamplesPrimaryRay; i++){\n" +
"Xs[i] = sampledXFor21Samples(x0, x1, i);\n" +
"highp vec3 p = (Xs[i] - x0) * ray3DDir + sp1;\n" +
"highp float so = getSolarOutScattering(p ,sunDir);\n" +
"if (so < 0.0){\n" +
"outScatteringFactors[i] = -1.0;\n" +
"} else{\n" +
"highp float io = getInnerOutScattering(x0, Xs[i], y0);\n" +
"outScatteringFactors[i] = (so + io);\n" +
"}\n" +
"scatteringFactors[i] = getDensityCoefficient(Xs[i], y0);\n" +
"}\n" +
"}\n" +
"float getAtmWavelengthIntensity(highp float gScale4PiKw4){\n" +
"highp float intensity = 0.0;\n" +
"highp float d_1 = scatteringFactors[0] * exp(- gScale4PiKw4 * outScatteringFactors[0]);\n" +
"for (int i = 1; i < nSamplesPrimaryRay; i++){\n" +
"highp float d = 0.0;\n" +
"if (outScatteringFactors[i] >= 0.0){\n" +
"d = scatteringFactors[i] * exp(- gScale4PiKw4 * outScatteringFactors[i]);\n" +
"}\n" +
"intensity += ((d + d_1)/2.0) * (Xs[i] - Xs[i-1]); //Trapezoidal integration\n" +
"d_1 = d; //For next iteration\n" +
"}\n" +
"return intensity;\n" +
"}\n" +
"void main() {\n" +
"highp vec3 rayDir = normalize(rayDirirection);\n" +
"highp vec3 p1 = uCameraPosition/atmRadius;\n" +
"highp vec3 p2 = (uCameraPosition + rayDir*1e10)/atmRadius;\n" +
"highp mat3 m = rayTo2D(p1, p2);\n" +
"highp vec2 mp1 = (m * p1).xy;\n" +
"highp vec2 mp2 = (m * p2).xy;\n" +
"if (mp1.x > mp2.x){\n" +
"highp vec2 aux = mp1;\n" +
"mp1 = mp2;\n" +
"mp2 = aux;\n" +
"highp vec3 aux3 = p1;\n" +
"p1 = p2;\n" +
"p2 = aux3;\n" +
"}\n" +
"bool atm;\n" +
"bool earth;\n" +
"bool shadow;\n" +
"highp vec4 ends = getRay2DEnds(mp1, mp2,\n" +
"m, currentSunDir,\n" +
"atm,\n" +
"earth,\n" +
"shadow);\n" +
"highp vec3 bgColor = vec3(0.,0.,0.);\n" +
"if (atm){\n" +
"const int nW = 9;\n" +
"highp vec3 CIElevels[9];\n" +
"CIElevels[0] = vec3(4.568698e-02, 2.536497e-04, 2.536274e-01); //w = 380.00 - 424.44\n" +
"CIElevels[1] = vec3(1.904479e-01, 1.411401e-02, 1.000000e+00); //w = 424.44 - 468.89\n" +
"CIElevels[2] = vec3(2.882708e-02, 1.250434e-01, 2.311921e-01); //w = 468.89 - 513.33\n" +
"CIElevels[3] = vec3(7.525619e-02, 2.688203e-01, 4.168597e-03); //w = 513.33 - 557.78\n" +
"CIElevels[4] = vec3(1.968837e-01, 1.856631e-01, 8.178416e-06); //w = 557.78 - 602.22\n" +
"CIElevels[5] = vec3(1.115830e-01, 5.053781e-02, 2.669158e-09); //w = 602.22 - 646.67\n" +
"CIElevels[6] = vec3(1.355325e-02, 6.385989e-03, 2.110279e-13); //w = 646.67 - 691.11\n" +
"CIElevels[7] = vec3(3.337662e-04, 4.290635e-04, 5.467493e-18); //w = 691.11 - 735.56\n" +
"CIElevels[8] = vec3(1.571319e-06, 1.715437e-05, 5.889087e-23); //w = 735.56 - 780.00\n" +
"highp float I0CIEKw4Scale = 3.654184e+00;\n" +
"highp float gScale4PiKw4[9];\n" +
"gScale4PiKw4[0] = 333.162685;\n" +
"gScale4PiKw4[1] = 219.071773;\n" +
"gScale4PiKw4[2] = 149.899974;\n" +
"gScale4PiKw4[3] = 105.999174;\n" +
"gScale4PiKw4[4] = 77.056384;\n" +
"gScale4PiKw4[5] = 57.351578;\n" +
"gScale4PiKw4[6] = 43.561685;\n" +
"gScale4PiKw4[7] = 33.678328;\n" +
"gScale4PiKw4[8] = 26.445582;\n" +
"highp vec3 atmCIE = vec3(.0,.0,.0);\n" +
"highp vec3 ini, end;\n" +
"if (ends.x - ends.y > 1e-2){\n" +
"ini = p1 + ((ends.x - mp1.x) / (mp2.x - mp1.x)) * (p2-p1);\n" +
"end = p1 + ((ends.y - mp1.x) / (mp2.x - mp1.x)) * (p2-p1);\n" +
"calculateScatteringFactorsAndXs(currentSunDir,\n" +
"ini, end,\n" +
"ends.x, ends.y, mp1.y);\n" +
"for (int i = 0; i < nW; i++){\n" +
"highp float intensity = getAtmWavelengthIntensity(gScale4PiKw4[i]); //gScale * 4 * Pi * K / w^-4\n" +
"atmCIE += CIElevels[i] * intensity;\n" +
"}\n" +
"}\n" +
"if (ends.w - ends.z > 1e-2){\n" +
"ini = p1 + ((ends.z - mp1.x) / (mp2.x - mp1.x)) * (p2-p1);\n" +
"end = p1 + ((ends.w - mp1.x) / (mp2.x - mp1.x)) * (p2-p1);\n" +
"calculateScatteringFactorsAndXs(currentSunDir,\n" +
"ini, end,\n" +
"ends.z, ends.w, mp1.y);\n" +
"for (int i = 0; i < nW; i++){\n" +
"highp float intensity = getAtmWavelengthIntensity(gScale4PiKw4[i]); //gScale * 4 * Pi * K / w^-4\n" +
"atmCIE += CIElevels[i] * intensity;\n" +
"}\n" +
"}\n" +
"highp vec3 rayDir = p2 - p1;\n" +
"highp float cosAngle = dot(normalize(currentSunDir), normalize(rayDir));\n" +
"highp float fr = (3.0/4.0)* (1.0+pow(cosAngle,2.0));\n" +
"atmCIE *= I0CIEKw4Scale * atmRadius * fr;\n" +
"highp float dimmingFactor = 1.0;// 1e-6;\n" +
"atmCIE *= dimmingFactor;\n" +
"highp vec3 atmColor = CIE2RGB * atmCIE;\n" +
"gl_FragColor = vec4(atmColor, 1.0);\n" +
"} else{\n" +
"gl_FragColor = vec4(1.,1.,0.,1.);\n" +
"}\n" +
"}\n");
    this->add(sourcesSphericalAtmosphere2);

    GPUProgramSources sourcesTextured2DMesh("Textured2DMesh",
 emptyString +  
"attribute vec2 aPosition2D;\n" +
"attribute vec2 aTextureCoord;\n" +
"uniform float uPointSize;\n" +
"varying vec2 TextureCoordOut;\n" +
"uniform vec2 uTranslation2D;\n" +
"uniform vec2 uViewPortExtent;\n" +
"void main() {\n" +
"vec2 pixel = aPosition2D;\n" +
"pixel.x -= uViewPortExtent.x / 2.0;\n" +
"pixel.y += uViewPortExtent.y / 2.0;\n" +
"gl_Position = vec4((pixel.x + uTranslation2D.x) / (uViewPortExtent.x / 2.0),\n" +
"(pixel.y - uTranslation2D.y) / (uViewPortExtent.y / 2.0),\n" +
"0, 1);\n" +
"TextureCoordOut = aTextureCoord;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"uniform sampler2D Sampler;\n" +
"void main() {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"}\n");
    this->add(sourcesTextured2DMesh);

    GPUProgramSources sourcesTexturedMesh("TexturedMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"varying vec2 TextureCoordOut;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"TextureCoordOut = aTextureCoord;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"uniform sampler2D Sampler;\n" +
"void main() {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"}\n");
    this->add(sourcesTexturedMesh);

    GPUProgramSources sourcesTexturedMesh_DirectionLight("TexturedMesh_DirectionLight",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"attribute vec3 aNormal;\n" +
"uniform mat4 uModelview;\n" +
"uniform mat4 uModel;\n" +
"uniform float uPointSize;\n" +
"varying vec2 TextureCoordOut;\n" +
"uniform vec3 uDiffuseLightDirection; //MUST BE NORMALIZED IN SHADER\n" +
"varying float diffuseLightIntensity;\n" +
"uniform vec3 uAmbientLightColor;\n" +
"uniform vec3 uDiffuseLightColor;\n" +
"varying vec3 lightColor;\n" +
"void main() {\n" +
"vec3 normalInModel = normalize( vec3(uModel * vec4(aNormal, 0.0) ));\n" +
"vec3 lightDirNormalized = normalize( uDiffuseLightDirection );\n" +
"float diffuseLightIntensity = max(dot(normalInModel, lightDirNormalized), 0.0);\n" +
"gl_Position = uModelview * aPosition;\n" +
"TextureCoordOut = aTextureCoord;\n" +
"gl_PointSize = uPointSize;\n" +
"lightColor = uAmbientLightColor + uDiffuseLightColor * diffuseLightIntensity;\n" +
"lightColor.x = min(lightColor.x, 1.0);\n" +
"lightColor.y = min(lightColor.y, 1.0);\n" +
"lightColor.z = min(lightColor.z, 1.0);\n" +
"}\n",
 emptyString +  
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n" +
"precision highp float;\n" +
"#else\n" +
"precision mediump float;\n" +
"#endif\n" +
"varying mediump vec2 TextureCoordOut;\n" +
"uniform sampler2D Sampler;\n" +
"varying vec3 lightColor;\n" +
"void main() {\n" +
"vec4 texColor = texture2D(Sampler, TextureCoordOut);\n" +
"gl_FragColor.r = texColor.r * lightColor.r;\n" +
"gl_FragColor.g = texColor.g * lightColor.r;\n" +
"gl_FragColor.b = texColor.b * lightColor.r;\n" +
"gl_FragColor.a = texColor.a;\n" +
"}\n");
    this->add(sourcesTexturedMesh_DirectionLight);

    GPUProgramSources sourcesTransformedTexCoorMultiTexturedMesh("TransformedTexCoorMultiTexturedMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"attribute vec2 aTextureCoord2;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"varying vec2 TextureCoordOut;\n" +
"varying vec2 TextureCoordOut2;\n" +
"uniform mediump vec2 uTranslationTexCoord;\n" +
"uniform mediump vec2 uScaleTexCoord;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"TextureCoordOut = (aTextureCoord * uScaleTexCoord) + uTranslationTexCoord;\n" +
"TextureCoordOut2 = aTextureCoord2;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"varying mediump vec2 TextureCoordOut2;\n" +
"uniform sampler2D Sampler;\n" +
"uniform sampler2D Sampler2;\n" +
"void main() {\n" +
"mediump vec4 tex1 = texture2D(Sampler, TextureCoordOut);\n" +
"mediump vec4 tex2 = texture2D(Sampler2, TextureCoordOut2);\n" +
"gl_FragColor = tex1 * tex2;\n" +
"}\n");
    this->add(sourcesTransformedTexCoorMultiTexturedMesh);

    GPUProgramSources sourcesTransformedTexCoorTexturedMesh("TransformedTexCoorTexturedMesh",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"uniform mediump vec2 uTranslationTexCoord;\n" +
"uniform mediump vec2 uScaleTexCoord;\n" +
"uniform mat4 uModelview;\n" +
"uniform float uPointSize;\n" +
"varying vec4 VertexColor;\n" +
"varying vec2 TextureCoordOut;\n" +
"void main() {\n" +
"gl_Position = uModelview * aPosition;\n" +
"TextureCoordOut = (aTextureCoord * uScaleTexCoord) + uTranslationTexCoord;\n" +
"gl_PointSize = uPointSize;\n" +
"}\n",
 emptyString +  
"varying mediump vec2 TextureCoordOut;\n" +
"varying mediump vec4 VertexColor;\n" +
"uniform sampler2D Sampler;\n" +
"void main() {\n" +
"gl_FragColor = texture2D(Sampler, TextureCoordOut);\n" +
"}\n");
    this->add(sourcesTransformedTexCoorTexturedMesh);

    GPUProgramSources sourcesTransformedTexCoorTexturedMesh_DirectionLight("TransformedTexCoorTexturedMesh_DirectionLight",
 emptyString +  
"attribute vec4 aPosition;\n" +
"attribute vec2 aTextureCoord;\n" +
"attribute vec3 aNormal;\n" +
"uniform mat4 uModelview;\n" +
"uniform mat4 uModel;\n" +
"uniform float uPointSize;\n" +
"varying vec2 TextureCoordOut;\n" +
"uniform mediump vec2 uTranslationTexCoord;\n" +
"uniform mediump vec2 uScaleTexCoord;\n" +
"uniform vec3 uDiffuseLightDirection; //MUST BE NORMALIZED\n" +
"varying float diffuseLightIntensity;\n" +
"uniform vec3 uAmbientLightColor;\n" +
"uniform vec3 uDiffuseLightColor;\n" +
"varying vec3 lightColor;\n" +
"void main() {\n" +
"vec3 normalInModel = normalize( vec3(uModel * vec4(aNormal, 0.0) ));\n" +
"vec3 lightDirNormalized = normalize( uDiffuseLightDirection );\n" +
"float diffuseLightIntensity = max(dot(normalInModel, lightDirNormalized), 0.0);\n" +
"gl_Position = uModelview * aPosition;\n" +
"TextureCoordOut = (aTextureCoord * uScaleTexCoord) + uTranslationTexCoord;\n" +
"gl_PointSize = uPointSize;\n" +
"lightColor = uAmbientLightColor + uDiffuseLightColor * diffuseLightIntensity;\n" +
"lightColor.x = min(lightColor.x, 1.0);\n" +
"lightColor.y = min(lightColor.y, 1.0);\n" +
"lightColor.z = min(lightColor.z, 1.0);\n" +
"}\n",
 emptyString +  
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n" +
"precision highp float;\n" +
"#else\n" +
"precision mediump float;\n" +
"#endif\n" +
"varying mediump vec2 TextureCoordOut;\n" +
"uniform sampler2D Sampler;\n" +
"varying vec3 lightColor;\n" +
"void main() {\n" +
"vec4 texColor = texture2D(Sampler, TextureCoordOut);\n" +
"gl_FragColor.r = texColor.r * lightColor.r;\n" +
"gl_FragColor.g = texColor.g * lightColor.r;\n" +
"gl_FragColor.b = texColor.b * lightColor.r;\n" +
"gl_FragColor.a = texColor.a;\n" +
"}\n");
    this->add(sourcesTransformedTexCoorTexturedMesh_DirectionLight);

  }

};
#endif
