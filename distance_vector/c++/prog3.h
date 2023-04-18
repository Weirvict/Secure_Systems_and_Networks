
#ifndef _PROG3_H_
#define _PROG3_H_

#define NUMENTITIES 4

/* a rtpkt is the packet sent from one routing update process to
   another via the call tolayer3() */
struct rtpkt {
	int sourceid;       /* id of sending router sending this pkt */
	int destid;         /* id of router to which pkt being sent 
			       (must be an immediate neighbor) */
	int mincost[NUMENTITIES];    /* min cost to node 0 ... NUMENTITIES-1 */
};

struct distance_table {
  int costs[NUMENTITIES][NUMENTITIES];
};

extern int TRACE;
extern double clocktime;

void rtinit0(int startvector[NUMENTITIES]);
void rtupdate0(const struct rtpkt *rcvdpkt);
void printdt0(void);
void linkhandler0(int linkid, int newcost);

void rtinit1(int startvector[NUMENTITIES]);
void rtupdate1(const struct rtpkt *rcvdpkt);
void printdt1(void);
void linkhandler1(int linkid, int newcost);

void rtinit2(int startvector[NUMENTITIES]);
void rtupdate2(const struct rtpkt *rcvdpkt);
void printdt2(void);
void linkhandler2(int linkid, int newcost);

void rtinit3(int startvector[NUMENTITIES]);
void rtupdate3(const struct rtpkt *rcvdpkt);
void printdt3(void);
void linkhandler3(int linkid, int newcost);

void creatertpkt(struct rtpkt *initrtpkt, int srcid, int destid, int mincosts[]);
void tolayer2(struct rtpkt packet);
void printdt(int entity, const distance_table&);

#endif
