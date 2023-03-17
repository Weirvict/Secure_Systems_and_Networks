package isu.edu.wrigjaso.reliable.app;

class Message {
	// a "msg" is the data unit passed from layer 5 (teachers code) to layer
	// 4 (students' code).  It contains the data (characters) to be delivered
	// to layer 5 via the students transport level protocol entities.
	public Message() {
		data = new char[Simulator.MSGLEN];
	}

	public char[] data;
}