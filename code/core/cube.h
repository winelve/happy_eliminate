#ifndef CUBE_H
#define CUBE_H

#include <QObject>

class Cube
{

private:
    int type_;


public:
    Cube();
    Cube(int type);

    void SetType(int type) { type_ = type; }
    int GetType() { return type_; }


    bool operator==(const Cube& other) const {
        return type_ == other.type_;
    }
};

#endif // CUBE_H
