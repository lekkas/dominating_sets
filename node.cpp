/* 
 * File:   node.cpp
 * Author: lekkas
 * 
 * Created on December 28, 2010, 5:36 AM
 */

#include "node.h"

node::node():x(0), y(0), r(0.0), g(0.0), b(0.0) {
    gotMessage = false;
    broadcasted = false;
}

node::node(int x, int y): x(x), y(y), r(0.0), g(0.0), b(0.0) {
    gotMessage = false;
    broadcasted = false;
}

node::node(const node& orig) {
    vector<node*> tmp;
    x = orig.x;
    y = orig.y;
    color = orig.color;
    if( !orig.neighbors.empty() ) {
        tmp = orig.neighbors;
    }
}

node::~node() {
}

void node::broadcast_receive(node *from, vector<node*> &MAC_sched) {
    // among others, if you are a gateway add yourself to the
    // IDEAL_MAC_BCAST Queue
    node *a;
    if(gateway) {
        if(!gotMessage) {
            setRGB(0.0, 0.6, 0.0);
            gotMessage = true;
            MAC_sched.push_back(this);
            for(unsigned int i = 0; i < neighbors.size(); i++ )
                forward.push_back(neighbors.at(i));
        }
        for(unsigned int i = 0; i < neighbors.size(); i++ ) {
            a = neighbors.at(i);
            if( dist(from, a) < R ) {
                forward.remove(a);
            }
        }
    }
    setRGB(0.0, 1.0, 0.0);
}

float node::dist(const node *a, const node *b) const {
    return sqrt( powf((a->getX()-b->getX()),2) + powf((a->getY()-b->getY()),2) );
}

bool node::hasNeigh(const node *n) const {
    for(unsigned int i = 0; i < neighbors.size(); i++) {
        if(n == neighbors.at(i) )
            return true;
    }
    return false;
}
void node::internal_status() {
    node *a, *b, *c;
    gateway = false;
    intermediate = false;
    intergateway = false;
    for(unsigned int i = 0; i < neighbors.size(); i++) {
        a = neighbors.at(i);
        for(unsigned int j = 0; j < neighbors.size(); j++) {
            b = neighbors.at(j);
            if( dist(a, b) > R )
                intermediate = true;
            if(intermediate) {
                intergateway = true;
                for(unsigned int i = 0; i < neighbors.size(); i++) {
                    covered = true;
                    a = neighbors.at(i);
                    for(unsigned int j = 0; j < neighbors.size(); j++) {
                        b = neighbors.at(j);
                        if( a == b) continue;
                        if(dist(a, b) > R || k > a->k)
                            covered = false;
                    }
                    if(covered)
                        intergateway = false;
                }
            }
            if(intergateway) {
                gateway = true;
                for(unsigned int i = 0; i < neighbors.size(); i++) {
                    a = neighbors.at(i);
                    for(unsigned int j = 0; j < neighbors.size(); j++) {
                        b = neighbors.at(j);
                        if(a == b) continue;
                        if(a->hasNeigh(b)) { // Found common neighbor
                            covered = true;
                            for(unsigned int l = 0; l < neighbors.size(); l++){
                                c = neighbors.at(l);
                                if( (dist(c, a) >  R && dist(c, b) > R) || ( k > a->k || k > b->k ) )
                                    covered = false;
                                
                            }
                            if(covered)
                                gateway = false;
                        }
                    }
                }
            }

        }
    }

    if(gateway) 
        setRGB(0.6, 0.0, 0.0);
}
