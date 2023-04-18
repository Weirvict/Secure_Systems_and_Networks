package edu.isu.wrigjaso.distancevector.app;

public class Entity3 extends Entity
{    
    // Perform any necessary initialization in the constructor
    public Entity3(int[] startingvector)
    {
    	// startingvector is an array of costs for this Entity.
    	// startingvector[3] will be 0 (zero cost to talk to yourself)
    	// if startingvector[n] == 999, then node 'n' is not actually connected
    	// to this node.
    }
 
    // Called when the simulator is ready to start handling packets.
    // This should be the first function that sends this Entity's vector to
    // its neighbors by calling NetworkSimulator.toLayer2()
    public void start() {
    }

    // Handle updates when a packet is received.  Students will need to call
    // NetworkSimulator.toLayer2() with new packets based upon what they
    // send to update.  Be careful to construct the source and destination of
    // the packet correctly.  Read the warning in NetworkSimulator.java for more
    // details.
    public void update(Packet p)
    {
    }

    // for extra credit, handle the case where the cost of a link changes.
    public void linkCostChangeHandler(int whichLink, int newCost)
    {
    }
     
    public void printDT()
    {
        System.out.println("         via");
        System.out.println(" D3 |   0   2");
        System.out.println("----+--------");
        for (int i = 0; i < NetworkSimulator.NUMENTITIES; i++)
        {
            if (i == 3)
            {
                continue;
            }
            
            System.out.print("   " + i + "|");
            for (int j = 0; j < NetworkSimulator.NUMENTITIES; j += 2)
            {
               
                if (distanceTable[i][j] < 10)
                {    
                    System.out.print("   ");
                }
                else if (distanceTable[i][j] < 100)
                {
                    System.out.print("  ");
                }
                else 
                {
                    System.out.print(" ");
                }
                
                System.out.print(distanceTable[i][j]);
            }
            System.out.println();
        }
    }
}
