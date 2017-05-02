#!/usr/bin/env bash
pathf=~/CLionProjects/parallel_programming/pg_research/matrix.txt
simpleResults=~/CLionProjects/parallel_programming/pg_research/simpleResult.txt
parallel1Results=~/CLionProjects/parallel_programming/pg_research/parallel2Result.txt
parallel2Results=~/CLionProjects/parallel_programming/pg_research/parallel4Result.txt
parallel3Results=~/CLionProjects/parallel_programming/pg_research/parallel8Result.txt
parallel4Results=~/CLionProjects/parallel_programming/pg_research/parallel10Result.txt
matrixsize1=10
matrixsize2=50
matrixsize3=100
prNum1=2
prNum2=4
prNum3=8
prNum4=10
iter1=10
iter2=50
iter3=100
#compiling programs
gcc main.c -osimple
gcc pageRankRandomizer.c -orandom
gcc speedUp.c -ospeedup
mpic++ main.cpp -std=gnu++0x -oparallel

rm ${simpleResults}
touch ${simpleResults}

rm ${parallel1Results}
touch ${parallel1Results}

rm ${parallel2Results}
touch ${parallel2Results}

rm ${parallel3Results}
touch ${parallel3Results}

rm ${parallel4Results}
touch ${parallel4Results}
#first step
parallel(){
iter="$1"
prNum="$2"
resultF="$3"
mpirun -np ${prNum} ./parallel ${pathf} ${iter}>>${resultF}
}

makeIter(){
iter="$1"
size="$2"
rm ${pathf}
touch ${pathf}
./random ${pathf} ${size} 2
echo "iterations $iter with matrix $size x $size"
./simple ${pathf} ${iter}>>${simpleResults}
parallel ${iter} ${prNum1} ${parallel1Results}
parallel ${iter} ${prNum2} ${parallel2Results}
parallel ${iter} ${prNum3} ${parallel3Results}
parallel ${iter} ${prNum4} ${parallel4Results}
}
makeIter ${iter1} ${matrixsize1}
makeIter ${iter2} ${matrixsize2}
makeIter ${iter3} ${matrixsize3}
./speedup 3 ${simpleResults} ${parallel1Results} ${parallel2Results} ${parallel3Results} ${parallel4Results}

