#include <stdio.h>
#include "prog3.h"

/* students to write the following two routines, and maybe some others */

static struct distance_table dt;

// Called during initialization of the simulator. This function
// should send its current vector to all its neighbors.
// startv is the initial vector for this node.
// startv[1] is 0 (zero cost to talk to itself)
// if startv[n] is 999, then node n is not connected to this node.
void
rtinit1(int startv[NUMENTITIES])
{
}

// Called when this entity has received a packet from one of its
// neighbors. It should implement the algorithm and if this node's
// vector has changed, it should send its vector to all its neighbors.
void
rtupdate1(const struct rtpkt *rcvdpkt)
{
}

// called at the end of simulation to print the distance table
// computed by this node.
void
printdt1(void)
{
	printf("             via   \n");
	printf("   D1 |    0     2 \n");
	printf("  ----|-----------\n");
	printf("     0|  %3d   %3d\n", dt.costs[0][0], dt.costs[2][2]);
	printf("dest 2|  %3d   %3d\n", dt.costs[0][2], dt.costs[2][2]);
	printf("     3|  %3d   %3d\n", dt.costs[0][3], dt.costs[2][3]);
}

// called when cost from 1 to linkid changes from current value to newcost
// You can leave this routine empty if you're an undergrad. If you want
// to use this routine, you'll need to change the value of the LINKCHANGE
// constant definition in prog3.c from 0 to 1
void
linkhandler1(int linkid, int newcost)   
{
}
