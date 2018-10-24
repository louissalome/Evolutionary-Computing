function individual(){
    this.genes = [];
    this.fitness;

    var num = [];
    var rd;

    // Generate a suffle of [0,N-1]
    //
    // Create the [0,N-1] vector
    for (var i=0;i<dimension;i++){
        num.push(i);
    }
    // Taking its elements randomly without repetition
    for (var i=1;i<=dimension;i++){
        rd = Math.floor(Math.random()*num.length);
        this.genes.push(num[rd]);
        num.splice(rd,1);
    }

    // Compute the fitness : the total distance of the path
    this.updateFitness = function(){
        this.fitness = 0;
        for (var i=1;i<dimension;i++){
            // add the euclidean distance
            //console.log(this.genes[i]);
            this.fitness += dist[graph[this.genes[i]][0]][graph[this.genes[i-1]][0]];
        }
        this.fitness += dist[graph[this.genes[dimension-1]][0]][graph[this.genes[0]][0]];
        //console.log(this.fitness);
    }

    // The mutation is just a swap
    //
    // We try for every gene if a swap will occur or not
    // with probability p = mutationRate
    
    /*
    this.mutate = function(mutationRate){
        
        var pos1 = Math.floor(Math.random()*(this.genes.length-1));
        var pos2 = Math.floor(Math.random()*(this.genes.length-1));
        if (dist[graph[this.genes[pos1]][0]][graph[this.genes[pos1+1]][0]]
            + dist[graph[this.genes[pos2]][0]][graph[this.genes[pos2+1]][0]]
                >
                dist[graph[this.genes[pos1]][0]][graph[this.genes[pos2+1]][0]]
                + dist[graph[this.genes[pos2]][0]][graph[this.genes[pos1+1]][0]])
                {
                    var tmp = this.genes[pos1];
                    this.genes[pos2] = this.genes[pos1];
                    this.genes[pos2] = tmp;
                }
    }
    */
    
    
    this.mutate = function(){
        var mutant = new individual();
        mutant.genes = [];
        for (var i=0;i<dimension;i++){
            mutant.genes.push(this.genes[i]);
        }
        
        var newPos=0;
        var indFirst = Math.floor(dimension*Math.random());
                if (Math.random() <6*mutationRate){
                    newPos = (Math.floor(RangeMutation*(Math.random()-0.5))+indFirst+1) % dimension;
                    if (newPos < 0){
                        newPos = dimension-1+newPos;
                    }
                }
                else{
                    newPos = Math.floor(Math.random()*dimension);
                }
                var tmp = mutant.genes[indFirst];
                mutant.genes[indFirst] = mutant.genes[newPos];
                mutant.genes[newPos] = tmp;


        for (var i=0;i<dimension;i++){
            if (Math.random()<mutationRate){
                var newPos=0;
                if (Math.random() <6*mutationRate){
                    newPos = (Math.floor(30*(Math.random()-0.5))+i+1) % dimension;
                    if (newPos < 0){
                        newPos = dimension-1+newPos;
                    }
                }
                else{
                    newPos = Math.floor(Math.random()*dimension);
                }
                var tmp = mutant.genes[i];
                mutant.genes[i] = mutant.genes[newPos];
                mutant.genes[newPos] = tmp;
            }
        }
        mutant.updateFitness();
        
        return mutant;
        /*
        if (mutant.fitness<this.fitness){
            this.genes = mutant.genes;
        }*/
    }
    

    this.crossover = function(mate){
        // This is the two-points crossover
        // Select a part of the genes of the mate
        var pos1 = Math.floor(Math.random()*this.genes.length);
        var pos2 = Math.floor(Math.random()*this.genes.length);
        var child = new individual();
        var index;
        if (pos1 > pos2){
            var tmp = pos1;
            pos1 = pos2;
            pos2 = tmp;
        }

        // Check what numbers are not in this part
        num = [];
        for (var i=0;i<dimension;i++){
            num.push(i);
        }

        child.genes = [];
        // Push this sequence at the beginning of a new child
        for (var i=pos1;i<=pos2;i++){
            index = num.indexOf(mate.genes[i]);
            num.splice(index,1);
            child.genes.push(mate.genes[i]);
        }

        var indEnd = this.genes.indexOf(mate.genes[pos2]);

        // Add the remaining part from this.genes
        //for (var i=dimension-1;i>-1;i--){
        for (var i=indEnd;i<dimension;i++){
            var tmp = this.genes[i];
            if (num.includes(tmp)){
                child.genes.push(tmp);
            }
        }
        for (var i=0;i<indEnd;i++){
            var tmp = this.genes[i];
            if (num.includes(tmp)){
                child.genes.push(tmp);
            }
        }

        // ONLY KEEP THE CHILD
        // IF IT S BETTER THAN
        // THE PARENT
        child.updateFitness();
        if (child.fitness < this.fitness){
            this.genes = child.genes;
        }
    }
}