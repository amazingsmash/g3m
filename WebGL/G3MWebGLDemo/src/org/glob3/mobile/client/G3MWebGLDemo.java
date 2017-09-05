

package org.glob3.mobile.client;

import org.glob3.mobile.generated.AtmosphereRenderer;
import org.glob3.mobile.generated.LayerSet;
import org.glob3.mobile.generated.LayerTilesRenderParameters;
import org.glob3.mobile.generated.OSMLayer;
import org.glob3.mobile.generated.Sector;
import org.glob3.mobile.generated.TimeInterval;
import org.glob3.mobile.generated.URL;
import org.glob3.mobile.generated.WMSLayer;
import org.glob3.mobile.generated.WMSServerVersion;
import org.glob3.mobile.specific.Downloader_WebGL;
import org.glob3.mobile.specific.G3MBuilder_WebGL;
import org.glob3.mobile.specific.G3MWidget_WebGL;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.Panel;
import com.google.gwt.user.client.ui.RootPanel;


public class G3MWebGLDemo
         implements
            EntryPoint {

   private static final String _g3mWidgetHolderId = "g3mWidgetHolder";
   private G3MWidget_WebGL     _widget            = null;


   @Override
   public void onModuleLoad() {
      /*
      ======================================================================
       WARNING / CUIDADO / ACHTUNG
      ----------------------------------------------------------------------         
       Don't touch this project for quick & dirty tests.  If you do it, 
       your commit rights will be revoked and you'll spend your after-life
       burning at the hell.
      ======================================================================
      */

      final G3MBuilder_WebGL builder = new G3MBuilder_WebGL();
      
      LayerSet layerSet = new LayerSet();
      
      WMSLayer blueMarble = new WMSLayer("bmng200405",
              new URL("http://www.nasa.network.com/wmsxxxx?", false),
              WMSServerVersion.WMS_1_1_0,
              Sector.fromDegrees(40.1240143280790858,
                                  -5.8964874640814313,
                                  40.3723148480663158,
                                  -5.4816079822178570),
              //                                          Sector::fromDegrees(0,
              //                                                              -90,
              //                                                              45,
              //                                                              0),
              //                                          Sector::fullSphere(),
              //                                          Sector::fromDegrees(39.13, -6.86, 39.66, -6.05),
              
              "image/jpeg",
              "EPSG:4326",
              "",
              false,
              null, //new LevelTileCondition(0, 6),
              //NULL,
              TimeInterval.fromDays(0),
              true,
              LayerTilesRenderParameters.createDefaultWGS84(Sector.fullSphere(), 0, 8),
              1);
      
      final String proxy = null; // "http://galileo.glob3mobile.com/" + "proxy.php?url="
      builder.setDownloader(new Downloader_WebGL( //
               8, // maxConcurrentOperationCount
               10, // delayMillis
               proxy));
      
     // layerSet.addLayer(blueMarble);
      

      builder.setAtmosphere(true);
      
      layerSet.addLayer(new OSMLayer(TimeInterval.fromDays(30)));
      
      builder.getPlanetRendererBuilder().setLayerSet(layerSet);
      
      builder.addRenderer(new AtmosphereRenderer());
      _widget = builder.createWidget();

      final Panel g3mWidgetHolder = RootPanel.get(_g3mWidgetHolderId);
      g3mWidgetHolder.add(_widget);
   }


}
