DataList="velocityy velocityz viscocity"
DataSource="/path/to/inputData/"
ResultSoure="/path/to/result/"
pltSource="/path/to/plot/motiCAS.py"

echo "==========================  COMPARISON AND SWQPPING OPERATION ==============================="
set -- $DataList
for i
do
	echo $err
	echo "####################### $i is being tested with approxMED #########################"

	Path=$DataSource"Miranda/"$i".d64"
	Source=$ResultSoure"CAS_approx.result"
	./CAS_FSM_ori $Path 256 384 384 D 3 >> $Source

	echo "####################### test $i is done ###########################"
done
python3 find.py $Source

set -- $DataList
for i
do      
        echo $err
        echo "####################### $i is being tested with qselect #########################"
        
        Path=$DataSource"Miranda/"$i".d64"
        Source=$ResultSoure"CAS_qselect.result"
        ./CAS_Main_qSelect $Path 256 384 384 D 3 >> $Source
        
        echo "####################### test $i is done ###########################"
done
python3 find.py $Source


set -- $DataList
for i
do      
        echo $err
        echo "####################### $i is being tested with EXACT-MED #########################"
        
        Path=$DataSource"Miranda/"$i".d64"
        Source=$ResultSoure"CAS_qsort.result"
        ./CAS_qsort $Path 256 384 384 D 3 >> $Source
        
        echo "####################### test $i is done ###########################"
done
python3 find.py $Source
python3 $pltSource

echo "===================== COMPARISON AND SWQPPING OPERATION DONE!!! ==============================="





