//
// Created by dbara on 22/04/20.
//

#ifndef FINDING_PRIMES_MASTERWORKER_HPP
#define FINDING_PRIMES_MASTERWORKER_HPP

#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace ff;
using namespace std;
using ull = unsigned long long;


static bool is_prime(ull n) {
    if (n <= 3) return n > 1; // 1 is not prime !

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (ull i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}


struct firstThirdStage : ff_node_t<vector<ull>> {
    explicit firstThirdStage(ull startingNumber, ull endingNumber, ull workers)
            : starting(startingNumber), ending(endingNumber), workers(workers) {
        primes.reserve((size_t) (ending - starting) / log(starting));
        ull total = ending - starting + 1;
        rangeSize = total / (workers * 2);
    }

    ull dispatchJobs(){
        ull r;
        for (r = starting; r + rangeSize - 1 <= ending; r += rangeSize) {
            //cout << "sending " << r << "~" << r + rangeSize - 1 << " to workers queue" << endl;
            ff_send_out(new pair<ull, ull>(r, r + rangeSize - 1));
            dispatchedCount += rangeSize;
        }
        return r;
    }

    void computeRemainingFrom(ull r){
        for (; r <= ending; r++) {
            //cout << "Master computing " << r << endl;
            if (is_prime(r)) {
                primes.insert(primes.begin(), r);
            }
        }
    }

    vector<ull> *svc(vector<ull> *task) override {
        if (task == nullptr) {
            //do dispatch
            ull r = dispatchJobs();
            computeRemainingFrom(r);
            return GO_ON;
        }
        vector<ull> *receivedData = task;
        if (!receivedData->empty()) {
            primes.insert(primes.end(), receivedData->begin(), receivedData->end());
        }
        delete task;
        //terminate everything when all data is collected
        nTasks += rangeSize;
        if (nTasks == dispatchedCount) return EOS;
        return GO_ON;
    }

    void svc_end() override {
        sort(primes.begin(), primes.end());
        //cout << "Found: " << primes.size() << " primes" << endl;
    }

    std::vector<ull> primes;
    const ull starting;
    const ull ending;
    const ull workers;
    ull rangeSize;
    ull nTasks = 0;
    ull dispatchedCount = 0;
};

struct secondStage : ff_node_t<pair<ull, ull>, vector<ull>> {
    std::vector<ull> *svc(pair<ull, ull> *task) override {
        auto *primes = new vector<ull>();
        ull num;
        while ((num = task->first++) <= task->second) {
            if (is_prime(num)) {
                primes->insert(primes->begin(), num);
            }
        }
        delete task;
        return primes;
    }
};

class MasterWorkerPrimes {
private:
    ull starting;
    ull ending;
    ull nWorkers;

public:
    MasterWorkerPrimes(ull startingNumber, ull endingNumber, ull workers) : starting(startingNumber),
                                                                            ending(endingNumber), nWorkers(workers) {
    }

    int run() {
        firstThirdStage firstThird(starting, ending, nWorkers);
        std::vector<std::unique_ptr<ff_node> > W;
        for (size_t i = 0; i < nWorkers; ++i) {
            W.push_back(make_unique<secondStage>());
        }

        ff_Farm<> farm(std::move(W), firstThird);
        farm.remove_collector();
        farm.wrap_around();

        ffTime(START_TIME);

        //printing heading
        //cout << "workers time" << endl;

        if (farm.run_and_wait_end() < 0) {
            error("running master-worker farm");
            return -1;
        }
        ffTime(STOP_TIME);
        cout << nWorkers << " " << ffTime(GET_TIME) << endl;
        return 0;
    }
};


#endif //FINDING_PRIMES_MASTERWORKER_HPP
