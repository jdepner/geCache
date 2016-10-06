
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


#ifndef _NAVO_FUNCTIONS_H_
#define _NAVO_FUNCTIONS_H_

#ifdef  __cplusplus
extern "C" {
#endif


#define NVFalse 0
#define NVTrue 1

#define __STDC_FORMAT_MACROS
#define __STDC_LIMIT_MACROS

#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>


#ifdef _MSC_VER
  #include <io.h>
  #include <process.h>
  #define SEPARATOR '\\'
  #define PATH_SEPARATOR ';'
#else
  #include <unistd.h>
  #define SEPARATOR '/'
  #define PATH_SEPARATOR ':'
#endif


  /*  Minimum bounding rectangle.  */

  typedef struct
  {
    double          min_y;
    double          min_x;
    double          max_y;
    double          max_x;
  } NV_F64_XYMBR;


  /*  Coordinate.  */

  typedef struct
  {
    double          x;
    double          y;
  } NV_F64_COORD2;


  /*  WGS-84 semi-major & semi-minor axes.  */

#ifndef NV_A0
  #define       NV_A0           6378137.0L
#endif

#ifndef NV_B0
  #define       NV_B0           6356752.314245L
#endif


  /*  Debugging stuff.  */
#ifndef NVFFL
  #define       NVFFL           __FILE__,__FUNCTION__,__LINE__
#endif

#ifndef DPRINT
  #define       DPRINT          fprintf (stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);fflush (stderr);
#endif



  /*  Macro for inline function angle.  */

#define direct_angle(omg, sn, cs, c4, css, c3, c2)        ( (omg) + (sn)*(cs)*(1.0e-6)*( (c4) + (css)*((c3) + (c2)*(css)) ) )



  uint8_t polygon_intersection (NV_F64_COORD2 *poly1, int32_t npol1, NV_F64_COORD2 *poly2, int32_t npol2);
  void direct(double phi, double alam, double fazi, double s, double *phipri, double *alampr);
  void invgp (double a0, double b0, double rlat1, double rlon1, double rlat2, double rlon2, double *dist, double *az);
  void newgp(double latobs, double lonobs, double az, double dist, double *lat, double *lon);


#ifdef  __cplusplus
}
#endif

#endif
