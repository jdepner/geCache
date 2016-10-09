
/********************************************************************************************* 

    geCache.hpp

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


/*  geCache class definitions.  */

#ifndef _GECACHE_HPP_
#define _GECACHE_HPP_

#include <cmath>
#include <cerrno>

#include "geCacheDef.hpp"
#include "version.hpp"


using namespace std;


void computeSize (MISC *misc, OPTIONS *options);


class geCache:public QMainWindow

{
  Q_OBJECT 


public:

  geCache (QWidget *parent = 0);
  ~geCache ();


protected:

  char            ge_tmp_name[2][1024], build_ge_tmp_name[2][1024];

  FILE            *ge_tmp_fp[2], *build_ge_tmp_fp[2];

  QProcess        *googleEarthProc, *buildGoogleEarthProc;

  OPTIONS         options;

  MISC            misc;

  QClipboard      *clipboard;

  QTabWidget      *geCacheTab, *shapeTab;

  QListWidget     *vertices;

  QTimer          *geCacheTimer;

  QAction         *bHelp;

  QLineEdit       *north, *south, *east, *west, *geName;

  QToolBar        *toolBar;

  QPushButton     *bGoogleEarth, *bGoogleEarthLink;

  QPushButton     *bBounds[8], *bPoly, *bClosePoly, *bClearPoly, *bBuildCache, *bSaveCache, *bLoadCache, *bCacheBrowse, *bWarningColor, *bFont;

  QColor          buttonBackgroundColor, buttonTextColor;

  QString         normalTextColorString, warningTextColorString, fontString, prev_clipboard_text, cache_snapshot;

  QSpinBox        *boxSize, *cacheUpdate;

  QComboBox       *iconSize;

  QLabel          *geCacheDir;

  QButtonGroup    *bGrp, *boundsGroup;

  QGroupBox       *progBox, *cacheBarBox;

  QProgressBar    *progress;

  uint8_t         build_direction, build_kill_flag, build_start_flag, restart_msg, already_gone, poly_define, poly_edit;

  int32_t         bounds_clicked, x_box_count, boxes_remaining, iteration_count, poly_edit_index;

  int64_t         start_timestamp, current_timestamp;


  void getClipboard ();
  void setWidgetStates ();
  void killGoogleEarth ();
  uint8_t positionGoogleEarth ();
  void killBuildGoogleEarth ();
  uint8_t positionBuildGoogleEarth ();
  void closeEvent (QCloseEvent *event);


protected slots:

  void slotGeCacheTabChanged (int tab);
  void slotShapeTabChanged (int tab);
  void slotGeCacheTimer ();

  void slotBoundsClicked (int id);
  void slotNorthEditingFinished ();
  void slotWestEditingFinished ();
  void slotEastEditingFinished ();
  void slotSouthEditingFinished ();

  void slotPolyClicked ();
  void slotClosePolyClicked ();
  void slotClearPolyClicked ();

  void slotGoogleEarthClicked (bool checked);
  void slotGoogleEarthError (QProcess::ProcessError error);
  void slotGoogleEarthDone (int exitCode, QProcess::ExitStatus exitStatus);
  void slotGoogleEarthLink (bool checked);

  void slotBuildGoogleEarthError (QProcess::ProcessError error);
  void slotBuildGoogleEarthDone (int exitCode, QProcess::ExitStatus exitStatus);
  void slotBuildCache ();

  void slotSaveCacheClicked ();
  void slotLoadCacheClicked ();

  void slotBoxSizeChanged (int value);
  void slotCacheUpdateChanged (int value);

  void slotPositionClicked (int id);
  void slotWarningColor ();

  void slotGeNameEditingFinished ();
  void slotCacheBrowseClicked ();

  void slotFont ();
  void slotIconSizeChanged (int index);

  void slotHelp ();
  void slotQuit ();


private:

  void keyPressEvent (QKeyEvent *e);

};

#endif
