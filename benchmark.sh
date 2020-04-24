#!/bin/sh
clear
echo "Running benchmark "

#for ((w = 1; w <= 250; w=w+1)); do
#    echo $w;
# done
# buildFolder="cmake-build-release-local"
MW_ExecutablePath="builds/finding_primes_MW"
PF_ExecutablePath="builds/finding_primes_PF"
filenameRoot="plotting-and-data/data/benchmark"
maxThreads=8

for dim in 1000000 10000000; do #number of elements
  echo ${dim}
  for exec in ${MW_ExecutablePath} ${PF_ExecutablePath}; do # for each version (MasterWorker and ParallelFor)
    if [ "${exec}" = "${MW_ExecutablePath}"  ]; then echo \"MasterWorker\">> ${filenameRoot}-${dim}.txt; fi #print the headings
    if [ "${exec}" = "${PF_ExecutablePath}" ]; then echo \"ParallelFor\">> ${filenameRoot}-${dim}.txt; fi
    : $((w = 1))
    while [ $((w <= maxThreads)) -ne 0 ]; do #for each thread configuration
      echo $w
      ./${exec} 1 ${dim} ${w} >>${filenameRoot}-${dim}.txt
      : $((w = w + 1))
      sleep 1s
    done
    echo >>${filenameRoot}-${dim}.txt #separate the two datasets in the file
    echo >>${filenameRoot}-${dim}.txt
  done
  echo
done

echo "Benchmark ended"
