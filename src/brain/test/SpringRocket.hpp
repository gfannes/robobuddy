#ifndef brain_test_SpringRocket_hpp
#define brain_test_SpringRocket_hpp

namespace test
{
    //Simulates a rocket connected a spring in gravity
    class SpringRocket
    {
        public:
            SpringRocket(double m, double k, double g):
                m_(m), k_(k), g_(g),
                x_(0), v_(0), t_(0){}

            double x() const {return x_;}
            void setThrust(double t){t_ = t;}

            void process(double dT)
            {
                v_ += dT*force_()/m_;
                x_ += dT*v_;
            }

        private:
            double force_() const
            {
                return -m_*g_ - k_*x_ + t_;
            }

            double x_;
            double v_;
            double t_;
            const double m_;
            const double k_;
            const double g_;
    };
}

#endif
