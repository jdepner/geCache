
/********************************************************************************************* 

    geCacheHelp.hpp

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


using namespace std;


const QChar helpDegC (0260);


QString geCacheAboutText = geCache::tr 
  ("<center><br><br><img source=\":/icons/geCache.png\"><br>"
   "<h3>geCache</h3></center><br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "This program is a Google Earth caching program that will run on Linux and Windows 10.  There are a few Google Earth caching programs for "
   "different versions of Windows but, as far as I know, none for Linux.  Until now.  The purpose of this program is to allow you to cache "
   "Google Earth data so that you can look at it in Google Earth while offline (for example, out in the middle of the boonies).<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "There are a few things you can do with this program.  First, build Google Earth cache data.  Second, save the cached data to a directory.  "
   "Third, load the cached data back into the Google Earth cache location for use offline.  You can also start and stop Google Earth from within "
   "geCache and use the <b>Ctrl-Shift-C</b> keypress in Google Earth to define the rectangular or polygonal area to be cached.<br><br>"


   "<center><h3>Some setup information</h3></center>"
   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "First, you must make sure that your PATH environment variable includes the directory that contains the Google Earth executable (or script in the "
   "case of Linux).  You'll need to know the name of the executable or script too.  The default in geCache is <b>googleearth.exe</b> on Windows.  "
   "On Linux, geCache will try both <b>googleearth</b> and <b>google-earth</b>.  If it can't find either it will default to <b>googleearth</b> "
   "and you'll have to find it, make sure it's in the PATH, and then manually set it in the <b>Preferences</b> tab.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "By default, geCache tries to find the Google Earth directory that contains the cache data the first time it starts.  On Linux it will always "
   "be set to <b>$HOME/.googleearth/Cache</b>.  On Windows it could be <b>$USERPROFILE\\AppData\\Local\\Google\\GoogleEarth</b> or "
   "<b>$USERPROFILE\\AppData\\LocalLow\\Google\\GoogleEarth</b>.  geCache will try to find it the first time it starts.  If it can't find it "
   "then you'll have to figure out where it is and set it manually in the <b>Preferences</b> tab.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "Note that geCache doesn't save or load <b><i>just</i></b> the cache.  The reason for this is that the name of the directory that the cache is stored in "
   "has changed with different versions of Google Earth.  The cache is usually the lion's share of the data in this directory anyway so it's kind of irrelevant.  "
   "Plus, this way I'm sure that I get everything I need to run offline.  Obviously, using these directories for cache means that <b>$USERPROFILE</b> "
   "must be set correctly on Windows and <b>$HOME</b> must be set correctly on Linux.  I have never seen either of these set incorrectly before, but you "
   "never know.  Since the existing cache gets deleted when you build or load cache, the first thing you should do the first time you run "
   "geCache is to <b><i><font color=\"#ff0000\">SAVE YOUR CACHE FOLDER!</font></i></b>  Just click on the <b>Save cache</b> button in the "
   "<b>Cache</b> tab and save it somewhere with a very descriptive name.  That way if the fewmets hit the windmill you can always load that directory "
   "back using geCache or restore it manually.<br><br>" 

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "You need to run Google Earth first (or run it from <b>Google Earth Preview</b> in the <b>Cache</b> tab) and set the Google Earth "
   "<b>Memory Cache Size</b> to the smallest allowable value (32MB at present).  This will force it to unload to disk more often.  You need to set the "
   "<b>Disk Cache Size</b> to the largest allowable value (2048MB at present) so that you can store as much information as possible in the Cache "
   "directory.  Both of these options can be set in Google Earth under <b>Tools->Options->Cache</b>.  You also need to increase your "
   "<b>Fly-To-Speed</b> (about half way is good) and set <b>Do not automatically tilt while zooming</b>.  These are under <b>Tools->Options->Navigation</b>.<br><br>"


   "<center><h3>How caching works</h3></center>"
   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "Google Earth, being a well-written and well-behaved application, tries to save you time by caching data for any areas that you have been viewing.  "
   "This makes the program run much faster (and takes a load off of the data servers) since it doesn't have to keep downloading the same images over "
   "and over.  The geCache program takes advantage of this behavior to allow you to run Google Earth without an internet connection by caching "
   "all of the data you might need for one or more areas that you want explore.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "The easiest way to set up caching data in geCache is to switch to the <b>Cache</b> tab, select the <b>Rectangle</b> tab, open Google Earth by pressing "
   "the <b>Google Earth</b> button <img source=\":/icons/Earth_Icon_small.png\">, navigate (fly, in Google Earth terms) to the area you're interested in, "
   "then use the corner buttons (e.g. <img source=\":/icons/compass_NW_small.png\">) to grab the northwest and southeast corners or the northeast and "
   "southwest corners from Google Earth.  If you want to set the boundaries individually you can use the boundary buttons "
   "(e.g. <img source=\":/icons/compass_E_small.png\">).  Just click on the button, move the cursor to the desired location in Google Earth, then press "
   "<b>Ctrl-Shift-C</b> to set the boundaries (e.g. north and west when using the northwest corner button or just east when using the east boundary button).  If "
   "you'd like to see the outline of the area you've selected you can click on the <b>Link</b> button <img source=\":/icons/Earth_link_small.png\">.  This "
   "will cause Google Earth to zoom to the approximate area that you have defined and draw the outline.  You can then use the corner and/or boundary buttons to "
   "refine the area.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "If the area you're interested in doesn't really fit well in a rectangle (e.g. a peninsula), you can switch to the <b>Polygon</b> tab (after you've defined "
   "a rectangle containing the area of interest) and define a polygon.  You must have Google Earth running and you must have it linked or the polygon buttons "
   "will be disabled.  To start defining a polygon, click on the <b>Add position</b> button </b><img source=\":/icons/add_poly_small.png\">.  When clicked, "
   "the button background will become the <b>Warning color</b>.  To add points, position the cursor in Google Earth and then press <b>Ctrl-Shift-C</b>.  Positions "
   "will be added no faster than once per second.  Google Earth will update the view every <b>Cache build update frequency</b> seconds and show the "
   "lines for the polygon.  When finished click the <b>Close polygon</b> <img source=\":/icons/close_poly_small.png\"> button.  You can clear the "
   "points by clicking the <b>Clear polygon</b> <img source=\":/icons/clear_poly_small.png\"> button.  You can also edit the polygon after closing it.  "
   "Just click on the <b>Add position</b> button, then position the cursor in Google Earth near the point you want to edit, and press <b>Ctrl-Shift-C</b>.  "
   "At that point the position in the polygon points list nearest to that location in Google Earth will be highlighted.  Once that happens just move the "
   "cursor in Google Earth to the location you want to move the position to, then press <b>Ctrl-Shift-C</b> again.  The polygon in Google Earth will not update "
   "until <b>Cache build update frequency</b> seconds has elapsed.  In order to perform the build using the polygon you must have the <b>Polygon</b> tab open "
   "when you start the build process.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "Once you've selected your area, set the initial area size, and set the update frequency, just press the <b>Build cache</b> button.  geCache will then "
   "begin building the cache.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "When geCache builds a cache using a rectangular area, Google Earth will be started and zoomed to a viewing area in the southwest corner of the <b>Area "
   "to be cached</b> (we'll call this the cache area) that is approximately a square the size (in meters) of the value in <b>Cache build initial area "
   "size</b> (for the sake of simplicity in the rest of this explanation we'll use 2000 meters for our initial size).  Google Earth will then wait "
   "with that 2000 meter square area displayed for 20 seconds before it moves to the east by 2000 meters.  The 20 second wait allows Google Earth to "
   "get completely stable prior to starting the caching process.  After the first move it will then wait <b>Cache build update frequency</b> seconds "
   "(we'll use 6 seconds for this explanation) before it moves to the east again.  All subsequent moves will be at 6 second intervals.  This will be "
   "repeated until the east side of the cache area is reached, at which time the viewing area will move north, pause for 6 seconds, and then begin "
   "moving west.  Once the viewing area reaches the western boundary of the cache area it will move north, pause for 6 seconds, and then go back east "
   "in 2000 meters steps at 6 second intervals.  This is called a <b>Snake dance</b> (named after the lines in Disney World).  When the entire area "
   "has been covered, the viewing area will be reset to the cache area and Google Earth will pause for twice the update frequency seconds prior to exiting.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "When geCache builds a cache using a polygonal area, the same procedure is followed except that the search rectangle is defined as the minimum bounding "
   "rectangle of the polygon and viewing areas (<b>Cache build initial area size</b> squares) that do not overlap with the polygon are skipped.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "Because Google Earth is run as a separate process, whenever the build process finishes I have to stop the script with a SIGKILL signal on Linux or "
   "just kill the QProcess on Windows.  If you're running from the command line, this results in a <b>QProcess: Destroyed while process is still "
   "running</b> message.  <b><i><font color=\"#ff0000\">DO NOT PANIC</font></i></b>, that's a normal thing.  The same message will appear whenever "
   "you close Google Earth using the <b>Google Earth</b> button or the <b>Close</b> button.  Because of this, if you change any settings in Google "
   "Earth they won't be saved unless you manually exit Google Earth by clicking on the <b>Exit</b> item in the Google Earth <b>File</b> menu.  The "
   "Google Earth button in geCache will return to normal if you exit from Google Earth that way.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "While the build is running the <b>Cache build progress</b> bar will be updated.  Since I'm doing my own timing internally, and Google Earth is "
   "doing its own timing internally, and never the twain shall meet, things may occasionally get a bit out of sync.  You will sometimes see waits of "
   "approximately twice the <b>Cache build update frequency</b>.  Due to this, the progress bar and the <b>Estimated remaining time</b> field in the "
   "progress bar title are <b><i><font color=\"#ff0000\">APPROXIMATE</font></i></b>.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "To give you an idea of how long it takes to cache an area, I cached a 70 by 60 kilometer area using 2000 meter initial area size and a 7 second "
   "update frequency in about 2 hours.  The estimated time to do the same area using 1000 meter initial area size was 8 hours so make sure you need "
   "that level of detail before you start caching.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "After the build has run you can save the Google Earth cache directory so that you can load it at a later time (like when you're driving your "
   "4x4 out in the boonies).<br><br>"

 
   "<center><h3>How every other stinking widget works</h3></center>"
   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "For more detailed information about each widget (e.g. button, field, label...) in <b>geCache</b> just click the "
   "<b>Help</b> button <img source=\":/icons/contextHelp_small.png\"> in the lower left corner and then click on the widget.  That wasn't so hard, "
   "was it?<br><br>"


   "<center><h3>Google Earth</h3></center>"
   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "This program launches Google Earth as an external process.  Google Earth is one of the most amazing applications on the planet (or of the "
   "planet for that matter).  If you don't have Google Earth just click on the following link and explore your way to the download page, download it, "
   "and then install it:<br>"


   "<center><b><a href=\"https://www.google.com/earth\">Google Earth</a></b></center><br><br>"
   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "Google Earth is a trademark of Google Inc. - keep that firmly in mind when using geCache.  In addition, neither this application nor the "
   "author of this application is endorsed by or affiliated in any way with Google Inc.<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "If you would like to use images or other goodies from Google Earth you should check out the "
   "<b><a href=\"https://www.google.com/permissions/\">Google permissions</a></b> page.<br><br>"


   "<center><h3>Disclaimer</h3></center>"
   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "The geCache program is Copyright (c) 2016 Jan C. Depner (jan@pfmabe.software) and is released (for the most part) under version 3 of the GNU "
   "General Public License (GPLv3).<br><br>"
   "geCache is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by "
   "the Free Software Foundation; either version 3 of the License, or (at your option) any later version.<br><br>"
   "geCache is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or "
   "FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.<br><br>"
   "You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., "
   "51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA<br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "Although most of this program is licensed under GPLv3 there are a few files that are in the public domain that were written "
   "either by myself, by one or more of my co-workers when I worked for the Naval Oceanographic Office, or by someone at NOAA (in FORTRAN, in "
   "the dim distant past, and then converted to C).  Those files are:<br>"
   "<ul>"
   "<li>qPosfix.cpp</li>"
   "<li>qPosfix.hpp</li>"
   "<li>functions.c</li>"
   "<li>functions.h</li>"
   "</ul><br><br>"

   "&nbsp;&nbsp;&nbsp;&nbsp;"
   "Each of these files has a statement at the beginning of the file explaining that they are in the public domain.<br><br>");

QString cacheGoogleEarthText = geCache::tr
  ("<img source=\":/icons/Earth_Icon_small.png\"> Click this button to start (or stop) the Google Earth application.  When clicked, Google "
   "Earth will open and display the area defined in the <b>Cache/preview area</b> fields.  As long the Google Earth link button is depressed, Google "
   "Earth will remain locked to the defined area.  If you press the Google Earth link button it will allow you to zoom and pan in "
   "Google Earth.  To return to the defined area just click the Google Earth link button again.<br><br>"
   "This button will be disabled if you do not have Google Earth installed on your computer.  To download and install Google Earth just switch to the "
   "<b>About</b> tab, scroll down until you get to the <b>Google Earth</b> section, click on the Google Earth link then follow the directions.<br><br>"
   "<b>IMPORTANT NOTE: Make sure that your PATH environment variable includes the folder that contains the Google Earth executable. "
   "If it's not in the PATH then the Google Earth button will be disabled.</b><br><br>"
   "<b>TIPS:"
   "<ul>"
   "<li>In the Google Earth navigation options you need to increase the <i>Fly-To Speed</i> unless, of course, you like watching "
   "the Google Earth globe slowly spin to your area of interest.</li>"
   "<li>In the Google Earth navigation options you need to set <i>Do not automatically tilt while zooming</i>.</li>"
   "<li>If you exit from Google Earth by closing it (not using the Google Earth button) you probably want to <i>Discard</i> any unsaved "
   "items in your <i>Temporary Places</i> folder.</li>"
   "</ul>"
   "</b>");

QString cacheGoogleEarthLinkText = geCache::tr
  ("<img source=\":/icons/Earth_link_small.png\"> Click this button to link/unlink Google Earth to/from geCache.  When the button is depressed, "
   "Google Earth will remain locked to the <b>Cache/preview area</b>.  When the button is not depressed you can zoom and pan without the view "
   "reverting to the <b>Cache/preview area</b> every few seconds.  When you want to go back to the <b>Cache/preview area</b>, just press this button "
   "again.<br><br>"
   "<b>IMPORTANT NOTE: This button will not cause an instantaneous reaction in Google Earth.  Google Earth will only check the link file at the "
   "time interval that you have set in <i>Cache build update frequency</i>.  If you have ADHLAS (that's Attention Defi...Hey Look A Squirrel) don't "
   "start slapping buttons all over the place when it doesn't immediately respond.</b>");

QString NWText = geCache::tr
  ("<img source=\":/icons/compass_NW_small.png\"> Click this button to retrieve the northwest corner of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired northwest corner and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its latitude and longitude components "
   "into the north latitude and west longitude boundaries respectively.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  To stop waiting for clipboard input just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString northText = geCache::tr
  ("Set the northern boundary of the area to be cached.  Allowable formats are:"
   "<ul>"
   "<li><b>Hemisphere</b> Degrees Minutes Seconds.decimal</li>"
   "<li><b>Hemisphere</b> Degrees Minutes.decimal</li>"
   "<li><b>Hemisphere</b> Degrees.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes Seconds.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes.decimal</li>"
   "<li><b>+/-</b>Degrees.decimal</li>"
   "</ul><br>"
   "You can also use the following formats (copied from Google Earth):"
   "<ul>"
   "<li>Degrees%1Minutes'Seconds.decimal, \" <b>Hemisphere</b></li>"
   "<li>Degrees%1Minutes.decimal' <b>Hemisphere</b></li>"
   "<li><b>+/-</b>Degrees.decimal%1</li>"
   "</ul><br>"
   "Where <b>Hemisphere</b> is <b>N</b>, <b>n</b>, <b>S</b>, or <b>s</b>, and <b>+/-</b> is either <b>+</i> or <b>-</i>.<br><br>").arg (helpDegC);

QString NEText = geCache::tr
  ("<img source=\":/icons/compass_NE_small.png\"> Click this button to retrieve the northeast corner of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired northeast corner and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its latitude and longitude components "
   "into the north latitude and east longitude boundaries respectively.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  If you decide that you don't want to copy and paste a position from Google Earth just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString NText = geCache::tr
  ("<img source=\":/icons/compass_N_small.png\"> Click this button to retrieve the northern boundary of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired northern boundary and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its latitude component "
   "into the north latitude boundary.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  If you decide that you don't want to copy and paste a position from Google Earth just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString westText = geCache::tr
  ("Set the western boundary of the area to be cached.  Allowable formats are:"
   "<ul>"
   "<li><b>Hemisphere</b> Degrees Minutes Seconds.decimal</li>"
   "<li><b>Hemisphere</b> Degrees Minutes.decimal</li>"
   "<li><b>Hemisphere</b> Degrees.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes Seconds.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes.decimal</li>"
   "<li><b>+/-</b>Degrees.decimal</li>"
   "</ul><br>"
   "You can also use the following formats (copied from Google Earth):"
   "<ul>"
   "<li>Degrees%1Minutes'Seconds.decimal\" <b>Hemisphere</b></li>"
   "<li>Degrees%1Minutes.decimal' <b>Hemisphere</b></li>"
   "<li><b>+/-</b>Degrees.decimal%1</li>"
   "</ul><br>"
   "Where <b>Hemisphere</b> is <b>E</b>, <b>e</b>, <b>W</b>, or <b>w</b>, and <b>+/-</b> is either <b>+</b> or <b>-</b>.<br><br>").arg (helpDegC);

QString WText = geCache::tr
  ("<img source=\":/icons/compass_W_small.png\"> Click this button to retrieve the western boundary of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired western boundary and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its longitude component "
   "into the west longitude boundary.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  If you decide that you don't want to copy and paste a position from Google Earth just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString EText = geCache::tr
  ("<img source=\":/icons/compass_E_small.png\"> Click this button to retrieve the eastern boundary of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired eastern boundary and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its longitude component "
   "into the east longitude boundary.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  If you decide that you don't want to copy and paste a position from Google Earth just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString eastText = geCache::tr
  ("Set the eastern boundary of the area to be cached.  Allowable formats are:"
   "<ul>"
   "<li><b>Hemisphere</b> Degrees Minutes Seconds.decimal</li>"
   "<li><b>Hemisphere</b> Degrees Minutes.decimal</li>"
   "<li><b>Hemisphere</b> Degrees.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes Seconds.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes.decimal</li>"
   "<li><b>+/-</b>Degrees.decimal</li>"
   "</ul><br>"
   "You can also use the following formats (copied from Google Earth):"
   "<ul>"
   "<li>Degrees%1Minutes'Seconds.decimal\" <b>Hemisphere</b></li>"
   "<li>Degrees%1Minutes.decimal' <b>Hemisphere</b></li>"
   "<li><b>+/-</b>Degrees.decimal%1</li>"
   "</ul><br>"
   "Where <b>Hemisphere</b> is <b>N</b>, <b>n</b>, <b>S</b>, or <b>s</b>, and <b>+/-</b> is either <b>+</b> or <b>-</b>.<br><br>").arg (helpDegC);

QString SText = geCache::tr
  ("<img source=\":/icons/compass_S_small.png\"> Click this button to retrieve the southern boundary of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired sothern boundary and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its latitude component "
   "into the south latitude boundary.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  If you decide that you don't want to copy and paste a position from Google Earth just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString SWText = geCache::tr
  ("<img source=\":/icons/compass_SW_small.png\"> Click this button to retrieve the southwest corner of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired southwest corner and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its latitude and longitude components "
   "into the south latitude and west longitude boundaries respectively.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  If you decide that you don't want to copy and paste a position from Google Earth just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString southText = geCache::tr
  ("Set the southern boundary of the area to be cached.  Allowable formats are:"
   "<ul>"
   "<li><b>Hemisphere</b> Degrees Minutes Seconds.decimal</li>"
   "<li><b>Hemisphere</b> Degrees Minutes.decimal</li>"
   "<li><b>Hemisphere</b> Degrees.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes Seconds.decimal</li>"
   "<li><b>+/-</b>Degrees Minutes.decimal</li>"
   "<li><b>+/-</b>Degrees.decimal</li>"
   "</ul><br>"
   "You can also use the following formats (copied from Google Earth):"
   "<ul>"
   "<li>Degrees%1Minutes'Seconds.decimal\" <b>Hemisphere</b></li>"
   "<li>Degrees%1Minutes.decimal' <b>Hemisphere</b></li>"
   "<li><b>+/-</b>Degrees.decimal%1</li>"
   "</ul><br>"
   "Where <b>Hemisphere</b> is <b>N</b>, <b>n</b>, <b>S</b>, or <b>s</b>, and <b>+/-</b> is either <b>+</b> or <b>-</b>.<br><br>").arg (helpDegC);

QString SEText = geCache::tr
  ("<img source=\":/icons/compass_SE_small.png\"> Click this button to retrieve the southeast corner of the <b>Cache/preview area</b> from Google "
   "Earth.  After clicking this button, position the cursor in Google Earth to the desired southeast corner and press <b>Ctrl-Shift-C</b>.  This will "
   "place the position from Google Earth into the clipboard.  geCache will retrieve the position and place its latitude and longitude components "
   "into the south latitude and east longitude boundaries respectively.  The button will remain pressed down until correct data is retrieved from the "
   "clipboard.  If you decide that you don't want to copy and paste a position from Google Earth just click this button again.<br><br>"
   "<b>IMPORTANT NOTE: Google Earth must have its position format set to one of the geodetic options.  Universal Transverse Mercator (UTM) will not "
   "work.  The position format is under Tools->Options in Google Earth.</b>");

QString meterWidthText = geCache::tr
  ("This is the width of the <b>Cache/preview area</b> based on the latitudes and longitudes defined in the <b>North</b>, <b>South</b>, <b>East</b>, "
   "and the <b>West</b> fields.");

QString meterHeightText = geCache::tr
  ("This is the height of the <b>Cache/preview area</b> based on the latitudes and longitudes defined in the <b>North</b>, <b>South</b>, <b>East</b>, "
   "and the <b>West</b> fields.");

QString rectEstTimeText = geCache::tr
  ("This is the estimated amount of time it will take to build the cache based on the <b>Cache/preview area</b>, the <b>Cache build initial area "
   "size</b>, and <b>Cache build update frequency</b>.  Please note that this is only an estimate.");

QString polyText = geCache::tr
  ("<img source=\":/icons/add_poly_small.png\"> Click this button to begin adding geographic positions to a polygon or to edit existing points in a completed "
   "polygon.  If you are defining the polygon, when clicked, the button background will become the <b>Warning color</b>.  To add points, position the cursor "
   "in Google Earth and then press <b>Ctrl-Shift-C</b>.  Positions will be added no faster than once per half second.  Google Earth will update the view "
   "every <b>Cache build update frequency</b> seconds and show the lines for the polygon.  When finished click the <b>Close polygon</b> "
   "<img source=\":/icons/close_poly_small.png\"> button.  You can clear the points by clicking the <b>Clear polygon</b> "
   "<img source=\":/icons/clear_poly_small.png\"> button.<br><br>"
   "You can also edit the polygon after closing it.  Just click on the <b>Add position</b> button, then position the cursor in Google Earth near the point you "
   "want to edit, and press <b>Ctrl-Shift-C</b>.  At that point, the position in the polygon points list nearest to that location in Google Earth will be "
   "highlighted.  Once that happens just move the cursor in Google Earth to the location you want to move the position to, then press <b>Ctrl-Shift-C</b> again.  "
   "The polygon in Google Earth will not update until <b>Cache build update frequency</b> seconds has elapsed.<br><br>"
   "<b>IMPORTANT NOTE: You must define a rectangular area, start Google Earth, and then link Google Earth to geCache before you can begin defining "
   "polygon points.  Also, you must clear the polygon points if you wish to define a new polygon.</b>");

QString closePolyText = geCache::tr
  ("<img source=\":/icons/close_poly_small.png\"> Click this button to finish defining the polygon.  The polygon will be automatically closed so "
   "you don't need to try to get near the initial polygon point.  Google Earth will update the view within <b>Cache build update frequency</b> seconds and show the "
   "finished polygon.");

QString clearPolyText = geCache::tr
  ("<img source=\":/icons/clear_poly_small.png\"> Click this button to clear all previously defined polygon points.");

QString verticesText = geCache::tr
  ("This is the list of polygon vertex positions.");

QString numBoxesText = geCache::tr
  ("This is the number of <b>Cache build initial area size</b> squares that was required to completely cover the geographic polygonal area defined by the "
   "<b>Polygon points</b> in the <b>Cache/preview area</b>.");

QString polyEstTimeText = geCache::tr
  ("This is the estimated amount of time it will take to build the cache based on the <b>Cache/preview area</b>, the <b>Cache build initial area "
   "size</b>, and <b>Cache build update frequency</b>.  Please note that this is only an estimate.");

QString boxSizeText = geCache::tr
  ("This is the area size (in meters) of the smallest area that will be used to build a new Google Earth cache.  The way the build process works for a "
   "rectangular area is that Google Earth will be started and zoomed to an area in the southwest corner of the <b>Cache/preview area</b> that is a square "
   "approximately this size.  Google Earth will then wait with that area displayed for 20 seconds before it moves to the east by the same size.  The 20 "
   "second wait allows Google Earth to get completely stable prior to starting the caching process.  After the first move it will then wait <b>Cache build "
   "update frequency</b> seconds before it moves to the east again.  All subsequent moves will be at that frequency.  This will be repeated until the east "
   "side of the <b>Cache/preview area</b> is reached, at which time the view will move north, pause for <b>Cache build update frequency</b> seconds, "
   "and then begins moving west.  Once the viewing area reaches the western boundary of the <b>Cache/preview area</b> it will move up, pause, and then "
   "go back east.  This is called a <b>Snake dance</b> (named after the lines in Disney World).  When the entire area "
   "has been covered, the viewing area will be reset to the cache area and Google Earth will pause for twice the update frequency seconds prior to exiting.<br><br>"
   "When geCache builds a cache using a polygonal area, the same procedure is followed except that the search rectangle is defined as the minimum bounding "
   "rectangle of the polygon and viewing areas (<b>Cache build initial area size</b> squares) that do not overlap with the polygon are skipped.<br><br>"
   "<b>IMPORTANT NOTE: Using an area size of 1000 meters will cause the process to take quite a while to run.  I've found that 3000 meters is a "
   "reasonable area size and it will get almost the highest resolution image.</b>");

QString cacheUpdateText = geCache::tr
  ("This is the update frequency (in seconds) at which the Google Earth view will move when building a new Google Earth cache.  The way the build "
   "process works is that Google Earth will be started and zoomed to an area in the southwest corner of the cache area that is approximately "
   "this size in width and height.  Google Earth will then wait with that area displayed for <b>Cache build update frequency</b> seconds before it "
   "moves to the east by approximately this amount.  This will be repeated until the east side of the cache area is reached, at which time the view "
   "will move north and then move west.  This is called a <b>Snake dance</b> (named after the lines in Disney World).  When the entire area "
   "has been covered, the viewing area will be reset to the cache area and Google Earth will pause for twice the update frequency seconds prior to exiting.<br><br>"
   "<b>IMPORTANT NOTE: This button will be disabled while Google Earth is running.  You must close Google Earth, change this value, and then restart "
   "Google Earth.<br><br>"
   "TIP: Using an area size of 1000 meters will cause the process to take quite a while to run.  I've found that 3000 meters is a "
   "reasonable area size and it will get pretty decent resolution images.</b>");

QString buildCacheText = geCache::tr
  ("Build a new Google Earth disk cache based on the area and options set in the <b>Cache</b> tab.  If the cache becomes too close to the maximum size you "
   "will be given the option to save the cache directory and continue or to cancel the build process.<br><br>"
   "<b>IMPORTANT NOTE: This button will be disabled if you are running Google Earth to preview an area. "
   "Also, it would be a bad idea to press this button if you are running Google Earth standalone.</b>");

QString killBuildCacheText = geCache::tr
  ("Kill the Google Earth process that is building a disk cache.");

QString saveCacheText = geCache::tr
  ("Build a new Google Earth disk cache based on the area and options set in the <b>Cache</b> tab.<br><br>"
   "<b>IMPORTANT NOTE: This button will be disabled if you are running Google Earth to preview an area. "
   "Also, it would be a bad idea to press this button if you are running Google Earth standalone.</b>");

QString loadCacheText = geCache::tr
  ("Load a saved Google Earth cache directory back to the Google Earth disk cache.<br><br>"
   "<b>IMPORTANT NOTE: This button will be disabled if you are running Google Earth to preview an area. "
   "Also, it would be a bad idea to press this button if you are running Google Earth standalone.</b>");

QString progressText = geCache::tr
  ("This progress bar shows the <b><i>approximate</i></b> progress of the cache build process.  Since I'm doing my own timing internally and Google "
   "Earth is doing its own timing externally, things may occasionally get a bit out of sync.  You will sometimes see waits of approximately twice "
   "the <b>Cache build update frequency</b>.  Due to this, the progress bar and the <b>Estimated remaining time</b> field in the progress bar "
   "title are <b><i>APPROXIMATE</i></b>.<br><br>");

QString positionFormatText = geCache::tr
  ("Select the format in which you want geographic positions to be displayed.");

QString warningColorText = geCache::tr
  ("Select the warning color that you want for the <b>Build cache</b> button when you are building a cache directory.  This is also used for the cache area "
   "boundary fields when you are trying to copy positions from Google Earth using the clipboard (i.e. you pressed one of the corner or boundary buttons).");

QString geNameText = geCache::tr
  ("Change the name of the executable or script that runs Google Earth on your system.  The default is <b>googleearth</b> on Linux or "
   "<b>googleearth.exe</b> on Windows but it may be different (e.g. <b>google-earth</b>).<br><br>"
   "<b>IMPORTANT NOTE: This is <i>not</i> the full path to the file (e.g. /usr/bin/googleearth or "
   "C:\\Program Files (x86)\\Google\\Google Earth\\client\\googleearth.exe) but just the file name (e.g. googleearth, googleearth.exe, "
   "google-earth).</b>");

QString geCacheDirText = geCache::tr
  ("This is the name of the directory that contains the Google Earth cache data.  By default, geCache tries to find this the first time it "
   "starts.  On Linux it is always set to $HOME/.googleearth/Cache.  On Windows it could be $USERPROFILE\\AppData\\Local\\Google\\GoogleEarth or "
   "$USERPROFILE\\AppData\\LocalLow\\Google\\GoogleEarth.  Note that I'm not saving or loading just the cache.  The reason for this is that the name "
   "of the directory that stores just the cache has changed with different versions of Google Earth.  The cache is usually the lion's share of the data "
   "in this directory anyway so it's kind of irrelevant.  Plus, this way I'm sure that we got everything we need to run offline.");

QString bCacheBrowseText = geCache::tr
  ("Click this button to browse for the directory that contains the Google Earth cache data.  By default, geCache tries to find this directory "
   "the first time it starts.  On Linux it is always set to $HOME/.googleearth/Cache.  On Windows it could be "
   "$USERPROFILE\\AppData\\Local\\Google\\GoogleEarth or $USERPROFILE\\AppData\\LocalLow\\Google\\GoogleEarth.  Note that I'm not saving or loading "
   "just the cache.  The reason for this is that the name of the directory that stores just the cache has changed with different versions of Google "
   "Earth.  The cache is usually the lion's share of the data in this directory anyway so it's kind of irrelevant.  Plus, this way I'm sure that we got "
   "everything we need to run offline.");

QString fontText = geCache::tr
  ("Click this button to change the font used for text in the application.");

QString iconSizeText = geCache::tr
  ("Set the size of most of the buttons that contain icons (e.g. Google Earth button).  The options are 16, 20, 24, 28, or 32 pixels.<br><br>"
   "<b>IMPORTANT NOTE: The button size will change when you select the size but the locations may not be entirely correct until you have exited "
   "and restarted the program.</b>");

QString closeText = geCache::tr
  ("Click this button to exit from the geCache program.  It will also kill any instance of Google Earth that you are running.");
