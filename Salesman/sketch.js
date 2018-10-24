// size settings
let wCanvas = 1200;
let hCanvas = Math.floor(wCanvas/2);
let scl = 5;
let h = Math.floor(hCanvas/scl);
let w = Math.floor(wCanvas/scl);


// variables for genetic algorithm
// these are the default values you can
// modify on the webpage
var dimension = graph.length;
var population;
var popMax = 50; //popSize
var mutationRate;
var generation=0;
var generationBestEver=0; //the one we plit on the right
var avgCurrent; // current average fitness
var noChange = 0;
var maxNoChange = 1000; // stop criterion
var rangeMutation;
var eliteN = Math.floor(0.3*popMax);
var eliteIndex;

/*
// buttons for settings
var inputDimension, inputPopMax, inputMutationRate;
var button;
*/
// variables for displaying the result
var bestPathEver = [];
var bestFitnessEver;

var dist = [];

// Compute distance between every node
function distances(graph){
    var dist = [];
    for (var i=0;i<dimension;i++){
        var line = [];
        for (var j=0;j<dimension;j++){
            line.push( Math.sqrt(
                            Math.pow(  graph[i][1]-graph[j][1] ,2 )
                          + Math.pow(  graph[i][2]-graph[j][2] ,2 )
                        ));
            }
        dist.push(line);
    }
    return dist;
}

function sortWithIndeces(toSort) {
    for (var i = 0; i < toSort.length; i++) {
      toSort[i] = [toSort[i], i];
    }
    toSort.sort(function(left, right) {
      return left[0] < right[0] ? -1 : 1;
    });
    toSort.sortIndices = [];
    for (var j = 0; j < toSort.length; j++) {
      toSort.sortIndices.push(toSort[j][1]);
      toSort[j] = toSort[j][0];
    }
    return toSort;
  }

// Main SETUP
function setup() {
    // Workspace
    var canvas = createCanvas(wCanvas,hCanvas);
    canvas.parent('canvas-holder'); 

    dist = distances(graph);
    scale(scl);

    population = new Population();
    population.updateAllFitness();
    population.updateElite();
    population.getBest();
    bestFitnessEver = population.bestIndividual.fitness;
    bestPathEver = population.bestIndividual.genes;
}


function draw() {
    //noLoop();
    
    noStroke(); 
    background(210);
    stroke(0);
    strokeWeight(0.2);
    line(Math.floor(w/2),0,Math.floor(w/2),h);

    generation++;
    noChange++;
    RangeMutation = Math.floor(2*dimension*Math.exp(-generation/5000))+7;
    mutationRate = 0.2*Math.exp(-generation/5000)+0.02;

    // Crossover
    population.crossoverAll();

    // Mutation Fast version
    population.updateAllFitness();
    
    population.mutateAll();

    /* 
    // Mutation Elite version
    population.updateElite();
    population.mutateEliteAll();
    */

    // Calculate fitness of each individual
    avgCurrent = population.avg;

    // Replace the worst by a new one
    // population.changeWorst();

    // Get the fittest individual
    population.getBest();

    if (population.bestIndividual.fitness<bestFitnessEver){
        bestFitnessEver = population.bestIndividual.fitness;
        bestPathEver = population.bestIndividual.genes;
        generationBestEver = generation;
        noChange = 0;
    }

    if (noChange === maxNoChange){
    //if (generation === 100){
        noLoop();
    }

    // LEFT
    //
    // Draw the best path of this generation
    textSize(8);
    fill(0);
    noStroke();
    textFont('Courier');
    text("Current best path :",5,15);
    textSize(5);
    text("Total distance   : "+population.bestIndividual.fitness,5,95);
    text("Average distance : "+avgCurrent,5,101);
    text("Mutation rate    : "+mutationRate,5,107);
    text("Range of mutation: "+RangeMutation,5,113);
    text("Generation       : "+generation,5,119);
    for (var i=0; i<dimension; i++){
        noStroke();
        fill(220,100,0);
        ellipse(5+graph[i][1],30+graph[i][2],1.6,1.6);
    }

    population.bestIndividual.genes
    strokeWeight(0.3);
    for (var i=1; i<dimension; i++){
        stroke(50);
        line(graph[population.bestIndividual.genes[i-1]][1]+5,
             graph[population.bestIndividual.genes[i-1]][2]+30,
             graph[population.bestIndividual.genes[i]][1]+5,
             graph[population.bestIndividual.genes[i]][[2]]+30);
    }
    line(graph[population.bestIndividual.genes[0]][1]+5,
         graph[population.bestIndividual.genes[0]][2]+30,
         graph[population.bestIndividual.genes[dimension-1]][1]+5,
         graph[population.bestIndividual.genes[dimension-1]][2]+30);
    fill(0);


    // RIGHT
    //
    // Draw the best path ever
    textSize(8);
    fill(0);
    noStroke();
    textFont('Courier');
    text("Best path ever :",Math.floor(w/2)+5,15);
    textSize(5);
    text("Total distance : "+bestFitnessEver,Math.floor(w/2)+5,95);
    text("Generation of the best : "+generationBestEver,Math.floor(w/2)+5,101);
    text("No new best since : "+noChange,Math.floor(w/2)+5,107);
    for (var i=0; i<dimension; i++){
        noStroke();
        fill(220,100,0);
        ellipse(Math.floor(w/2)+5+graph[i][1],30+graph[i][2],1.6,1.6);
    }
    strokeWeight(0.3);
    for (var i=1; i<dimension; i++){
        stroke(50);
        line(graph[bestPathEver[i-1]][1]+5+Math.floor(w/2),
             graph[bestPathEver[i-1]][2]+30,
             graph[bestPathEver[i]][1]+5+Math.floor(w/2),
             graph[bestPathEver[i]][[2]]+30);
    }
    line(graph[bestPathEver[0]][1]+5+Math.floor(w/2),
         graph[bestPathEver[0]][2]+30,
         graph[bestPathEver[dimension-1]][1]+5+Math.floor(w/2),
         graph[bestPathEver[dimension-1]][2]+30);
    fill(0);

    /*
    textSize(25);
    fill(0);
    textFont('Courier');
    text("set dimension",825,120);
    text("set pop size",825,170);
    text("set mutation rate",825,220);

    textSize(25);
    text("Current generation "+generation,825, 400);
    text("Current best fitness "+population.bestIndividual.fitness,825, 500);
    fill(255,0,0);
    text("Best fitness ever "+bestFitnessEver,825, 600);
    fill(0);
        
    // Draw the new best
    for(var i=0;i<dimension;i++){
        fill(100,100,255);
        rect(i*caseWidth,bestPosEver[i]*caseHeight,caseWidth,caseHeight);
    }

    // Draw the queens
    for(var i=0;i<dimension;i++){
        image(queenimg,i*caseWidth,population.bestIndividual.genes[i]*caseHeight,caseWidth,caseHeight);
    }

    if(population.bestIndividual.fitness == 0){
        noLoop();
    }
    */
}