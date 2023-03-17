package isu.edu.wrigjaso.reliable.app;

abstract class Entity {
	
	// called from layer5 when a message is ready to be sent by the application
	abstract void output(Message message);

	// called from layer 3, when a packet arrives for layer 4
	abstract void input(Packet packet);

	/* called when timer goes off */
	abstract void timerinterrupt();

	// Provided: call this function to start your timer
	abstract void starttimer(double increment);
	
	// Provided: call this function to stop your timer
	abstract void stoptimer();
	
	// Provided: call this function when you have data ready for layer5
	abstract void tolayer5(char[] data);
	
	// Provided: call this function to send a layer3 packet
	abstract void tolayer3(Packet packet);
}
