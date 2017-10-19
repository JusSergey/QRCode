//
// Created by heart on 19.10.17.
//

#include "AIVision.h"

#include <bitset>
#include <fstream>
#include <iostream>
#include <unistd.h>
using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::pair;
using std::make_pair;
using std::bitset;

static constexpr auto MIN_WHITE = 96;

AIVision::AIVision(const char *path, int width, int height)
{
    initMatrx(path, width, height);
    //initLRUD();
    //calculation();
//    printAsInt();
//    initLRUD();
//    system("clear");
//    prnt();
//    calculation();
//    lrud.print();
}

AIVision::AIVision(const RGBA *rgba, size_t len, int width, int height)
{
    initMatrx(rgba, len, width, height);
    initLRUD();
    calculation();
}

int64_t getSize(ifstream &fl) {
    int64_t currpos = fl.tellg();
    fl.seekg(0, std::ios::end);
    int64_t pos = fl.tellg();
    fl.seekg(currpos);
    return pos;
}

void AIVision::initMatrx(const RGBA *rgba, size_t len, int width, int height)
{
    matrx.setData(rgba, len, width, height);
}

void AIVision::initMatrx(const char *pathToFile, int width, int height)
{
    // teste

    ifstream file(pathToFile);
    size_t size = getSize(file);
    ofstream o("/mnt/sdcard/outlog.log");
    o << size;
    o.flush();
    o.close();

    //char *data = new char[size];
    /*file.read(data, size);
    //matrx.setData(reinterpret_cast<const RGBA *> (data), size/sizeof(RGBA), width, height);
    delete []data;
    */
    file.close();
}


////// FINDER BORDER


void AIVision::initLRUD()
{

    struct Info {
        XY xy;
        _4XY::ID id;
        XY dir;
        Info(const XY &xy, const XY &dir, _4XY::ID id)
                : xy(xy), dir(dir), id(id) {}
    };

    auto l = { Info(matrx.getPointOnMat(0, 0.5), XY{1, 0}, _4XY::ID::LEFT),
               Info(matrx.getPointOnMat(1, 0.5), XY{-1,0}, _4XY::ID::RIGHT),
               Info(matrx.getPointOnMat(0.5, 1), XY{0,-1}, _4XY::ID::UP),
               Info(matrx.getPointOnMat(0.5, 0), XY{0, 1}, _4XY::ID::DOWN) };

    auto funcCondition = [](RGBA rgba) -> bool { return rgba.medium() > MIN_WHITE; };
//    cout << '\n';

    for (const Info &d : l) {
        d.xy.print();
        //cout << '\n';

        bool start_condition = funcCondition(matrx.get(d.xy));
        //int count = 0;
        for (XY xy = XY(d.xy.x, d.xy.y); matrx.isValidPoint(xy); xy.add(d.dir)) {
            if (start_condition != funcCondition(matrx.get(xy))){
                //lrud.left = xy;
                lrud[d.id] = xy;
                break;
            }
            // modify data for see
            //matrx.get(xy) = 0;
            //count++;
            /*prnt();
            cout << '\n';
            xy.print();
            cout.flush();
             */
        // std::cin.get();
            //system("clear");

        }

    }

}

////////

void AIVision::prnt()
{
    for (int i = 0; i < matrx.WIDTH; ++i) {
        for (int j = 0; j < matrx.HEIGHT; ++j) {
            cout << (matrx.get(j, i).medium() > MIN_WHITE ? 'W' : '.') << ' ';
        }
        cout.put('\n');
    }
}

void AIVision::printAsInt()
{
    for (int i = 0; i < matrx.WIDTH; ++i) {
        for (int j = 0; j < matrx.HEIGHT; ++j) {
            cout << static_cast<int>(matrx.get(i, j).medium()) << ' ';
        }
        cout.put('\n');
    }
}

void AIVision::calculation()
{
    //sx - startX, ex - endX
    int sx = lrud[_4XY::ID::LEFT].x,
            sy = lrud[_4XY::ID::DOWN].y,
            ex = lrud[_4XY::ID::RIGHT].x,
            ey = lrud[_4XY::ID::UP].y;

    using calc_t = long double;
    calc_t stepx = static_cast<calc_t>(ex - sx) / (QR_N);
    calc_t stepy = static_cast<calc_t>(ey - sy) / (QR_N);
    /*
    cout << (ex-sx) << ' ' << (ey-sy) << '\n';
    cout << stepx << ' ' << stepy << '\n';

    cout << matrx.WIDTH << ' ' << matrx.HEIGHT << '\n';
    cout << sx << ' ' << sy << ' ' << ex << ' ' << ey << '\n';

    cout << "\n~~~~~ BITS ~~~~~\n";
    */
    uid = 0;
    static_assert(sizeof(uid)*8 >= QR_N*QR_N, "sizeof(uid) must be >= (QR_N*QRN).");

    // int ix = 0, iy = 0;

    for (calc_t y = sy + stepy / 2; y < ey; y += stepy) {
        // iy++;
        // ix=0;
        for (calc_t x = sx + stepx / 2; x < ex; x += stepx) {
            uid <<= 1;
            uid |= (matrx.get(static_cast<int>(x), static_cast<int>(y)).medium() < MIN_WHITE ? 1 : 0);
            // cout << (matrx.get(static_cast<int>(x), static_cast<int>(y)).medium() > MIN_WHITE ? 'W' : '.');
            // ++ix;
        }
    }
    /*
    cout << '\n';
    cout << "iters[xy]: " << ix << ' ' << iy << '\n';
    cout << "uid: " << uid << '\n'
         << "bin: " << (bitset<QR_N * QR_N>(uid)) << '\n';

    cout << "\n~~~~~ **** ~~~~~\n";
    */

}

void XY::add(const XY &xy) {
//    cout << "\nteste1: " << x << ' ' << xy.x << ' ' << y << ' ' << xy.y << '\n';
    x += xy.x; y += xy.y;
//    cout << "\nteste2: " << x << ' ' << xy.x << ' ' << y << ' ' << xy.y << '\n';
}

void XY::print() const {
    cout << '[' << x << ':' << y << "]";
    cout.flush();
}

XY &XY::operator =(const XY &xy)
{
    x = xy.x;
    y = xy.y;
    return *this;
}

AIVision::MatrixRGBA::MatrixRGBA(const RGBA *array, size_t size, int WID, int HEI)
{
    setData(array, size, WID, HEI);
}

void AIVision::MatrixRGBA::setData(const RGBA *array, size_t size, int WID, int HEI) {
    WIDTH = WID;
    HEIGHT = HEI;

    ofstream o("/mnt/sdcard/outlog.log");
    o << "sz: " << size;
    o.flush();
    o.close();

    rgba_mat.resize(size * sizeof(array[0]));

    memcpy(reinterpret_cast<      void *>(rgba_mat.data()),
           reinterpret_cast<const void *>(array),
           size * sizeof(array[0]));

}

XY AIVision::MatrixRGBA::getPointOnMat(float xx, float yy) const {
    return XY(xx*(WIDTH-1), yy*(HEIGHT-1));
}

bool AIVision::MatrixRGBA::isValidPoint(const XY &xy) const {
    return xy.x > -1 && xy.x < WIDTH && xy.y > -1 && xy.y < HEIGHT;
}

XY &_4XY::operator[](_4XY::ID id) {
    //ofstream o("/mnt/sdcard/outlog.log", std::ios::app);
//    o << "out: " << size;
    //o.flush();
    //o.close();
    //return up;

    switch (id) {
        case ID::LEFT:  return left; break;
        case ID::RIGHT: return right; break;
        case ID::UP:    return up; break;
        case ID::DOWN:  return down; break;
        default: {
            cout.operator <<((int)id).flush(); throw("out of range: undefined value.[_4XY]");
        };
    }

}

void _4XY::print() {
    cout << "l: ";  left.print();
    cout << " r: "; right.print();
    cout << " u: "; up.print();
    cout << " d: "; down.print();
    cout << '\n';
}
