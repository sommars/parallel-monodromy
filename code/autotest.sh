rm ~/outorig.txt
rm ~/outnew.txt

echo "simple test for 1 thread"
./pmonodromy ./graph-examples/random.graph -s 0 -old >> ~/outorig.txt
./pmonodromy ./graph-examples/random.graph -s 0 >> ~/outnew.txt
diff ~/outorig.txt ~/outnew.txt

rm ~/outorig.txt
rm ~/outnew.txt

echo "simple test for 2 threads"
./pmonodromy ./graph-examples/random.graph -s 0 -t 2 -old >> ~/outorig.txt
./pmonodromy ./graph-examples/random.graph -s 0 -t 2 >> ~/outnew.txt
diff ~/outorig.txt ~/outnew.txt

rm ~/outorig.txt
rm ~/outnew.txt

echo "simple test for 14 threads"
./pmonodromy ./graph-examples/random.graph -s 0 -t 14 -old >> ~/outorig.txt
./pmonodromy ./graph-examples/random.graph -s 0 -t 14 >> ~/outnew.txt
diff ~/outorig.txt ~/outnew.txt
