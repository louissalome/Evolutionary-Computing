import org.uncommons.watchmaker.framework.factories.AbstractCandidateFactory;

import java.util.Random;

public class MyFactory extends AbstractCandidateFactory<double[]> {

    private int dimension;

    public MyFactory(int dimension) {
        this.dimension = dimension;
    }

    public double[] generateRandomCandidate(Random random) {
        double[] solution = new double[dimension];

        // your implementation:
        double r;
        for (int i=0; i < dimension; i++) {
            // x from -5.0 to 5.0
        	r = (double) Math.random() * 10 - 5;
        	solution[i] = r;
        }
        return solution;
    }
}
