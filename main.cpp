//
// Created by dbara on 22/04/20.
//

#include <iostream>
#include <vector>
#include "MasterWorker.hpp"
#include "ParallelForExec.h"


using ull = unsigned long long;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "use: " << argv[0] << " number1 number2 nWorkers\n";
        return -1;
    }
    ull n1 = std::stoll(argv[1]);
    ull n2 = std::stoll(argv[2]);
    ull nWorkers = stoll(argv[3]);

    auto pfp = new ParallelForExec(n1, n2, nWorkers);
    pfp->run();

    //formatting for gnuplot
    cout << endl << endl;

    auto *mwp = new MasterWorkerPrimes(n1, n2, nWorkers);
    mwp->run();

    delete pfp;
    delete mwp;


}
