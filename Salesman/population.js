function Population(){
    this.population = [];
    this.elite = [];
    this.bestIndividual; // = new individual(dimension,mutationRate);
    this.avg;

    // Initialze population
    for (var i=0; i<popMax;i++){
         this.population[i] = new individual();
    }

    // Update fitness of every individual
    this.updateAllFitness = function(){
        this.avg = 0;
        for (var i=0;i<popMax;i++){
            this.population[i].updateFitness();
            this.avg += this.population[i].fitness/popMax;
        }
    }

    // Get the best individual
    this.getBest = function(){
        var min=Infinity;
        var indBest=0;
        for (var i=0;i<popMax;i++){
            if (this.population[i].fitness < min){
                min = this.population[i].fitness;
                indBest = i;
            }
        }
        this.bestIndividual = this.population[indBest];
    }

    // Sort fitness
    this.updateElite = function(){
        var fitnessArray = [];
        for (var i=0;i<popMax;i++){
            fitnessArray.push(this.population[i].fitness);
        }
        eliteIndex = sortWithIndeces(fitnessArray).sortIndices.splice(0,eliteN);
    }

    /*
    this.changeWorst = function(){
        var min=0;
        var indWorst=0;
        for (var i=0;i<popMax;i++){
            if (this.population[i].fitness > min){
                min = this.population[i].fitness;
                indWorst = i;
            }
        }
        this.population[indWorst] = new individual();
        this.population[indWorst].updateFitness();
    }
    */

    // Mutate every individual
    this.mutateEliteAll = function(){
        // Let's build a new population
        var newPopulation = [];
        // Frist we keep our elite, the eliteN fittest elements
        for (var i=0;i<eliteN;i++){
            newPopulation.push(this.population[eliteIndex[i]]);
        }
        // Then let's see what candidates we have for the next 
        // popMax-eliteN remaining individuals for the next generation
        var candidates = [];
        var candidatesFitness = [];

        // We mutate every individual of the previous 
        // generations and this give us popMax new candidates
        var mutant;
        for (var i=0;i<popMax;i++){
            mutant = this.population[i].mutate()
            candidates.push(mutant);
            candidatesFitness.push(mutant.fitness);
        }
        // we also add the non-elite of the previous generation
        for (var i=0;i<popMax;i++){
            if (eliteIndex.includes(i) === false){
                candidates.push(this.population[i]);
                candidatesFitness.push(this.population[i]);
            }
        }

        // To sum up
        // pop = popMax individuals
        // mutants = popMax new
        // nextpop = N best previous one + best( (popMax-N)previous + popMax mutants)
        // We keep the top popMax - N
        var candidatesBestIndex = sortWithIndeces(candidatesFitness).sortIndices.splice(0,popMax-eliteN);
        for (var i=0;i<candidatesBestIndex.length;i++){
            newPopulation.push(candidates[candidatesBestIndex[i]]);
        }
        this.population = newPopulation;
    }

    this.mutateAll = function(){
        // To sum up
        // pop = popMax individuals
        // mutants = popMax new
        // nextpop = N best previous one + best( (popMax-N)previous + popMax mutants)
        // We keep the top popMax - N
        var mutant;
        for (var i=0;i<popMax;i++){
            mutant = this.population[i].mutate();
            if (mutant.fitness < this.population[i].fitness){
                this.population[i] = mutant;
            }
        }
    }

    // Perform crossover
    this.crossoverAll = function(){
        for (var i=0;i<popMax;i++){
        
        // Turnament selection with 3 individuals
        var mate = new individual(); ;
        let i1 = Math.floor(popMax*Math.random());
        let i2 = Math.floor(popMax*Math.random());
        let i3 = Math.floor(popMax*Math.random());
        if (this.population[i1].fitness > this.population[i2].fitness){
            if (this.population[i1].fitness > this.population[i3].fitness){
                mate = this.population[i1];
            }
            else{
                mate = this.population[i3];
            }
        }
        else {
            if (this.population[i2].fitness>this.population[i3].fitness){
                mate = this.population[i2];
            }
            else {
                mate = this.population[i3];
            }
        }          
        this.population[i].crossover(mate);
        }
    }
}