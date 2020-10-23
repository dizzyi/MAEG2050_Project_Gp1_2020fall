#ifndef Robot_Leg
#define Robot_Leg
#include <math.h>

class RobotLeg
{
private:
    const double a, b, c, d, f, alpha;
    double x, y, theta, phi ,r;
public:
    RobotLeg(double _a, double _b, double _c, double _d, double _f, double _alpha):a(_a), b(_b), c(_c), d(_d), f(_f), alpha(_alpha) {};

    double Inverse_Cosine(double r, double side, double top);

    void Update(double _x, double _y);
};


#endif
