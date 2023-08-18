//
//  intObject.h
//  233218_WET#1
//
//  Created by Baraa Egbaria on 24/04/2022.
//

#ifndef WET1_INTOBJECT_H
#define WET1_INTOBJECT_H


class intObject {
public:
    int num;

    intObject() : num(0) {};

    intObject(int num) : num(num) {}
    ~intObject() = default;

    bool operator>(intObject object) {
        if (this->num > object.num)
            return true;

        return false;
    }

    bool operator<(intObject object) {
        return !((*this) > object);
    }

    bool operator==(const intObject object) {

        return (this->num == object.num);
    }

    bool operator!=(intObject object) {
        return !((*this) == object);
    }

    intObject &operator=(intObject &other)
    {

        this->num = other.num;
        return (*this);
    }

};


#endif //WET1_INTOBJECT_H
