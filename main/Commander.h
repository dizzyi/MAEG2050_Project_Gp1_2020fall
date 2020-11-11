#ifndef Commander_def
#define Commander_der
#include <math.h>

class Commander
{
private:
    const double a, b, c, d, f, alpha;
    double x, y, theta, phi ,r;
public:
    Commander(double _a, double _b, double _c, double _d, double _f, double _alpha):a(_a), b(_b), c(_c), d(_d), f(_f), alpha(_alpha) {};

    double Inverse_Cosine(double r, double side, double top);

    void UpdateAng();

    double get_theta(long T){
      if(T < 1200) return -15;
      if(T < 2400) return 0;
      return -70;
      return 180 - theta / M_PI * 180.;
    } 

    double get_phi(long T){
      if(T < 1200) return 30;
      if(T < 2400) return 70;
      return 40;
      return phi / M_PI * 180;
    }

    long get_theta_num(){
      return (long)round(( M_PI - theta) * 155610. / 2. / M_PI);
    } 

    long get_phi_num(){
      return (long)round(( M_PI - phi  ) * 155610. / 2. / M_PI);
    } 

    void UpdateXY(long T){
      x = 0;
      y = -500;
      if( T > 120 && T < 240) x = 100;
      if( T > 240)            y = -450; 
      /*
      if(T < 120 ){
        y = -500.;
        x = T / 120. * 100;
      }
      else if(T < 240){
        y = -500 + (T - 120)/ 120. * 100.;
        x = (T - 120) / 120. * -100 + 100;
      }
      else{
        x = 0;
        y = -400 - (T - 240)/ 120. * 100.;
      }*/
    }
};


#endif
