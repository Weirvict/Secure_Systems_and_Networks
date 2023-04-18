
/* a rtpkt is the packet sent from one routing update process to
   another via the call tolayer3() */
struct rtpkt {
	int sourceid;       /* id of sending router sending this pkt */
	int destid;         /* id of router to which pkt being sent 
			       (must be an immediate neighbor) */
	int mincost[4];    /* min cost to node 0 ... 3 */
};

struct distance_table {
  int costs[4][4];
};

extern int TRACE;

void rtinit0(void);
void rtupdate0(struct rtpkt *rcvdpkt);
void printdt0(struct distance_table *dtptr);
void linkhandler0(int linkid, int newcost);

void rtinit1(void);
void rtupdate1(struct rtpkt *rcvdpkt);
void printdt1(struct distance_table *dtptr);
void linkhandler1(int linkid, int newcost);

void rtinit2(void);
void rtupdate2(struct rtpkt *rcvdpkt);
void printdt2(struct distance_table *dtptr);
void linkhandler2(int linkid, int newcost);

void rtinit3(void);
void rtupdate3(struct rtpkt *rcvdpkt);
void printdt3(struct distance_table *dtptr);
void linkhandler3(int linkid, int newcost);

void creatertpkt(struct rtpkt *initrtpkt, int srcid, int destid, int mincosts[]);
void tolayer2(struct rtpkt packet);
