/* 
 * File:   key.h
 * Author: lekkas
 *
 * Created on December 28, 2010, 3:11 PM
 */

#ifndef KEY_H
#define	KEY_H

using namespace std;
        
class key {
public:
    int deg, x, y;

    key();
    key(int degree, int x, int y) {this->deg = degree; this->x = x, this->y = y; }
    key(const key& orig);
    virtual ~key();

    bool operator >(const key& op) const;
    bool operator <(const key& op) const;
    bool operator ==(const key& op) const;

private:

};

#endif	/* KEY_H */

