Dominating sets algorithm simulation
====================================

Lekkas Kostas

Project for Mobile and Ubiquitous computing course, University of Thessaly,  fall 2010.


Description
-----------

This project is an OpenGL simulation of the algorithm described in the following papers:

* "Ivan Stojmenovic, Mahtab Seddigh, Jovisa D. Zunic: 
 Dominating Sets and Neighbor Elimination-Based Broadcasting Algorithms 
 in Wireless Networks. IEEE Transactions on Parallel and Distributed Systems, 
 vol. 13, no. 1, pp. 14-25, 2002."

* "Ivan Stojmenovic: Comments and Corrections to "Dominating Sets 
 and Neighbor Elimination-Based Broadcasting 
 Algorithms in Wireless Networks". IEEE Transactions on Parallel 
 and Distributed Systems, vol. 15, no. 11, pp. 1054-1055, 2004."

Dependencies
------------

The application has been tested on Linux only and depends on the freeglut library.

Compiling
---------

Simply invoke:

	make

Usage
-----

Simply run

	./dom_sets [input_file]

The `input_file` is a text file with coordinates of the nodes. If the nodes are placed within broadcast range, a communication link is automatically drawed. This range is hard-coded as `BCAST_RADIUS` in `main.cpp`.The coordinates use the lower left corner of the simulation window as the origin point.