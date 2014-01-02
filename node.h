/* 
 * File:   node.h
 * Author: lekkas
 *
 * Created on December 28, 2010, 5:36 AM
 */

#ifndef NODE_H
#define	NODE_H

#include <vector>
#include <list>
#include <cmath>
#include "key.h"

using namespace std;

class node {
public:
    vector<node*> neighbors;
    list<node*> forward;
    key k;
    bool gotMessage;
    bool broadcasted;
    

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    int getX() const { return x; }
    int getY() const { return y; }
    void setRGB(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    float getR() const {return r;}
    float getG() const {return g;}
    float getB() const {return b;}
    float dist(const node *a, const node *b) const;

    bool hasNeigh(const node *n) const;
    bool isGateway(void) const {return gateway;}
    void setBcast_seq(int b) { this->Bcast_seq = b; }
    int getBcast_seq() const { return this->Bcast_seq; }

    /*
     * internal_status() and broadcast_receive are described in
     * the "Dominating sets and neighbor elimination-based broadcasting
     * algorithms in wireless networks" paper.
     */
    void internal_status(void);
    void broadcast_receive(node *from, vector<node*> &MAC_sched);
    
    node();
    node(int x, int y);
    node(const node& orig);
    virtual ~node();

private:
    bool gateway, intermediate, intergateway, covered;
    int x, y;
    float r, g, b;
    int color;
    const static int R = 100;   //Range of radio 
    int Bcast_seq;
};

#endif	/* NODE_H */

