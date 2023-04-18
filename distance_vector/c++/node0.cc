#include <stdio.h>
#include "prog3.h"

/* students to write the following two routines, and maybe some others */

static struct distance_table dt;

// Called during initialization of the simulator. This function
// should send its current vector to all its neighbors.
// startv is the initial vector for this node.
// startv[0] is 0 (zero cost to talk to itself)
// if startv[n] is 999, then node n is not connected to this node.
void
rtinit0(int startv[NUMENTITIES])
{
}

// Called when this entity has received a packet from one of its
// neighbors. It should implement the algorithm and if this node's
// vector has changed, it should send its vector to all its neighbors.
void
rtupdate0(const struct rtpkt *rcvdpkt)
{
}

// called at the end of simulation to print the distance table
// computed by this node.
void
printdt0(void)
{
	printf("                via     \n");
	printf("   D0 |    1     2    3 \n");
	printf("  ----|-----------------\n");
	printf("     1|  %3d   %3d   %3d\n", dt.costs[1][1],
	       dt.costs[1][2],dt.costs[1][3]);
	printf("dest 2|  %3d   %3d   %3d\n",dt.costs[2][1],
	       dt.costs[2][2],dt.costs[2][3]);
	printf("     3|  %3d   %3d   %3d\n",dt.costs[3][1],
	       dt.costs[3][2],dt.costs[3][3]);
}

// called when cost from 0 to linkid changes from current value to newcost
// You can leave this routine empty if you're an undergrad.
void
linkhandler0(int linkid, int newcost)   
{
}
