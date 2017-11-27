//
//  G3MRasterLayersDemoScene.cpp
//  G3MApp
//
//  Created by Diego Gomez Deck on 11/16/13.
//

#include "G3MRasterLayersDemoScene.hpp"

#include "G3MDemoModel.hpp"

#include <G3MiOSSDK/LayerSet.hpp>
#include <G3MiOSSDK/MapBoxLayer.hpp>
#include <G3MiOSSDK/MapQuestLayer.hpp>
#include <G3MiOSSDK/WMSLayer.hpp>
#include <G3MiOSSDK/LevelTileCondition.hpp>
#include <G3MiOSSDK/OSMLayer.hpp>
#include <G3MiOSSDK/BingMapsLayer.hpp>
#include <G3MiOSSDK/URLTemplateLayer.hpp>
#include <G3MiOSSDK/G3MWidget.hpp>
#include <G3MiOSSDK/TimeInterval.hpp>
#include <G3MiOSSDK/MercatorTiledLayer.hpp>
#include <G3MiOSSDK/Geodetic3D.hpp>
#include <G3MiOSSDK/PlanetRenderer.hpp>

#include <G3MiOSSDK/MapzenDEMProvider.hpp>
#include <G3MiOSSDK/G3MContext.hpp>
#include <G3MiOSSDK/IThreadUtils.hpp>
#include <G3MiOSSDK/GTask.hpp>


void G3MRasterLayersDemoScene::createLayerSet(LayerSet* layerSet) {
  MapBoxLayer* mboxOSMLayer = new MapBoxLayer("examples.map-cnkhv76j",
                                              TimeInterval::fromDays(30),
                                              true,
                                              2);
  mboxOSMLayer->setTitle("Map Box OSM");
  mboxOSMLayer->setEnable(true);
  layerSet->addLayer(mboxOSMLayer);
  
  
  MapQuestLayer* mqOSM = MapQuestLayer::newOSM(TimeInterval::fromDays(30));
  mqOSM->setEnable(false);
  mqOSM->setTitle("MapQuest OSM");
  layerSet->addLayer(mqOSM);
  
  
  MapQuestLayer* mqlAerial = MapQuestLayer::newOpenAerial(TimeInterval::fromDays(30));
  mqlAerial->setTitle("MapQuest Aerial");
  mqlAerial->setEnable(false);
  layerSet->addLayer(mqlAerial);
  
  
  MapBoxLayer* mboxAerialLayer = new MapBoxLayer("examples.map-m0t0lrpu",
                                                 TimeInterval::fromDays(30),
                                                 true,
                                                 2);
  mboxAerialLayer->setTitle("Map Box Aerial");
  mboxAerialLayer->setEnable(false);
  layerSet->addLayer(mboxAerialLayer);
  
  
  MapBoxLayer* mboxTerrainLayer = new MapBoxLayer("examples.map-qogxobv1",
                                                  TimeInterval::fromDays(30),
                                                  true,
                                                  2);
  mboxTerrainLayer->setTitle("Map Box Terrain");
  mboxTerrainLayer->setEnable(false);
  layerSet->addLayer(mboxTerrainLayer);
  
  
  WMSLayer* blueMarble = new WMSLayer("bmng200405",
                                      URL("http://www.nasa.network.com/wms?"),
                                      WMS_1_1_0,
                                      Sector::fullSphere(),
                                      "image/jpeg",
                                      "EPSG:4326",
                                      "",
                                      false,
                                      new LevelTileCondition(0, 18),
                                      TimeInterval::fromDays(30),
                                      true);
  blueMarble->setTitle("Nasa Blue Marble (WMS)");
  blueMarble->setEnable(false);
  layerSet->addLayer(blueMarble);
  
  
  OSMLayer* osmLayer = new OSMLayer(TimeInterval::fromDays(30));
  osmLayer->setTitle("Open Street Map");
  osmLayer->setEnable(false);
  layerSet->addLayer(osmLayer);
  
  
  BingMapsLayer* bingMapsAerialLayer = new BingMapsLayer(BingMapType::Aerial(),
                                                         "AnU5uta7s5ql_HTrRZcPLI4_zotvNefEeSxIClF1Jf7eS-mLig1jluUdCoecV7jc",
                                                         TimeInterval::fromDays(30));
  bingMapsAerialLayer->setTitle("Bing Aerial");
  bingMapsAerialLayer->setEnable(true);
  layerSet->addLayer(bingMapsAerialLayer);
  
  
  BingMapsLayer* bingMapsAerialWithLabels = new BingMapsLayer(BingMapType::AerialWithLabels(),
                                                              "AnU5uta7s5ql_HTrRZcPLI4_zotvNefEeSxIClF1Jf7eS-mLig1jluUdCoecV7jc",
                                                              TimeInterval::fromDays(30));
  bingMapsAerialWithLabels->setTitle("Bing Aerial With Labels");
  bingMapsAerialWithLabels->setEnable(false);
  layerSet->addLayer(bingMapsAerialWithLabels);
  
  BingMapsLayer* bingMapsCollinsBart = new BingMapsLayer(BingMapType::CollinsBart(),
                                                         "AnU5uta7s5ql_HTrRZcPLI4_zotvNefEeSxIClF1Jf7eS-mLig1jluUdCoecV7jc",
                                                         TimeInterval::fromDays(30));
  bingMapsCollinsBart->setTitle("Bing Collins Bart");
  bingMapsCollinsBart->setEnable(false);
  layerSet->addLayer(bingMapsCollinsBart);
  
  
  std::vector<std::string> subdomains;
  subdomains.push_back("0.");
  subdomains.push_back("1.");
  subdomains.push_back("2.");
  subdomains.push_back("3.");
  MercatorTiledLayer* meteoritesLayer = new MercatorTiledLayer("http://",
                                                               "tiles.cartocdn.com/osm2/tiles/meteoritessize",
                                                               subdomains,
                                                               "png",
                                                               TimeInterval::fromDays(90),
                                                               false,
                                                               2,
                                                               17,
                                                               true // isTransparent
                                                               );
  meteoritesLayer->setTitle("CartoDB Meteorites");
  meteoritesLayer->setEnable(false);
  layerSet->addLayer(meteoritesLayer);
  
  
  URLTemplateLayer* arcGISOverlayLayerTest = URLTemplateLayer::newMercator("http://www.fairfaxcounty.gov/gis/rest/services/DMV/DMV/MapServer/export?dpi=96&transparent=true&format=png8&bbox={west}%2C{south}%2C{east}%2C{north}&bboxSR=3857&imageSR=3857&size={width}%2C{height}&f=image",
                                                                           Sector::fullSphere(),
                                                                           true,
                                                                           2,
                                                                           18,
                                                                           TimeInterval::fromDays(30),
                                                                           true,
                                                                           1,
                                                                           new LevelTileCondition(12, 18));
  arcGISOverlayLayerTest->setTitle("ESRI ArcGis Online");
  arcGISOverlayLayerTest->setEnable(false);
  layerSet->addLayer(arcGISOverlayLayerTest);
  
  
  { // Uruguay (WMS)
    
    WMSLayer* unidadesPaisajisticas = new WMSLayer("Unidades_Paisajisticas",
                                                   URL("http://www.dinama.gub.uy/sia/ecosistemas/ecosistemas.cgi"),
                                                   WMS_1_1_0,
                                                   Sector::fromDegrees(-34.9739999999999966, -58.4393000000000029,
                                                                       -30.0854999999999997, -53.1811000000000007),
                                                   "image/png", // format
                                                   "EPSG:4326",
                                                   "",          // style
                                                   true,        // isTransparent
                                                   NULL,        // condition,
                                                   TimeInterval::fromDays(30),
                                                   true,        // readExpired
                                                   NULL,        // parameters
                                                   0.85f         // transparency
                                                   );
    unidadesPaisajisticas->setTitle("Uruguay_Unidades_Paisajisticas");
    unidadesPaisajisticas->setEnable(false);
    layerSet->addLayer(unidadesPaisajisticas);
    
    
    WMSLayer* ambientesAcuaticos = new WMSLayer("ambientes_acuaticos",
                                                URL("http://www.dinama.gub.uy/sia/ecosistemas/ecosistemas.cgi"),
                                                WMS_1_1_0,
                                                Sector::fromDegrees(-40.0005999999999986, -59.4996000000000009,
                                                                    -32.9996000000000009, -49.4981000000000009),
                                                "image/png", // format
                                                "EPSG:4326",
                                                "",          // style
                                                true,        // isTransparent
                                                NULL,        // condition,
                                                TimeInterval::fromDays(30),
                                                true,        // readExpired
                                                NULL,        // parameters
                                                0.85f         // transparency
                                                );
    ambientesAcuaticos->setTitle("Uruguay_Ambientes_Acuaticos");
    ambientesAcuaticos->setEnable(false);
    layerSet->addLayer(ambientesAcuaticos);
  }
}


void G3MRasterLayersDemoScene::rawActivate(const G3MContext* context) {
#warning JM at work!
  PlanetRenderer* planetRenderer = getModel()->getPlanetRenderer();
  //planetRenderer->setShowStatistics(true);
  planetRenderer->setIncrementalTileQuality(true);
  
  createLayerSet( getModel()->getLayerSet() );
  
  //  getModel()->getG3MWidget()->setCameraPosition(Geodetic3D::fromDegrees(27.05392035893508762, -15.534032544664327702, 26904));
  //  getModel()->getG3MWidget()->setCameraHeadingPitchRoll(Angle::zero(), Angle::fromDegrees(-13), Angle::zero());
  
  
  class MyGTask: public GTask{
    G3MWidget* _w;
    const TimeInterval _t;
    const Geodetic3D _g;
    const Angle _heading;
    const Angle _pitch;
    
    const bool _linearTiming;
    const bool _linearHeight;
    public:
    MyGTask(G3MWidget* w, const TimeInterval& t, const Geodetic3D& g, const Angle& heading, const Angle& pitch,
            const bool linearTiming,
            const bool linearHeight):
    _w(w),_t(t),_heading(heading), _g(g), _pitch(pitch),_linearTiming(linearTiming), _linearHeight(linearHeight){}
    void run(const G3MContext* context){
      _w->setAnimatedCameraPosition(_t,_g,_heading,_pitch,_linearTiming, _linearHeight);
    }
  };
  
  
  
//  addPeriodicalTask(TimeInterval::fromSeconds(10.0), new MyGTask(getModel()->getG3MWidget()));
  
  getModel()->getG3MWidget()->getG3MContext()->getThreadUtils()->invokeAfterInRendererThread(TimeInterval::fromSeconds(5.0),
                                                                                             new MyGTask(getModel()->getG3MWidget(),
                                                                                                         TimeInterval::fromSeconds(24.0),
                                                                                                         Geodetic3D::fromDegrees(46.10433157368725432, 9.0796246961508124684, 12402.946573206238099),
                                                                                                         Angle::fromDegrees(-81.256883),
                                                                                                         Angle::fromDegrees(-90.0),
                                                                                                         false, false),
                                                                                             true);
  
  getModel()->getG3MWidget()->getG3MContext()->getThreadUtils()->invokeAfterInRendererThread(TimeInterval::fromSeconds(29.0),
                                                                                             new MyGTask(getModel()->getG3MWidget(),
                                                                                                         TimeInterval::fromSeconds(4.0),
                                                                                                         Geodetic3D::fromDegrees(46.10433157368725432, 9.0796246961508124684, 12402.946573206238099),
                                                                                                         Angle::fromDegrees(-81.256883),
                                                                                                         Angle::fromDegrees(-14.927761),
                                                                                                         false, false),
                                                                                             true);
  
  
  
  getModel()->getG3MWidget()->getG3MContext()->getThreadUtils()->invokeAfterInRendererThread(TimeInterval::fromSeconds(33.0),
                                                                                             new MyGTask(getModel()->getG3MWidget(),
                                                                                                         TimeInterval::fromSeconds(40.0),
                                                                                                         Geodetic3D::fromDegrees(46.237684148153611829, 10.303181845788804694, 12452.692968591822137),
                                                                                                         Angle::fromDegrees(-82.136257),
                                                                                                         Angle::fromDegrees(-14.927761),
                                                                                                         false, true),
                                                                                             true);

  
  
  
  //  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(10.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
  //    getModel()->getG3MWidget()->setAnimatedCameraPosition(TimeInterval::fromSeconds(20.0),
  //                                                          Geodetic3D::fromDegrees(0, 0, 1e7),
  //                                                          Geodetic3D::fromDegrees(46.10433157368725432, 9.0796246961508124684, 12402.946573206238099), Angle::zero(), Angle::fromDegrees(-81.256883), Angle::zero(), Angle::fromDegrees(-14.927761));
  //  });
  
  
  planetRenderer->setDEMProvider( new MapzenDEMProvider("mapzen-ZB6FqMg",
                                                        DownloadPriority::HIGHER,
                                                        TimeInterval::fromDays(0),
                                                        false, /* readExpired */
                                                        0      /* deltaHeight */) );
  planetRenderer->setVerticalExaggeration(2.5);
}

void G3MRasterLayersDemoScene::rawSelectOption(const std::string& option,
                                               int optionIndex) {
  LayerSet* layerSet = getModel()->getLayerSet();
  layerSet->disableAllLayers();
  
  if (option == "MapBox OSM") {
    layerSet->getLayerByTitle("Map Box OSM")->setEnable(true);
  }
  else if (option == "Open Street Map") {
    layerSet->getLayerByTitle("Open Street Map")->setEnable(true);
  }
  else if (option == "MapBox Terrain") {
    layerSet->getLayerByTitle("Map Box Terrain")->setEnable(true);
  }
  else if (option == "MapBox Aerial") {
    layerSet->getLayerByTitle("Map Box Aerial")->setEnable(true);
  }
  else if (option == "CartoDB Meteorites") {
    layerSet->getLayerByTitle("MapQuest OSM")->setEnable(true);
    layerSet->getLayerByTitle("CartoDB Meteorites")->setEnable(true);
  }
  else if (option == "MapQuest Aerial") {
    layerSet->getLayerByTitle("MapQuest Aerial")->setEnable(true);
  }
  else if (option == "MapQuest OSM") {
    layerSet->getLayerByTitle("MapQuest OSM")->setEnable(true);
  }
  else if (option == "Nasa Blue Marble (WMS)") {
    layerSet->getLayerByTitle("Nasa Blue Marble (WMS)")->setEnable(true);
  }
  else if (option == "ESRI ArcGis Online") {
    layerSet->getLayerByTitle("Map Box Aerial")->setEnable(true);
    layerSet->getLayerByTitle("ESRI ArcGis Online")->setEnable(true);
    
    getModel()->getG3MWidget()->setAnimatedCameraPosition(Geodetic3D::fromDegrees(38.6,
                                                                                  -77.2,
                                                                                  30000));
  }
  else if (option == "Bing Aerial") {
    layerSet->getLayerByTitle("Bing Aerial")->setEnable(true);
  }
  else if (option == "Bing Aerial with Labels") {
    layerSet->getLayerByTitle("Bing Aerial With Labels")->setEnable(true);
  }
  else if (option == "Bing Collins Bart") {
    layerSet->getLayerByTitle("Bing Collins Bart")->setEnable(true);
  }
  else if (option == "Uruguay (WMS)") {
    layerSet->getLayerByTitle("Nasa Blue Marble (WMS)")->setEnable(true);
    layerSet->getLayerByTitle("Uruguay_Unidades_Paisajisticas")->setEnable(true);
    layerSet->getLayerByTitle("Uruguay_Ambientes_Acuaticos")->setEnable(true);
    
    // just uruguay land
    //    getModel()->getG3MWidget()->setAnimatedCameraPosition(Geodetic3D::fromDegrees(-32.568042356920749114,
    //                                                                                  -56.383545687724506479,
    //                                                                                  1190419));
    
    // land and water area
    getModel()->getG3MWidget()->setAnimatedCameraPosition(Geodetic3D::fromDegrees(-35.093636856649169431,
                                                                                  -56.098973158344456635,
                                                                                  2139700));
  }
  else {
    ILogger::instance()->logError("option \"%s\" not supported", option.c_str());
  }
}
