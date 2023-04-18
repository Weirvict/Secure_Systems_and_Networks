#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <random>
#include <set>
#include "prog3.h"

bool linkchanges = false;

/* ******************************************************************
   Programming assignment 3: implementing distributed, asynchronous,
   distance vector routing.

   THIS IS THE MAIN ROUTINE.  IT SHOULD NOT BE TOUCHED AT ALL BY STUDENTS!
**********************************************************************/
/* for my debugging */
int TRACE = 1;

void
creatertpkt(struct rtpkt *initrtpkt, int srcid, int destid, int mincosts[])
{
	int i;
	initrtpkt->sourceid = srcid;
	initrtpkt->destid = destid;
	for (i=0; i<4; i++)
		initrtpkt->mincost[i] = mincosts[i];
}  


/*****************************************************************
 ***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 2 and below network environment:
  - emulates the tranmission and delivery (with no loss and no
    corruption) between two physically connected nodes
  - calls the initializations routines rtinit0, etc., once before
    beginning emulation

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/

template<class T>
class Prng {
public:
	Prng<T>(int seed) {
		gen = std::default_random_engine(seed);
		dist = std::uniform_real_distribution<T>(0.0, 1.0);
	};
	Prng<T>() = delete;

	T operator()(void) {
		return dist(gen);
	}

	T next() { return dist(gen); }

private:
	std::uniform_real_distribution<T> dist;
	std::default_random_engine gen;
};

static Prng<double>* prng;

class event {
public:
	double evtime;           /* event time */
	int evtype;             /* event type code */
	int eventity;           /* entity where event occurs */
	struct rtpkt *rtpktptr; /* ptr to packet (if any) assoc w/ this event */

	event() = delete;

	event(double _evtime, int _evtype, int _eventity):
		evtime(_evtime),
		evtype(_evtype),
		eventity(_eventity),
		rtpktptr(nullptr) {}

	event(float _evtime, int _evtype, int _eventity, struct rtpkt *_pkt):
		evtime(_evtime),
		evtype(_evtype),
		eventity(_eventity),
		rtpktptr(_pkt) {}

	bool operator<(const event& rhs) const {
		return evtime < rhs.evtime;
	}
};

std::set<event> evlist;

/* possible events: */
#define  FROM_LAYER2     2
#define  LINK_CHANGE     10

double clocktime = 0.000;

static void init(int seed);

static int connectcosts[4][4] = {
	{ 0, 1, 3, 7},
	{ 1, 0, 1, 999},
	{ 3, 1, 0, 2},
	{7, 999, 2, 0},
};

/* initialize by hand since not all compilers allow array initilization */

static bool
strstartswith(const char *needle, const char *haystack) {
	return strncmp(needle, haystack, strlen(needle));
}

static void
usage(const char *name) {
	fprintf(stderr, "%s [trace=%%d] [linkchange] [seed=%%d]\n", name);
	exit(1);
}

static bool
get_int(const char *buf, int *p) {
	char *ep;
	long lval;

	errno = 0;
	lval = strtol(buf, &ep, 0);
	if (buf[0] == '\0' || *ep != '\0')
		return true;
	if ((errno == ERANGE && (lval == LONG_MAX || lval == LONG_MIN)) ||
	    (lval > INT_MAX || lval < INT_MIN))
		return true;
	*p = (int)lval;
	return false;
}

int
main(int argc, char *argv[])
{
	int seedval = 0;

	for (int i = 1; i < argc; i++) {
		if (!strstartswith("seed=", argv[i])) {
			const char *num = strchr(argv[i], '=') + 1;
			if (get_int(num, &seedval)) {
				fprintf(stderr, "invalid number %s\n", num);
				return 1;
			}
			continue;
		}

		if (!strcmp("linkchange", argv[i])) {
			linkchanges = true;
			continue;
		}

		if (!strstartswith("trace=", argv[i])) {
			const char *num = strchr(argv[i], '=') + 1;
			if (get_int(num, &TRACE) || TRACE < 0) {
				fprintf(stderr, "invalid number %s\n", num);
				return 1;
			}
			continue;
		}

		usage(argv[0]);
		return (1);
	}

	init(seedval);

	for (;;) {
		if (evlist.empty())
			goto terminate;

		auto it = evlist.begin();
		event evt = *it;
		evlist.erase(it);

		if (TRACE>1) {
			printf("MAIN: rcv event, t=%.3f, at %d", evt.evtime, evt.eventity);
			if (evt.evtype == FROM_LAYER2 ) {
				printf(" src:%2d,",evt.rtpktptr->sourceid);
				printf(" dest:%2d,",evt.rtpktptr->destid);
				printf(" contents: %3d %3d %3d %3d\n", 
				       evt.rtpktptr->mincost[0], evt.rtpktptr->mincost[1],
				       evt.rtpktptr->mincost[2], evt.rtpktptr->mincost[3]);
			}
		}
		clocktime = evt.evtime;    /* update time to next event time */
		if (evt.evtype == FROM_LAYER2 ) {
			if (evt.eventity == 0) 
				rtupdate0(evt.rtpktptr);
			else if (evt.eventity == 1) 
				rtupdate1(evt.rtpktptr);
			else if (evt.eventity == 2) 
				rtupdate2(evt.rtpktptr);
			else if (evt.eventity == 3) 
				rtupdate3(evt.rtpktptr);
			else { printf("Panic: unknown event entity\n"); exit(0); }
		}
		else if (evt.evtype == LINK_CHANGE ) {
			if (clocktime<10001.0) {
				linkhandler0(1,20);
				linkhandler1(0,20);
			}
			else   {
				linkhandler0(1,1);
				linkhandler1(0,1);
			}
		}
		else
		{
			printf("Panic: unknown event type\n");
			exit(0);
		}

		if (evt.evtype == FROM_LAYER2)
			delete evt.rtpktptr;	/* free memory for packet, if any */
	}
   

terminate:
	printf("\nSimulator terminated at t=%f, no packets in medium\n", clocktime);

	printdt0();
	printdt1();
	printdt2();
	printdt3();
}

/* initialize the simulator */
static void
init(int seedval)
{
	int i;
	double sum, avg;

	prng = new Prng<double>(seedval);

	sum = 0.0;                /* test random number generator for students */
	for (i=0; i<1000; i++)
		sum=sum+prng->next();
	avg = sum/1000.0;
	if (avg < 0.25 || avg > 0.75) {
		printf("It is likely that random number generation on your machine\n" ); 
		printf("is different from what this emulator expects.  Please take\n");
		printf("a look at the routine prng implementaion in the emulator code. Sorry. \n");
		exit(1);
	}

	clocktime=0.0;                /* initialize time to 0.0 */

	int costs0[NUMENTITIES] = { 0, 1, 2, 3 };
	rtinit0(connectcosts[0]);
	rtinit1(connectcosts[1]);
	rtinit2(connectcosts[2]);
	rtinit3(connectcosts[3]);

	/* initialize future link changes */
	if (linkchanges)   {
		evlist.insert(event(10000.0, LINK_CHANGE, -1));
		evlist.insert(event(20000.0, LINK_CHANGE, -1));
	}
  
}

void
printevlist(void)
{
	printf("--------------\nEvent List Follows:\n");
	for (auto const &q : evlist)
		printf("Event time: %f, type: %d entity: %d\n", q.evtime, q.evtype, q.eventity);
	printf("--------------\n");
}


/************************** TOLAYER2 ***************/
void
tolayer2(struct rtpkt packet)
{
	struct rtpkt *mypktptr;
	double lastime;
	int i;

    
	/* be nice: check if source and destination id's are reasonable */
	if (packet.sourceid < 0 || packet.sourceid > 3) {
		printf("WARNING: illegal source id in your packet, ignoring packet!\n");
		return;
	}

	if (packet.destid < 0 || packet.destid > 3) {
		printf("WARNING: illegal dest id in your packet, ignoring packet!\n");
		return;
	}
	if (packet.sourceid == packet.destid)  {
		printf("WARNING: source and destination id's the same, ignoring packet!\n");
		return;
	}
	if (connectcosts[packet.sourceid][packet.destid] == 999)  {
		printf("WARNING: source and destination not connected, ignoring packet!\n");
		return;
	}


	/* make a copy of the packet student just gave me since he/she may decide */
	/* to do something with the packet after we return back to him/her */ 
	mypktptr = new rtpkt();
	mypktptr->sourceid = packet.sourceid;
	mypktptr->destid = packet.destid;
	for (i = 0; i < 4; i++)
		mypktptr->mincost[i] = packet.mincost[i];
	if (TRACE > 2)  {
		printf("    TOLAYER2: source: %d, dest: %d\n              costs:", 
		       mypktptr->sourceid, mypktptr->destid);
		for (i = 0; i < 4; i++)
			printf("%d  ",mypktptr->mincost[i]);
		printf("\n");
	}

	/* compute the arrival time of packet at the other end.
	   medium can not reorder, so make sure packet arrives between 1 and 10
	   time units after the latest arrival time of packets
	   currently in the medium on their way to the destination */
	lastime = clocktime;
	for (auto const &q : evlist)
		if ((q.evtype == FROM_LAYER2  && q.eventity == packet.destid)) 
			lastime = q.evtime;

	/* create future event for arrival of packet at the other side */
	auto evptr = event(lastime + 2.0 * prng->next(),
		FROM_LAYER2,		/* packet will pop out from layer3 */
		packet.destid);	/* event occurs at other entity */
	evptr.rtpktptr = mypktptr;       /* save ptr to my copy of packet */

	if (TRACE>2)  
		printf("    TOLAYER2: scheduling arrival on other side\n");

	evlist.insert(evptr);
}
