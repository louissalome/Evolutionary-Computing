// imports
let queenimg;

// size settings
let wCanvas = 1200;
let hCanvas = 800;

// variables for genetic algorithm
// these are the default values you can
// modify on the webpage
var dimension = 16;
var population;
var popMax = 100;
var mutationRate = 0.06;
var generation=0;

// variables for displaying the result
var bestPosEver = [];
var bestFitnessEver;

// buttons for settings
var inputDimension, inputPopMax, inputMutationRate;
var button;

// preload image
function preload() {
    queenimg = loadImage('queen.png');
}

function setinputs() {
    dimension = inputDimension.value();
    popMax = inputPopMax.value();
    mutationRate = inputMutationRate.value();
    generation = 0;
    setup();
    draw();
    loop();
}

// Main SETUP
function setup() {
    // Workspace
    var canvas = createCanvas(wCanvas,hCanvas);
    canvas.parent('canvas-holder'); 
    
    // list of buttons
    inputDimension = createInput();
    inputDimension.position(canvas.position().x+hCanvas+220,canvas.position().y+100);
    inputPopMax = createInput();
    inputPopMax.position(canvas.position().x+hCanvas+220,canvas.position().y+150);
    inputMutationRate = createInput();
    inputMutationRate.position(canvas.position().x+hCanvas+220,canvas.position().y+200);

    button = createButton('Launch');
    button.mousePressed(setinputs);
    button.position(canvas.position().x+hCanvas+220,canvas.position().y+250);

    // Initialize population
    population = new Population(dimension,popMax,mutationRate);
    population.updateAllFitness();
    population.getBest();
    bestPosEver = population.bestIndividual.genes;
    bestFitnessEver = population.bestIndividual.fitness;
}


function draw() {
    generation++;

    // Crossover
    population.crossoverAll();

    // Mutation
    population.muatateAll();

    // Calculate fitness of each individual
    population.updateAllFitness();

    // Get the fittest individual
    population.getBest();

    if (population.bestIndividual.fitness<bestFitnessEver){
        bestFitnessEver = population.bestIndividual.fitness;
        bestPosEver = population.bestIndividual.genes;
    }

    noStroke(); 
    background(255);  

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

    // Draw the board
    var caseWidth = height/dimension;
    var caseHeight = caseWidth;
    	for (var x = 0; x < dimension; x++) {
            for (var y = 0; y < dimension; y++) {
                if ((x+y)%2===0){
                    fill(255, 204, 153);
                }
                else{
                    fill(153, 51, 0);
                }
                rect(x*caseWidth,y*caseHeight,caseWidth,caseHeight);
            }
        }
        
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
}