//
//  CompositeTileImageProvider.h
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 4/23/14.
//
//

#ifndef __G3MiOSSDK__CompositeTileImageProvider__
#define __G3MiOSSDK__CompositeTileImageProvider__

#include "CanvasTileImageProvider.hpp"
#include <vector>
#include "TileImageListener.hpp"

class CompositeTileImageContribution;


class CompositeTileImageProvider : public CanvasTileImageProvider {
private:

  class ChildContribution {
  public:
    const bool                   _isError;
    const bool                   _isCanceled;
    const IImage*                _image;
    const std::string            _imageId;
    const TileImageContribution* _contribution;
    const std::string            _error;

    static const ChildContribution* image(const IImage*                image,
                                          const std::string&           imageId,
                                          const TileImageContribution* contribution);

    static const ChildContribution* error(const std::string& error);

    static const ChildContribution* cancelation();

  private:

    ChildContribution(const bool                   isError,
                      const bool                   isCanceled,
                      const IImage*                image,
                      const std::string&           imageId,
                      const TileImageContribution* contribution,
                      const std::string&           error) :
    _isError(isError),
    _isCanceled(isCanceled),
    _image(image),
    _imageId(imageId),
    _contribution(contribution),
    _error(error)
    {
    }
  };
  
  class Composer {
  private:
    const std::string                     _tileId;
    TileImageListener*                    _listener;
    const bool                            _deleteListener;
    const CompositeTileImageContribution* _compositeContribution;
    std::vector<const ChildContribution*> _contributions;
    const int                             _contributionsSize;

    int _stepsDone;
    void stepDone();
    void done();

    bool _anyError;
    bool _anyCancelation;
    ~Composer();

  public:
    Composer(const std::string& tileId,
             TileImageListener* listener,
             bool deleteListener,
             const CompositeTileImageContribution* compositeContribution);

    void imageCreated(const std::string&           tileId,
                      const IImage*                image,
                      const std::string&           imageId,
                      const TileImageContribution* contribution,
                      const int                    index);

    void imageCreationError(const std::string& error,
                            const int          index);

    void imageCreationCanceled(const int index);
    
  };
  

  class ChildTileImageListener : public TileImageListener {
  private:
    Composer* _composer;
    const int _index;

  public:
    ChildTileImageListener(Composer* composer,
                           int index) :
    _composer(composer),
    _index(index)
    {
    }

    void imageCreated(const std::string&           tileId,
                      const IImage*                image,
                      const std::string&           imageId,
                      const TileImageContribution* contribution);

    void imageCreationError(const std::string& tileId,
                            const std::string& error);

    void imageCreationCanceled(const std::string& tileId);

  };


  std::vector<TileImageProvider*> _children;
  int                             _childrenSize;

protected:
  ~CompositeTileImageProvider();

public:
  CompositeTileImageProvider() :
  _childrenSize(0)
  {
  }

  void addProvider(TileImageProvider* child) {
    _children.push_back(child);
    _childrenSize = _children.size();
  }

  const TileImageContribution* contribution(const Tile* tile);

  void create(const Tile* tile,
              const TileImageContribution* contribution,
              const Vector2I& resolution,
              long long tileDownloadPriority,
              bool logDownloadActivity,
              TileImageListener* listener,
              bool deleteListener);

  void cancel(const Tile* tile);
  
};

#endif