
//TODO ********************* Student-callable ROUTINES ***********************

package isu.edu.wrigjaso.reliable.app;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

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

public class Simulator 
{
	// change to true if you're doing extra credit
	// and fill in the function Entity.output()
	static boolean BIDIRECTIONAL = false;
	
	// length of payload in messages
	static final int MSGLEN = 20;

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
	
	private static class Event implements Comparable<Event> {
		private double evtime;
		private int evtype;
		private Entity eventity;
		private Packet pkt;
		
		// event types:
		static final int TIMER_INTERRUPT = 0;
		static final int FROM_LAYER5 = 1;
		static final int FROM_LAYER3 = 2;

		@Override
		public int compareTo(Event obj) {
			return Double.compare(this.evtime, obj.evtime);
		}
	}
	
	static void usage() {
		System.err.println("sim [trace=%%d] [seed=%%d] [messages=%%d] [corruptprob=%%f] [lossprob=%%f] [bidirectional");
		System.err.println("defaults:");
		System.err.printf("\ttrace=%d\n", DEFAULT_TRACE);
		System.err.printf("\tseed=%d\n", DEFAULT_SEED);
		System.err.printf("\tmessages=%d\n", DEFAULT_MESSAGES);
		System.err.printf("\tcorruptprob=%f\n", DEFAULT_CORRUPTPROB);
		System.err.printf("\tlossprob=%f\n", DEFAULT_LOSSPROB);
		System.err.printf("\tlambda=%f\n", DEFAULT_LAMBDA);
	}

	public final static void main(String[] args) {
		System.out.println("-----  Stop and Wait Network Simulator Version 1.1 --------");
		System.out.println();

		seed = DEFAULT_SEED;
		TRACE = DEFAULT_TRACE;
		nsimmax = DEFAULT_MESSAGES;
		lossprob = DEFAULT_LOSSPROB;
		corruptprob = DEFAULT_CORRUPTPROB;
		lambda = DEFAULT_LAMBDA;

		for (String s: args) {
			if (s.startsWith("seed=")) {
				try {
					if (s.length() == 5)
						throw new NumberFormatException();
					seed = Long.parseLong(s.substring(5));
				}
				catch (NumberFormatException nfe) {
					System.err.println("bad argument: " + s);
					System.exit(1);
				}
				continue;
			}
			
			if (s.startsWith("trace=")) {
				try {
					if (s.length() == 6)
						throw new NumberFormatException();
					TRACE = Integer.parseInt(s.substring(6));
					if (TRACE < 0)
						throw new NumberFormatException();
				}
				catch (NumberFormatException nfe) {
					System.err.println("bad argument: " + s);
					System.exit(1);
				}
				continue;
			}

			if (s.startsWith("messages=")) {
				try {
					if (s.length() == 9)
						throw new NumberFormatException();
					nsimmax = Integer.parseInt(s.substring(9));
					if (nsimmax < 0)
						throw new NumberFormatException();
				}
				catch (NumberFormatException nfe) {
					System.err.println("bad argument: " + s);
					System.exit(1);
				}
				continue;
			}

			if (s.startsWith("corruptprob=")) {
				try {
					if (s.length() == 12)
						throw new NumberFormatException();
					corruptprob = Double.parseDouble(s.substring(12));
					if (corruptprob < 0.0 || corruptprob > 1.0)
						throw new NumberFormatException();
				}
				catch (NumberFormatException nfe) {
					System.err.println("bad argument: " + s);
					System.exit(1);
				}
				continue;
			}

			if (s.startsWith("lossprob=")) {
				try {
					if (s.length() == 9)
						throw new NumberFormatException();
					lossprob = Double.parseDouble(s.substring(9));
					if (lossprob < 0.0 || lossprob > 1.0)
						throw new NumberFormatException();
				}
				catch (NumberFormatException nfe) {
					System.err.println("bad argument: " + s);
					System.exit(1);
				}
				continue;
			}

			if (s.startsWith("lambda=")) {
				try {
					if (s.length() == 7)
						throw new NumberFormatException();
					lambda = Double.parseDouble(s.substring(7));
					if (lambda <= 0.0)
						throw new NumberFormatException();
				}
				catch (NumberFormatException nfe) {
					System.err.println("bad argument: " + s);
					System.exit(1);
				}
				continue;
			}
			
			if (s.equals("bidirectional")) {
				BIDIRECTIONAL = true;
				continue;
			}

			usage();
			System.exit(1);
		}
		
		evlist = new ArrayList<Event>();

		init();
		EA = new EntityA();
		EB = new EntityB();

		generate_next_arrival();     // initialize event list

		while (!evlist.isEmpty()) {
			Event e = evlist.remove(0);
			if (TRACE >= 2) {
				System.out.printf("\nEVENT time: %f,", e.evtime);
				System.out.printf("  type: %d", e.evtype);
				if (e.evtype == Event.TIMER_INTERRUPT)
					System.out.print(", timerinterrupt  ");
				else if (e.evtype == Event.FROM_LAYER5)
					System.out.print(", fromlayer5 ");
				else
					System.out.print(", fromlayer3 ");
				System.out.println(" " + e.eventity);
			}

			time = e.evtime;        /* update time to next event time */

			if (e.evtype == Event.FROM_LAYER5) {
				if (nsim != nsimmax) {
					generate_next_arrival();   /* set up future arrival */
				}
				
				Message msg2give = new Message();
				/* fill in msg to give with string of same letter */
				for (int i = 0; i < msg2give.data.length; i++)
					msg2give.data[i] = (char)('A' + (nsim % 26));
				if (TRACE > 2) {
					System.out.print("          MAINLOOP: data given to student: ");
					for (int i=0; i<20; i++) 
						System.out.printf("%c", msg2give.data[i]);
					System.out.println();
				}
				nsim++;
				e.eventity.output(msg2give);
			}
			else if (e.evtype ==  Event.FROM_LAYER3) {
				Packet pkt2give = new Packet();
				pkt2give.seqnum = e.pkt.seqnum;
				pkt2give.acknum = e.pkt.acknum;
				pkt2give.checksum = e.pkt.checksum;
				for (int i = 0; i < pkt2give.payload.length; i++)
					pkt2give.payload[i] = e.pkt.payload[i];

				// deliver packet to appropriate entity
				e.eventity.input(pkt2give);
			}
			else if (e.evtype == Event.TIMER_INTERRUPT)
				e.eventity.timerinterrupt();

			else {
				System.out.println("INTERNAL PANIC: unknown event type");
				System.exit(1);
			}
		}
		System.out.printf(" Simulator terminated at time %f\n after sending %d msgs from layer5\n", time, nsim);
	}

	static void init() {
		rand = new Random(seed);
		ntolayer3 = 0;
		nlost = 0;
		ncorrupt = 0;
		time=0.0;                    // initialize time to 0.0
	}

	static void insertevent(Event p) {
		if (TRACE > 2) {
			System.out.printf("            INSERTEVENT: future time will be %f\n", p.evtime);
		}
		evlist.add(p);
		Collections.sort(evlist);
	}

	static void starttimer(Entity entity, double increment) {
		if (TRACE > 2)
			System.out.printf("          START TIMER: starting timer at %f\n",time);
		
		// be nice: check to see if timer is already started, if so, then  warn
		for (Event e: evlist) {
			if (e.evtype == Event.TIMER_INTERRUPT && e.eventity == entity) {
				System.out.println("Warning: attempt to start a timer that is already started\n");
				return;
			}
		}
		// create future event for when timer goes off
		Event e = new Event();
		e.evtime =  time + increment;
		e.evtype =  Event.TIMER_INTERRUPT;
		e.eventity = entity;
		insertevent(e);
	}
	
	///* called by students routine to cancel a previously-started timer */
	static void stoptimer(Entity entity) {
		if (TRACE>2)
			System.out.printf("          STOP TIMER: stopping timer at %f\n",time);
		for (Event e: evlist) {
			if (e.evtype == Event.TIMER_INTERRUPT && e.eventity == entity) {
				evlist.remove(e);
				return;
			}
			System.out.println("Warning: unable to cancel your timer. It wasn't running.");
		}
	}

	static void printevlist() {
		System.out.println("--------------");
		System.out.println("Event List Follows:");
		for (Event e: evlist)
			System.out.printf("Event time: %f, type: %d entity: %d\n", e.evtime, e.evtype, e.eventity);
		System.out.println("--------------");
	}

	static void tolayer5(Entity entity, char[] datasent) {
		if (TRACE>2) {
			System.out.print("          TOLAYER5: data received: ");
			for (int i = 0; i < datasent.length; i++)
				System.out.printf("%c", datasent[i]);
			System.out.println();
		}
	}

	//************************* TOLAYER3 ***************
	static void tolayer3(Entity entity, Packet packet) {
		ntolayer3++;

		// simulate losses:
		if (rand.nextDouble() < lossprob)  {
			nlost++;
			if (TRACE > 0)    
				System.out.println("          TOLAYER3: packet being lost\n");
			return;
		}

		// make a copy of the packet student just gave me since he/she may decide
		// to do something with the packet after we return back to him/her
		Packet mypkt = new Packet();
		mypkt.seqnum = packet.seqnum;
		mypkt.acknum = packet.acknum;
		mypkt.checksum = packet.checksum;
		for (int i = 0; i < mypkt.payload.length; i++)
			mypkt.payload[i] = packet.payload[i];

		if (TRACE>2)  {
			System.out.printf("          TOLAYER3: seq: %d, ack %d, check: %d ", mypkt.seqnum,
			       mypkt.acknum,  mypkt.checksum);
			for (int i=0; i < mypkt.payload.length; i++)
				System.out.printf("%c", mypkt.payload[i]);
			System.out.println();
		}

		// create future event for arrival of packet at the other side
		Event e = new Event();
		// packet will pop out from layer3
		e.evtype =  Event.FROM_LAYER3;
		// event occurs at other entity
		e.eventity = (entity == EA) ? EB : EA;
		// save my copy of packet
		e.pkt = mypkt;

		// finally, compute the arrival time of packet at the other end.
		// medium can not reorder, so make sure packet arrives between 1 and 10
		// time units after the latest arrival time of packets
		// currently in the medium on their way to the destination
		double lastime = time;

		for (Event q : evlist)
			if (q.evtype == Event.FROM_LAYER3 && q.eventity == e.eventity)
				lastime = q.evtime;

		e.evtime =  lastime + 1.0 + 9.0 * rand.nextDouble();

		// simulate corruption:
		if (rand.nextDouble() < corruptprob) {
			ncorrupt++;
			double x = rand.nextDouble();
			if (x < .75)
				mypkt.payload[0] = 'Z';		// corrupt payload
			else if (x < .875)
				mypkt.seqnum = 999999;
			else
				mypkt.acknum = 999999;
			if (TRACE>0)    
				System.out.println("          TOLAYER3: packet being corrupted");
		}  

		if (TRACE>2)  
			System.out.println("          TOLAYER3: scheduling arrival on other side");
		insertevent(e);
	} 

	//********************* EVENT HANDLINE ROUTINES *******
	//  The next set of routines handle the event list
	//*****************************************************
	static void	generate_next_arrival() {
		// x is uniform on [0,2*lambda]
		// having mean of lambda
		double x = lambda * rand.nextDouble() * 2.0;

		if (TRACE > 2)
			System.out.println("          GENERATE NEXT ARRIVAL: creating new arrival");

		Event e = new Event();
		e.evtime =  time + x;
		e.evtype =  Event.FROM_LAYER5;
		if (BIDIRECTIONAL && (rand.nextDouble() > 0.5) )
			e.eventity = EB;
		else
			e.eventity = EA;
		insertevent(e);
	}

	static final int DEFAULT_TRACE = 1;
	static final int DEFAULT_SEED = 9999;
	static final int DEFAULT_MESSAGES = 10;
	static final double DEFAULT_CORRUPTPROB = 0.0;
	static final double DEFAULT_LOSSPROB = 0.0;
	static final double DEFAULT_LAMBDA = 0.1;

	static int TRACE;             /* for my debugging */
	static int nsim = 0;              /* number of messages from 5 to 4 so far */ 
	static int nsimmax = 0;           /* number of msgs to generate, then stop */
	static double time = 0.000;
	static double lossprob;            /* probability that a packet is dropped  */
	static double corruptprob;         /* probability that one bit is packet is flipped */
	static double lambda;              /* arrival rate of messages from layer 5 */   

	static int ntolayer3;             /* number sent into layer 3 */
	static int nlost;                 /* number lost in media */
	static int ncorrupt;              /* number corrupted by media*/
	static long seed;
	static List<Event> evlist;
	static EntityA EA;
	static EntityB EB;
	static Random rand;
}