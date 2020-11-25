#ifndef Commander_def
#define Commander_der
#include <math.h>

class Commander
{
private:
    const double a, b, c, d, f, alpha;
    double x, y, theta, phi ,r, x_e, y_e, r_e;
public:
    Commander(double _a, double _b, double _c, double _d, double _f, double _alpha):a(_a), b(_b), c(_c), d(_d), f(_f), alpha(_alpha) {};

    double Inverse_Cosine(double r, double side, double top);

    void UpdateAng();

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
