
#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <complex>
#include <numeric>
#include <cstring>
#include <vector>
#include <string>
#include <cstdio>
#include <queue>
#include <cmath>
#include <map>
#include <set>
#include <process.h>
#include <ctime>
#include <stdlib.h>


using namespace std;

#define next			__next
#define sz(a)			int((a).size())
#define FOR(i, a, b)	for (int i(a); i < b; ++i)
#define REP(i, n)		FOR(i, 0, n)
#define CL(a, b)		memset(a, b, sizeof a)
#define pb				push_back

typedef vector <int> vi;

namespace Random {
	unsigned next() {
		static unsigned x = 1987657173, y = 712356789, z = 531288629, w = 138751267;
		unsigned t = x ^ x << 11;
		x = y, y = z, z = w;
		return w = ((t ^ t >> 8) ^ w) ^ w >> 19;
	}
	template <class I> void shuffle(I begin, I end) {
		for (I i = begin; i != end; ++i)
			swap(*i, begin[next() % ((i - begin) + 1)]);
	}
};

#define N	99
#define M	9
#define S	(N * 100)

class bitset {
	unsigned d[(S >> 5) + 1];
	int n;
public:
	void set(int i) {
		int m = i >> 5;
		for (; n <= m; d[n++] = 0);
		d[m] |= 1 << i;
	}
	void operator = (const bitset &b) {
		n = b.n;
		memcpy(d, b.d, n << 2);
	}
	bool operator [] (int i) const {
		return (i >> 5) < n && (d[i >> 5] & 1 << i) != 0;
	}
    void clear() {
        for (int i = 0; i <= (S >> 5); ++i) {
            d[i] = 0;
        }
        n = 0;
    }
	void shiftor(const bitset &b, int sh, int m) {
		for (m >>= 5; n <= m; d[n++] = 0);
		unsigned *s = d + (sh >> 5), *e = d + n;
		if (s >= e) return;
		const unsigned *i = b.d;
		if (sh &= 31) {
			unsigned c = 0;
			int rh = 32 - sh;
			e = min(e, s + b.n);
			for (; (e - s) & 3; ++s, c = i++[0])
				s[0] |= (i[0] << sh) | (c >> rh);
			for (; s != e; ) {
				s[0] |= (i[0] << sh) | (c >> rh);
				s[1] |= (i[1] << sh) | (i[0] >> rh);
				s[2] |= (i[2] << sh) | (i[1] >> rh);
				s[3] |= (i[3] << sh) | (i[2] >> rh);
				c = i[3];
				s += 4;
				i += 4;
			}
			s[0] |= c >> rh;
		} else {
			for (; s != e; ++s, ++i)
				s[0] |= i[0];
		}
	}
};

bitset f[N];

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int a[N], p[N], row[M], q[M], v[N], *ve = v, n, m;
/*
"q" defines the current permutation of havens (in which order to try)
"p" defines for each ship the haven it belongs to (-1 means no haven so far)
"v" defines the current permutation of ships
"ve" points to the first assigned ship (given all assigned ships are moved towards the end of "v")
*/
int first[M], next[N];

bool fit(int k) {
	for (int h = row[k], *i = v, s = 0; i != ve; ++i) {
        f[(i - v) + 1] = f[(i - v)];
		s = min(s + a[*i], h);
		f[(i - v) + 1].shiftor(f[(i - v)], a[*i], s);
		if (f[(i - v) + 1][h]) {
			for (int *j = i; h; --j){
				if (!f[j - v][h]) {
					p[*j] = k;
					h -= a[*j];
				}
			}
			for (; i >= v; --i)
				if (~p[*i]) {
					next[*i] = first[k];
					first[k] = *i;
					*i = *--ve;
				}
			return true;
		}
	}
	return false;
}

void cancel(int k) {
	for (int i = first[k]; ~i; i = next[i])
		p[*ve++ = i] = -1;
	first[k] = -1;
}

bool solve() {
	if (m == 1) return true;
	--m;
	REP (i, m)
		if (fit(q[i])) {
			for (int j = i; j < m; ++j) swap(q[j], q[j + 1]);
			if (solve()) return ++m, true;
			for (int j = m; j > i; --j) swap(q[j - 1], q[j]);
			cancel(q[i]);
		}
	return ++m, false;
}

void mySwap(int *i, int *j) {
   int t = *i;
   *i = *j;
   *j = t;
}

int sumLength(int arr[9]){
    int Sum = 0;
    for (int i=0;i<9;i++){
        Sum+=arr[i];
    }
    return Sum;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define FAIL exit(0)

class Individual {
public:
    int nShips;
    int mRows;
    int myShips[99];
    int myRowsLength[9];
    int myRowsTotalLength[9];
    int myRows[9][99];
    double fitness;

    void init(){
        nShips = 30 + rand() %70;
        mRows = 7 + rand() % 2;

        // initialise ships
        for (int i=0;i<99;i++){
            if (i<nShips){
                myShips[i]= 1 + rand()%99 ;
            }
            else {
                myShips[i]=0;
            }
        }
        // initialise rows
        for (int i=0;i<9;i++){
            myRowsLength[i] = 0;
            myRowsTotalLength[i] = 0;
            for (int j=0;j<99;j++){
                myRows[i][j]=0;
            }
        }
        // fill rows
        for (int i=0;i<nShips;i++){
            int ind = rand() % mRows;
            myRowsTotalLength[ind] += myShips[i];
            myRows[ind][myRowsLength[ind]] = myShips[i];
            myRowsLength[ind]++;
        }
    }

    void printInfo(){
        cout << "nShips : " << nShips << endl;
        cout << "mRows : " << mRows << endl;
        for (int i=0;i<9;i++){
            cout << "row length :" << myRowsLength[i] << endl;
            cout << "row total length :" << myRowsTotalLength[i] << endl;
            for (int j=0;j<99;j++){
                cout << myRows[i][j] << " ";
            }
            cout <<endl;
        }
    }

    void crossover(Individual mate){
        /*
        We will do a two point crossovers on the rows
        We will take for example the second & third rows
        and then add rows from our individual.
        */

        int point1 = rand() % mate.mRows;
        int point2 = rand() % mate.mRows;
        if (point2 < point1){
            mySwap(&point1,&point2);
        }
        for (int i=point1; i<point2; i++){
            for (int j=0; j<99; j++){
                myRows[i][j] = mate.myRows[i][j];
            }
            myRowsLength[i] = mate.myRowsLength[i];
            myRowsTotalLength[i] = mate.myRowsTotalLength[i];
        }
        mRows = 0;
        for (int i=0;i<9;i++){
            if (myRowsLength[i] > 0){
                mRows++;
            }
        }

        // we have to check that the number of ships is still less than 99
        while (sumLength(myRowsLength)>99){
            mRows--;
            for (int j=0;j<99;j++){
                myRows[mRows][j]=0;
            }
            myRowsLength[mRows]=0;
            myRowsTotalLength[mRows]=0;
        };

        nShips = sumLength(myRowsLength);

        mRows = 0;
        for (int i=0;i<9;i++){
            if (myRowsLength[i] > 0){
                mRows++;
            }
        }
    }

    void mutate(){
        int nbOfMutations = rand() % 5;

        for (int i=0; i<nbOfMutations;i++){
            // different mutations are tried here

            // swap 2 ships from different rows
            if (((double) rand() / (RAND_MAX)) < 0.3){
                int whichRow1 = rand() % mRows;
                int whichRow2 = rand() % mRows;

                int indexRow1 = rand() % myRowsLength[whichRow1];
                int indexRow2 = rand() % myRowsLength[whichRow2];

                mySwap( &myRows[whichRow1][indexRow1] , &myRows[whichRow2][indexRow2] );
                myRowsTotalLength[whichRow1] += myRows[whichRow1][indexRow1] - myRows[whichRow2][indexRow2];
                myRowsTotalLength[whichRow2] += myRows[whichRow2][indexRow2] - myRows[whichRow1][indexRow1];
            }

            // erase a ship and change it in a new one
            if (((double) rand() / (RAND_MAX)) < 0.3){
                int whichRow = rand() % mRows;
                int indexRow = rand() % myRowsLength[whichRow];
                int newShip = 1 + rand() % 99;
                myRowsTotalLength[whichRow] += newShip - myRows[whichRow][indexRow];
                myRows[whichRow][indexRow] = newShip;
            }

            // add a ship
            if (((double) rand() / (RAND_MAX)) < 0.3){
                if (nShips<99){
                    int whichRow = rand() % mRows;
                    int newShip = 1 + rand() % 99;
                    myRowsLength[whichRow] += 1;
                    myRowsTotalLength[whichRow] += newShip;
                    myRows[whichRow][myRowsLength[whichRow]-1] = newShip;
                    nShips +=1;
                }
            }
        }
    }

    void getFitness(){
        // initialize all variables for solving
        n = nShips;
        m = mRows;
        int ind =0;
        for (int i=0;i<9;i++){
            for (int j=0;j<99;j++){
                if (myRows[i][j] >0){
                    myShips[ind] = myRows[i][j];
                    ind++;
                }
            }
        }
        for (int i=0;i<n;i++){
            a[i] = myShips[i];
            p[i] = -1,*ve++ = i;
        }
        for (int i=0;i<m;i++){
            row[i] = myRowsTotalLength[i];
            q[i] = i;
        }
        // solving
        clock_t begin = clock();
        while (true) {
            Random::shuffle(a, a + n);
            Random::shuffle(q, q + m);
            if (solve()) break;
        }
        clock_t end = clock();
        fitness = double(end - begin) / CLOCKS_PER_SEC; // save fitness
        // reinitialize everything
        ve = v;
        for (int i=0; i<N;i++) {
            f[i].clear();
        }
        f[0].set(0);
        CL(first, -1);
    }
};

class Population{
public:
    int popSize;
    Individual pop[100];
    double bestFitness;
    Individual bestIndividual;

    void init(){
        popSize = 100;
        for (int i=0; i<popSize;i++){
            pop[i].init();
        }
        bestIndividual = pop[0];
        bestFitness = 0.0;
    }

    void getAllFitness(){
        for (int i=0; i<popSize;i++){
            pop[i].getFitness();
        }
    }

    void getBestFitness(){
        for (int i=0; i<popSize;i++){
            if (pop[i].fitness > bestFitness){
                bestFitness = pop[i].fitness;
                bestIndividual = pop[i];
            }
        }
    }

    void mutateAll(){
        for (int i=0; i<popSize;i++){
            Individual mutant;
            mutant = pop[i];
            mutant.mutate();
            mutant.getFitness();
            if (mutant.fitness>pop[i].fitness){
                //cout <<"nice mutation" << endl;
                pop[i]=mutant;
            }
        }
    }

    void crossoverAll(){
        for (int i=0; i<popSize;i++){
            Individual child;
            child = pop[i];
            // turnament strategy with 2 individuals
            int ind1 = rand() % popSize;
            int ind2 = rand() % popSize;
            if (pop[ind1].fitness > pop[ind2].fitness){
                child.crossover(pop[ind1]);
            }
            else{
                child.crossover(pop[ind2]);
            }
            if (child.fitness>pop[i].fitness){
                cout <<"nice crossover" << endl;
                pop[i]=child;
            }
        }
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int main() {
	f[0].set(0);
	CL(first, -1);

	// Let's evolve !
    int nbOfGeneration = 5000;
    Population population;
    population.init();

	for (int gen=1;gen<=nbOfGeneration;gen++){
        population.mutateAll();
        //population.pop[20].printInfo();
        population.getAllFitness();
        population.getBestFitness();
        population.crossoverAll();

        if (gen%200==0){
            cout <<"Generation" << gen << " : " << population.bestFitness << endl;
        }

	}
	population.bestIndividual.printInfo();
	cout <<"Generation " << nbOfGeneration << " : " << population.bestFitness << endl;
	//}
    return 0;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


        /*
        // solving
        clock_t begin = clock();
        while (true) {
            Random::shuffle(a, a + n);
            Random::shuffle(q, q + m);
            if (solve()) break;
        }
        clock_t end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;


        individual.setFitness(elapsed_secs);
        cout << individual.fitness << endl;


        if (elapsed_secs>bestTime){
            bestTime = elapsed_secs;
            cout << "New score : " << elapsed_secs <<endl;

            fit(*q);
            REP (i, m) {
                vi ships;
                for (int j = first[i]; ~j; j = next[j])
                    ships.pb(a[j]);
                printf("%d\n", sz(ships));
                REP (j, sz(ships)) {
                    if (j) putchar(' ');
                    printf("%d", ships[j]);
                }
                puts("");
            }
        }

        // reinitialize everything
        ve = v;
        for (int i=0; i<N;i++) {
            f[i].clear();
        }
        f[0].set(0);
        CL(first, -1);*/

	/*
	// print solution
	cout << "TIME ELAPSED : " << elapsed_secs;
        fit(*q);
        REP (i, m) {
            vi ships;
            for (int j = first[i]; ~j; j = next[j])
                ships.pb(a[j]);
            printf("%d\n", sz(ships));
            REP (j, sz(ships)) {
                if (j) putchar(' ');
                printf("%d", ships[j]);
            }
            puts("");
        }
    */
	//system("pause");




/*

template<typename T> bool readOrFail(T &target, char const *message) {
    if (!(cin >> target)) {
        printf("PE %s\n", message);
        FAIL;
    }
    return 0;
}

void read() {
    readOrFail(n, "Could not read N"); //read number of ships
    readOrFail(m, "Could not read M"); //read number of rows
	if (m < 2 || m > 9) {
	    printf("PE The number of havens is %d, which is not in [2;9]\n", m);
	    FAIL;
	}
	int ship = 0;
    REP (i, m) {
    // for each row
        int count, size = 0;
        readOrFail(count, "Could not read the number of ships in a haven");
        // get number of ships in this row
        if (count < 1 || count > 100) {
    	    printf("PE The number of ships in haven #%d is %d, which is not in [1;99]\n", i + 1, count);
	        FAIL;
        }
        REP(j, count) {
        // for each ship in this row
            if (ship == 99) {
                printf("PE There are more than 99 ships\n");
                FAIL;
            }
            readOrFail(a[ship], "Could not read the ship's length");
            // store this ship length
            if (a[ship] < 1 || a[ship] > 100) {
                printf("PE Ship #%d in haven ain#%d has length %d, which is not in [1;100]\n", j + 1, i + 1, a[ship]);
                FAIL;
            }
            p[ship] = -1, *ve++ = ship; // remind that this ship exists //store index of ships ?
            size += a[ship]; // get total size of the row
            ++ship; // next ship
        }
        row[i] = size; // store the total size
        q[i] = i; // store the number of the row
    }
}
*/


