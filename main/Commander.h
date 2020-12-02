#ifndef Commander_def
#define Commander_der
#include <math.h>

class Commander
{
private:
    const double a, b, c, d, f, alpha;
    double x, y, theta, phi ,r, x_e, y_e, r_e;

    double Inverse_Cosine(double r, double side, double top)
    {
      return acos( 
                   ( 
                     pow(side,2) + pow(r,2) - pow(top,2) 
                   ) 
                   /
                   ( 
                     2 * side * r 
                   )                 
                 );
    }
    
public:
    Commander(double _a, double _b, double _c, double _d, double _f, double _alpha):a(_a), b(_b), c(_c), d(_d), f(_f), alpha(_alpha) {};

    void UpdateAng()
    {
      
      r = sqrt( pow(x,2) + pow(y,2));
      Serial.print("r: ");Serial.print(r);Serial.print(" ");
      
      double theta_2 = Inverse_Cosine(r,b,f);
      Serial.print("theta_2: ");Serial.print(theta_2/M_PI*180);Serial.print(" ");
      
      theta = -   atan( y / x )               * ( x >  0 )
              + ( atan(  x / y ) + M_PI / 2 ) * ( x <  0 )
              +   M_PI / 2                    * ( x == 0 );
                
      theta += theta_2;

      
      Serial.print("theta: ");Serial.print(theta/M_PI*180);Serial.print(" ");
    
      double beta = asin( sin(theta_2) * r / f );
      if(r > f && beta < theta_2 ) beta = M_PI - beta;

      Serial.print("beta: ");Serial.print(beta/M_PI*180);Serial.print(" ");
    
      double delta = - theta - beta + alpha + M_PI;
      Serial.print("delta: ");Serial.print(delta/M_PI*180);Serial.print(" ");
      
      x_e =   b * cos( -theta ) +  c * cos( delta );
      y_e =   b * sin( -theta ) +  c * sin( delta );
      
      r_e =  sqrt( pow(x_e - a, 2) + pow(y_e, 2) );
      
      phi =     Inverse_Cosine( r_e, b, d)
            +   atan( y_e / ( x_e - a ) )             * ( x_e <  a )
            + ( atan( (a - x_e ) / y_e ) + M_PI / 2 ) * ( x_e >  a )
            +   M_PI / 2                              * ( x_e == a );  
    }

    double get_theta(){/*
      if(T < 120) return -38;
      if(T < 240) return 10;
      return -74;*/
      return 90 - theta / M_PI * 180.;
    } 

    double get_phi(){/*
      if(T < 120) return 35;
      if(T < 240) return 75;
      return 38; */
      return phi / M_PI * 180 - 90;
    }

    long get_theta_num(){
      return (long)round(( M_PI - theta) * 155610. / 2. / M_PI);
    } 

    long get_phi_num(){
      return (long)round(( M_PI - phi  ) * 155610. / 2. / M_PI);
    } 

    void UpdateXY(long T){
      if(T < 120){
        x = T/120.0 * 300 + -50;
        y = -550;
      }
      else if(T < 240){
        x = 250 - (T/120.0 - 1) * 250;
        y = -550 + pow((T/120.0 - 1), 1) * 100;
      }
      else{
        x = -(T/120.0 - 2) * 50;
        y = -450 + (T/120.0 - 2)*(-100);
      }
    }

    double getX(){
      return x;
    }
    double getY(){
      return y;
    }
    double getXe(){
      return x_e;
    }
    double getYe(){
      return y_e;
    }
};


#endif
