//
// Created by dbara on 24/04/20.
//

#ifndef FINDING_PRIMES_PARALLELFOREXEC_H
#define FINDING_PRIMES_PARALLELFOREXEC_H


#include <cstddef>
#include <ff/parallel_for.hpp>
#include <iostream>
#include <mutex>

using namespace ff;
using namespace std;
using ull = unsigned long long;

class ParallelForExec {
private:
    const ull starting;
    const ull ending;
    const ull nWorkers;

public:
    ParallelForExec(const ull startingNumber, const ull endingNumber, const std::size_t workers)
            : nWorkers(workers), starting(startingNumber), ending(endingNumber) {}

    void run() {
        vector<ull> primes;
        std::mutex primesLock;
        primes.reserve((size_t) (ending - starting) / log(starting));

        ff::ParallelFor pf(nWorkers);

        ff::ffTime(ff::START_TIME);
        //printing heading
        cout << "\"ParallelFor\"" << endl;

        //parallel_for with step length 1 and chunk size 10 (dynamic)
        pf.parallel_for(starting, ending,
                        1,
                        10,
                        [&](ull num) {
                            if (is_prime(num)) {
                                primesLock.lock();
                                primes.push_back(num);
                                primesLock.unlock();
                            }
                        });
        ff::ffTime(ff::STOP_TIME);
        cout << nWorkers << " " << ffTime(GET_TIME) << endl;

    }

    static bool is_prime(unsigned long long n) {
        if (n <= 3) return n > 1;

        if (n % 2 == 0 || n % 3 == 0) return false;

        for (unsigned long long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        }
        return true;
    }
};

#endif //FINDING_PRIMES_PARALLELFOREXEC_H
