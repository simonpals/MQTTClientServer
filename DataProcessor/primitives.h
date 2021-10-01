#ifndef PRIMITIVES_H
#define PRIMITIVES_H


namespace misc {

inline constexpr float cEps = 1e-2;
inline constexpr std::string_view cServerURI{"tcp://localhost:1883"}; //"broker.hivemq.com"
inline constexpr const char *cDelim = ";";


inline constexpr const char* configTopic{"simulation/config/"};
inline constexpr const char* dataStrTopic{"simulation/data/"};


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

inline uint32_t generateUniqueID()
{
    static uint32_t id = 0;
    return ++id;
}

inline float floatRand()
{ //thread safe
    static thread_local std::mt19937 generator{static_cast<unsigned>(
        std::chrono::system_clock::now().time_since_epoch().count())};

    std::uniform_real_distribution<float> distribution(-1,1);
    return distribution(generator);
}

inline float signumRand() {
    return sgn(floatRand());
}

union uF2ChArr
{
    float f;
    unsigned char s[sizeof(float)];
};

struct Point3D
{
    float x;
    float y;
    float z;

    explicit operator float() const { return sqrtf(x*x + y*y + z*z); }
    operator std::string() const {
        return std::to_string(x)+cDelim
                +std::to_string(y)+cDelim
                +std::to_string(z);
    }

    static Point3D unitVector() { return Point3D{1,1,1}; }
    static Point3D randUnitVector() { return Point3D{signumRand(),signumRand(),signumRand()}; }
};

inline const Point3D operator+(const Point3D &la, const Point3D &ra)
{
    return Point3D{la.x+ra.x,la.y+ra.y,la.z+ra.z};
}

inline const Point3D operator-(const Point3D &la, const Point3D &ra)
{
    return Point3D{la.x-ra.x,la.y-ra.y,la.z-ra.z};
}

inline const Point3D operator+(const Point3D &la, float ra)
{
    return Point3D{la.x+ra,la.y+ra,la.z+ra};
}

inline const Point3D operator*(const Point3D &la, float ra)
{
    return Point3D{la.x*ra,la.y*ra,la.z*ra};
}

struct RestrictedRegion3D
{
    Point3D topLeftFront;
    Point3D bottomRightBack;

    operator std::string() const {
        return static_cast<std::string>(topLeftFront)
                + cDelim + static_cast<std::string>(bottomRightBack);
    }

    float diagonal() const { return static_cast<float>(topLeftFront - bottomRightBack); }
    bool isInsideRegion(const Point3D &p) const {
        return p.x >= topLeftFront.x && p.x <= bottomRightBack.x
                && p.y <= topLeftFront.y && p.y >= bottomRightBack.y
                && p.z <= topLeftFront.z && p.y >= bottomRightBack.z;
    }
};

using Point_t = Point3D;
using Region_t = RestrictedRegion3D;

};

#endif // PRIMITIVES_H
