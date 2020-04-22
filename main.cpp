/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 */
/*
 * Author: Massimo Torquati <torquati@di.unipi.it>
 * Date:   October 2014
 */
/* This program prints to the STDOUT all prime numbers
 * in the range (n1,n2) where n1 and n2 are command line
 * parameters.
 *
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ff/utils.hpp>
#include "master_worker.hpp"


using ull = unsigned long long;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "use: " << argv[0] << " number1 number2 [print=off|on] nworkers\n";
        return -1;
    }
    ull n1 = std::stoll(argv[1]);
    ull n2 = std::stoll(argv[2]);
    ull nWorkers = stoll(argv[3]);
    //bool print_primes = false;
    //if (argc >= 4) print_primes = (std::string(argv[3]) == "on");

    auto* mwp = new MasterWorkerPrimes(n1, n2, nWorkers);
    mwp->run();
}
