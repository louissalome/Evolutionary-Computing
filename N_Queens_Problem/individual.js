function individual(dimension,mutationRate){
    this.genes = [];
    this.fitness;

    var num = [];
    var rd;

    for (var i=0;i<dimension;i++){
        num.push(i);
    }

    for (var i=1;i<=dimension;i++){
        rd = Math.floor(Math.random()*num.length);
        this.genes.push(num[rd]);
        num.splice(rd,1);
    }

    this.updateFitness = function(){
        this.fitness = 0;
        for (var i=0;i<dimension;i++){
            for (var j=0;j<i;j++){
                if (i+this.genes[i] === j+this.genes[j]){
                    this.fitness++;
                }
                if (i-this.genes[i] === j-this.genes[j]){
                    this.fitness++;
                }
            }
        }
    }

    this.mutate = function(mutationRate){
        if (Math.random()<mutationRate){
            var pos1 = Math.floor(Math.random()*this.genes.length);
            var pos2 = Math.floor(Math.random()*this.genes.length);
            var tmp = this.genes[pos1];
            this.genes[pos1] = this.genes[pos2];
            this.genes[pos2] = tmp;
        }
    }

    this.crossover = function(mate){
        // Select a part of the genes of the mate
        var pos1 = Math.floor(Math.random()*this.genes.length);
        var pos2 = Math.floor(Math.random()*this.genes.length);
        var child = new individual(dimension,mutationRate);
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

        // Add the remaining part from this.genes
        for (var i=0;i<dimension;i++){
            var tmp = this.genes[i];
            if (num.includes(tmp)){
                child.genes.push(tmp);
            }
        }
        this.genes = child.genes;
    }
}