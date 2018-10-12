import org.uncommons.watchmaker.framework.*;
import org.uncommons.watchmaker.framework.operators.EvolutionPipeline;
import org.uncommons.watchmaker.framework.selection.RouletteWheelSelection;
//import org.uncommons.watchmaker.framework.selection.TournamentSelection;
import org.uncommons.watchmaker.framework.termination.GenerationCount;

import java.util.ArrayList;
//import java.util.Arrays;
import java.util.Random;

public class MyAlg {

    private static boolean found09;
    private static boolean found099;
    
	public static void main(String[] args) {
        int dimension = 100; // dimension of problem
        int populationSize = 20; // size of population
        int generations = 50001; // number of generations
        found09 = true;
        found099 = true;

        Random random = new Random(); // random

        CandidateFactory<double[]> factory = new MyFactory(dimension); // generation of solutions

        ArrayList<EvolutionaryOperator<double[]>> operators = new ArrayList<EvolutionaryOperator<double[]>>();
        operators.add(new MyCrossover()); // Crossover
        operators.add(new MyMutation()); // Mutation
        EvolutionPipeline<double[]> pipeline = new EvolutionPipeline<double[]>(operators);

        SelectionStrategy<Object> selection = new RouletteWheelSelection(); // Selection operator

        FitnessEvaluator<double[]> evaluator = new FitnessFunction(dimension); // Fitness function

        EvolutionEngine<double[]> algorithm = new SteadyStateEvolutionEngine<double[]>(
                factory, pipeline, evaluator, selection, populationSize, false, random);

        algorithm.addEvolutionObserver(new EvolutionObserver<Object>() {
            public void populationUpdate(PopulationData<?> populationData) {
                double bestFit = populationData.getBestCandidateFitness();
                if (bestFit > 9 && found09) {
                	found09 = false;
                	System.out.println("*********************************************");
                	System.out.println("Generation " + populationData.getGenerationNumber() + ": ");
                	System.out.println("\tBest fit : "+ bestFit);
                	System.out.println("\tPop size = " + populationData.getPopulationSize());
                	System.out.println("*********************************************");
                }
                if (bestFit > 9.9 && found099) {
                	found099 = false;
                	System.out.println("*********************************************");
                	System.out.println("Generation " + populationData.getGenerationNumber() + ": ");
                	System.out.println("\tBest fit : "+ bestFit);
                	System.out.println("\tPop size = " + populationData.getPopulationSize());
                	System.out.println("*********************************************");
                }
                if (populationData.getGenerationNumber()%1000==0) {
                	System.out.println("Generation " + populationData.getGenerationNumber() + ": ");
                    System.out.println("\tBest fit : "+ bestFit);
                    
                    //System.out.println("\tBest solution = " + Arrays.toString((double[])populationData.getBestCandidate()));
                    //System.out.println("\tPop size = " + populationData.getPopulationSize());
                }
            }
        });

        TerminationCondition terminate = new GenerationCount(generations);
        algorithm.evolve(populationSize, 1, terminate);
    }
}
