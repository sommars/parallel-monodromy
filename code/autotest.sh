echo "simple test for $1 thread on seed $2"
./pmonodromy ./graph-examples/random.graph -s $2 -t $1 -old >> ./outorig.txt
./pmonodromy ./graph-examples/random.graph -s $2 -t $1 >> ./outnew.txt
diff ./outorig.txt ./outnew.txt

rm ./outorig.txt
rm ./outnew.txt
