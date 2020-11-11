#include "Commander.h"
#include <math.h>

double Commander::Inverse_Cosine(double r, double side, double top)
{
  /*
  double r_sq = pow(r,2);
  double side_sq = pow(side,2);
  double top_sq = pow(top,2);

  double costheup = side_sq + r_sq - top_sq;

  double costhedown = 2 * side * r;

  return acos( costheup / costhedown );
*/
  return acos( ( pow(side,2) + pow(r,2) - pow(top,2) ) /
               ( 2 * side * r )                         );
}


void Commander::UpdateAng()
{
  
  r = sqrt( pow(x,2) + pow(y,2));
  double theta_2 = Inverse_Cosine(r,b,f);
  theta =    atan(- y / x )             * ( x >  0 )
          + (atan(  x / y )+ M_PI / 2 ) * ( x <  0 )
          +  M_PI / 2                   * ( x == 0 );
            
  theta += theta_2;

  double beta = asin( sin(theta_2) / f * r );

  double delta = theta + beta - alpha - M_PI;

  double x_e =   b * cos(theta) +   c * cos(delta);
  double y_e =   b * sin(-theta) +  c * sin(-delta);
  
  double r_e =  sqrt( pow(x_e - a, 2) + pow(y_e, 2) );

  phi =     Inverse_Cosine( r_e, b, d)
        +   atan( y_e / ( x_e - a ) )             * ( x_e <  a )
        + ( atan( (a - x_e ) / y_e ) + M_PI / 2 ) * ( x_e >  a )
        +   M_PI / 2                              * ( x_e == a );  
}
