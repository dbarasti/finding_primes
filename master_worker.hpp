//
// Created by dbara on 22/04/20.
//

#ifndef FINDING_PRIMES_MASTER_WORKER_HPP
#define FINDING_PRIMES_MASTER_WORKER_HPP

#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <iostream>
#include <vector>

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


struct firstThirdStage : ff_node_t<std::vector<ull>> {
    explicit firstThirdStage(ull startingNumber, ull endingNumber) : starting(startingNumber), ending(endingNumber) {}

    int svc_init() override {
        primes.reserve((size_t) (ending - starting) / log(starting));
        return 0;
    };

    vector<ull> *svc(vector<ull> *task) override {
        if (task == nullptr) {
            //do dispatch. this is just a test code
            ull prime;
            while ((prime = starting++) <= ending) {
                cout << "sending " << prime << " to workers queue" << endl;
                auto* send = new vector<ull>();
                send->insert(send->begin(), prime);
                ff_send_out(send);
                dispatchedCount++;
            }
            return GO_ON;
        }
        vector<ull> &receivedData = *task; // is this efficient with a vector??
        if (!receivedData.empty()){
            std::cout << "collector received " << "data back" << "\n";
            primes.insert(primes.end(), receivedData.begin(), receivedData.end());
        }
        //terminate everything when all data is collected
        if (++nTasks == dispatchedCount) return EOS;
        return GO_ON;
    }

    void svc_end() override {
        sort(primes.begin(), primes.end());
        cout << "Primes found: " << endl;
        for (auto &&prime: primes) {
            cout << prime << ", ";
        }
    }

    std::vector<ull> primes;
    ull starting;
    const ull ending;
    ull nTasks = 0;
    ull dispatchedCount = 0;
};

struct secondStage : ff_node_t<vector<ull>> {
    std::vector<ull> *svc(vector<ull> *task) override {
        auto *result = new vector<ull>();
        for (auto &&num: *task) {
            if (is_prime(num)) {
                result->insert(result->begin(), num);
            }
        }
        return result;
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
        firstThirdStage firstThird(starting, ending);
        std::vector<std::unique_ptr<ff_node> > W;
        for (size_t i = 0; i < nWorkers; ++i){
            W.push_back(make_unique<secondStage>());
        }

        ff_Farm<float> farm(std::move(W), firstThird);
        farm.remove_collector(); // needed because the collector is present by default in the ff_Farm
        farm.wrap_around();   // this call creates feedbacks from Workers to the Emitter
        //farm.set_scheduling_ondemand(); // optional

        ffTime(START_TIME);
        if (farm.run_and_wait_end() < 0) {
            error("running farm");
            return -1;
        }
        ffTime(STOP_TIME);
        std::cout << "\nTime: " << ffTime(GET_TIME) << "ms\n";
        return 0;
    }
};


#endif //FINDING_PRIMES_MASTER_WORKER_HPP
