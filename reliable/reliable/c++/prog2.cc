#include <iostream>
#include <random>
#include <map>
#include <memory>

using std::cout;
using std::endl;

// XXX seeded RNG
// XXX move to unique pointers for event map
// XXX move to unique pointers for event packets
// XXX create real classes for event, packets, etc
// XXX make classes for the two entities
// XXX sanity check the simulation parameters

/* ******************************************************************
   ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
   are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
   or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
   (although some can be lost).
**********************************************************************/

#define OFF 0
#define ON 1
#define A 0
#define B 1

#define BIDIRECTIONAL 0 /* change to 1 if you're doing extra credit */
						/* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
struct msg
{
	char data[20];
};

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct pkt
{
	int seqnum;
	int acknum;
	int checksum;
	char payload[20];
};

void tolayer5(int AorB, char datasent[20]);
void stoptimer(int AorB);
void starttimer(int AorB, float increment);
void tolayer3(int AorB, struct pkt packet);

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
	cout << __func__ << " called " << endl;
}

/* need be completed only for extra credit */
void B_output(struct msg message)
{
	cout << __func__ << " called " << endl;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
	cout << __func__ << " called " << endl;
}

/* called when A's timer goes off */
void A_timerinterrupt(void)
{
	cout << __func__ << " called " << endl;
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(void)
{
	cout << __func__ << " called " << endl;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
	cout << __func__ << " called " << endl;
}

/* called when B's timer goes off */
void B_timerinterrupt(void)
{
	cout << __func__ << endl;
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(void)
{
	cout << __func__ << endl;
}

/*****************************************************************
 ***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 3 and below network environment:
  - emulates the tranmission and delivery (possibly with bit-level corruption
	and packet loss) of packets across the layer 3/4 interface
  - handles the starting/stopping of a timer, and generates timer
	interrupts (resulting in calling students timer handler).
  - generates message to be sent (passed from later 5 to 4)

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/
void init(void);
void generate_next_arrival(void);
float jimsrand(void);
void insertevent(struct event *p);
void printevlist(void);

struct event
{
	float evtime;		/* event time */
	int evtype;			/* event type code */
	int eventity;		/* entity where event occurs */
	struct pkt *pktptr; /* ptr to packet (if any) assoc w/ this event */
};

std::multimap<float, event *> evmap;

/* possible events: */
#define TIMER_INTERRUPT 0
#define FROM_LAYER5 1
#define FROM_LAYER3 2

int TRACE = 1;	 /* for my debugging */
int nsim = 0;	 /* number of messages from 5 to 4 so far */
int nsimmax = 0; /* number of msgs to generate, then stop */
float nettime = 0.000;
float lossprob;	   /* probability that a packet is dropped  */
float corruptprob; /* probability that one bit is packet is flipped */
float lambda;	   /* arrival rate of messages from layer 5 */
int ntolayer3;	   /* number sent into layer 3 */
int nlost;		   /* number lost in media */
int ncorrupt;	   /* number corrupted by media*/

int main(int argc, char *argv[])
{
	struct event *eventptr;
	struct msg msg2give;
	struct pkt pkt2give;

	int i, j;

	init();
	A_init();
	B_init();

	while (!evmap.empty())
	{
		printevlist();

		eventptr = evmap.begin()->second;
		evmap.erase(evmap.begin());

		if (TRACE >= 2)
		{
			cout << endl
				 << "EVENT time: " << eventptr->evtime
				 << " time: " << eventptr->evtype;
			switch (eventptr->evtype)
			{
			case FROM_LAYER3:
				cout << ", fromlayer3";
				break;
			case FROM_LAYER5:
				cout << ", fromlayer5";
				break;
			case TIMER_INTERRUPT:
				cout << ", timerinterrupt";
				break;
			default:
				cout << ", unknown event!";
			}

			cout << " entity: " << eventptr->eventity << endl;
		}

		nettime = eventptr->evtime; /* update time to next event time */

		if (eventptr->evtype == FROM_LAYER5)
		{
			if (nsim == nsimmax)
			{
				// we've generated all the layer5 data for this simulation
				continue;
			}

			generate_next_arrival(); /* set up future arrival */
			/* fill in msg to give with string of same letter */
			j = nsim % 26;
			for (i = 0; i < 20; i++)
				msg2give.data[i] = 97 + j;
			if (TRACE > 2)
			{
				cout << "          MAINLOOP: data given to student: ";
				for (i = 0; i < 20; i++)
					cout << msg2give.data[i];
				cout << endl;
			}
			nsim++;
			if (eventptr->eventity == A)
				A_output(msg2give);
			else
				B_output(msg2give);
		}
		else if (eventptr->evtype == FROM_LAYER3)
		{
			pkt2give.seqnum = eventptr->pktptr->seqnum;
			pkt2give.acknum = eventptr->pktptr->acknum;
			pkt2give.checksum = eventptr->pktptr->checksum;
			for (i = 0; i < 20; i++)
				pkt2give.payload[i] = eventptr->pktptr->payload[i];
			if (eventptr->eventity == A) /* deliver packet by calling */
				A_input(pkt2give);		 /* appropriate entity */
			else
				B_input(pkt2give);
			delete eventptr->pktptr;
		}
		else if (eventptr->evtype == TIMER_INTERRUPT)
		{
			if (eventptr->eventity == A)
				A_timerinterrupt();
			else
				B_timerinterrupt();
		}
		else
		{
			cout << "INTERNAL PANIC: unknown event type" << endl;
		}
		delete eventptr;
	}

terminate:
	cout << " Simulator terminated at time " << nettime << endl;
	cout << " after sending " << nsim << " msgs from layer5" << endl;
}

/* initialize the simulator */
void init(void)
{
	int i;
	float sum, avg;

#if 1
	printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
	printf("Enter the number of messages to simulate: ");
	scanf("%d",&nsimmax);
	printf("Enter  packet loss probability [enter 0.0 for no loss]:");
	scanf("%f",&lossprob);
	printf("Enter packet corruption probability [0.0 for no corruption]:");
	scanf("%f",&corruptprob);
	printf("Enter average time between messages from sender's layer5 [ > 0.0]:");
	scanf("%f",&lambda);
	printf("Enter TRACE:");
	scanf("%d",&TRACE);
#else
	nsimmax = 10;
	lossprob = 0.0;
	corruptprob = 0.2;
	lambda = 1.0;
	TRACE = 3;
#endif

	ntolayer3 = 0;
	nlost = 0;
	ncorrupt = 0;

	nettime = 0.0;			 /* initialize time to 0.0 */
	generate_next_arrival(); /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand(void)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> distf(0.0f, 1.0f);
	return distf(rng);
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival(void)
{
	double x;
	struct event *evptr;

	if (TRACE > 2)
		cout << "          GENERATE NEXT ARRIVAL: creating new arrival" << endl;

	x = lambda * jimsrand() * 2; /* x is uniform on [0,2*lambda] */
	/* having mean of lambda        */
	evptr = new event();
	evptr->evtime = nettime + x;
	evptr->evtype = FROM_LAYER5;
	if (BIDIRECTIONAL && (jimsrand() > 0.5))
		evptr->eventity = B;
	else
		evptr->eventity = A;
	insertevent(evptr);
}

void insertevent(struct event *p)
{
	if (TRACE > 2)
	{
		cout << "            INSERTEVENT: time is" << nettime << endl;
		cout << "            INSERTEVENT: future time will be " << p->evtime << endl;
	}
	evmap.insert(std::pair<float, event *>(p->evtime, p));
}

void printevlist(void)
{
	struct event *q;

	cout << "--------------" << endl;
	for (auto it = evmap.cbegin(); it != evmap.cend(); ++it)
		cout << "Event time: " << it->second->evtime
			 << ", type: " << it->second->evtype
			 << ", entity: " << it->second->eventity << endl;
	cout << "--------------" << endl;
}

/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
/* A or B is trying to stop timer */
void stoptimer(int AorB)
{
	if (TRACE > 2)
		cout << "          STOP TIMER: stopping timer at " << nettime << endl;

	for (auto it = evmap.begin(); it != evmap.end(); ++it)
	{
		if (it->second->eventity == AorB && it->second->evtype == TIMER_INTERRUPT)
		{
			evmap.erase(it);
			return;
		}
	}

	cout << "Warning: unable to cancel your timer. It wasn't running." << endl;
}

void starttimer(int AorB, float increment)
{

	struct event *q;
	struct event *evptr;

	if (TRACE > 2)
		cout << "          START TIMER: starting timer at " << nettime << endl;

	/* be nice: check to see if timer is already started, if so, then  warn */
	for (auto em : evmap)
	{
		if (em.second->eventity == AorB && em.second->evtype == TIMER_INTERRUPT)
			cout << "Warning: attempt to start a timer that is already started" << endl;
	}

	/* create future event for when timer goes off */
	evptr = new event();
	evptr->evtime = nettime + increment;
	evptr->evtype = TIMER_INTERRUPT;
	evptr->eventity = AorB;
	insertevent(evptr);
}

/************************** TOLAYER3 ***************/
void tolayer3(int AorB, struct pkt packet)
{
	struct pkt *mypktptr;
	struct event *evptr, *q;
	float lastime, x;
	int i;

	ntolayer3++;

	/* simulate losses: */
	if (jimsrand() < lossprob)
	{
		nlost++;
		if (TRACE > 0)
			cout << "          TOLAYER3: packet being lost" << endl;
		return;
	}

	/* make a copy of the packet student just gave me since he/she may decide */
	/* to do something with the packet after we return back to him/her */
	mypktptr = new pkt();
	mypktptr->seqnum = packet.seqnum;
	mypktptr->acknum = packet.acknum;
	mypktptr->checksum = packet.checksum;
	for (i = 0; i < 20; i++)
		mypktptr->payload[i] = packet.payload[i];
	if (TRACE > 2)
	{
		cout << "          TOLAYER3: seq: " << mypktptr->seqnum
			 << ", ack " << mypktptr->acknum
			 << ", check " << mypktptr->checksum << " ";
		for (i = 0; i < 20; i++)
			cout << mypktptr->payload[i];
		cout << endl;
	}

	/* create future event for arrival of packet at the other side */
	evptr = new event();
	evptr->evtype = FROM_LAYER3;	  /* packet will pop out from layer3 */
	evptr->eventity = (AorB + 1) % 2; /* event occurs at other entity */
	evptr->pktptr = mypktptr;		  /* save ptr to my copy of packet */
	/* finally, compute the arrival time of packet at the other end.
	   medium can not reorder, so make sure packet arrives between 1 and 10
	   time units after the latest arrival time of packets
	   currently in the medium on their way to the destination */
	lastime = nettime;

	for (const auto em : evmap)
		if (em.second->evtype == FROM_LAYER3 && em.second->eventity == evptr->eventity)
			lastime = em.second->evtime;
	evptr->evtime = lastime + 1 + 9 * jimsrand();

	/* simulate corruption: */
	if (jimsrand() < corruptprob)
	{
		ncorrupt++;
		if ((x = jimsrand()) < .75)
			mypktptr->payload[0] = 'Z'; /* corrupt payload */
		else if (x < .875)
			mypktptr->seqnum = 999999;
		else
			mypktptr->acknum = 999999;
		if (TRACE > 0)
			cout << "          TOLAYER3: packet being corrupted" << endl;
	}

	if (TRACE > 2)
		cout << "          TOLAYER3: scheduling arrival on other side" << endl;
	insertevent(evptr);
}

void tolayer5(int AorB, char datasent[20])
{
	int i;
	if (TRACE > 2)
	{
		cout << "          TOLAYER5: data received: ";
		for (i = 0; i < 20; i++)
			cout << datasent[i];
		cout << endl;
	}
}
