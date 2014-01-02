/* 
 * File:   key.cpp
 * Author: lekkas
 * 
 * Created on December 28, 2010, 3:11 PM
 */

#include "key.h"

key::key():deg(0), x(0), y(0) {
}

key::key(const key& orig) {
    deg = orig.deg;
    x = orig.x;
    y = orig.y;
}

key::~key() {
}

bool key::operator<(const key& op) const {
    if(deg < op.deg)
        return true;
    else if(deg > op.deg)
        return false;
    else {
        if(x < op.x)
            return true;
        else if (x > op.x)
            return false;
        else {
            if(y < op.y)
                return true;
            else
                return false;
        }
    }
}

bool key::operator==(const key& op) const {
    return false;
}

bool key::operator>(const key& op) const {
    if(deg > op.deg)
        return true;
    else if(deg < op.deg)
        return false;
    else {
        if(x > op.x)
            return true;
        else if (x < op.x)
            return false;
        else {
            if(y > op.y)
                return true;
            else
                return false;
        }
    }
}
