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

    void UpdateAng(double _x, double _y);

    short get_setpoint(){
      return 0;
    }
};


#endif
