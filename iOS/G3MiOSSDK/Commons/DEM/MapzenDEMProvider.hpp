//
//  MapzenDEMProvider.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/13/16.
//
//

#ifndef MapzenDEMProvider_hpp
#define MapzenDEMProvider_hpp

#include "WebMercatorPyramidDEMProvider.hpp"

#include "TimeInterval.hpp"
#include "DownloadPriority.hpp"
#include <string>

class FloatBufferDEMGrid;
class Sector;


class MapzenDEMProvider : public WebMercatorPyramidDEMProvider {
private:
  static int _instanceCounter;

  const std::string _apiKey;

  const long long    _downloadPriority;
#ifdef C_CODE
  const TimeInterval _timeToCache;
#endif
#ifdef JAVA_CODE
  private final TimeInterval _timeToCache;
#endif
  const bool         _readExpired;


  const std::string _instanceID;

#ifdef C_CODE
  const G3MContext* _context;
#endif
#ifdef JAVA_CODE
  private G3MContext _context;
#endif


  bool _rootGridDownloaded;
  bool _errorDownloadingRootGrid;

  long long requestTile(int z,
                   int x,
                   int y,
                   const Sector& sector);

protected:
  ~MapzenDEMProvider();

public:

  MapzenDEMProvider(const std::string&  apiKey,
                    long long           downloadPriority,
                    const TimeInterval& timeToCache,
                    bool                readExpired,
                    const double        deltaHeight);

  RenderState getRenderState();

  void initialize(const G3MContext* context);

  void cancel();

  void onGrid(int z,
              int x,
              int y,
              FloatBufferDEMGrid* grid);

  void onDownloadError(int z,
                       int x,
                       int y);
  
  long long requestDataFor(const PyramidNode* node);
  
  void cancelDataRequest(long long requestID);

};

#endif
