import org.uncommons.watchmaker.framework.EvolutionaryOperator;

import java.util.List;
import java.util.Random;

public class MyMutation implements EvolutionaryOperator<double[]> {
	int thisGenerationNumber=0;
	
    public List<double[]> apply(List<double[]> population, Random random) {
        // initial population
        // need to change individuals, but not their number!
    	
    	
    	// a mutation happens in 1 percent of the cases
    	float mutationRate = 0.01f;
    	int popSize = population.size();
    	int genesLength = population.get(0).length;
    	
    	thisGenerationNumber++;
    	float minimutationAmplitude = 0.0001f;
    	int std = 10000;
    	float mutationAmplitude = 
    			(float) ((float) 
    			(5-minimutationAmplitude)*
    			(Math.exp((double)-(thisGenerationNumber/std)*(thisGenerationNumber/std))))
    			+minimutationAmplitude;
    	
        // your implementation:
    	
    	// For each individual
    	for (int i=0;i<popSize;i++) {
    		
    		// Small mutation
    		// In 99% of the cases, small mutations can occur on genes
    		if (Math.random()<0.99) {
    			
	    		for (int j=0;j< genesLength ;j++) {
	    			if (Math.random()< mutationRate) {
	    				// Small mutation of the gene, close from the previous value
	    				population.get(i)[j] += mutationAmplitude*((double) Math.random() - 0.5);
	    				if (population.get(i)[j]>5.0) {population.get(i)[j]=(double) 5.0;}
	    				if (population.get(i)[j]<-5.0) {population.get(i)[j]=(double) -5.0;}
					}
	    		}	
	    			
    		}
    		
    		// Big mutation
    		// In 1% of the case, we change 5% of the genes to get a very different individual
    		else {
    			for (int j=0;j< genesLength ;j++) {
        			if (Math.random()< 0.05) {
		    		// new gene
    				population.get(i)[j] += mutationAmplitude*((double) Math.random() - 0.5);
    				if (population.get(i)[j]>5.0) {population.get(i)[j]=(double) 5.0;}
    				if (population.get(i)[j]<-5.0) {population.get(i)[j]=(double) -5.0;}}
    				
    			}
    		}
    	}

        //result population
        return population;
    }
}
