function Population(dimension,popMax,mutationRate){
    this.population = [];
    this.fitness;
    this.bestIndividual;// = new individual(dimension,mutationRate);

    for (var i=0; i<popMax;i++){
         this.population[i] = new individual(dimension,mutationRate);
    }

    this.updateAllFitness = function(){
        for (var i=0;i<popMax;i++){
            this.population[i].updateFitness();
        }
    }

    this.getBest = function(){
        var min=100;
        var indBest=0;
        for (var i=0;i<popMax;i++){
            if (this.population[i].fitness < min){
                min = this.population[i].fitness;
                indBest = i;
            }
        }
        this.bestIndividual = this.population[indBest];
    }

    this.muatateAll = function(){
        for (var i=0;i<popMax;i++){
            this.population[i].mutate();
        }
    }

    this.crossoverAll = function(){
        var mate = this.bestIndividual;
        for (var i=0;i<popMax;i++){
            this.population[i].crossover(mate);
        }
    }
}