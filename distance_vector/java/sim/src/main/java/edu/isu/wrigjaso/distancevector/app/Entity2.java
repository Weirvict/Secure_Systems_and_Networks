package edu.isu.wrigjaso.distancevector.app;

public class Entity2 extends Entity
{    
    // Perform any necessary initialization in the constructor
    private int[] pkt_mincost = new int[NetworkSimulator.NUMENTITIES];
    public Entity2(int[] startingvector)
    {
    	// startingvector is an array of costs for this Entity.
    	// startingvector[2] will be 0 (zero cost to talk to yourself)
    	// if startingvector[n] == 999, then node 'n' is not actually connected
    	// to this node.
        distanceTable[0][0] = NetworkSimulator.cost[0][2];
        distanceTable[1][1] = NetworkSimulator.cost[1][2];
        distanceTable[2][2] = NetworkSimulator.cost[2][2];
        distanceTable[3][3] = NetworkSimulator.cost[3][2];

        for(int i = 0; i < NetworkSimulator.NUMENTITIES; i++)
            pkt_mincost[i] = distanceTable[i][i];
    }
 
    // Called when the simulator is ready to start handling packets.
    // This should be the first function that sends this Entity's vector to
    // its neighbors by calling NetworkSimulator.toLayer2()
    public void start() {
        NetworkSimulator.toLayer2(new Packet(2,0,pkt_mincost));
        NetworkSimulator.toLayer2(new Packet(2,1,pkt_mincost));
        NetworkSimulator.toLayer2(new Packet(2,3,pkt_mincost));
    }

    // Handle updates when a packet is received.  Students will need to call
    // NetworkSimulator.toLayer2() with new packets based upon what they
    // send to update.  Be careful to construct the source and destination of
    // the packet correctly.  Read the warning in NetworkSimulator.java for more
    // details.
    public void update(Packet p)
    {
        int newcost;

        for(int i = 0; i < NetworkSimulator.NUMENTITIES; i++)
        {
            boolean changed = false;
            newcost = distanceTable[p.getSource()][p.getSource()] + p.getMincost(i);

            distanceTable[i][p.getSource()] = newcost;

            if(distanceTable[i][p.getSource()]<pkt_mincost[i]){
                pkt_mincost[i] = distanceTable[i][p.getSource()];
                changed=true;
                //System.out.print("work!1");

            }
            if(changed == true){
                NetworkSimulator.toLayer2(new Packet(2,0,pkt_mincost));
                NetworkSimulator.toLayer2(new Packet(2,1,pkt_mincost));
                NetworkSimulator.toLayer2(new Packet(2,3,pkt_mincost));
            }
        }
        this.printDT();
    }

    // for extra credit, handle the case where the cost of a link changes.
    public void linkCostChangeHandler(int whichLink, int newCost)
    {
    }
     
    public void printDT()
    {
        System.out.println();
        System.out.println("           via");
        System.out.println(" D2 |   0   1   3");
        System.out.println("----+------------");
        for (int i = 0; i < NetworkSimulator.NUMENTITIES; i++)
        {
            if (i == 2)
            {
                continue;
            }
            
            System.out.print("   " + i + "|");
            for (int j = 0; j < NetworkSimulator.NUMENTITIES; j++)
            {
                if (j == 2)
                {
                    continue;
                }
                
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
