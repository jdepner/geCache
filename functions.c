
/*********************************************************************************************

    This is public domain software that was developed by or for the National Oceanic and
    Atmospheric Administration, the U.S. Naval Oceanographic Office, and/or the U.S. Army
    Corps of Engineers.

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



#include "functions.h"



/***************************************************************************/
/*!
  - Module Name:        inside_polygon

  - Programmer(s):      Jan C. Depner

  - Date Written:       December 2014

  - Purpose:            Based on the CrossingsMultiplyTest method described
                        by E. Haines:
                        http://erich.realtimerendering.com/ptinpoly/


  - Arguments (inside_polygon):
                        - poly     =   Array of NV_F64_COORD2 structures holding
                                       the polygon points
                        - npol     =   Number of points in the polygon
                        - x        =   X value of point to be tested
                        - y        =   Y value of point to be tested

  - Return Value:       1 if point is inside polygon, 0 if outside

  - Caveats:            The last point of the polygon must be the same as
                        the first point and the vertices of the polygon must
			be in order around the polygon.

****************************************************************************/

int32_t inside_polygon (NV_F64_COORD2 *poly, int32_t npol, double x, double y)
{
  int32_t    i, j, yflag0, yflag1, inside_flag;

  inside_flag = 0;


  j = npol - 1;
  yflag0 = (poly[j].y >= y);

  for (i = 0 ; i < npol ; i++)
    {
      yflag1 = (poly[i].y >= y);


      if (yflag0 != yflag1)
	{
	  if (((poly[i].y - y) * (poly[j].x - poly[i].x) >= (poly[i].x - x) * (poly[j].y - poly[i].y)) == yflag1)
	    {
	      inside_flag = !inside_flag;
	    }
	}

      yflag0 = yflag1;
      j = i;
    }

  return (inside_flag);
}



/************************************************************************/
/*!

  - Module Name:    line_intersection

  - Programmer:     Jan C. Depner

  - Date Written:   September 2006

  - Purpose:        Determines if two lines intersect.  If so it
                    returns the intersection point.

  - Inputs:
                    - x1            =  starting x coordinate line 1
                    - y1            =  starting y coordinate line 1
                    - x2            =  ending x coordinate line 1
                    - y2            =  ending y coordinate line 1
                    - x3            =  starting x coordinate line 2
                    - y3            =  starting y coordinate line 2
                    - x4            =  ending x coordinate line 2
                    - y4            =  ending y coordinate line 2
                    - x             =  intersection x coordinate
                    - y             =  intersection y coordinate

  - Outputs:
                    - 0 = no intersection
                    - 1 = lines intersect but the segments don't
                    - 2 = segments intersect



  - Referenced from: http://en.wikipedia.org/wiki/Line_segment_intersection


  - Derived from: http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
                  Intersection point of two lines (2 dimensions)
                  Paul Bourke, April 1989



  - This note describes the technique and algorithm for determining the intersection point of two lines
    (or line segments) in 2 dimensions.

  <pre>
                             P3        P2
                              \        /
                        line b \      /
                                \    /
                                 \  /
                                  \/
                                  /\
                                 /  \
                                /    \
                        line a /      \
                              /        \
                             P1        P4


    The equations of the lines are

    Pa = P1 + ua ( P2 - P1 )

    Pb = P3 + ub ( P4 - P3 )

    Solving for the point where Pa = Pb gives the following two equations in two unknowns (ua and ub) 
    x1 + ua (x2 - x1) = x3 + ub (x4 - x3)

    and
    y1 + ua (y2 - y1) = y3 + ub (y4 - y3)

    Solving gives the following expressions for ua and ub

    Substituting either of these into the corresponding equation for the line gives the intersection point.
    For example the intersection point (x,y) is
    x = x1 + ua (x2 - x1)

    y = y1 + ua (y2 - y1)


    Notes:

    * The denominators for the equations for ua and ub are the same.

    * If the denominator for the equations for ua and ub is 0 then the two lines are parallel.

    * If the denominator and numerator for the equations for ua and ub are 0 then the two lines are coincident.

    * The equations apply to lines, if the intersection of line segments is required then it is only necessary to test
      if ua and ub lie between 0 and 1. Whichever one lies within that range then the corresponding line segment
      contains the intersection point. If both lie within the range of 0 to 1 then the intersection point is within
      both line segments. 

</pre>
 ************************************************************************/

int32_t line_intersection (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double *x, double *y)
{
  double denominator, ua, ub;


  denominator = ((y4 -y3) * (x2 - x1) - (x4 -x3) * (y2 - y1));

  if (denominator == 0.0) return (0);


  ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
  ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

  *x = x1 + ua * (x2 - x1);
  *y = y1 + ua * (y2 - y1);

  if (ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0) return (2);


  return (1);
}



/***************************************************************************/
/*!

  - Module Name:        polygon_intersection

  - Programmer(s):      Jan C. Depner

  - Date Written:       December 11, 2009

  - Purpose:            This function just tells you whether the polygons
                        intersect, not what the intersection is.  This is a
                        brute force method and I'm sure there are better
                        implementations but I really don't need the speed
                        here.

  - Arguments:          poly1        - first polygon
                        poly1_count  - vertex count for first polygon
                        poly2        - second polygon
                        poly2_count  - vertex count for second polygon

  - Return Value:       NVTrue if they intersect, otherwise NVFalse

****************************************************************************/

uint8_t polygon_intersection (NV_F64_COORD2 *poly1, int32_t poly1_count, NV_F64_COORD2 *poly2, int32_t poly2_count)
{
  int32_t              i, j;
  double               x, y;


  /*  Check each point of polygon 1 to see if any lie inside polygon 2.  */

  for (i = 0 ; i < poly1_count ; i++)
    {
      if (inside_polygon (poly2, poly2_count, poly1[i].x, poly1[i].y)) return (NVTrue);
    }


  /*  Check each point of polygon 2 to see if any lie inside polygon 1.  */

  for (i = 0 ; i < poly2_count ; i++)
    {
      if (inside_polygon (poly1, poly1_count, poly2[i].x, poly2[i].y)) return (NVTrue);
    }


  /*  Now we have to check for intersection of any of the lines of each polygon.  */

  for (i = 1 ; i < poly1_count ; i++)
    {
      for (j = 1 ; j < poly2_count ; j++)
        {
          if (line_intersection (poly1[i - 1].x, poly1[i - 1].y, poly1[i].x, poly1[i].y, poly2[j - 1].x, poly2[j - 1].y, poly2[j].x, poly2[j].y, &x, &y) == 2)
            return (NVTrue);
        }
    }


  /*  Now we check the polygon 2 closing line segment against the polygon 1 segments (without the closing segment).  */

  for (i = 1 ; i < poly1_count ; i++)
    {
      if (line_intersection (poly1[i - 1].x, poly1[i - 1].y, poly1[i].x, poly1[i].y, poly2[poly2_count - 1].x, poly2[poly2_count - 1].y,
                             poly2[0].x, poly2[0].y, &x, &y) == 2) return (NVTrue);
    }
      

  /*  Now we check the polygon 1 closing line segment against the polygon 2 segments (without the closing segment).  */

  for (i = 1 ; i < poly2_count ; i++)
    {
      if (line_intersection (poly2[i - 1].x, poly2[i - 1].y, poly2[i].x, poly2[i].y, poly1[poly1_count - 1].x, poly1[poly1_count - 1].y,
                             poly1[0].x, poly1[0].y, &x, &y) == 2) return (NVTrue);
    }
      

  /*  Finally, we check the closing segments against each other.  */

  if (line_intersection (poly2[poly2_count - 1].x, poly2[poly2_count - 1].y, poly2[0].x, poly2[0].y, poly1[poly1_count - 1].x, poly1[poly1_count - 1].y,
                         poly1[0].x, poly1[0].y, &x, &y) == 2) return (NVTrue);


  /*  If we got here there is no intersection.  */

  return (NVFalse);
}



/******************************************************************************/

void direct(double phi, double alam, double fazi, double s, double *phipri, double *alampr)

/*! <pre>

     ..................................................................

        computes the geodetic position (latitude,longitude) and   
        azimuth of an observed station from a station of known      
        geodetic position, with azimuth and distance to the observed
        station given. 

        the spheroid of reference is the clarke spheroid of 1866.

        evaluation is based on equations for the forward position   
        computation developed by the u.s. coast and geodetic survey.
        this method is valid for distances up to 600 miles.         

        reference:
        simmons,lansing g., natural tables for the calculation of   
        geodetic positions, u.s. coast and geodetic survey special  
        publication no.241, 1949.  

        description of parameters
        phi    - geodetic latitude of known station, 
                 in seconds of arc (input)
        alam   - geodetic longitude of known station,
                 in seconds of arc (input)
        fazi   - geodetic forward azimuth of known station,
                 in seconds of arc (input)
        s      - geodetic distance, in meters (input)
        phipri - geodetic latitude of observed station,  
                 in seconds of arc (output)
        alampr - geodetic longitude of observed station,  
                 in seconds of arc (output)

        authors:
        original author   - rosemary e. riordan /noaa/nos/c54x2.
        subroutine author - milton stein /noaa/nos/c542. 

        modified by yann king ,12/30/86 to include datum option.

        modified 10/21/87 by tom stepka.

        moved to vax/vms and fortran 77.
        there are now integers or do loops to cause f77 conversion problems.

        put all horizontal datum constants into common block hdatum.
        call routine hdinit to load these constants with the proper values.

        the original verison of this routine contained a discrepency in
        its definitions of the constants z2 and z4.  in gpxy and marc these
        are called w1 and w3, respectively, and are defined having the
        same magnitude but have the opposite sign.  therefore, in order to
        keep all common block definitions consistent for this version, z2 and
        z4 will change signs in this verison of direct.  fortunately they
        are only used once, as arguments to the inline funcion angle, so
        to keep direct running the same, i changed the signs of z2 and z4
        in this function call.

        modified by tim rulon 22-mar-89

        corrected underflow problem when faz very close to multiple
        of 90 degrees and s is small. did not cause a problem under
        ordinary circumstances. caused floating point trap.

     ..................................................................

        Converted to C by David Fabre, Planning Systems Inc., 15 Mar 94
        for NRL for NAVO

        Had to change the macro "angle" to "direct_angle" due to conflict with 
        Qt 4 libraries.  Jan Depner, 10/19/07

</pre>*/
{

  /* wgs 84 / nad 83 parameters taken from hdinit routine */

  static double axis = 6378137.0,
                esq = 6.69438002292318e-3,
                z2 = 0.1104825480468667,
                z3 = 21.25880271589918,
                z4 = 5048.250737106752,
                z1 = 6367449.145771138,
                w2 = 0.23832988623869,
                w3 = 29.36926254762117,
                w4 = 5022.894084128594,
                w1 = 0.1570487611454482e-6;


  /* local constant */

  static double arc1 = 0.484813681110e-5;


  /* local variables */

  double fazj, x, y, b, phj, sina, ef, xcor, xpri, a, ycor, ypri, cosa,
    cssq, y0, y1, omega, sin1, cos1, css1, phi1, faca, v, va, y2,
    facb, facc, cay, y3, omegb, sin2, cos2, css2, h, applam,
    asinco, dellam;


/****        determined these unnecessary DHF
        double delphi, phimid, applap;
****/


  /*computation of geodetic position of observed station */

  fazj = fazi*arc1;
  x = s * sin(fazj);
  if (fabs(x) < 1.0e-3) x = 0.001;
  y = -s * cos(fazj);
  if (fabs(y) < 1.0e-3) y = 0.001;
  b = pow(y/10000.0, 2.0);
  phj = phi*arc1;
  sina = sin(phj);
  ef = (pow(1.0 - esq*sina*sina, 2.0)*1.0e15)/(3.0*axis*axis*(1.0 - esq));
  xcor = b*ef/2.0;
  xpri = x - xcor*x*1.0e-7;  
  a = pow(xpri/10000.0, 2.0);  
  ycor = ef*a;
  ypri = y + ycor*y*1.0e-7;  
  cosa = cos(phj);
  cssq = cosa*cosa;
  y0 = z1*direct_angle(phj, sina, cosa, -z4, cssq, z3, -z2);
  y1 = y0 + ypri;
  omega = w1*y1;
  sin1 = sin(omega);
  cos1 = cos(omega);
  css1 = cos1*cos1;
  phi1 = direct_angle(omega, sin1, cos1, w4, css1, w3, w2);
  sin1 = sin(phi1);
  cos1 = cos(phi1);
  faca = (sqrt(1.0 - esq*sin1*sin1)) / (2.0*axis);
  v = sin1/cos1*faca*1.0e8;
  va = v*a;
  y2 = y1 - va;
  facb = (1.0 + 3.0*( (sin1*sin1)/(cos1*cos1) )) / (3.0*(sin1/cos1));
  facc = (3.0*esq*sin1*cos1) / (1.0 - esq);
  cay = (faca*(facb - facc))*1.0e6;
  y3 = y2 + cay*pow(va/1000.0, 2.0);
  omegb = w1*y3;
  sin2 = sin(omegb);
  cos2 = cos(omegb);
  css2 = cos2*cos2;
  *phipri = direct_angle(omegb, sin2, cos2, w4, css2, w3, w2);


/****        delphi is related only to the appalp and phimid assignments below DHF
        delphi = phj - *phipri;
****/

  h = sqrt(1.0 - esq*pow(sin(*phipri), 2.0)) / (axis*cos(*phipri)*arc1);
  applam = h*xpri;
  asinco = (v*va)/15.0;
  dellam = applam + applam*asinco*1.0e-7;
  *alampr = alam - dellam;


/****        the 2 assignments below are made for no apparent reason so
             I took them out, perhaps they were output at one time but
             not necessary here DHF
             appalp = dellam * (sin(phj) + sin(*phipri)) / (1.0 + cos(delphi));
             phimid = (phj + *phipri)/2.0e0;
****/


  *phipri /= arc1;

  if (fabs(*alampr) > 648000.0)
    {
      if (*alampr > 0.0)
        {
          *alampr -= 1296000.0;
        }
      else if (*alampr < 0.0)
        {
          *alampr += 1296000.0;
        }
    }

  return;

} /* direct */


/******************************************************************************/




/***************************************************************************/
/*!

  - Module Name:        invgp

  - Programmer:         Unknown

  - Date Written:       When dinosaurs roamed the earth.

  - Modified:           Jan C. Depner, converted to C and changed the
                        arguments to degrees instead of radians.

  - Date:               September, 1992

  - Purpose:            Given the semi-major axis, semi-minor axis, and
                        two geographic positions this routine will compute
                        the distance between the two gp's and the azimuth
                        (clockwise from north) from the first gp to the
                        second gp.

  - Inputs:
                        - a0                  =   semi-major axis in meters
                        - b0                  =   semi-minor axis in meters
                        - rlat1               =   start latitude in degrees
                        - rlon1               =   start longitude in degrees
                        - rlat2               =   end latitude in degrees
                        - rlon2               =   end longitude in degrees
                        - dist                =   distance in meters
                        - az                  =   azimuth in degrees

  - Outputs:            none

****************************************************************************/

void invgp (double a0, double b0, double rlat1, double rlon1, double rlat2, double rlon2, double *dist, double *az)
{
  double              drlat1, drlat2, drlon1, drlon2, dell, beta1, sbeta1, cbeta1, beta2, sbeta2, cbeta2, adell, sidel, codel, a, b,
                      siphi, cophi, q1, q2, c, em, phi, phisq, csphi, ctphi, psyco, term1, term2, term3, term4, term5, term6, xlam1, tan;
  int32_t             n;
  static int32_t      first = 1;
  static double       pi = 3.141592653589793, twopi = 6.283185307179586, rad_to_deg = 57.2957795147195,
                      tiny = .0000000000000000000000000000001, flat, flat2, f1, f2, f3, f4, f5, f6, f7, f8;

  if (first)
    {
      flat = 1.0l - (b0 / a0);
      flat2 = flat * flat;
      f1 = flat2 * 1.25;
      f2 = flat2 * 0.5;
      f3 = flat2 * 0.25;
      f4 = flat2 * 0.125;
      f5 = flat2 * 0.0625;
      f6 = flat + flat2;
      f7 = f6 + 1.0;
      f8 = f6 * 0.5;

      first = 0;
    }

  drlat1 = rlat1 / rad_to_deg;
  drlat2 = rlat2 / rad_to_deg;
  drlon1 = rlon1 / rad_to_deg;
  drlon2 = rlon2 / rad_to_deg;

  beta1 = atan ((1.0 - flat) * sin (drlat1) / cos (drlat1));
  sbeta1 = sin (beta1);
  cbeta1 = cos (beta1);
  beta2 = atan ((1.0 - flat) * sin (drlat2) / cos (drlat2));
  sbeta2 = sin (beta2);
  cbeta2 = cos (beta2);

  dell = drlon1 - drlon2;
  adell = fabs (dell);

  if (drlon1 * drlon2 < 0.0)
    {
      adell = fabs (drlon1) + fabs (drlon2);
      dell = adell;
      if (drlon1 < 0.0) dell = - adell;

      if (adell > pi)
        {
          adell = twopi - adell;
          dell = adell;
          if (drlon1 > 0.0) dell = - adell;
        }
    }

  adell = twopi - adell;
  sidel = sin (adell);
  codel = cos (adell);
  a = sbeta1 * sbeta2;
  b = cbeta1 * cbeta2;
  cophi = a + b * codel;
  q1 = sidel * cbeta2;
  q1 *= q1;
  q2 = sbeta2 * cbeta1 - sbeta1 * cbeta2 * codel;
  q2 *= q2;
  siphi = sqrt (q1 + q2);
  c = b * sidel / siphi;
  em = 1.0 - c * c;

  phi = atan (siphi / (sqrt (1.0 - siphi * siphi) + tiny));

  if (cophi < 0.0) phi = pi - phi;
  phisq = phi * phi;
  csphi = 1.0 / siphi;
  ctphi = cophi / siphi;
  psyco = siphi / cophi;


  /*  Compute distance.                                               */

  term1 = f7 * phi;
  term2 = a * (f6 * siphi - f2 * phisq * csphi);
  term3 = em * (f2 * phisq * ctphi - f8 * (phi + psyco));
  term4 = a * a * f2 * psyco;
  term5 = em * em * (f5 * (phi + psyco) - f2 * phisq * ctphi - f4 * psyco * cophi * cophi);
  term6 = a * em * f2 * (phisq * csphi + psyco * cophi);
  *dist = b0 * (term1 + term2 + term3 - term4 + term5 + term6);


  /*  Compute azimuth.                                                */

  term1 = f6 * phi;
  term2 = a * (f2 * siphi + flat2 * phisq * csphi);
  term3 = em * (f3 * psyco + flat2 * phisq * ctphi - f1 * phi);
  xlam1 = c * (term1 - term2 + term3) + adell;
  q1 = sbeta2 * cbeta1 - cos (xlam1) * sbeta1 * cbeta2;
  q2 = sin (xlam1) * cbeta2;
  if (q1 == 0.0) q1 = tiny;
  tan = q2 / q1;
  *az = atan (tan);


  /*  Put azimuth in proper quadrant.                                 */

  n = 3;
  if (dell * tan < 0.0) n = 4;
  if (dell < 0.0) n = n - 2;
  if (q1 > 0.0 && dell == 0.0) n = 1;
  q2 = n;
  q1 = q2 * pi - pi - *az;
  if (n >= 3) q1 = (q2 - 2.0) * pi + *az;
  *az = q1 * rad_to_deg;
}




/******************************************************************************/
/*!

  - notes:
      - 1. This function was written to replace newgp of "when dinosaurs
           roamed".  It is simply a toplevel runner of the direct routine
           that was converted to C from NOAA's FORTRAN code.
      - 2. latobs, lonobs are the coordinates of the observed position (degrees)
      - 3. az is the azimuth from North and must be in [0, 360)
      - 4. dist is in meters
      - 5. *lat, *lon will contain the coordinate values of the directed
           position
      - 6. function direct works in arc seconds, azimuth from south, and meters
      - 7. function azNtoS takes the azimuth from North and gives you azimuth
           from South
      - 8. the ellipsoid constants for newgp are hard-coded to be those of
           wgs 84
*/

void newgp (double latobs, double lonobs, double az, double dist, double *lat, double *lon)
{
  double phipri, alampr;
  void direct(double, double, double, double, double *, double *);
  double azNtoS(double);


  /* the conditional below was put in because direct bombed out when
     on the equator going east or west
  */

  if ( latobs == 0.0 && (az == 90.0 || az == 270.0) ) latobs += 1.0e-37;

  direct(3600.0*latobs, 3600.0*lonobs, 3600.0*azNtoS(az), dist, &phipri, &alampr);

  *lat = phipri/3600.0;
  *lon = alampr/3600.0;
} /* newgp */



/******************************************************************************/

/* azFromNorth must be in [0, 360)
*/

double azNtoS(double azFromNorth)
{
  double azFromSouth;

  azFromSouth = azFromNorth + 180.0;

  if (azFromSouth > 360.0) azFromSouth -= 360.0;
        
  return azFromSouth;

} /* azNtoS */
