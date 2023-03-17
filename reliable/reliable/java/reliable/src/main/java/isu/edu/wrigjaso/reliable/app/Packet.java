package isu.edu.wrigjaso.reliable.app;

public class Packet {
	// a packet is the data unit passed from layer 4 (students code) to layer
	// 3 (teachers code).  Note the pre-defined packet structure, which all
	// students must follow.
	public int seqnum;
	public int acknum;
	public int checksum;
	public char[] payload;

	public Packet() {
		payload = new char[Simulator.MSGLEN];
	}
}