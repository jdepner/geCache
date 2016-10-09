
/********************************************************************************************* 

    geCache.cpp

    Copyright (c) 2016, Jan C. Depner


    This file is part of geCache.

    geCache is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    geCache is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with geCache.  If not, see <http://www.gnu.org/licenses/>.

*********************************************************************************************/


/***************************************************************************\

    Module :        geCache

    Programmer :    Jan C. Depner

    Date :          September 7, 2016

    Purpose :       Simple program to cache Google Earth data for offline
                    use.

\***************************************************************************/

#include "geCache.hpp"
#include "geCacheHelp.hpp"

geCache::geCache (QWidget *parent):
  QMainWindow (parent, 0)
{
  uint8_t envin (OPTIONS *options);
  void set_defaults (MISC *misc, OPTIONS *options, uint8_t reset);


  QResource::registerResource ("/icons.rcc");


  strcpy (misc.version, VERSION);


  //  Initialize some important variables.

  googleEarthProc = NULL;
  buildGoogleEarthProc = NULL;
  build_direction = 0;
  build_kill_flag = NVFalse;
  build_start_flag = NVFalse;
  bounds_clicked = NO_BOUNDS;
  poly_define = NVFalse;
  poly_edit = 0;
  prev_clipboard_text = "";
  restart_msg = NVFalse;
  already_gone = NVFalse;
  x_box_count = 0;


  //  Trying to make QToolTips easier to read.

  qApp->setStyleSheet ("QToolTip { border: 2px solid black; padding: 2px; border-radius: 2px; color: black; background: white }");


  //  Have to set the focus policy or keypress events don't work properly at first in Focus Follows Mouse mode

  setFocusPolicy (Qt::WheelFocus);
  setFocus (Qt::ActiveWindowFocusReason);


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/geCache.png"));


  //  Set all of the defaults

  set_defaults (&misc, &options, NVFalse);


  //  Get the user's defaults if available

  envin (&options);


  //  Set the window size and location from the saved settings

  this->resize (options.window_width, options.window_height);
  this->move (options.window_x, options.window_y);



  // Set the application font

  qApp->setFont (options.font);


  //  Check to see if we have Google Earth.

  misc.googleearth_available = NVTrue;
  QString ge_exe = QStandardPaths::findExecutable (options.ge_name);
  if (ge_exe.isEmpty ()) misc.googleearth_available = NVFalse;



  QFrame *frame = new QFrame (this, 0);

  setCentralWidget (frame);

  QVBoxLayout *mainBox = new QVBoxLayout (frame);
  mainBox->setMargin (5);
  mainBox->setSpacing (5);


  //  Main tab widget

  geCacheTab = new QTabWidget ();
  geCacheTab->setTabPosition (QTabWidget::North);




  //************************************* About tab ***************************************************

  QGroupBox *aboutBox = new QGroupBox (this);
  QVBoxLayout *aboutBoxLayout = new QVBoxLayout;
  aboutBox->setLayout (aboutBoxLayout);


  QTextBrowser *aboutTextBrowser = new QTextBrowser (this);
  aboutTextBrowser->setOpenExternalLinks (true);
  aboutTextBrowser->setWhatsThis (tr ("See, it really works.  Now stop fooling around and get to work!"));

  aboutTextBrowser->insertHtml (geCacheAboutText);
  aboutTextBrowser->moveCursor (QTextCursor::Start);

  aboutBoxLayout->addWidget (aboutTextBrowser);


  geCacheTab->addTab (aboutBox, tr ("About"));
  geCacheTab->setTabToolTip (ABOUT_TAB, tr ("Information about geCache"));
  geCacheTab->setTabWhatsThis (ABOUT_TAB, tr ("This tab contains information about geCache, including some tips."));




  //  ******************************************* Cache tab ***************************************************
  //
  //  Programmer note: Some widgets in this section do not have tool tips set here because they may be disabled
  //  when Google Earth is running.  The tool tips get set in setWidgetStates so that the reason they are
  //  disabled will be evident in the tool tip.  When they get enabled, the tool tip is set to more useful
  //  information for each widget.

  QGroupBox *cacheBox = new QGroupBox (this);
  QVBoxLayout *cacheBoxLayout = new QVBoxLayout;
  cacheBox->setLayout (cacheBoxLayout);


  cacheBarBox = new QGroupBox (tr ("Google Earth Preview"), this);
  cacheBarBox->setToolTip (tr ("Start/stop Google Earth to preview areas"));
  cacheBarBox->setWhatsThis (cacheGoogleEarthText);
  QHBoxLayout *cacheBarBoxLayout = new QHBoxLayout;
  cacheBarBox->setLayout (cacheBarBoxLayout);


  toolBar = new QToolBar ();
  cacheBarBoxLayout->addWidget (toolBar);


  //  I'm using QPushButtons in the QToolBar because I like the look of the QToolBar but I need the raised look of the QPushButtons.  There may be a better 
  //  way to do this but this works.

  bGoogleEarth = new QPushButton (this);
  bGoogleEarth->setIcon (QIcon (":/icons/Earth_Icon.png"));
  bGoogleEarth->setWhatsThis (cacheGoogleEarthText);
  bGoogleEarth->setCheckable (true);
  connect (bGoogleEarth, SIGNAL (clicked (bool)), this, SLOT (slotGoogleEarthClicked (bool)));
  toolBar->addWidget (bGoogleEarth);

  toolBar->addSeparator ();

  bGoogleEarthLink = new QPushButton (this);
  bGoogleEarthLink->setIcon (QIcon (":/icons/Earth_link.png"));
  bGoogleEarthLink->setToolTip (tr ("Link geCache to Google Earth"));
  bGoogleEarthLink->setWhatsThis (cacheGoogleEarthLinkText);
  bGoogleEarthLink->setCheckable (true);
  connect (bGoogleEarthLink, SIGNAL (clicked (bool)), this, SLOT (slotGoogleEarthLink (bool)));
  toolBar->addWidget (bGoogleEarthLink);


  cacheBoxLayout->addWidget (cacheBarBox);




  //  Shape tab widget

  shapeTab = new QTabWidget ();
  shapeTab->setTabPosition (QTabWidget::North);
  cacheBoxLayout->addWidget (shapeTab);



  char hem;
  QString ltstring, lnstring;
  double deg, min, sec;


  QGroupBox *rectAreaBox = new QGroupBox ();
  QHBoxLayout *rectAreaBoxLayout = new QHBoxLayout;
  rectAreaBox->setLayout (rectAreaBoxLayout);

  QGroupBox *rectBox = new QGroupBox (tr ("Cache/preview area"), this);
  QVBoxLayout *rectBoxLayout = new QVBoxLayout;
  rectBox->setLayout (rectBoxLayout);
  QHBoxLayout *rectTopLayout = new QHBoxLayout;
  QHBoxLayout *rectTopMidLayout = new QHBoxLayout;
  QHBoxLayout *rectMidLayout = new QHBoxLayout;
  QHBoxLayout *rectBotMidLayout = new QHBoxLayout;
  QHBoxLayout *rectBotLayout = new QHBoxLayout;
  rectBoxLayout->addLayout (rectTopLayout);
  rectBoxLayout->addLayout (rectTopMidLayout);
  rectBoxLayout->addLayout (rectMidLayout);
  rectBoxLayout->addLayout (rectBotMidLayout);
  rectBoxLayout->addLayout (rectBotLayout);

  rectBox->setToolTip (tr ("Set the rectangular latitude and longitude boundaries of the area you would like to cache or preview"));
  rectBox->setWhatsThis (tr ("Set the rectangular latitude and longitude boundaries of the area you would like to cache or preview"));


  boundsGroup = new QButtonGroup (this);
  boundsGroup->setExclusive (false);
  connect (boundsGroup, SIGNAL (buttonClicked (int)), this, SLOT (slotBoundsClicked (int)));


  bBounds[NW_BOUNDS] = new QPushButton (this);
  bBounds[NW_BOUNDS]->setCheckable (true);
  bBounds[NW_BOUNDS]->setIcon (QIcon (":/icons/compass_NW.png"));
  bBounds[NW_BOUNDS]->setWhatsThis (NWText);
  boundsGroup->addButton (bBounds[NW_BOUNDS], NW_BOUNDS);
  rectTopLayout->addWidget (bBounds[NW_BOUNDS]);

  rectTopLayout->addStretch (10);

  north = new QLineEdit (this);
  north->setWhatsThis (northText);
  ltstring = qFixpos (options.cache_mbr.max_y, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);
  north->setText (ltstring);
  connect (north, SIGNAL (editingFinished ()), this, SLOT (slotNorthEditingFinished ()));
  connect (north, SIGNAL (returnPressed ()), this, SLOT (slotNorthEditingFinished ()));
  rectTopLayout->addWidget (north);

  rectTopLayout->addStretch (10);

  bBounds[NE_BOUNDS] = new QPushButton (this);
  bBounds[NE_BOUNDS]->setCheckable (true);
  bBounds[NE_BOUNDS]->setIcon (QIcon (":/icons/compass_NE.png"));
  bBounds[NE_BOUNDS]->setWhatsThis (NEText);
  boundsGroup->addButton (bBounds[NE_BOUNDS], NE_BOUNDS);
  rectTopLayout->addWidget (bBounds[NE_BOUNDS]);


  bBounds[NORTH_BOUNDS] = new QPushButton (this);
  bBounds[NORTH_BOUNDS]->setCheckable (true);
  bBounds[NORTH_BOUNDS]->setIcon (QIcon (":/icons/compass_N.png"));
  bBounds[NORTH_BOUNDS]->setWhatsThis (NText);
  boundsGroup->addButton (bBounds[NORTH_BOUNDS], NORTH_BOUNDS);
  rectTopMidLayout->addWidget (bBounds[NORTH_BOUNDS]);
  rectTopMidLayout->setAlignment (bBounds[NORTH_BOUNDS], Qt::AlignCenter);


  west = new QLineEdit (this);
  west->setWhatsThis (westText);
  lnstring = qFixpos (options.cache_mbr.min_x, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);
  west->setText (lnstring);
  connect (west, SIGNAL (editingFinished ()), this, SLOT (slotWestEditingFinished ()));
  connect (west, SIGNAL (returnPressed ()), this, SLOT (slotWestEditingFinished ()));


  rectMidLayout->addWidget (west);


  bBounds[WEST_BOUNDS] = new QPushButton (this);
  bBounds[WEST_BOUNDS]->setCheckable (true);
  bBounds[WEST_BOUNDS]->setIcon (QIcon (":/icons/compass_W.png"));
  bBounds[WEST_BOUNDS]->setWhatsThis (WText);
  boundsGroup->addButton (bBounds[WEST_BOUNDS], WEST_BOUNDS);
  rectMidLayout->addWidget (bBounds[WEST_BOUNDS]);


  rectMidLayout->addStretch (10);


  bBounds[EAST_BOUNDS] = new QPushButton (this);
  bBounds[EAST_BOUNDS]->setCheckable (true);
  bBounds[EAST_BOUNDS]->setIcon (QIcon (":/icons/compass_E.png"));
  bBounds[EAST_BOUNDS]->setWhatsThis (EText);
  boundsGroup->addButton (bBounds[EAST_BOUNDS], EAST_BOUNDS);
  rectMidLayout->addWidget (bBounds[EAST_BOUNDS]);


  east = new QLineEdit (this);
  east->setWhatsThis (eastText);
  lnstring = qFixpos (options.cache_mbr.max_x, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);
  east->setText (lnstring);
  connect (east, SIGNAL (editingFinished ()), this, SLOT (slotEastEditingFinished ()));
  connect (east, SIGNAL (returnPressed ()), this, SLOT (slotEastEditingFinished ()));


  rectMidLayout->addWidget (east);


  bBounds[SOUTH_BOUNDS] = new QPushButton (this);
  bBounds[SOUTH_BOUNDS]->setCheckable (true);
  bBounds[SOUTH_BOUNDS]->setIcon (QIcon (":/icons/compass_S.png"));
  bBounds[SOUTH_BOUNDS]->setWhatsThis (SText);
  boundsGroup->addButton (bBounds[SOUTH_BOUNDS], SOUTH_BOUNDS);
  rectBotMidLayout->addWidget (bBounds[SOUTH_BOUNDS]);
  rectBotMidLayout->setAlignment (bBounds[SOUTH_BOUNDS], Qt::AlignCenter);


  bBounds[SW_BOUNDS] = new QPushButton (this);
  bBounds[SW_BOUNDS]->setCheckable (true);
  bBounds[SW_BOUNDS]->setIcon (QIcon (":/icons/compass_SW.png"));
  bBounds[SW_BOUNDS]->setWhatsThis (SWText);
  boundsGroup->addButton (bBounds[SW_BOUNDS], SW_BOUNDS);
  rectBotLayout->addWidget (bBounds[SW_BOUNDS]);

  rectBotLayout->addStretch (10);

  south = new QLineEdit (this);
  south->setToolTip (tr ("Set the southern latitude boundary of the area"));
  south->setWhatsThis (southText);
  ltstring = qFixpos (options.cache_mbr.min_y, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);
  south->setText (ltstring);
  connect (south, SIGNAL (editingFinished ()), this, SLOT (slotSouthEditingFinished ()));
  connect (south, SIGNAL (returnPressed ()), this, SLOT (slotSouthEditingFinished ()));
  rectBotLayout->addWidget (south);

  rectBotLayout->addStretch (10);

  bBounds[SE_BOUNDS] = new QPushButton (this);
  bBounds[SE_BOUNDS]->setCheckable (true);
  bBounds[SE_BOUNDS]->setIcon (QIcon (":/icons/compass_SE.png"));
  bBounds[SE_BOUNDS]->setWhatsThis (SEText);
  boundsGroup->addButton (bBounds[SE_BOUNDS], SE_BOUNDS);
  rectBotLayout->addWidget (bBounds[SE_BOUNDS]);


  rectAreaBoxLayout->addWidget (rectBox);


  QGroupBox *rectAreaInfoBox = new QGroupBox (tr ("Cache build area information"), this);
  QVBoxLayout *rectAreaInfoBoxLayout = new QVBoxLayout;
  rectAreaInfoBox->setLayout (rectAreaInfoBoxLayout);
  QHBoxLayout *rectAreaInfoTopLayout = new QHBoxLayout;
  QHBoxLayout *rectAreaInfoMidLayout = new QHBoxLayout;
  QHBoxLayout *rectAreaInfoBotLayout = new QHBoxLayout;
  rectAreaInfoBoxLayout->addLayout (rectAreaInfoTopLayout);
  rectAreaInfoBoxLayout->addLayout (rectAreaInfoMidLayout);
  rectAreaInfoBoxLayout->addLayout (rectAreaInfoBotLayout);

  rectAreaInfoBox->setToolTip (tr ("Information about the size of the area and estimated duration of the cache build process"));
  rectAreaInfoBox->setWhatsThis (tr ("Information about the size of the area and estimated duration of the cache build process"));


  misc.meterWidth = new QLabel (tr ("Width = 000000.0 meters"), this);
  misc.meterWidth->setToolTip (tr ("This is the width of the area in meters"));
  misc.meterWidth->setWhatsThis (meterWidthText);
  rectAreaInfoTopLayout->addWidget (misc.meterWidth);
  rectAreaInfoTopLayout->setAlignment (misc.meterWidth, Qt::AlignCenter);

  misc.meterHeight = new QLabel (tr ("Height = 000000.0 meters"), this);
  misc.meterHeight->setToolTip (tr ("This is the height of the area in meters"));
  misc.meterHeight->setWhatsThis (meterHeightText);
  rectAreaInfoMidLayout->addWidget (misc.meterHeight);
  rectAreaInfoMidLayout->setAlignment (misc.meterHeight, Qt::AlignCenter);

  misc.rectEstTime = new QLabel (tr ("Estimated time to build = 00:00:00"), this);
  misc.rectEstTime->setToolTip (tr ("This is the estimated amount of time it will take to build the cache"));
  misc.rectEstTime->setWhatsThis (rectEstTimeText);
  rectAreaInfoBotLayout->addWidget (misc.rectEstTime);
  rectAreaInfoBotLayout->setAlignment (misc.rectEstTime, Qt::AlignCenter);


  rectAreaBoxLayout->addWidget (rectAreaInfoBox, 10);



  shapeTab->addTab (rectAreaBox, tr ("Rectangle"));
  shapeTab->setTabToolTip (RECT_TAB, tr ("Set Google Earth cache area to a rectangle"));
  shapeTab->setTabWhatsThis (RECT_TAB, "Set Google Earth cache area to a geographic rectangle.");


  QGroupBox *polyAreaBox = new QGroupBox ();
  QHBoxLayout *polyAreaBoxLayout = new QHBoxLayout;
  polyAreaBox->setLayout (polyAreaBoxLayout);

  QGroupBox *polyBox = new QGroupBox (tr ("Cache/preview area"), this);
  QVBoxLayout *polyBoxLayout = new QVBoxLayout;
  polyBox->setLayout (polyBoxLayout);
  QHBoxLayout *polyTopLayout = new QHBoxLayout;
  QHBoxLayout *polyBotLayout = new QHBoxLayout;
  polyBoxLayout->addLayout (polyTopLayout);
  polyBoxLayout->addLayout (polyBotLayout);

  polyBox->setToolTip (tr ("Set the latitude and longitude polygon for the area you would like to cache or preview"));
  polyBox->setWhatsThis (tr ("Set the latitude and longitude polygon for the area you would like to cache or preview"));



  bPoly = new QPushButton (this);
  bPoly->setIcon (QIcon (":/icons/add_poly.png"));
  bPoly->setWhatsThis (polyText);
  bPoly->setCheckable (false);
  connect (bPoly, SIGNAL (clicked ()), this, SLOT (slotPolyClicked ()));
  polyTopLayout->addWidget (bPoly);

  bClosePoly = new QPushButton (this);
  bClosePoly->setIcon (QIcon (":/icons/close_poly.png"));
  bClosePoly->setWhatsThis (closePolyText);
  bClosePoly->setCheckable (false);
  connect (bClosePoly, SIGNAL (clicked ()), this, SLOT (slotClosePolyClicked ()));
  polyTopLayout->addWidget (bClosePoly);

  bClearPoly = new QPushButton (this);
  bClearPoly->setIcon (QIcon (":/icons/clear_poly.png"));
  bClearPoly->setWhatsThis (clearPolyText);
  bClearPoly->setCheckable (false);
  connect (bClearPoly, SIGNAL (clicked ()), this, SLOT (slotClearPolyClicked ()));
  polyTopLayout->addWidget (bClearPoly);


  QGroupBox *vertexBox = new QGroupBox (tr ("Polygon points"), this);
  QHBoxLayout *vertexBoxLayout = new QHBoxLayout;
  vertexBox->setLayout (vertexBoxLayout);

  vertices = new QListWidget (this);
  vertices->setSelectionMode (QAbstractItemView::SingleSelection);
  vertices->setWhatsThis (verticesText);
  vertexBoxLayout->addWidget (vertices);


  //  Populate the polygon vertices if any were saved and restored.

  for (uint32_t i = 0 ; i < options.polygon.size () ; i++)
    {
      ltstring = qFixpos (options.polygon[i].y, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);
      lnstring = qFixpos (options.polygon[i].x, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);
      vertices->addItem (ltstring + " " + lnstring);
    }


  polyBotLayout->addWidget (vertexBox, 10);


  polyAreaBoxLayout->addWidget (polyBox);


  QGroupBox *polyAreaInfoBox = new QGroupBox (tr ("Cache build area information"), this);
  QVBoxLayout *polyAreaInfoBoxLayout = new QVBoxLayout;
  polyAreaInfoBox->setLayout (polyAreaInfoBoxLayout);
  QHBoxLayout *polyAreaInfoTopLayout = new QHBoxLayout;
  QHBoxLayout *polyAreaInfoBotLayout = new QHBoxLayout;
  polyAreaInfoBoxLayout->addLayout (polyAreaInfoTopLayout);
  polyAreaInfoBoxLayout->addLayout (polyAreaInfoBotLayout);

  polyAreaInfoBox->setToolTip (tr ("Information about the size of the area and estimated duration of the cache build process"));
  polyAreaInfoBox->setWhatsThis (tr ("Information about the size of the area and estimated duration of the cache build process"));


  misc.numBoxes = new QLabel (tr ("Number of areas = 00000"), this);
  misc.numBoxes->setToolTip (tr ("This is the number of areas used to completely cover the polygon"));
  misc.numBoxes->setWhatsThis (numBoxesText);
  polyAreaInfoTopLayout->addWidget (misc.numBoxes);
  polyAreaInfoTopLayout->setAlignment (misc.numBoxes, Qt::AlignCenter);

  misc.polyEstTime = new QLabel (tr ("Estimated time to build = 00:00:00"), this);
  misc.polyEstTime->setToolTip (tr ("This is the estimated amount of time it will take to build the cache"));
  misc.polyEstTime->setWhatsThis (polyEstTimeText);
  polyAreaInfoBotLayout->addWidget (misc.polyEstTime);
  polyAreaInfoBotLayout->setAlignment (misc.polyEstTime, Qt::AlignCenter);


  polyAreaBoxLayout->addWidget (polyAreaInfoBox, 10);


  shapeTab->addTab (polyAreaBox, tr ("Polygon"));
  shapeTab->setTabToolTip (POLY_TAB, tr ("Set Google Earth cache area to a polygon"));
  shapeTab->setTabWhatsThis (POLY_TAB, "Set Google Earth cache area to a geographic polygon.");



  QHBoxLayout *cacheOpBoxLayout = new QHBoxLayout;
  cacheBoxLayout->addLayout (cacheOpBoxLayout);

  QGroupBox *bsBox = new QGroupBox (tr ("Cache build initial area size"), this);
  bsBox->setToolTip (tr ("Change the size (in meters) of the smallest area used for building the cache"));
  bsBox->setWhatsThis (boxSizeText);
  QHBoxLayout *bsBoxLayout = new QHBoxLayout;
  bsBox->setLayout (bsBoxLayout);
  
  boxSize = new QSpinBox (bsBox);
  boxSize->setRange (500, 20000);
  boxSize->setSingleStep (1000);
  boxSize->setWhatsThis (boxSizeText);
  boxSize->setValue (options.build_box_size);
  connect (boxSize, SIGNAL (valueChanged (int)), this, SLOT (slotBoxSizeChanged (int)));
  bsBoxLayout->addWidget (boxSize);
  cacheOpBoxLayout->addWidget (bsBox);


  QGroupBox *cufBox = new QGroupBox (tr ("Cache build update frequency"), this);
  cufBox->setToolTip (tr ("Change the frequency (in seconds) for the cache build process"));
  cufBox->setWhatsThis (cacheUpdateText);
  QHBoxLayout *cufBoxLayout = new QHBoxLayout;
  cufBox->setLayout (cufBoxLayout);

  cacheUpdate = new QSpinBox (cufBox);
  cacheUpdate->setRange (4, 60);
  cacheUpdate->setSingleStep (1);
  cacheUpdate->setWhatsThis (cacheUpdateText);
  cacheUpdate->setValue (options.cache_update_frequency);
  connect (cacheUpdate, SIGNAL (valueChanged (int)), this, SLOT (slotCacheUpdateChanged (int)));
  cufBoxLayout->addWidget (cacheUpdate);
  cacheOpBoxLayout->addWidget (cufBox);



  QHBoxLayout *loadBoxLayout = new QHBoxLayout;
  cacheBoxLayout->addLayout (loadBoxLayout);

  bBuildCache = new QPushButton (tr ("Build cache"), this);
  bBuildCache->setWhatsThis (buildCacheText);
  connect (bBuildCache, SIGNAL (clicked ()), this, SLOT (slotBuildCache ()));
  loadBoxLayout->addWidget (bBuildCache);


  //  Set the button colors for buttons with active/inactive processes.

  QPalette buttonPalette = bBuildCache->palette ();
  buttonBackgroundColor = buttonPalette.color (QPalette::Normal, QPalette::Button);
  buttonTextColor = buttonPalette.color (QPalette::Normal, QPalette::ButtonText);

  normalTextColorString = QString ("color: rgba(%1,%2,%3,%4); ").arg (buttonTextColor.red ()).arg (buttonTextColor.green ()).arg (buttonTextColor.blue ()).arg
    (buttonTextColor.alpha ());


  int32_t hue, sat, val;
  options.warning_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      warningTextColorString = "color: white; ";
    }
  else
    {
      warningTextColorString = "color: black; ";
    }


  bSaveCache = new QPushButton (tr ("Save cache"), this);
  bSaveCache->setWhatsThis (saveCacheText);
  connect (bSaveCache, SIGNAL (clicked ()), this, SLOT (slotSaveCacheClicked ()));
  loadBoxLayout->addWidget (bSaveCache);

  bLoadCache = new QPushButton (tr ("Load cache"), this);
  bLoadCache->setWhatsThis (loadCacheText);
  connect (bLoadCache, SIGNAL (clicked ()), this, SLOT (slotLoadCacheClicked ()));
  loadBoxLayout->addWidget (bLoadCache);


  progBox = new QGroupBox (tr ("Cache build progress"), this);
  progBox->setToolTip (tr ("This is the progress bar for the cache build process"));
  progBox->setWhatsThis (progressText);
  QHBoxLayout *progBoxLayout = new QHBoxLayout;
  progBox->setLayout (progBoxLayout);

  progress = new QProgressBar (this);
  progress->setToolTip (tr ("This is the progress bar for the cache build process"));
  progress->setWhatsThis (progressText);
  progress->setRange (0, 100);
  progBoxLayout->addWidget (progress);

  cacheBoxLayout->addWidget (progBox);


  geCacheTab->addTab (cacheBox, tr ("Cache"));
  geCacheTab->setTabToolTip (CACHE_TAB, tr ("Modify cache data in Google Earth"));
  geCacheTab->setTabWhatsThis (CACHE_TAB, "This tab is where you can preview areas in Google Earth and build/save/load cache data for Google Earth.");



  //  ******************************************* Preferences tab *********************************************

  QGroupBox *prefBox = new QGroupBox (this);
  QVBoxLayout *prefBoxLayout = new QVBoxLayout;
  prefBox->setLayout (prefBoxLayout);


  QGroupBox *fbox = new QGroupBox (tr ("Position Format"), this);
  fbox->setToolTip (tr ("Change the position format used throughout geCache"));
  fbox->setWhatsThis (positionFormatText);

  QRadioButton *hdms = new QRadioButton (tr ("Hemisphere Degrees Minutes Seconds.decimal"));
  QRadioButton *hdm_ = new QRadioButton (tr ("Hemisphere Degrees Minutes.decimal"));
  QRadioButton *hd__ = new QRadioButton (tr ("Hemisphere Degrees.decimal"));
  QRadioButton *sdms = new QRadioButton (tr ("+/-Degrees Minutes Seconds.decimal"));
  QRadioButton *sdm_ = new QRadioButton (tr ("+/-Degrees Minutes.decimal"));
  QRadioButton *sd__ = new QRadioButton (tr ("+/-Degrees.decimal"));

  hdms->setToolTip (tr ("Change the position format used throughout geCache"));
  hdm_->setToolTip (tr ("Change the position format used throughout geCache"));
  hd__->setToolTip (tr ("Change the position format used throughout geCache"));
  sdms->setToolTip (tr ("Change the position format used throughout geCache"));
  sdm_->setToolTip (tr ("Change the position format used throughout geCache"));
  sd__->setToolTip (tr ("Change the position format used throughout geCache"));

  hdms->setWhatsThis (positionFormatText);
  hdm_->setWhatsThis (positionFormatText);
  hd__->setWhatsThis (positionFormatText);
  sdms->setWhatsThis (positionFormatText);
  sdm_->setWhatsThis (positionFormatText);
  sd__->setWhatsThis (positionFormatText);

  bGrp = new QButtonGroup (this);
  bGrp->setExclusive (true);
  connect (bGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotPositionClicked (int)));

  bGrp->addButton (hdms, 0);
  bGrp->addButton (hdm_, 1);
  bGrp->addButton (hd__, 2);
  bGrp->addButton (sdms, 3);
  bGrp->addButton (sdm_, 4);
  bGrp->addButton (sd__, 5);

  QHBoxLayout *fboxSplit = new QHBoxLayout;
  QVBoxLayout *fboxLeft = new QVBoxLayout;
  QVBoxLayout *fboxRight = new QVBoxLayout;
  fboxSplit->addLayout (fboxLeft);
  fboxSplit->addLayout (fboxRight);
  fboxLeft->addWidget (hdms);
  fboxLeft->addWidget (hdm_);
  fboxLeft->addWidget (hd__);
  fboxRight->addWidget (sdms);
  fboxRight->addWidget (sdm_);
  fboxRight->addWidget (sd__);
  fbox->setLayout (fboxSplit);

  prefBoxLayout->addWidget (fbox, 1);


  bGrp->button (options.position_form)->setChecked (true);


  QHBoxLayout *googleBoxLayout = new QHBoxLayout;
  prefBoxLayout->addLayout (googleBoxLayout);

  QGroupBox *gnBox = new QGroupBox (tr ("Google Earth Name"), this);
  gnBox->setToolTip (tr ("Set the name of the executable or script that runs Google Earth on your system"));
  gnBox->setWhatsThis (geNameText);
  QHBoxLayout *gnBoxLayout = new QHBoxLayout;
  gnBox->setLayout (gnBoxLayout);

  geName = new QLineEdit (this);
  geName->setToolTip (tr ("Set the name of the executable or script that runs Google Earth on your system"));
  geName->setWhatsThis (geNameText);
  geName->setText (options.ge_name);
  connect (geName, SIGNAL (editingFinished ()), this, SLOT (slotGeNameEditingFinished ()));
  gnBoxLayout->addWidget (geName);
  googleBoxLayout->addWidget (gnBox);

  QGroupBox *gcBox = new QGroupBox (tr ("Google Earth Cache"), this);
  gcBox->setToolTip (tr ("Set the name of the directory that contains the Google Earth cache data"));
  gcBox->setWhatsThis (geCacheDirText);
  QHBoxLayout *gcBoxLayout = new QHBoxLayout;
  gcBox->setLayout (gcBoxLayout);

  bCacheBrowse = new QPushButton (this);
  bCacheBrowse->setIcon (QIcon (":/icons/fileopen.png"));
  bCacheBrowse->setToolTip (tr ("Browse for the directory that contains the Google Earth cache data"));
  bCacheBrowse->setWhatsThis (bCacheBrowseText);
  connect (bCacheBrowse, SIGNAL (clicked ()), this, SLOT (slotCacheBrowseClicked ()));
  gcBoxLayout->addWidget (bCacheBrowse);

  geCacheDir = new QLabel (this);
  geCacheDir->setToolTip (tr ("The name of the directory that contains the Google Earth cache data"));
  geCacheDir->setWhatsThis (geCacheDirText);
  geCacheDir->setText (options.ge_dir);
  gcBoxLayout->addWidget (geCacheDir, 10);

  googleBoxLayout->addWidget (gcBox, 10);


  QHBoxLayout *opBoxLayout = new QHBoxLayout;
  prefBoxLayout->addLayout (opBoxLayout);


  QGroupBox *colorBox = new QGroupBox (tr ("Warning color"), this);
  colorBox->setToolTip (tr ("Change warning color used for buttons with active processes"));
  colorBox->setWhatsThis (warningColorText);
  QHBoxLayout *colorBoxLayout = new QHBoxLayout;
  colorBox->setLayout (colorBoxLayout);

  bWarningColor = new QPushButton (colorBox);
  bWarningColor->setToolTip (tr ("Change warning color used for buttons with active processes"));
  bWarningColor->setWhatsThis (warningColorText);
  connect (bWarningColor, SIGNAL (clicked ()), this, SLOT (slotWarningColor ()));
  colorBoxLayout->addWidget (bWarningColor);
  opBoxLayout->addWidget (colorBox);

  QGroupBox *fontBox = new QGroupBox (tr ("Font"), this);
  fontBox->setToolTip (tr ("Change font for geCache"));
  fontBox->setWhatsThis (fontText);
  QHBoxLayout *fontBoxLayout = new QHBoxLayout;
  fontBox->setLayout (fontBoxLayout);

  bFont = new QPushButton (fontBox);
  bFont->setToolTip (tr ("Change font for geCache"));
  bFont->setWhatsThis (fontText);
  bFont->setText (options.font.toString ());
  connect (bFont, SIGNAL (clicked ()), this, SLOT (slotFont ()));
  fontBoxLayout->addWidget (bFont);
  opBoxLayout->addWidget (fontBox);


  QGroupBox *iconBox = new QGroupBox (tr ("Button icon size"), this);
  iconBox->setToolTip (tr ("Set the size (in pixels) of the button icons"));
  iconBox->setWhatsThis (iconSizeText);
  QHBoxLayout *iconBoxLayout = new QHBoxLayout;
  iconBox->setLayout (iconBoxLayout);
  
  iconSize = new QComboBox (iconBox);
  iconSize->setToolTip (tr ("Set the size (in pixels) of the button icons"));
  iconSize->setWhatsThis (iconSizeText);
  iconSize->setEditable (false);
  iconSize->addItem ("16");
  iconSize->addItem ("20");
  iconSize->addItem ("24");
  iconSize->addItem ("28");
  iconSize->addItem ("32");
  iconBoxLayout->addWidget (iconSize);
  opBoxLayout->addWidget (iconBox);

  switch (options.icon_size)
    {
    case 16:
      iconSize->setCurrentIndex (0);
      break;

    case 20:
      iconSize->setCurrentIndex (1);
      break;

    case 24:
    default:
      iconSize->setCurrentIndex (2);
      break;

    case 28:
      iconSize->setCurrentIndex (3);
      break;

    case 32:
      iconSize->setCurrentIndex (4);
      break;
    }
  connect (iconSize, SIGNAL (currentIndexChanged (int)), this, SLOT (slotIconSizeChanged (int)));


  geCacheTab->addTab (prefBox, tr ("Preferences"));
  geCacheTab->setTabToolTip (PREF_TAB, tr ("Set geCache preferences"));
  geCacheTab->setTabWhatsThis (PREF_TAB, tr ("This tab is used to modify geCache preferences."));


  mainBox->addWidget (geCacheTab);



  //  ********************************************** Actions **************************************************

  QHBoxLayout *actions = new QHBoxLayout (0);
  mainBox->addLayout (actions);

  QPushButton *bHelp = new QPushButton (this);
  bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
  bHelp->setToolTip (tr ("Enter What's This mode for help"));
  connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
  actions->addWidget (bHelp);

  actions->addStretch (10);

  QPushButton *quitButton = new QPushButton (tr ("&Quit"), this);
  quitButton->setToolTip (tr ("Exit from geCache [Ctrl+q]"));
  quitButton->setWhatsThis (closeText);
  quitButton->setShortcut (tr ("ctrl+q"));
  connect (quitButton, SIGNAL (clicked ()), this, SLOT (slotQuit ()));
  actions->addWidget (quitButton);


  setWidgetStates ();


  //  Connect to the clipboard so we can snag Google Earth Ctrl-Shift-C position data.

  clipboard = qApp->clipboard ();


  //  I would have preferred to use the "changed" signal but it only worked on Linux (the documentation says it shouldn't work anywhere).  Ah well, I'll check 
  //  the clipboard in the timer (slotGeCacheTimer) if one of the bounds buttons or the add polygon position button has been pressed (bounds_clicked != NO_BOUNDS).

  //connect (clipboard, SIGNAL (changed (QClipboard::Mode)), this, SLOT (slotClipboardChanged (QClipboard::Mode)));


  //  Set the cache timer to every 500 milliseconds (for the mathematically challenged, that's 1/2 second).

  geCacheTimer = new QTimer (this);
  connect (geCacheTimer, SIGNAL (timeout ()), this, SLOT (slotGeCacheTimer ()));
  geCacheTimer->start (500);


  //  Compute the size of the cache box in meters.

  computeSize (&misc, &options);


  //  Google Earth warning

  if (!misc.googleearth_available)
    {
      QMessageBox::warning (this, tr ("geCache"),
                            tr ("Can't find Google Earth (%1) in your PATH.  Make sure the <b>Google Earth Name</b> field in the ").arg (options.ge_name) +
                            tr ("<b>Preferences</b> tab is correct and that the directory containing that file is included in your PATH environment variable."));

      geCacheTab->setCurrentIndex (PREF_TAB);
    }
  else
    {
      //  Set the current tab to the last used tab.

      geCacheTab->setCurrentIndex (options.start_tab);
      shapeTab->setCurrentIndex (options.shape_tab);
    }

  connect (geCacheTab, SIGNAL (currentChanged (int)), this, SLOT (slotGeCacheTabChanged (int)));
  connect (shapeTab, SIGNAL (currentChanged (int)), this, SLOT (slotShapeTabChanged (int)));
}



geCache::~geCache ()
{
}



void geCache::slotGeCacheTabChanged (int tab)
{
  options.start_tab = tab;
}



void geCache::slotShapeTabChanged (int tab)
{
  options.shape_tab = tab;
  computeSize (&misc, &options);


  //  If we switched tabs we may need to redraw (assuming we're linked, which will be checked in positionGoogleEarth).

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running) positionGoogleEarth ();
}



//  Gets the latest entry in the global clipboard.  Looking for Google Earth lat/lon coordinates.

void geCache::getClipboard ()
{
  QString lat_text, lon_text, ltstring, lnstring;
  double lat_degs, lon_degs, deg, min, sec;
  char hem;


  QString text = clipboard->text ().simplified ();


  //  If there aren't two degree symbols then something else got slapped into the clipboard.

  if (text.count (degC) != 2) return;


  //  Instead of clearing the clipboard I'll just make sure that I don't use the exact same string more than once.  If the "changed" signal worked in Windows
  //  I wouldn't have to do this.

  if (text == prev_clipboard_text) return;
  prev_clipboard_text = text;


  //  If the string contains S or N then it has hemisphere and, thus, 3 spaces instead of 1.

  if (text.contains ('N') || text.contains ('S'))
    {
      lat_text = text.section (' ', 0, 1);
      lon_text = text.section (' ', 2, 3);
    }
  else
    {
      lat_text = text.section (' ', 0, 0);
      lon_text = text.section (' ', 1, 1);
    }

  qPosfix (lat_text, &lat_degs, QPOS_LAT);
  ltstring = qFixpos (lat_degs, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);
  qPosfix (lon_text, &lon_degs, QPOS_LON);
  lnstring = qFixpos (lon_degs, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);

  if (poly_define || poly_edit)
    {
      if (poly_define)
        {
          vertices->addItem (ltstring + " " + lnstring);

          NV_F64_COORD2 pnt = {lon_degs, lat_degs};
          options.polygon.push_back (pnt);
        }
      else
        {
          //  If poly_edit is 1 we want to find the nearest point in the vertex list, highlight it in the list, set poly_edit to 2,
          //  then wait for the next point in the clipboard.

          if (poly_edit == 1)
            {
              double min_dist = 999.0;

              for (uint32_t i = 0 ; i < options.polygon.size () ; i++)
                {
                  double y_diff = (options.polygon[i].y - lat_degs);
                  double x_diff = (options.polygon[i].x - lon_degs);
                  double dist = sqrt (y_diff * y_diff + x_diff *x_diff);

                  if (dist < min_dist)
                    {
                      min_dist = dist;
                      poly_edit_index = i;
                    }
                }

              poly_edit = 2;

              vertices->item (poly_edit_index)->setSelected (true);
              vertices->scrollToItem (vertices->item (poly_edit_index));
            }
          else
            {
              options.polygon[poly_edit_index].y = lat_degs;
              options.polygon[poly_edit_index].x = lon_degs;

              poly_edit = 0;

              vertices->item (poly_edit_index)->setSelected (false);

              poly_edit_index = 0;

              vertices->scrollToItem (vertices->item (poly_edit_index));
            }
        }
    }
  else
    {
      switch (bounds_clicked)
        {
        case NO_BOUNDS:
          break;


        case NW_BOUNDS:
          north->setText (ltstring);
          west->setText (lnstring);
          options.cache_mbr.max_y = lat_degs;
          options.cache_mbr.min_x = lon_degs;
          break;


        case NE_BOUNDS:
          north->setText (ltstring);
          east->setText (lnstring);
          options.cache_mbr.max_y = lat_degs;
          options.cache_mbr.max_x = lon_degs;
          break;


        case SW_BOUNDS:
          south->setText (ltstring);
          west->setText (lnstring);
          options.cache_mbr.min_y = lat_degs;
          options.cache_mbr.min_x = lon_degs;
          break;


        case SE_BOUNDS:
          south->setText (ltstring);
          east->setText (lnstring);
          options.cache_mbr.min_y = lat_degs;
          options.cache_mbr.max_x = lon_degs;
          break;


        case NORTH_BOUNDS:
          north->setText (ltstring);
          options.cache_mbr.max_y = lat_degs;
          break;


        case WEST_BOUNDS:
          west->setText (lnstring);
          options.cache_mbr.min_x = lon_degs;
          break;


        case EAST_BOUNDS:
          east->setText (lnstring);
          options.cache_mbr.max_x = lon_degs;
          break;


        case SOUTH_BOUNDS:
          south->setText (ltstring);
          options.cache_mbr.min_y = lat_degs;
          break;
        }

      bounds_clicked = NO_BOUNDS;


      for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setChecked (false);


      computeSize (&misc, &options);
    }


  setWidgetStates ();


  //  If we're running Google Earth for preview, we're allowing changes to take effect even if we're linked (scary, nyet).

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running) positionGoogleEarth ();
}



//  Timer - timeout signal.

void
geCache::slotGeCacheTimer ()
{
  QString            ltstring, lnstring, geo_string, string;


  int64_t sizeDir (const QString &source);
  uint8_t copyDir (const QString &source, const QString &dest);


  //  I'm using this instead of the "changed" signal (since it doesn't work on Windows).  Basically, the user clicked one of the bounds buttons on the Cache
  //  tab so corner_clicked got set to 1 = NW, 2 = NE, 3 = SW, or 4 = SE.

  if (bounds_clicked) getClipboard ();


  //  If a cache build is running...

  if (buildGoogleEarthProc && buildGoogleEarthProc->state () == QProcess::Running)
    {
      //  We want to wait 20 seconds (40 counts) after we first start Google Earth just to make sure that it has settled down.

      if (!build_start_flag || misc.second_count > 40)
        {
          build_start_flag = NVFalse;

          if ((misc.second_count / 2) >= options.cache_update_frequency)
            {
              misc.second_count = -1;

              int64_t cache_size = sizeDir (options.ge_dir);
              float size_num = 0;
              QString sizeStr;


              //  We're too close to the max cache size so we need to offer the user a chance to save cache and continue.

              if (cache_size >= 2100000000)
                {
                  QMessageBox msgBox;
                  msgBox.setText (tr ("The cache directory has almost reached maximum size."));
                  msgBox.setInformativeText (tr ("Do you want to save the cache directory and continue to build or cancel the build process?"));
                  msgBox.setStandardButtons (QMessageBox::Save | QMessageBox::Cancel);
                  msgBox.setDefaultButton (QMessageBox::Save);
                  int ret = msgBox.exec ();

                  switch (ret)
                    {
                    case QMessageBox::Save:

                      slotSaveCacheClicked ();

                      
                      //  Remove the Google Earth cache directory.

                      QDir (options.ge_dir).removeRecursively ();


                      //  Copy the snapshot back into the cache directory

                      copyDir (cache_snapshot, options.ge_dir);


                      //  Reset the build MBR to the west side of the current row.

                      build_direction = 0;
                      misc.view_area_mbr.min_x = misc.build_area_mbr.min_x;
                      misc.view_area_mbr.max_x = misc.view_area_mbr.min_x + misc.box_size_x_deg;



                      //  Check the box for overlap if we're doing a polygon.

                      if (misc.poly_flag)
                        {
                          NV_F64_COORD2 mbr_poly[4];

                          mbr_poly[0].x = misc.view_area_mbr.min_x;
                          mbr_poly[0].y = misc.view_area_mbr.min_y;
                          mbr_poly[1].x = misc.view_area_mbr.min_x;
                          mbr_poly[1].y = misc.view_area_mbr.max_y;
                          mbr_poly[2].x = misc.view_area_mbr.max_x;
                          mbr_poly[2].y = misc.view_area_mbr.max_y;
                          mbr_poly[3].x = misc.view_area_mbr.max_x;
                          mbr_poly[3].y = misc.view_area_mbr.min_y;

                          int32_t poly_size = (int32_t) options.polygon.size ();

                          if (!polygon_intersection (mbr_poly, 4, options.polygon.data (), poly_size)) misc.view_flag = NVFalse;
                        }


                      //  Subtract the count of the boxes we've completed on this row (because we're going to do them again).

                      iteration_count -= x_box_count;
                      x_box_count = 0;
                      break;


                    case QMessageBox::Cancel:

                      killBuildGoogleEarth ();
                      return;
                      break;
                    }
                }
              else if (cache_size > 1073741824)
                {
                  size_num = (double) cache_size / 1073741824.0;
                  sizeStr.sprintf ("%.1fG", size_num);
                }
              else
                {
                  size_num = (double) cache_size / 1048576.0;
                  sizeStr.sprintf ("%.1fM", size_num);
                }


              progress->setValue (iteration_count++);


              boxes_remaining--;


              int32_t remaining = (boxes_remaining + 2) * options.cache_update_frequency;


              //  Wait twice the update frequency with the box reset to the whole area.

              if (build_kill_flag || remaining <= 0) remaining = 2 * options.cache_update_frequency;


              int32_t hour = remaining / 3600;
              int32_t minute = (remaining / 60) % 60;
              int32_t second = remaining % 60;

              progBox->setTitle (tr ("Cache build progress - Estimated time remaining - %1:%2:%3 - Cache size %4").arg (hour, 2, 10, zero).arg (minute, 2, 10, zero).arg
                                 (second, 2, 10, zero).arg (sizeStr));

              qApp->processEvents ();


              positionBuildGoogleEarth ();


              //  Check for the kill flag (which will be set by positionBuildGoogleEarth when we exceed the northern bounds).

              if (build_kill_flag)
                {
                  //  This will position the view over the entire area.

                  positionBuildGoogleEarth ();


                  if (misc.poly_flag)
                    {
                      progress->setValue (misc.poly_iterations - 1);
                    }
                  else
                    {
                      progress->setValue (misc.iterations - 1);
                    }
                  qApp->processEvents ();


#ifdef _MSC_VER

                  int32_t wait_time = remaining * 1000;
                  Sleep (wait_time);

#else

                  int32_t wait_time = remaining;
                  sleep (wait_time);

#endif

                  killBuildGoogleEarth ();


                  QMessageBox msgBox;
                  msgBox.setText (tr ("The cache build has finished."));
                  msgBox.setInformativeText (tr ("Do you want to save the cache directory?"));
                  msgBox.setStandardButtons (QMessageBox::Save | QMessageBox::Cancel);
                  msgBox.setDefaultButton (QMessageBox::Save);
                  int ret = msgBox.exec ();

                  switch (ret)
                    {
                    case QMessageBox::Save:
                      slotSaveCacheClicked ();
                      break;

                    case QMessageBox::Cancel:
                      break;
                    }
                }
            }
        }

      misc.second_count++;
    }
}



void
geCache::keyPressEvent (QKeyEvent *e)
{
  //  I have no idea why this is happening but the damn bBuildCache button is snagging keyboard focus.  It's not even the last
  //  widget built on that tab!  I've tried setting focus to other widgets and that doesn't change anything.  I don't want the
  //  build cache button getting pushed if you happen to press the enter key so I'm throwing it away.  I'm setting it up as a
  //  switch because I might use it for something else later.

  switch (e->key ())
    {
    case Qt::Key_Return:
      if (bBuildCache->hasFocus ()) return;
      break;
    }

  e->accept ();
}



void 
geCache::slotBoundsClicked (int id)
{
  //  While one bounds retrieval is active we just want to discard any other bounds button clicks.

  if (bounds_clicked != NO_BOUNDS && bounds_clicked != id)
    {
      bBounds[id]->setChecked (false);
      return;
    }


  //  This makes sure that we don't accidentally ingest something that has been sitting in the clipboard.

  prev_clipboard_text = clipboard->text ().simplified ();


  if (bBounds[id]->isChecked ())
    {
      bounds_clicked = id;
    }
  else
    {
      bounds_clicked = NO_BOUNDS;
    }


  setWidgetStates ();
}



void 
geCache::slotNorthEditingFinished ()
{
  char hem;
  QString ltstring;
  double deg, min, sec;


  QString text = north->text ().simplified ();


  //  If this was pasted from Google Earth there will be two degree symbols.  If so, we want to strip off the longitude and then process it.
  //  This is just in case someone wanted to do it the hard way instead of using the boundary buttons.

  if (text.count (degC) == 2)
    {
      QString lat_text;


      //  If the string contains S or N then it's has hemisphere and, thus, 3 spaces instead of 1.

      if (text.contains ('N') || text.contains ('S'))
        {
          lat_text = text.section (' ', 0, 1);
        }
      else
        {
          lat_text = text.section (' ', 0, 0);
        }

      text = lat_text;
    }


  qPosfix (text, &options.cache_mbr.max_y, QPOS_LAT);

  ltstring = qFixpos (options.cache_mbr.max_y, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);

  north->setText (ltstring);

  computeSize (&misc, &options);


  //  If we're running Google Earth for preview, we're allowing changes to take effect even if we're linked (scary, nyet).

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running) positionGoogleEarth ();
}



void 
geCache::slotWestEditingFinished ()
{
  char hem;
  QString lnstring;
  double deg, min, sec;


  QString text = west->text ().simplified ();


  //  If this was pasted from Google Earth there will be two degree symbols.  If so, we want to strip off the latitude and then process it.
  //  This is just in case someone wanted to do it the hard way instead of using the boundary buttons.

  if (text.count (degC) == 2)
    {
      QString lon_text;


      //  If the string contains S or N then it's has hemisphere and, thus, 3 spaces instead of 1.

      if (text.contains ('N') || text.contains ('S'))
        {
          lon_text = text.section (' ', 2, 3);
        }
      else
        {
          lon_text = text.section (' ', 1, 1);
        }

      text = lon_text;
    }


  qPosfix (text, &options.cache_mbr.min_x, QPOS_LON);

  lnstring = qFixpos (options.cache_mbr.min_x, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);

  west->setText (lnstring);

  computeSize (&misc, &options);


  //  If we're running Google Earth for preview, we're allowing changes to take effect even if we're linked (scary, nyet).

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running) positionGoogleEarth ();
}



void 
geCache::slotEastEditingFinished ()
{
  char hem;
  QString lnstring;
  double deg, min, sec;


  QString text = east->text ().simplified ();


  //  If this was pasted from Google Earth there will be two degree symbols.  If so, we want to strip off the latitude and then process it.
  //  This is just in case someone wanted to do it the hard way instead of using the boundary buttons.

  if (text.count (degC) == 2)
    {
      QString lon_text;


      //  If the string contains S or N then it's has hemisphere and, thus, 3 spaces instead of 1.

      if (text.contains ('N') || text.contains ('S'))
        {
          lon_text = text.section (' ', 2, 3);
        }
      else
        {
          lon_text = text.section (' ', 1, 1);
        }

      text = lon_text;
    }


  qPosfix (text, &options.cache_mbr.max_x, QPOS_LON);

  lnstring = qFixpos (options.cache_mbr.max_x, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);

  east->setText (lnstring);

  computeSize (&misc, &options);


  //  If we're running Google Earth for preview, we're allowing changes to take effect even if we're linked (scary, nyet).

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running) positionGoogleEarth ();
}



void 
geCache::slotSouthEditingFinished ()
{
  char hem;
  QString ltstring;
  double deg, min, sec;


  QString text = south->text ().simplified ();


  //  If this was pasted from Google Earth there will be two degree symbols.  If so, we want to strip off the longitude and then process it.
  //  This is just in case someone wanted to do it the hard way instead of using the boundary buttons.

  if (text.count (degC) == 2)
    {
      QString lat_text;


      //  If the string contains S or N then it's has hemisphere and, thus, 3 spaces instead of 1.

      if (text.contains ('N') || text.contains ('S'))
        {
          lat_text = text.section (' ', 0, 1);
        }
      else
        {
          lat_text = text.section (' ', 0, 0);
        }

      text = lat_text;
    }


  qPosfix (text, &options.cache_mbr.min_y, QPOS_LAT);

  ltstring = qFixpos (options.cache_mbr.min_y, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);

  south->setText (ltstring);

  computeSize (&misc, &options);


  //  If we're running Google Earth for preview, we're allowing changes to take effect even if we're linked (scary, nyet).

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running) positionGoogleEarth ();
}



void 
geCache::slotPolyClicked ()
{
  //  If we are already defining a polygon disregard the button click.

  if (poly_define) return;


  //  If we already have a polygon defined we want to edit the polygon points.

  if (options.polygon.size ())
    {
      poly_edit = NVTrue;
    }
  else
    {
      poly_define = NVTrue;
    }


  setWidgetStates ();
}



void 
geCache::slotClosePolyClicked ()
{
  poly_define = NVFalse;


  setWidgetStates ();

  positionGoogleEarth ();


  //  Figure out how many boxes we have to scan.

  computeSize (&misc, &options);
}



void 
geCache::slotClearPolyClicked ()
{
  poly_define = NVFalse;
  options.polygon.clear ();
  vertices->clear ();


  positionGoogleEarth ();


  setWidgetStates ();
}



uint8_t 
geCache::positionGoogleEarth ()
{
  if ((ge_tmp_fp[1] = fopen (ge_tmp_name[1], "w")) == NULL)
    {
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Unable to open temporary Google Earth KML file!"));
      return (-1);
    }


  fprintf (ge_tmp_fp[1], "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf (ge_tmp_fp[1], "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
  fprintf (ge_tmp_fp[1], "  <Document>\n");


  //  Set up the box if we're linked

  if (misc.ge_linked)
    {
      //  This is the box

      fprintf (ge_tmp_fp[1], "    <Style id=\"Transparent\">\n");
      fprintf (ge_tmp_fp[1], "      <LineStyle>\n");
      fprintf (ge_tmp_fp[1], "        <width>1.5</width>\n");
      fprintf (ge_tmp_fp[1], "      </LineStyle>\n");
      fprintf (ge_tmp_fp[1], "      <PolyStyle>\n");
      fprintf (ge_tmp_fp[1], "        <color>00000000</color>\n");
      fprintf (ge_tmp_fp[1], "        <outline>1</outline>\n");
      fprintf (ge_tmp_fp[1], "        <fill>0</fill>\n");
      fprintf (ge_tmp_fp[1], "      </PolyStyle>\n");
      fprintf (ge_tmp_fp[1], "    </Style>\n");


      //  If we have a completely defined polygon and we're on the polygon tab we don't want to display the rectangle.  Conversely, if we are on the 
      //  rectangle tab or we're still defining the polygon (poly_define) or we don't have any polygon points we want to display the rectangle.

      if (options.shape_tab == RECT_TAB || (options.polygon.size () > 1 && poly_define) || options.polygon.size () == 0)
        {
          fprintf (ge_tmp_fp[1], "    <Placemark>\n");
          fprintf (ge_tmp_fp[1], "      <name>geCache displayed area</name>\n");
          fprintf (ge_tmp_fp[1], "      <styleUrl>#Transparent</styleUrl>\n");
          fprintf (ge_tmp_fp[1], "      <Polygon>\n");
          fprintf (ge_tmp_fp[1], "        <extrude>1</extrude>\n");
          fprintf (ge_tmp_fp[1], "        <tessellate>1</tessellate>\n");
          fprintf (ge_tmp_fp[1], "        <altitudeMode>clampToGround</altitudeMode>\n");
          fprintf (ge_tmp_fp[1], "        <outerBoundaryIs>\n");
          fprintf (ge_tmp_fp[1], "          <LinearRing>\n");
          fprintf (ge_tmp_fp[1], "            <coordinates>\n");
          fprintf (ge_tmp_fp[1], "              %.11f,%.11f,10\n", options.cache_mbr.min_x, options.cache_mbr.min_y);
          fprintf (ge_tmp_fp[1], "              %.11f,%.11f,10\n", options.cache_mbr.min_x, options.cache_mbr.max_y);
          fprintf (ge_tmp_fp[1], "              %.11f,%.11f,10\n", options.cache_mbr.max_x, options.cache_mbr.max_y);
          fprintf (ge_tmp_fp[1], "              %.11f,%.11f,10\n", options.cache_mbr.max_x, options.cache_mbr.min_y);
          fprintf (ge_tmp_fp[1], "              %.11f,%.11f,10\n", options.cache_mbr.min_x, options.cache_mbr.min_y);
          fprintf (ge_tmp_fp[1], "            </coordinates>\n");
          fprintf (ge_tmp_fp[1], "          </LinearRing>\n");
          fprintf (ge_tmp_fp[1], "        </outerBoundaryIs>\n");
          fprintf (ge_tmp_fp[1], "      </Polygon>\n");
          fprintf (ge_tmp_fp[1], "    </Placemark>\n");
        }


      //  If we have or are defining a polygon...

      if (options.shape_tab == POLY_TAB && options.polygon.size () > 1)
        {
          fprintf (ge_tmp_fp[1], "    <Placemark>\n");
          fprintf (ge_tmp_fp[1], "      <name>geCache polygon</name>\n");
          fprintf (ge_tmp_fp[1], "      <styleUrl>#Transparent</styleUrl>\n");


          //  If we haven't closed it we want line segments.

          if (poly_define)
            {
              fprintf (ge_tmp_fp[1], "      <LineString>\n");
              fprintf (ge_tmp_fp[1], "        <tessellate>1</tessellate>\n");
              fprintf (ge_tmp_fp[1], "        <altitudeMode>clampToGround</altitudeMode>\n");
              fprintf (ge_tmp_fp[1], "        <coordinates>\n");
              for (uint32_t i = 0 ; i < options.polygon.size () ; i++)
                fprintf (ge_tmp_fp[1], "          %.11f,%.11f,10\n", options.polygon[i].x, options.polygon[i].y);
              fprintf (ge_tmp_fp[1], "        </coordinates>\n");
              fprintf (ge_tmp_fp[1], "      </LineString>\n");
            }


          //  If we have closed it we want a polygon.

          else
            {
              fprintf (ge_tmp_fp[1], "      <Polygon>\n");
              fprintf (ge_tmp_fp[1], "        <tessellate>1</tessellate>\n");
              fprintf (ge_tmp_fp[1], "        <altitudeMode>clampToGround</altitudeMode>\n");
              fprintf (ge_tmp_fp[1], "        <outerBoundaryIs>\n");
              fprintf (ge_tmp_fp[1], "          <LinearRing>\n");
              fprintf (ge_tmp_fp[1], "            <coordinates>\n");
              for (uint32_t i = 0 ; i < options.polygon.size () ; i++)
                fprintf (ge_tmp_fp[1], "              %.11f,%.11f,10\n", options.polygon[i].x, options.polygon[i].y);
              fprintf (ge_tmp_fp[1], "              %.11f,%.11f,10\n", options.polygon[0].x, options.polygon[0].y);
              fprintf (ge_tmp_fp[1], "            </coordinates>\n");
              fprintf (ge_tmp_fp[1], "          </LinearRing>\n");
              fprintf (ge_tmp_fp[1], "        </outerBoundaryIs>\n");
              fprintf (ge_tmp_fp[1], "      </Polygon>\n");
            }

          fprintf (ge_tmp_fp[1], "    </Placemark>\n");
        }


      fprintf (ge_tmp_fp[1], "  </Document>\n");
      fprintf (ge_tmp_fp[1], "</kml>\n");
    }
  else
    {
      fprintf (ge_tmp_fp[1], "  </Document>\n");
      fprintf (ge_tmp_fp[1], "</kml>\n");
    }


  fclose (ge_tmp_fp[1]);

  return (0);
}



//!  This is where we launch (or kill) Google Earth for cache preview.

void 
geCache::slotGoogleEarthClicked (bool checked)
{
  if (checked)
    {
      //  Make sure we have values in the BOUNDS line edit boxes and that they make sense.

      if (north->text ().isEmpty () || south->text ().isEmpty () || west->text ().isEmpty () || east->text ().isEmpty ())
        {
          QMessageBox::warning (this, tr ("geCache Area bounds"), tr ("You must set all four area fields."));
          return;
        }


      double tmp;

      qPosfix (north->text (), &options.cache_mbr.max_y, QPOS_LAT);
      qPosfix (south->text (), &options.cache_mbr.min_y, QPOS_LAT);
      qPosfix (east->text (), &options.cache_mbr.max_x, QPOS_LON);
      qPosfix (west->text (), &options.cache_mbr.min_x, QPOS_LON);

      if (options.cache_mbr.max_y < options.cache_mbr.min_y)
        {
          tmp = options.cache_mbr.min_y;
          options.cache_mbr.min_y = options.cache_mbr.max_y;
          options.cache_mbr.max_y = tmp;
        }

      if (options.cache_mbr.max_x < options.cache_mbr.min_x)
        {
          if ((options.cache_mbr.max_x < 0.0 && options.cache_mbr.min_x < 0.0) || (options.cache_mbr.max_x >= 0.0 && options.cache_mbr.min_x >= 0.0))
            {
              tmp = options.cache_mbr.min_x;
              options.cache_mbr.min_x = options.cache_mbr.max_x;
              options.cache_mbr.max_x = tmp;
            }
        }
    }


  //  Start me up (as Mick would say)

  if (checked)
    {
      //  Google Earth should be unlinked for cache preview.

      misc.ge_linked = NVFalse;

      bGoogleEarthLink->setChecked (false);


      QString arg;
      QStringList arguments;

      arguments.clear ();


      sprintf (ge_tmp_name[0], "geCache_GE_%d_tmp_link.kml", misc.process_id);
      sprintf (ge_tmp_name[1], "geCache_GE_%d_tmp_look.kml", misc.process_id);


      if ((ge_tmp_fp[0] = fopen (ge_tmp_name[0], "w")) == NULL)
        {
          QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Unable to open temporary Google Earth link file!"));
          return;
        }


      //  Get the full path names.

      QString geFile = QFileInfo (QString (ge_tmp_name[0])).absoluteFilePath ();
      QString geFile2 = QFileInfo (QString (ge_tmp_name[1])).absoluteFilePath ();


      //  Put the full names back into the character strings.

      strcpy (ge_tmp_name[0], geFile.toLatin1 ());
      strcpy (ge_tmp_name[1], geFile2.toLatin1 ());


      //  Build the "look at" file.

      if (positionGoogleEarth ())
        {
          fclose (ge_tmp_fp[0]);
          remove (ge_tmp_name[0]);
          return;
        }


      fprintf (ge_tmp_fp[0], "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
      fprintf (ge_tmp_fp[0], "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
      fprintf (ge_tmp_fp[0], "  <NetworkLink>\n");
      fprintf (ge_tmp_fp[0], "    <name>NetworkLink</name>\n");
      fprintf (ge_tmp_fp[0], "    <flyToView>1</flyToView>\n");
      fprintf (ge_tmp_fp[0], "    <Link>\n");
      fprintf (ge_tmp_fp[0], "      <href>%s</href>\n", ge_tmp_name[1]);
      fprintf (ge_tmp_fp[0], "      <refreshMode>onInterval</refreshMode>\n");
      fprintf (ge_tmp_fp[0], "      <refreshInterval>%d</refreshInterval>\n", options.cache_update_frequency);
      fprintf (ge_tmp_fp[0], "    </Link>\n");
      fprintf (ge_tmp_fp[0], "  </NetworkLink>\n");
      fprintf (ge_tmp_fp[0], "</kml>\n");

      fclose (ge_tmp_fp[0]);


      arguments << geFile;


      googleEarthProc = new QProcess (this);

      connect (googleEarthProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotGoogleEarthError (QProcess::ProcessError)));
      connect (googleEarthProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotGoogleEarthDone (int, QProcess::ExitStatus)));

      googleEarthProc->start (options.ge_name, arguments);

      qApp->setOverrideCursor (Qt::WaitCursor);
      qApp->processEvents ();

      googleEarthProc->waitForStarted ();

      qApp->restoreOverrideCursor ();

      misc.second_count = 0;
      setWidgetStates ();
    }


  //  Kill it...

  else
    {
      killGoogleEarth ();
    }
}



//  This kills the Google Earth process, removes the temporary KML file, and resets some stuff.

void 
geCache::killGoogleEarth ()
{
#ifdef _MSC_VER

  //  On Windows, Google Earth is a normal application so we can use the normal "kill" to get rid of it.

  if (googleEarthProc->state () == QProcess::Running)
    {
      disconnect (googleEarthProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotGoogleEarthError (QProcess::ProcessError)));
      disconnect (googleEarthProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotGoogleEarthDone (int, QProcess::ExitStatus)));

      googleEarthProc->kill ();
    }


#else


  //  On Linux, Google Earth is run from a script so we can't use the normal "kill" technique because all we'll kill that way will be the script.
  //  That leaves Google Earth still running.  What we need to do on Linux is to find the Google Earth script that belongs to the current user,
  //  find its child, and kill that.

  if (googleEarthProc->state () == QProcess::Running)
    {
      disconnect (googleEarthProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotGoogleEarthError (QProcess::ProcessError)));
      disconnect (googleEarthProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotGoogleEarthDone (int, QProcess::ExitStatus)));

      Q_PID pid = googleEarthProc->pid ();
      QProcess killer;
      QStringList params;
      params << "--ppid";
      params << QString::number (pid);
      params << "-o";
      params << "pid";
      params << "--noheaders";
      killer.start ("/bin/ps", params, QIODevice::ReadOnly);

      if (killer.waitForStarted (-1))
        { 
          if (killer.waitForFinished (-1))
            {
              QByteArray temp = killer.readAllStandardOutput ();
              QString str = QString::fromLocal8Bit (temp);
              QStringList list = str.split ("\n");

              for (int32_t i = 0 ; i < list.size () ; i++)
                {
                  if (!list.at (i).isEmpty ()) kill (list.at (i).toInt (), SIGKILL);
                }
            }
        }


      //  Now kill the script (this probably isn't necessary).

      googleEarthProc->kill ();
    }

#endif


  delete (googleEarthProc);
  googleEarthProc = NULL;


  misc.ge_linked = NVFalse;


  bGoogleEarthLink->setChecked (false);


  setWidgetStates ();


  remove (ge_tmp_name[0]);
  remove (ge_tmp_name[1]);


  bGoogleEarth->setChecked (false);
}



//!  Error callback from the Google Earth process.

void 
geCache::slotGoogleEarthError (QProcess::ProcessError error)
{
  switch (error)
    {
    case QProcess::FailedToStart:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Unable to start Google Earth!"));
      break;

    case QProcess::Crashed:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Google Earth crashed!"));
      break;

    case QProcess::Timedout:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Google Earth timed out!"));
      break;

    case QProcess::WriteError:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("There was a write error in Google Earth!"));
      break;

    case QProcess::ReadError:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("There was a read error in Google Earth!"));
      break;

    case QProcess::UnknownError:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Google Earth died with an unknown error!"));
      break;
    }


  if (googleEarthProc) killGoogleEarth ();
}



void 
geCache::slotGoogleEarthDone (int exitCode ATTR_UNUSED, QProcess::ExitStatus exitStatus ATTR_UNUSED)
{
#ifdef _MSC_VER
UNREFERENCED_PARAMETER (exitCode);
UNREFERENCED_PARAMETER (exitStatus);
#endif


  //  It's already dead but killGoogleEarth will clean up all details (as Don Henley would say).

  if (googleEarthProc) killGoogleEarth ();
}



void 
geCache::slotGoogleEarthLink (bool checked ATTR_UNUSED)
{
#ifdef _MSC_VER
UNREFERENCED_PARAMETER (checked);
#endif

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running)
    {
      misc.ge_linked = !misc.ge_linked;

      positionGoogleEarth ();
    }


  setWidgetStates ();
}



uint8_t 
geCache::positionBuildGoogleEarth ()
{
  NV_F64_XYMBR actual_mbr;


  //  Last time through we want to show the entire area.

  if (build_kill_flag)
    {
      actual_mbr = misc.build_area_mbr;
    }
  else
    {
      actual_mbr.min_x = misc.view_area_mbr.min_x + misc.x_border;
      actual_mbr.max_x = misc.view_area_mbr.max_x - misc.x_border;
      actual_mbr.min_y = misc.view_area_mbr.min_y + misc.y_border;
      actual_mbr.max_y = misc.view_area_mbr.max_y - misc.y_border;
    }


  if ((build_ge_tmp_fp[1] = fopen (build_ge_tmp_name[1], "w")) == NULL)
    {
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Unable to open temporary Google Earth KML file!"));
      return (-1);
    }


  fprintf (build_ge_tmp_fp[1], "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf (build_ge_tmp_fp[1], "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
  fprintf (build_ge_tmp_fp[1], "  <Document>\n");


  //  If we're doing a polygon build, we only want to change the file if the current box overlaps the polygon.  The flag will always be 
  //  set to NVTrue for rectangle builds.

  if (misc.view_flag)
    {

      //  This is the box

      fprintf (build_ge_tmp_fp[1], "    <Style id=\"Transparent\">\n");
      fprintf (build_ge_tmp_fp[1], "      <LineStyle>\n");
      fprintf (build_ge_tmp_fp[1], "        <width>1.5</width>\n");
      fprintf (build_ge_tmp_fp[1], "      </LineStyle>\n");
      fprintf (build_ge_tmp_fp[1], "      <PolyStyle>\n");
      fprintf (build_ge_tmp_fp[1], "        <color>00000000</color>\n");


      //  The last time through we want to draw the box

      if (build_kill_flag)
        {
          fprintf (build_ge_tmp_fp[1], "        <outline>1</outline>\n");
        }
      else
        {
          fprintf (build_ge_tmp_fp[1], "        <outline>0</outline>\n");
        }

      fprintf (build_ge_tmp_fp[1], "        <fill>0</fill>\n");
      fprintf (build_ge_tmp_fp[1], "      </PolyStyle>\n");
      fprintf (build_ge_tmp_fp[1], "    </Style>\n");
      fprintf (build_ge_tmp_fp[1], "    <Placemark>\n");
      fprintf (build_ge_tmp_fp[1], "      <name>geCache displayed area</name>\n");
      fprintf (build_ge_tmp_fp[1], "      <styleUrl>#Transparent</styleUrl>\n");
      fprintf (build_ge_tmp_fp[1], "      <Polygon>\n");
      fprintf (build_ge_tmp_fp[1], "        <extrude>1</extrude>\n");


      //  We also want to tesselate the box on the last time through.

      if (build_kill_flag)
        {
          fprintf (build_ge_tmp_fp[1], "        <tessellate>1</tessellate>\n");
          fprintf (build_ge_tmp_fp[1], "        <altitudeMode>clampToGround</altitudeMode>\n");
        }
      else
        {
          fprintf (build_ge_tmp_fp[1], "        <altitudeMode>relativeToGround</altitudeMode>\n");
        }


      fprintf (build_ge_tmp_fp[1], "        <outerBoundaryIs>\n");
      fprintf (build_ge_tmp_fp[1], "          <LinearRing>\n");
      fprintf (build_ge_tmp_fp[1], "            <coordinates>\n");
      fprintf (build_ge_tmp_fp[1], "              %.11f,%.11f,10\n", actual_mbr.min_x, actual_mbr.min_y);
      fprintf (build_ge_tmp_fp[1], "              %.11f,%.11f,10\n", actual_mbr.min_x, actual_mbr.max_y);
      fprintf (build_ge_tmp_fp[1], "              %.11f,%.11f,10\n", actual_mbr.max_x, actual_mbr.max_y);
      fprintf (build_ge_tmp_fp[1], "              %.11f,%.11f,10\n", actual_mbr.max_x, actual_mbr.min_y);
      fprintf (build_ge_tmp_fp[1], "              %.11f,%.11f,10\n", actual_mbr.min_x, actual_mbr.min_y);
      fprintf (build_ge_tmp_fp[1], "            </coordinates>\n");
      fprintf (build_ge_tmp_fp[1], "          </LinearRing>\n");
      fprintf (build_ge_tmp_fp[1], "        </outerBoundaryIs>\n");
      fprintf (build_ge_tmp_fp[1], "      </Polygon>\n");
      fprintf (build_ge_tmp_fp[1], "    </Placemark>\n");
    }

  fprintf (build_ge_tmp_fp[1], "  </Document>\n");
  fprintf (build_ge_tmp_fp[1], "</kml>\n");

  fclose (build_ge_tmp_fp[1]);


  //  If we set the kill flag the last time, just return;

  if (build_kill_flag) return (0);


  do
    {
      switch (build_direction)
        {
        case 0:

          //  Move right one box

          misc.view_area_mbr.min_x += misc.box_size_x_deg;


          //  If we've passed the end of the area...

          if (misc.view_area_mbr.max_x >= misc.build_area_mbr.max_x)
            {
              x_box_count = 0;


              //  Reset min_x to the previous location.

              misc.view_area_mbr.min_x -= misc.box_size_x_deg;


              //  Move up one box

              misc.view_area_mbr.min_y += misc.box_size_y_deg;


              //  If we've passed the top of the area, we're done.

              if (misc.view_area_mbr.min_y >= misc.build_area_mbr.max_y)
                {
                  build_kill_flag = NVTrue;
                  misc.view_flag = NVTrue;
                }
              else
                {
                  build_direction = 1;
                }
            }
          else
            {
              x_box_count++;
            }

          break;

        case 1:

          //  Move left one box

          misc.view_area_mbr.min_x -= misc.box_size_x_deg;


          //  If we've passed the end of the area...

          if (misc.view_area_mbr.max_x <= misc.build_area_mbr.min_x)
            {
              x_box_count = 0;


              //  Reset min_x to the previous location.

              misc.view_area_mbr.min_x += misc.box_size_x_deg;


              //  Move up one box

              misc.view_area_mbr.min_y += misc.box_size_y_deg;


              //  If we've passed the top of the area, we're done.

              if (misc.view_area_mbr.min_y >= misc.build_area_mbr.max_y)
                {
                  build_kill_flag = NVTrue;
                  misc.view_flag = NVTrue;
                }
              else
                {
                  build_direction = 0;
                }
            }
          else
            {
              x_box_count++;
            }

          break;
        }

      if (!build_kill_flag)
        {
          misc.view_area_mbr.max_y = misc.view_area_mbr.min_y + misc.box_size_y_deg;
          misc.view_area_mbr.max_x = misc.view_area_mbr.min_x + misc.box_size_x_deg;


          //  Check the box for overlap if we're doing a polygon.

          if (misc.poly_flag)
            {
              NV_F64_COORD2 mbr_poly[4];

              mbr_poly[0].x = misc.view_area_mbr.min_x;
              mbr_poly[0].y = misc.view_area_mbr.min_y;
              mbr_poly[1].x = misc.view_area_mbr.min_x;
              mbr_poly[1].y = misc.view_area_mbr.max_y;
              mbr_poly[2].x = misc.view_area_mbr.max_x;
              mbr_poly[2].y = misc.view_area_mbr.max_y;
              mbr_poly[3].x = misc.view_area_mbr.max_x;
              mbr_poly[3].y = misc.view_area_mbr.min_y;

              int32_t poly_size = (int32_t) options.polygon.size ();


              misc.view_flag = polygon_intersection (mbr_poly, 4, options.polygon.data (), poly_size);
            }
        }
    } while (!misc.view_flag);

  return (0);
}



void 
geCache::slotBuildCache ()
{
  uint8_t copyDir (const QString &source, const QString &dest);


  //  If we were previewing an area in Google Earth, kill it and start a new session for the build.

  if (googleEarthProc && googleEarthProc->state () == QProcess::Running) killGoogleEarth ();


  //  If the build process is running, pressing the build button again will kill the process.

  if (buildGoogleEarthProc && buildGoogleEarthProc->state () == QProcess::Running)
    {
      killBuildGoogleEarth ();
    }
  else
    {
      //  Remove the Google Earth cache directory.

#ifdef _MSC_VER

      if (options.ge_dir.at (1) != ':' || options.ge_dir.at (2) != '\\')
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache folder path must be fully qualified (e.g. start with DRIVE_LETTER:\\)"));
          return;
        }

      if (!QFile (options.ge_dir).exists ())
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache folder %1 does not exist!").arg (options.ge_dir));
          return;
        }

#else

      if (options.ge_dir.at (0) != '/')
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache directory path must be fully qualified (i.e. start with /)"));
          return;
        }

      if (!QFile (options.ge_dir).exists ())
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache directory %1 does not exist!").arg (options.ge_dir));
          return;
        }

#endif

      QDir (options.ge_dir).removeRecursively ();


      //  Make sure we have values in the bounds line edit boxes and that they make sense.

      if (north->text ().isEmpty () || south->text ().isEmpty () || west->text ().isEmpty () || east->text ().isEmpty ())
        {
          QMessageBox::warning (this, tr ("geCache Area bounds"), tr ("You must set all four area fields."));
          return;
        }


      double tmp;

      qPosfix (north->text (), &options.cache_mbr.max_y, QPOS_LAT);
      qPosfix (south->text (), &options.cache_mbr.min_y, QPOS_LAT);
      qPosfix (east->text (), &options.cache_mbr.max_x, QPOS_LON);
      qPosfix (west->text (), &options.cache_mbr.min_x, QPOS_LON);

      if (options.cache_mbr.max_y < options.cache_mbr.min_y)
        {
          tmp = options.cache_mbr.min_y;
          options.cache_mbr.min_y = options.cache_mbr.max_y;
          options.cache_mbr.max_y = tmp;
        }

      if (options.cache_mbr.max_x < options.cache_mbr.min_x)
        {
          if ((options.cache_mbr.max_x < 0.0 && options.cache_mbr.min_x < 0.0) || (options.cache_mbr.max_x >= 0.0 && options.cache_mbr.min_x >= 0.0))
            {
              tmp = options.cache_mbr.min_x;
              options.cache_mbr.min_x = options.cache_mbr.max_x;
              options.cache_mbr.max_x = tmp;
            }
        }


      QString arg;
      QStringList arguments;

      arguments.clear ();


      sprintf (build_ge_tmp_name[0], "geCache_GE_%d_tmp_build_link.kml", misc.process_id);
      sprintf (build_ge_tmp_name[1], "geCache_GE_%d_tmp_build_look.kml", misc.process_id);


      if ((build_ge_tmp_fp[0] = fopen (build_ge_tmp_name[0], "w")) == NULL)
        {
          QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Unable to open temporary Google Earth link file!"));
          return;
        }


      //  Get the full path names.

      QString geFile = QFileInfo (QString (build_ge_tmp_name[0])).absoluteFilePath ();
      QString geFile2 = QFileInfo (QString (build_ge_tmp_name[1])).absoluteFilePath ();


      //  Put the full names back into the character strings.

      strcpy (build_ge_tmp_name[0], geFile.toLatin1 ());
      strcpy (build_ge_tmp_name[1], geFile2.toLatin1 ());


      //  Figure out how many iterations it will take to do the build so that we can set up a progress bar.

      computeSize (&misc, &options);
      iteration_count = 0;

      int32_t hour, minute, second;

      if (misc.poly_flag)
        {
          if (misc.total_poly_time > 86400)
            {
              QMessageBox::warning (this, tr ("geCache Build cache"), tr ("The estimated time to complete the cache build is more than a day!."));
              return;
            }

          hour = misc.total_poly_time / 3600;
          minute = (misc.total_poly_time / 60) % 60;
          second = misc.total_poly_time % 60;

          progress->setRange (0, misc.poly_iterations);
          boxes_remaining = misc.poly_iterations;
        }
      else
        {
          if (misc.total_rect_time > 86400)
            {
              QMessageBox::warning (this, tr ("geCache Build cache"), tr ("The estimated time to complete the cache build is more than a day!."));
              return;
            }

          hour = misc.total_rect_time / 3600;
          minute = (misc.total_rect_time / 60) % 60;
          second = misc.total_rect_time % 60;

          progress->setRange (0, misc.iterations);
          boxes_remaining = misc.iterations;
        }

      progBox->setTitle (tr ("Cache build progress - Estimated time remaining - %1:%2:%3").arg (hour, 2, 10, zero).arg (minute, 2, 10, zero).arg (second, 2, 10, zero));

      qApp->processEvents ();


      //  Build the "look at" file.

      if (positionBuildGoogleEarth ())
        {
          fclose (build_ge_tmp_fp[0]);
          remove (build_ge_tmp_name[0]);
          return;
        }


      fprintf (build_ge_tmp_fp[0], "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
      fprintf (build_ge_tmp_fp[0], "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
      fprintf (build_ge_tmp_fp[0], "  <NetworkLink>\n");
      fprintf (build_ge_tmp_fp[0], "    <name>NetworkLink</name>\n");
      fprintf (build_ge_tmp_fp[0], "    <flyToView>1</flyToView>\n");
      fprintf (build_ge_tmp_fp[0], "    <Link>\n");
      fprintf (build_ge_tmp_fp[0], "      <href>%s</href>\n", build_ge_tmp_name[1]);
      fprintf (build_ge_tmp_fp[0], "      <refreshMode>onInterval</refreshMode>\n");
      fprintf (build_ge_tmp_fp[0], "      <refreshInterval>%d</refreshInterval>\n", options.cache_update_frequency);
      fprintf (build_ge_tmp_fp[0], "    </Link>\n");
      fprintf (build_ge_tmp_fp[0], "  </NetworkLink>\n");
      fprintf (build_ge_tmp_fp[0], "</kml>\n");

      fclose (build_ge_tmp_fp[0]);


      arguments << geFile;


      buildGoogleEarthProc = new QProcess (this);

      connect (buildGoogleEarthProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotBuildGoogleEarthError (QProcess::ProcessError)));
      connect (buildGoogleEarthProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotBuildGoogleEarthDone (int, QProcess::ExitStatus)));


      buildGoogleEarthProc->start (options.ge_name, arguments);

      qApp->setOverrideCursor (Qt::WaitCursor);
      qApp->processEvents ();

      buildGoogleEarthProc->waitForStarted ();

      qApp->restoreOverrideCursor ();

      misc.second_count = 0;
      build_start_flag = NVTrue;


      //  We need a snapshot of the newly created cache directory in case we max out the current one.  We'll pause here for a few seconds to
      //  make sure Google Earth has started nicely, then we'll copy the cache directory.

#ifdef _MSC_VER

      Sleep (3000);

#else

      sleep (3);

#endif
 
      QDir cache_parent = QFileInfo (options.ge_dir).absoluteDir ();

      cache_snapshot = cache_parent.absolutePath () + SEPARATOR + "cache_snapshot";

      if (QDir (cache_snapshot).exists ()) QDir (cache_snapshot).removeRecursively ();

      copyDir (options.ge_dir, cache_snapshot);
    }


  setWidgetStates ();
}



//  This kills the Google Earth process and removes the temporary KML file.

void 
geCache::killBuildGoogleEarth ()
{
#ifdef _MSC_VER

  //  On Windows, Google Earth is a normal application so we can use the normal "kill" to get rid of it.

  if (buildGoogleEarthProc->state () == QProcess::Running)
    {
      disconnect (buildGoogleEarthProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotBuildGoogleEarthError (QProcess::ProcessError)));
      disconnect (buildGoogleEarthProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotBuildGoogleEarthDone (int, QProcess::ExitStatus)));

      buildGoogleEarthProc->kill ();
    }


#else


  //  On Linux, Google Earth is run from a script so we can't use the normal "kill" technique because all we'll kill that way will be the script.
  //  That leaves Google Earth still running.  What we need to do on Linux is to find the Google Earth script that belongs to the current user,
  //  find its child, and kill that.

  if (buildGoogleEarthProc->state () == QProcess::Running)
    {
      disconnect (buildGoogleEarthProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotBuildGoogleEarthError (QProcess::ProcessError)));
      disconnect (buildGoogleEarthProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotBuildGoogleEarthDone (int, QProcess::ExitStatus)));

      Q_PID pid = buildGoogleEarthProc->pid ();
      QProcess killer;
      QStringList params;
      params << "--ppid";
      params << QString::number (pid);
      params << "-o";
      params << "pid";
      params << "--noheaders";
      killer.start ("/bin/ps", params, QIODevice::ReadOnly);

      if (killer.waitForStarted (-1))
        { 
          if (killer.waitForFinished (-1))
            {
              QByteArray temp = killer.readAllStandardOutput ();
              QString str = QString::fromLocal8Bit (temp);
              QStringList list = str.split ("\n");

              for (int32_t i = 0 ; i < list.size () ; i++)
                {
                  if (!list.at (i).isEmpty ()) kill (list.at (i).toInt (), SIGKILL);
                }
            }
        }


      //  Now kill the script (this probably isn't necessary).

      buildGoogleEarthProc->kill ();
    }

#endif

  delete (buildGoogleEarthProc);

  buildGoogleEarthProc = NULL;

  build_direction = 0;
  build_kill_flag = NVFalse;

  progBox->setTitle (tr ("Cache build progress"));

  setWidgetStates ();

  progress->reset ();


  qApp->processEvents ();


  remove (build_ge_tmp_name[0]);
  remove (build_ge_tmp_name[1]);


  //  Get rid of the cache snapshot directory

  if (QDir (cache_snapshot).exists ()) QDir (cache_snapshot).removeRecursively ();
}



//!  Error callback from the build Google Earth process.

void 
geCache::slotBuildGoogleEarthError (QProcess::ProcessError error)
{
  switch (error)
    {
    case QProcess::FailedToStart:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Unable to start Google Earth!"));
      break;

    case QProcess::Crashed:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Google Earth crashed!"));
      break;

    case QProcess::Timedout:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Google Earth timed out!"));
      break;

    case QProcess::WriteError:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("There was a write error in Google Earth!"));
      break;

    case QProcess::ReadError:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("There was a read error in Google Earth!"));
      break;

    case QProcess::UnknownError:
      QMessageBox::critical (this, tr ("geCache Google Earth"), tr ("Google Earth died with an unknown error!"));
      break;
    }


  if (buildGoogleEarthProc) killBuildGoogleEarth ();
}



void 
geCache::slotBuildGoogleEarthDone (int exitCode ATTR_UNUSED, QProcess::ExitStatus exitStatus ATTR_UNUSED)
{
#ifdef _MSC_VER
UNREFERENCED_PARAMETER (exitCode);
UNREFERENCED_PARAMETER (exitStatus);
#endif


  //  It's already dead but killBuildGoogleEarth will clean up all details (as Don Henley would say).

  if (buildGoogleEarthProc) killBuildGoogleEarth ();
}



void 
geCache::slotSaveCacheClicked ()
{
  uint8_t copyDir (const QString &source, const QString &dest);


  QFileDialog *fd = new QFileDialog (this, tr ("geCache Save cache"));
  fd->setViewMode (QFileDialog::List);
  fd->setOption (QFileDialog::DontUseNativeDialog, true);


  fd->setFileMode (QFileDialog::AnyFile);


  if (fd->exec () == QDialog::Accepted) 
    {
      //  Save the directory that we were in when we selected a directory.

      options.stash_dir = fd->directory ().absolutePath ();

      QStringList files = fd->selectedFiles ();

      QString file = files.at (0);

      if (file.isEmpty ())
        {
          QMessageBox::warning (this, tr ("geCache Save cache"), tr ("You must select an existing directory or enter a name for a new directory."));
          return;
        }


      QString save_dir = QFileInfo (file).absoluteFilePath ();
      if (QDir (save_dir).exists ())
        {
          if (!QFileInfo (save_dir).isDir ())
            {
              QMessageBox::warning (this, tr ("geCache Save cache"), tr ("Output must be a directory, not a file."));
              return;
            }

          QDir (save_dir).removeRecursively ();
        }


      //  Copy the Google Earth cache directory.

#ifdef _MSC_VER

      if (options.ge_dir.at (1) != ':' || options.ge_dir.at (2) != '\\')
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache folder path must be fully qualified (e.g. start with DRIVE_LETTER:\\)"));
          return;
        }

      if (!QFile (options.ge_dir).exists ())
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache folder %1 does not exist!").arg (options.ge_dir));
          return;
        }

#else

      if (options.ge_dir.at (0) != '/')
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache directory path must be fully qualified (i.e. start with /)"));
          return;
        }

      if (!QFile (options.ge_dir).exists ())
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache directory %1 does not exist!").arg (options.ge_dir));
          return;
        }

#endif


      qApp->setOverrideCursor (Qt::WaitCursor);
      qApp->processEvents ();

      copyDir (options.ge_dir, save_dir);

      qApp->restoreOverrideCursor ();
    }
}



void 
geCache::slotLoadCacheClicked ()
{
  uint8_t copyDir (const QString &source, const QString &dest);


  QFileDialog *fd = new QFileDialog (this, tr ("geCache Load cache"));
  fd->setViewMode (QFileDialog::List);
  fd->setOption (QFileDialog::DontUseNativeDialog, true);
  fd->setOption (QFileDialog::ShowDirsOnly, true);


  fd->setFileMode (QFileDialog::Directory);


  if (fd->exec () == QDialog::Accepted) 
    {
      //  Save the directory that we were in when we selected a directory.

      options.stash_dir = fd->directory ().absolutePath ();

      QStringList files = fd->selectedFiles ();

      QString file = files.at (0);

      if (file.isEmpty ()) file = fd->directory ().absolutePath ();


      //  Remove the Google Earth cache directory.

#ifdef _MSC_VER

      if (options.ge_dir.at (1) != ':' || options.ge_dir.at (2) != '\\')
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache folder path must be fully qualified (e.g. start with DRIVE_LETTER:\\)"));
          return;
        }

      if (!QFile (options.ge_dir).exists ())
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache folder %1 does not exist!").arg (options.ge_dir));
          return;
        }

#else

      if (options.ge_dir.at (0) != '/')
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache directory path must be fully qualified (i.e. start with /)"));
          return;
        }

      if (!QFile (options.ge_dir).exists ())
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Google Earth cache directory %1 does not exist!").arg (options.ge_dir));
          return;
        }

#endif

      QDir (options.ge_dir).removeRecursively ();


      QString load_dir = QFileInfo (file).absoluteFilePath ();

      qApp->setOverrideCursor (Qt::WaitCursor);
      qApp->processEvents ();

      if (!copyDir (load_dir, options.ge_dir))
        {
          QMessageBox::warning (this, tr ("geCache Error"), tr ("Can't copy loaded cache directory to cache!"));
          qApp->restoreOverrideCursor ();
          return;
        }

      qApp->restoreOverrideCursor ();
    }
}



//  Change the cache box size.

void 
geCache::slotBoxSizeChanged (int value)
{
  options.build_box_size = value;

  computeSize (&misc, &options);
}



//  Change the cache update frequency.

void 
geCache::slotCacheUpdateChanged (int value)
{
  options.cache_update_frequency = value;

  computeSize (&misc, &options);
}



//  Change the position format.

void 
geCache::slotPositionClicked (int id)
{
  options.position_form = id;


  QString ltstring, lnstring;
  char hem;
  double deg, min, sec;

  ltstring = qFixpos (options.cache_mbr.max_y, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);
  north->setText (ltstring);
  ltstring = qFixpos (options.cache_mbr.min_y, &deg, &min, &sec, &hem, QPOS_LAT, options.position_form);
  south->setText (ltstring);
  lnstring = qFixpos (options.cache_mbr.max_x, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);
  east->setText (lnstring);
  lnstring = qFixpos (options.cache_mbr.min_x, &deg, &min, &sec, &hem, QPOS_LON, options.position_form);
  west->setText (lnstring);
}



//  Change the warning color

void
geCache::slotWarningColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (options.warning_color, this, tr ("geCache Warning Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) options.warning_color = clr;

  int32_t hue, sat, val;

  options.warning_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      warningTextColorString = "color: white; ";
    }
  else
    {
      warningTextColorString = "color: black; ";
    }

  setWidgetStates ();
}



//  Change the font

void
geCache::slotFont ()
{
  bool ok;
  QFont font = QFontDialog::getFont (&ok, options.font, this);
  if (ok)
    {
      options.font = font;

      setWidgetStates ();
    }
}



//  Change the Google Earth executable or script name

void
geCache::slotGeNameEditingFinished ()
{
  options.ge_name = geName->text ();

  misc.googleearth_available = NVTrue;
  QString ge_exe = QStandardPaths::findExecutable (options.ge_name);
  if (ge_exe.isEmpty ())
    {
      misc.googleearth_available = NVFalse;

      QMessageBox::warning (this, tr ("geCache"),
                            tr ("Can't find Google Earth (%1) in your PATH.  Make sure the <b>Google Earth Name</b> field in the ").arg (options.ge_name) +
                            tr ("<b>Preferences</b> tab is correct and that the directory containing that file is included in your PATH environment variable."));

      geCacheTab->setCurrentIndex (PREF_TAB);
    }
  else
    {
      setWidgetStates ();
    }
}



void 
geCache::slotCacheBrowseClicked ()
{
  QFileDialog *fd = new QFileDialog (this, tr ("geCache Google Earth cache"));
  fd->setViewMode (QFileDialog::List);
  fd->setOption (QFileDialog::DontUseNativeDialog, true);
  fd->setOption (QFileDialog::ShowDirsOnly, true);


  fd->setFileMode (QFileDialog::Directory);


  if (fd->exec () == QDialog::Accepted) 
    {
      QStringList files = fd->selectedFiles ();

      QString file = files.at (0);

      if (file.isEmpty ()) file = fd->directory ().absolutePath ();

      options.ge_dir = file;

      geCacheDir->setText (options.ge_dir);
    }
}



void
geCache::slotIconSizeChanged (int index)
{
  switch (index)
    {
    case 0:
      options.icon_size = 16;
      break;

    case 1:
      options.icon_size = 20;
      break;

    case 2:
      options.icon_size = 24;
      break;

    case 3:
      options.icon_size = 28;
      break;

    case 4:
      options.icon_size = 32;
      break;
    }

  setWidgetStates ();
}



void
geCache::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



//  Somebody pressed the little X in the window decoration.  DOH!

void 
geCache::closeEvent (QCloseEvent *event ATTR_UNUSED)
{
#ifdef _MSC_VER
UNREFERENCED_PARAMETER (event);
#endif

 if (!already_gone) slotQuit ();
}



void 
geCache::slotQuit ()
{
  void envout (OPTIONS *options);


  //  Check to see if Google Earth is running.  This will also get rid of the temporary KML files.

  if (googleEarthProc) killGoogleEarth ();
  if (buildGoogleEarthProc) killBuildGoogleEarth ();


  //  Use frame geometry to get the absolute x and y.

  QRect tmp = this->frameGeometry ();
  options.window_x = tmp.x ();
  options.window_y = tmp.y ();


  //  Use geometry to get the width and height.

  tmp = this->geometry ();
  options.window_width = tmp.width ();
  options.window_height = tmp.height ();


  //  Save the user's defaults

  envout (&options);


  //  We don't want the closeEvent (above) to call slotQuit again if we're closing normally.

  already_gone = NVTrue;


  close ();
}



//  Set all the widget states that might need to be changed due to option/state changes.

void
geCache::setWidgetStates ()
{
  qApp->setFont (options.font);

  bFont->setText (options.font.toString ());

  fontString = QString ("font: ") + options.font.toString () + QString ("; ");


  QSize iconSize (options.icon_size, options.icon_size);
  bGoogleEarth->setIconSize (iconSize);
  bGoogleEarthLink->setIconSize (iconSize);
  for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setIconSize (iconSize);
  bPoly->setIconSize (iconSize);
  bClosePoly->setIconSize (iconSize);
  bClearPoly->setIconSize (iconSize);


  QString bstring, fstring, tstring, bc;


  //  Google Earth is not available (or not in the PATH) on this system...

  if (!misc.googleearth_available)
    {
      bGoogleEarth->setEnabled (false);
      for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setEnabled (false);
      north->setEnabled (false);
      west->setEnabled (false);
      east->setEnabled (false);
      south->setEnabled (false);
      bPoly->setEnabled (false);
      bClosePoly->setEnabled (false);
      bClearPoly->setEnabled (false);
      boxSize->setEnabled (false);
      cacheUpdate->setEnabled (false);
      bBuildCache->setEnabled (false);
      bSaveCache->setEnabled (false);
      bLoadCache->setEnabled (false);
      bGoogleEarthLink->setEnabled (false);

      bstring = tr ("This button is disabled because Google Earth is not available or is not in the PATH");
      fstring = tr ("This field is disabled because Google Earth is not available or is not in the PATH");

      bGoogleEarth->setToolTip (bstring);
      for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setToolTip (bstring);
      north->setToolTip (fstring);
      west->setToolTip (fstring);
      east->setToolTip (fstring);
      south->setToolTip (fstring);
      bPoly->setToolTip (fstring);
      bClosePoly->setToolTip (bstring);
      bClearPoly->setToolTip (bstring);
      boxSize->setToolTip (bstring);
      cacheUpdate->setToolTip (fstring);
      bBuildCache->setToolTip (bstring);
      bSaveCache->setToolTip (bstring);
      bLoadCache->setToolTip (bstring);
      bGoogleEarthLink->setToolTip (bstring);
    }


  //   Google Earth is available and in the PATH...

  else
    {
      //  We're running Google Earth to build cache...

      if (buildGoogleEarthProc)
        {
          geCacheTab->setTabEnabled (PREF_TAB, false);
          bGoogleEarth->setEnabled (false);
          bGoogleEarthLink->setEnabled (false);
          for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setEnabled (false);
          north->setEnabled (false);
          west->setEnabled (false);
          east->setEnabled (false);
          south->setEnabled (false);
          bPoly->setEnabled (false);
          bClosePoly->setEnabled (false);
          bClearPoly->setEnabled (false);
          boxSize->setEnabled (false);
          cacheUpdate->setEnabled (false);
          bSaveCache->setEnabled (false);
          bLoadCache->setEnabled (false);

          bstring = tr ("This button is disabled because a cache build is running");
          fstring = tr ("This field is disabled because a cache build is running");
          tstring = tr ("This tab is disabled because a cache build is running");


          geCacheTab->setTabToolTip (PREF_TAB, tstring);
          bGoogleEarth->setToolTip (bstring);
          bGoogleEarthLink->setToolTip (bstring);
          for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setToolTip (bstring);
          north->setToolTip (fstring);
          west->setToolTip (fstring);
          east->setToolTip (fstring);
          south->setToolTip (fstring);
          bPoly->setToolTip (bstring);
          bClosePoly->setToolTip (bstring);
          bClearPoly->setToolTip (bstring);
          boxSize->setToolTip (bstring);
          boxSize->setToolTip (fstring);
          cacheUpdate->setToolTip (fstring);
          bSaveCache->setToolTip (bstring);
          bLoadCache->setToolTip (bstring);
        }


      //  We're not running Google Earth to build cache...

      else
        {
          geCacheTab->setTabEnabled (PREF_TAB, true);


          //  We're running Google Earth to preview the area to be cached...

          if (googleEarthProc)
            {
              bGoogleEarth->setEnabled (true);
              bGoogleEarthLink->setEnabled (true);
              bBuildCache->setEnabled (true);

              bSaveCache->setEnabled (false);
              bLoadCache->setEnabled (false);


              bstring = tr ("This button is disabled because Google Earth is running to preview an area");

              bSaveCache->setToolTip (bstring);
              bLoadCache->setToolTip (bstring);


              //  I'm going to leave these enabled even when linked to allow the user to easily adjust the bounds of the already defined area.  Can this 
              //  cause problems?  No doubt, but stupidity is not illegal.

              for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setEnabled (true);
              north->setEnabled (true);
              west->setEnabled (true);
              east->setEnabled (true);
              south->setEnabled (true);


              //  We're previewing the area to be cached and we're linked...

              if (misc.ge_linked)
                {
                  bPoly->setEnabled (true);

                  if (options.polygon.size ())
                    {
                      if (poly_define)
                        {
                          bClosePoly->setEnabled (true);
                        }
                      else
                        {
                          bClosePoly->setEnabled (false);
                        }

                      bClearPoly->setEnabled (true);
                    }
                  else
                    {
                      bClosePoly->setEnabled (false);
                      bClearPoly->setEnabled (false);
                    }

                  boxSize->setEnabled (false);
                  cacheUpdate->setEnabled (false);

                  fstring = tr ("This field is disabled because Google Earth is running to preview an area and it is linked to geCache");

                  boxSize->setToolTip (fstring);
                  cacheUpdate->setToolTip (fstring);
                }


              //  We're previewing the area to be cached but we're not linked...

              else
                {
                  boxSize->setEnabled (true);
                  cacheUpdate->setEnabled (true);

                  bPoly->setEnabled (false);

                  if (options.polygon.size ())
                    {
                      if (poly_define)
                        {
                          bClosePoly->setEnabled (true);
                        }
                      else
                        {
                          bClosePoly->setEnabled (false);
                        }
                      bClearPoly->setEnabled (true);
                    }
                  else
                    {
                      bClosePoly->setEnabled (false);
                      bClearPoly->setEnabled (false);
                      bClosePoly->setToolTip (bstring);
                      bClearPoly->setToolTip (bstring);
                    }

                  bstring = tr ("This button is disabled because Google Earth is running to preview an area and it is not linked to geCache");

                  bPoly->setToolTip (bstring);
                }
            }


          //  We're not running Google Earth...

          else
            {
              bGoogleEarth->setEnabled (true);
              bGoogleEarthLink->setEnabled (false);
              for (int32_t i = 0 ; i < 8 ; i++) bBounds[i]->setEnabled (true);
              bBuildCache->setEnabled (true);
              bSaveCache->setEnabled (true);
              bLoadCache->setEnabled (true);
              boxSize->setEnabled (true);
              cacheUpdate->setEnabled (true);
              north->setEnabled (true);
              west->setEnabled (true);
              east->setEnabled (true);
              south->setEnabled (true);


              bPoly->setEnabled (false);
              bClosePoly->setEnabled (false);

              bstring = tr ("This button is disabled because Google Earth is running to preview an area and it is not linked to geCache");

              bPoly->setToolTip (bstring);
              bClosePoly->setToolTip (bstring);

              if (options.polygon.size ())
                {
                  bClearPoly->setEnabled (true);
                }
              else
                {
                  bClearPoly->setEnabled (false);
                  bClearPoly->setToolTip (bstring);
                }
            }
        }
    }


  //  Set the correct tool tips for enabled widgets.

  if (geCacheTab->isTabEnabled (PREF_TAB)) geCacheTab->setTabToolTip (3, tr ("Set geCache preferences"));

  if (bGoogleEarth->isEnabled ())
    {
      if (googleEarthProc)
        {
          bGoogleEarth->setToolTip (tr ("Shut down Google Earth"));
        }
      else
        {
          bGoogleEarth->setToolTip (tr ("Start Google Earth to preview areas"));
        }
    }

  if (misc.ge_linked)
    {
      if (bGoogleEarthLink->isEnabled ()) bGoogleEarthLink->setToolTip (tr ("Unlink geCache from Google Earth"));
    }
  else
    {
      if (bGoogleEarthLink->isEnabled ()) bGoogleEarthLink->setToolTip (tr ("Link geCache to Google Earth"));
    }



  bc = fontString + warningTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (options.warning_color.red ()).arg
    (options.warning_color.green ()).arg (options.warning_color.blue ()).arg (options.warning_color.alpha ());

  switch (bounds_clicked)
    {
    case NO_BOUNDS:
      if (north->isEnabled ()) north->setToolTip (tr ("Set the northern latitude boundary of the area"));
      if (south->isEnabled ()) south->setToolTip (tr ("Set the southern latitude boundary of the area"));
      if (west->isEnabled ()) west->setToolTip (tr ("Set the western longitude boundary of the area"));
      if (east->isEnabled ()) east->setToolTip (tr ("Set the eastern longitude boundary of the area"));
      if (bBounds[NW_BOUNDS]->isEnabled ()) bBounds[NW_BOUNDS]->setToolTip (tr ("Click this button to set the northwest corner in Google Earth (Ctrl-Shift-C)"));
      if (bBounds[NE_BOUNDS]->isEnabled ()) bBounds[NE_BOUNDS]->setToolTip (tr ("Click this button to set the northeast corner in Google Earth (Ctrl-Shift-C)"));
      if (bBounds[SW_BOUNDS]->isEnabled ()) bBounds[SW_BOUNDS]->setToolTip (tr ("Click this button to set the southwest corner in Google Earth (Ctrl-Shift-C)"));
      if (bBounds[SE_BOUNDS]->isEnabled ()) bBounds[SE_BOUNDS]->setToolTip (tr ("Click this button to set the southeast corner in Google Earth (Ctrl-Shift-C)"));
      if (bBounds[NORTH_BOUNDS]->isEnabled ()) bBounds[NORTH_BOUNDS]->setToolTip (tr ("Click this button to set the northern boundary in Google Earth (Ctrl-Shift-C)"));
      if (bBounds[WEST_BOUNDS]->isEnabled ()) bBounds[WEST_BOUNDS]->setToolTip (tr ("Click this button to set the western boundary in Google Earth (Ctrl-Shift-C)"));
      if (bBounds[EAST_BOUNDS]->isEnabled ()) bBounds[EAST_BOUNDS]->setToolTip (tr ("Click this button to set the eastern boundary in Google Earth (Ctrl-Shift-C)"));
      if (bBounds[SOUTH_BOUNDS]->isEnabled ()) bBounds[SOUTH_BOUNDS]->setToolTip (tr ("Click this button to set the southern boundary in Google Earth (Ctrl-Shift-C)"));

      bc = fontString + normalTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (buttonBackgroundColor.red ()).arg
        (buttonBackgroundColor.green ()).arg (buttonBackgroundColor.blue ()).arg (buttonBackgroundColor.alpha ());

      north->setStyleSheet (bc);
      south->setStyleSheet (bc);
      west->setStyleSheet (bc);
      east->setStyleSheet (bc);

      break;


    case NW_BOUNDS:

      north->setStyleSheet (bc);
      west->setStyleSheet (bc);

      north->setToolTip (tr ("Select northwest corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));
      west->setToolTip (tr ("Select northwest corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;


    case NE_BOUNDS:

      north->setStyleSheet (bc);
      east->setStyleSheet (bc);

      north->setToolTip (tr ("Select northeast corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));
      east->setToolTip (tr ("Select northeast corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;


    case SW_BOUNDS:

      south->setStyleSheet (bc);
      west->setStyleSheet (bc);

      south->setToolTip (tr ("Select southwest corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));
      west->setToolTip (tr ("Select southwest corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;


    case SE_BOUNDS:

      south->setStyleSheet (bc);
      east->setStyleSheet (bc);

      south->setToolTip (tr ("Select southeast corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));
      east->setToolTip (tr ("Select southeast corner location in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;


    case NORTH_BOUNDS:

      north->setStyleSheet (bc);

      north->setToolTip (tr ("Select northern boundary in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;


    case WEST_BOUNDS:

      west->setStyleSheet (bc);

      west->setToolTip (tr ("Select western boundary in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;


    case EAST_BOUNDS:

      east->setStyleSheet (bc);

      east->setToolTip (tr ("Select eastern boundary in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;


    case SOUTH_BOUNDS:

      south->setStyleSheet (bc);

      south->setToolTip (tr ("Select southern boundary in Google Earth then press <b>Ctrl-Shift-C</b>"));

      break;
    }


  if (poly_define || poly_edit)
    {
      bc = fontString + warningTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (options.warning_color.red ()).arg
        (options.warning_color.green ()).arg (options.warning_color.blue ()).arg (options.warning_color.alpha ());
    }
  else
    {
      bc = fontString + normalTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (buttonBackgroundColor.red ()).arg
        (buttonBackgroundColor.green ()).arg (buttonBackgroundColor.blue ()).arg (buttonBackgroundColor.alpha ());
    }
  bPoly->setStyleSheet (bc);

  if (bPoly->isEnabled ()) bPoly->setToolTip (tr ("Add positions to, or edit positions in, the polygon"));
  if (bClosePoly->isEnabled ()) bClosePoly->setToolTip (tr ("Close the polygon"));
  if (bClearPoly->isEnabled ())   bClearPoly->setToolTip (tr ("Clear all positions in the polygon"));


  if (boxSize->isEnabled ()) boxSize->setToolTip (tr ("Change the size (in meters) of the smallest area used for building the cache"));

  if (bBuildCache->isEnabled ())
    {
      if (buildGoogleEarthProc && buildGoogleEarthProc->state () == QProcess::Running)
        {
          QString bc = fontString + warningTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (options.warning_color.red ()).arg
            (options.warning_color.green ()).arg (options.warning_color.blue ()).arg (options.warning_color.alpha ());
          bBuildCache->setStyleSheet (bc);

          bBuildCache->setText (tr ("Kill cache build"));
          bBuildCache->setToolTip (tr ("Kill Google Earth cache build process"));
          bBuildCache->setWhatsThis (killBuildCacheText);
        }
      else
        {
          QString bc = fontString + normalTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (buttonBackgroundColor.red ()).arg
            (buttonBackgroundColor.green ()).arg (buttonBackgroundColor.blue ()).arg (buttonBackgroundColor.alpha ());
          bBuildCache->setStyleSheet (bc);

          bBuildCache->setText (tr ("Build cache"));
          bBuildCache->setToolTip (tr ("Build Google Earth cache"));
          bBuildCache->setWhatsThis (buildCacheText);
        }
    }

  bSaveCache->setToolTip (tr ("Build Google Earth cache"));
  if (bSaveCache->isEnabled ()) bSaveCache->setToolTip (tr ("Save Google Earth cache"));
  if (bLoadCache->isEnabled ()) bLoadCache->setToolTip (tr ("Load Google Earth cache"));
  if (cacheUpdate->isEnabled ()) cacheUpdate->setToolTip (tr ("Change the frequency (in seconds) for the cache build process"));

  bc = fontString + warningTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (options.warning_color.red ()).arg
    (options.warning_color.green ()).arg (options.warning_color.blue ()).arg (options.warning_color.alpha ());
  bWarningColor->setStyleSheet (bc);
}
