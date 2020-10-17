#ifndef Robot_Leg
#define Robot_Leg

class RobotLeg
{
private:
    const double a, b, c, d, f, alpha;
    double x, y, theta, phi ,r;
public:
    Robot_Leg(double _a, double _b, double _c, double _e, double _f, double _alpha,);

    double Inverse_Cosine(double x, double y, double side, double top);

    void Update();
}


#endif
