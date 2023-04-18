package edu.isu.wrigjaso.distancevector.app;

public class Project
{
	public final static void main(String[] argv)
	{
		NetworkSimulator simulator;

		int trace = 0;
		boolean hasLinkChange = false;
		long seed = -1;

		seed = System.currentTimeMillis();

		System.out.println("Network Simulator v1.0");

		for (String s: argv) {
			if (s.startsWith("seed=")) {
				if (s.length() == 5) {
					System.out.println("missing seed value");
					System.exit(1);
				}

				try {
					seed = Long.parseLong(s.substring(5));
				}
				catch (NumberFormatException nfe) {
					System.out.println("bad seed config:" + s);
					System.exit(1);
				}

				continue;
			}

			if (s.equals("linkchange")) {
				hasLinkChange = true;
				continue;
			}

			if (s.startsWith("trace=")) {
				if (s.length() == 6) {
					System.out.println("missing trace value");
					System.exit(1);
				}

				try {
					trace = Integer.parseInt(s.substring(6));
					if (trace < 0)
						throw new NumberFormatException();
				}
				catch (NumberFormatException nfe) {
					System.out.println("bad trace config:" + s);
					System.exit(1);
				}
				continue;
			}
		}

		simulator = new NetworkSimulator(hasLinkChange, trace, seed);
		simulator.runSimulator();
	}
}
