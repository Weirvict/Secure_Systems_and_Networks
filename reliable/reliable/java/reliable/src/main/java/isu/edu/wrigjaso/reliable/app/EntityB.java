package isu.edu.wrigjaso.reliable.app;

public class EntityB extends Entity {
	
	// Initialize anything you need here
	EntityB() {
		System.out.println(this.getClass().getName() + ":" +
                new Throwable().getStackTrace()[0].getMethodName() + " called.");
		// TODO add some code
	}

	// Called when layer5 wants to introduce new data into the stream
	// For EntityB, this function does not need to be filled in unless
	// you're doing the extra credit, bidirectional part of the assignment
	@Override
	void output(Message message) {
		System.out.println(this.getClass().getName() + ":" +
                new Throwable().getStackTrace()[0].getMethodName() + " called.");
		// TODO add some code
	}
	
	// Called when the network has a packet for this entity
	@Override
	void input(Packet pkt) {
		System.out.println(this.getClass().getName() + ":" +
                new Throwable().getStackTrace()[0].getMethodName() + " called.");
		// TODO add some code
		tolayer5(pkt.payload);
	}

	// called when your timer has expired
	@Override
	void timerinterrupt() {
		System.out.println(this.getClass().getName() + ":" +
                new Throwable().getStackTrace()[0].getMethodName() + " called.");
		// TODO add some code
	}

	// From here down are functions you may call that interact with the simulator.
	// You should not need to modify these functions.

	// Provided: call this function to start your timer
	@Override
	void starttimer(double increment) {
		Simulator.starttimer(this, increment);
	}
	
	// Provided: call this function to stop your timer
	@Override
	void stoptimer() {
		Simulator.stoptimer(this);
	}
	
	// Provided: call this function when you have data ready for layer5
	@Override
	void tolayer5(char[] data) {
		Simulator.tolayer5(this, data);
	}
	
	// Provided: call this function to send a layer3 packet
	@Override
	void tolayer3(Packet packet) {
		Simulator.tolayer3(this, packet);
	}
	
	// Provided: string representation of this object
	public String toString() {
		return "EntityB";
	}

}
