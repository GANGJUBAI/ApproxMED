#geneZipf.sh
#Author: Fenfang Li
#Time:6/2023

dataSize="1000 10000 100000 1000000 10000000 100000000"
s="1 1.2 1.5 1.8"
fileSourcePath="/path/to/data_generator/"


set -- $dataSize
for i 
do
    echo $err
    echo "**************************** Zipf $i is being generated ***********************************"
    count=$(echo $i | grep -o 0 | wc -l)
    set -- $s
    for j 
    do
        echo "**************************** Zipf_$i"_"$s is being generated ***********************************"

        fileName=$fileSourcePath"Zipf_"$j"_[1,10]_"$count".bin"

        python3 geneZipf.py $i $j $fileName

        echo "**************************** Zipf_$j"_"$i is done ***********************************"
    done 

    echo "**************************** Zipf $i is done ********************************"
done


