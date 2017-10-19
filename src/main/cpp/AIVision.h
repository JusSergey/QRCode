//
// Created by heart on 19.10.17.
//

#ifndef QRCODEEX_AIVISION_H
#define QRCODEEX_AIVISION_H


#include <cstring>
#include <vector>
#include <limits>
using std::size_t;
//#include <iostream>
//using std::cout;

using uch = unsigned char;
using uint= unsigned int;

using UID_t = unsigned long long;

static constexpr auto QR_N = 8;



struct RGBA {

    union {
        uch a, b, g, r;
        uint color;
    };

    RGBA(uint color = 0) : color(color) {}
    //RGBA(uch r, uch g, uch b, uch a) : r(r), g(g), b(b), a(a) {}

    inline unsigned char medium() {
        unsigned int result = r;
        result += g;
        result += b;
        return static_cast<unsigned char>(result / 3);
    }

    operator unsigned int & () {
        return color;
    }
};

struct XY {
    int x, y;
    XY(int xx = 0, int yy = 0) : x(xx), y(yy) {}
    void add(const XY &xy);
    void print() const;
    XY &operator = (const XY &xy);
};

struct _4XY {
    XY left, right, up, down;
    enum class ID : char { LEFT, RIGHT, UP, DOWN };
    inline XY &operator[](ID id);
    void print();
};

class AIVision
{
    static constexpr auto W = 160;
    static constexpr auto H = 160;
public:
    struct MatrixRGBA {
        int WIDTH, HEIGHT;

        MatrixRGBA(const RGBA *array = nullptr, size_t size = 0, int WID = W, int HEI = H);

        std::vector<RGBA> rgba_mat;

        inline RGBA &get(int x, int y) { return rgba_mat[y*WIDTH + x]; }

        inline RGBA &get(XY xy) { return rgba_mat[xy.y*WIDTH + xy.x]; }

        void setData(const RGBA *array, size_t size, int WID = 0, int HEI = 0);

        XY getPointOnMat(float xx, float yy) const;

        inline bool isValidPoint(const XY &xy) const;

    } matrx;
    _4XY lrud;
    UID_t uid;

public:
    AIVision(const char *path, int width, int height);
    AIVision(const RGBA *rgba, size_t len, int width, int height);
    inline UID_t getUID() const { return uid; }

private:
    void initMatrx(const char *pathToFile, int width, int height);
    void initMatrx(const RGBA *rgba, size_t len, int width, int height);
    void initLRUD();
    void prnt();
    void printAsInt();
    void calculation();
};

template <typename ...ARGS>
AIVision *create(ARGS ...args) {
    const char *c = nullptr;
    return new AIVision(c, 1, 1);
}


#endif //QRCODEEX_AIVISION_H
