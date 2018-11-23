#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <math.h>

using namespace std;

/*
*** This is a TSP problem solver with a genetic algorithm. ***

Here is the strategy :
+ We initialize a population of 100 paths.
+ We make it evolve for 10000s generations.
+ The strategy is elitist, you survive iff you bring an improvement.
+ Mutation :
    number of mutation decreases exponentially from 10 to 1 with generation.
    1 mutation either swap 2 cities or move 1 in the path.
+ Crossover :
    crossover with a 3-individuals tournament.
    almost classic 2 points crossover.
*/

int N = 395; // number of cities
int A_best; // optimal length
string line_; // line_ for reading text file
int dist[395][395]; // store distances
int numbers[395]; // integers from 0 to N-1 to initialise path
int popSize = 100; // population size
int nbMaxofMutation = 20; // maximum number of mutation in one call of mutate()

// tuning maximum number of mutation
void updateHyperParameters(int gen){
    nbMaxofMutation = 10*exp(-gen/20000.0f) + 2;
}

// swap 2 integers
void mySwap(int *i, int *j) {
   int t = *i;
   *i = *j;
   *j = t;
}

// read file containing distances
void read(){
    ifstream file_("tsp.in.txt");

    if (file_.is_open()){
        file_ >> N >> A_best;
        getline(file_,line_);

        int indexLine = 0;
        int indexColumn;
        int tmp=0;
        while(getline(file_,line_)){
            stringstream ssin(line_);
            indexColumn = 0;
            while (ssin.good()){
                ssin >> tmp;
                dist[indexLine+1][indexColumn]=tmp;
                dist[indexColumn][indexLine+1]=tmp;
                ++indexColumn;
            }
            ++indexLine;
        }
        file_.close();
    }
    else{
        cout<<"file is not open"<<endl;
    }
}

// print basic informations about the problem
void printInfo(){
    cout << "N            : " << N << endl;
    cout << "A_best       : " << A_best << endl;
    cout << "Optimal*1.05 : " << 1.05*A_best << endl;
    /*
    // print beginning of distance array
    for (int i=0;i<10;i++){
        for (int j=0;j<10;j++){
            cout << dist[i][j] << " ";
        }
        cout << endl;
    }
    */
}

////////////////////////////////////////////
////////     CLASS   INDIVIDUAL     ////////
////////////////////////////////////////////

class Individual{
public:
    int genes[395];
    int fitness;

    // initialise individual
    void init(){
        srand(time(0));
        random_shuffle(&numbers[0],&numbers[N]);
        for (int i=0;i<N;i++){
            genes[i] = numbers[i];
        }
    }

    // print path
    void printGenes(){
        for (int i=0;i<N;i++){
            cout << genes[i]+1 << " " ;
        }
    }

    // compute length of the path
    void getFitness(){
        fitness=0;
        for (int i=0;i<N-1;i++){
            fitness += dist[genes[i]][genes[i+1]];
        }
        fitness += dist[genes[N-1]][genes[0]]; // this is a loop !
    }

    // mutate individual
    void mutate(){
        int nbOfMutation = rand() % nbMaxofMutation;
        for (int mut=0;mut<nbOfMutation;mut++){
            if (((double) rand() / (RAND_MAX)) < 0.5f){
            // first possible mutation : swap 2 cities
                int ind1 = rand() % N;
                int ind2 = rand() % N;
                mySwap( &genes[ind1],&genes[ind2]);
            }
            else {
            // second possible mutation : move 1 city
                int ind1 = rand() % N;
                int ind2 = rand() % N;
                if (ind2>ind1){
                    for (int i=ind1;i<ind2-1;i++){
                        mySwap( &genes[i],&genes[i+1]);
                    }
                }
                else{
                    for (int i=ind2;i<ind1-1;i++){
                        mySwap( &genes[i],&genes[i+1]);
                    }
                }

            }
        }
    }

    // cross individual with another one
    void crossover(Individual mate){
        // this is a 2-points crossover

        // pick 2 points
        int ind1 = rand() % N;
        int ind2 = rand() % N;
        if (ind2<ind1){
            mySwap(&ind1,&ind2);
        }

        // take numbers from 0 to N-1 to check what's missing
        int myNumbers[N];
        for (int i=0;i<N;i++){
            myNumbers[i]=i;
        }

        // create new genotype
        int newGenes[N];

        // fill it first with ind2-ind1 first genes
        int indNew=0;
        for (int i=ind1;i<=ind2;i++){
            myNumbers[mate.genes[i]] = -1;
            newGenes[indNew] = mate.genes[i];
            indNew++;
        }

        // get index of mate.genes[ind2] in genes
        // restart from this gene
        int indRestart = 0;
        while (genes[indRestart] != mate.genes[ind2]){
            indRestart++;
        }
        if (indRestart == N-1){
            indRestart = 0;
        }
        else{
            indRestart++;
        }

        // restart from the gene after this index
        for (int i=indRestart;i<N;i++){
            if (myNumbers[genes[i]] != -1){
                newGenes[indNew] = genes[i];
                indNew++;
                myNumbers[genes[i]] = -1;
            }
        }
        for (int i=0;i<indRestart;i++){
            if (myNumbers[genes[i]] != -1){
                newGenes[indNew] = genes[i];
                indNew++;
            }
        }

        // copy the gene sequence
        for (int i=0;i<N;i++){
            genes[i] = newGenes[i];
        }
    }

};

////////////////////////////////////////////
////////     CLASS   POPULATION     ////////
////////////////////////////////////////////

class Population{
public:
    Individual pop[100];
    Individual bestIndividual;
    int bestFitness;

    // initialise population
    void init(){
        bestFitness = 1000000;
        for (int i=0;i<popSize;i++){
            pop[i].init();
        }
        bestIndividual = pop[0];
    }

    // compute fitness of each individual
    void getAllFitness(){
        for (int i=0;i<popSize;i++){
            pop[i].getFitness();
        }
    }

    // get best individual and best fitness of the population
    void getBestFitness(){
        for (int i=0; i<popSize;i++){
            if (pop[i].fitness < bestFitness){
                bestFitness = pop[i].fitness;
                bestIndividual = pop[i];
            }
        }
    }

    // mutate each individual of the population
    void mutateAll(){
        for (int i=0; i<popSize;i++){
            Individual mutant;
            mutant = pop[i];
            mutant.mutate();
            mutant.getFitness();
            // elitist strategy : keep mutant iff it is best than the original
            if (mutant.fitness<pop[i].fitness){
                pop[i]=mutant;
            }
        }
    }

    // cross every individual with the winner of the tournament
    void crossoverAll(){
        for (int i=0; i<popSize;i++){
            Individual child;
            child = pop[i];
            // this is a 3-individuals tournament
            int ind1 = rand() % popSize;
            int ind2 = rand() % popSize;
            int ind3 = rand() % popSize;
            if (pop[ind1].fitness>pop[ind2].fitness){
                if (pop[ind1].fitness>pop[ind3].fitness){
                    child.crossover(pop[ind1]);
                }
                else{
                    child.crossover(pop[ind3]);
                }
            }
            else{
                if (pop[ind2].fitness>pop[ind3].fitness){
                    child.crossover(pop[ind2]);
                }
                else{
                    child.crossover(pop[ind3]);
                }
            }
            // elitist strategy : keep child iff it is best than his parent
            child.getFitness();
            if (child.fitness<pop[i].fitness){
                pop[i]=child;
            }
        }
    }
};

/////////////////////////////////////////
////////     MAIN   FUNCTION     ////////
/////////////////////////////////////////

int main() {
    // read input
    read();
    printInfo();
    for (int i=0;i<N;i++){
        numbers[i] = i;
    }

    clock_t begin = clock();

    // initialise population
    Population population;
    population.init();

    // Let's evolve !
    int nbOfGeneration = 50000;
	for (int gen=1;gen<=nbOfGeneration;gen++){
        population.mutateAll();
        population.getAllFitness();
        population.crossoverAll();
        if (gen % 1000 == 0){
            updateHyperParameters(gen);
        }
        if (gen % 10000 == 0){
            population.getBestFitness();
            cout << "********************"<< endl;
            cout << "generation " << gen << "    : " << population.bestFitness << endl;
            cout << "max nb of mutations : " << nbMaxofMutation<<endl;
            cout << "current / optimal   : " << double(population.bestFitness)/A_best<<endl;
            cout << "total time          : " << double(clock() - begin) / CLOCKS_PER_SEC << " seconds" << endl;
            //population.bestIndividual.printGenes();
        }
	}
    return 0;
}
