import org.uncommons.watchmaker.framework.operators.AbstractCrossover;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MyCrossover extends AbstractCrossover<double[]> {
    protected MyCrossover() {
        super(1);
    }

    protected List<double[]> mate(double[] p1, double[] p2, int i, Random random) {
        ArrayList<double[]> children = new ArrayList<double[]>();
        // your implementation:
        int len = p1.length;
        int crossingPointStart;
        int crossingPointEnd;
        int crossingPoint;
        int tmp;
                
        // Initialise our 2 children by copying their parents
        double [] child1 = new double[len];
        double [] child2 = new double[len];
        for (int j=0;j<len;j++) {
        	child1[j]=p1[j];
        	child2[j]=p2[j];
        }
        
        
        // We will use 2 different crossovers
        if (Math.random()<0.5) {
        	// The 2-point crossover
            crossingPointStart = (int) Math.floor(Math.random()*len);
	        crossingPointEnd = (int) Math.floor(Math.random()*(len));
	 
	        if (crossingPointStart > crossingPointEnd) {
	        	tmp = crossingPointEnd;
	        	crossingPointEnd = crossingPointStart;
	        	crossingPointStart = tmp;
	        }
	        
	        for (int j=0; j<len; j++) {
	        	if (crossingPointStart<=j && j<=crossingPointEnd) {
	        		child1[j] = p2[j];
	        	}
	        }
	        
	        crossingPointStart = (int) Math.floor(Math.random()*len);
	        crossingPointEnd = (int) Math.floor(Math.random()*(len));
	 
	        if (crossingPointStart > crossingPointEnd) {
	        	tmp = crossingPointEnd;
	        	crossingPointEnd = crossingPointStart;
	        	crossingPointStart = tmp;
	        }
	        
	        // The 1 point crossover
	        for (int j=0; j<len; j++) {
	        	if (!(crossingPointStart<=j && j<=crossingPointEnd)) {
	        		child2[j] = p1[j];
	        	}
	        }
        }
        
        else {
        	//The 1 point crossover
            crossingPoint = (int) Math.floor(Math.random()*len);
	        
            for (int j=0; j<=crossingPoint; j++) {
	        	child2[j] = p1[j];
	    	}
            
        	//The 1 point crossover
            crossingPoint = (int) Math.floor(Math.random()*len);
	        
            for (int j=crossingPoint; j<len; j++) {
	        	child1[j] = p2[j];
	    	}
        }    
	        
          
        children.add(child1);
    	children.add(child2);
        return children;
    }
}
