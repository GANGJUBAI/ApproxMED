#runFGMED.sh
#Author: Fenfang Li
#Time: 6/1/2023

# Double Dataset
DataSet1="diffusivity pressure velocityx velocityy velocityz viscocity"

# Float Dataset
DataSet2="CLOUDf48 Pf48 PRECIPf48 QCLOUDf48 QGRAUPf48
          QICEf48 QRAINf48 QSNOWf48 QVAPORf48 TCf48
          Uf48 Vf48 Wf48 CLOUDf48_lg PRECIPf48_lg
          QCLOUDf48_lg QGRAUPf48_lg QICEf48_lg QRAINf48_lg QSNOWf48_lg"

DataSet3="baryon_density dark_matter_density temperature velocity_x velocity_y velocity_z"
DataSet4="power_0708"
#DataSet5="sequence-0xxxx" (the number of sequence is from 0 to 9999) 
DataSet6="lena"
#DataSet7="Zipf_s_[1,10]_datasize.bin"
#DataSet7Size="1000 10000 100000 1000000 10000000 100000000"
s="1 1.2 1.5 1.8"
#DataSet8="normal_10m(m=magnitude)"
#DataSet8Size="1000 10000 100000 1000000"
#DataSet9="constant_10m(m=magnitude)" the size of dataset9 is same to dataset7, thus use the "DataSet7Size"


#Source set
DataSource="/path/to/inputData/"
DataSource2="/path/to/data_generator/"
ResultSoure="/path/to/result/"


#Run 
set -- $DataSet1
for i
do
       echo $err
       echo " ********************  $i is being processed  with FG-MED ***********************************"

       Path=$DataSource"Miranda/"$i".d64"
       Result=$ResultSoure"3D/Time_MirandaFG-MED.result"
       ./Main_FSM_FG $Path 256 384 384 D 3 >> $Result
       
       #Result=$ResultSoure"2D/Time_MirandaFG-MED.result"
       #./Main_FSM_FG $Path 256 384 384 D 2 >> $Result

       echo "****************************  process $i is done  *****************************"
done
python3 find.py $Result


set -- $DataSet2
for i
do
       echo $err
       echo " ******************** $i is being processed with FG-MED  ***********************************"

       Path=$DataSource"Hurricane-ISABEL/100x500x500/"$i".bin.f32"
       Result=$ResultSoure"3D/Time_HurricaneFG-MED.result"
       ./Main_FSM_FG $Path 500 500 100 F 3 >> $Result
       
       #Result=$ResultSoure"2D/Time_HurricaneFG-MED.result"
       #./Main_FSM_FG $Path 500 500 100 F 2 >> $Result

       echo "**************************** process $i is done ********************************"
done
python3 find.py $Result

set -- $DataSet3
for i
do
       echo $err
       echo " ******************** $i is being processed with FG-MED  ***********************************"

       Path=$DataSource"EXASKY-NYX/"$i".f32"
       Result=$ResultSoure"3D/Time_NYXFG-MED.result"
       ./Main_FSM_FG $Path 512 512 512 F 3 >> $Result
       
       #Result=$ResultSoure"2D/Time_NYXFG-MED.result"
       #./Main_FSM_FG $Path 512 512 512 F 2 >> $Result
       
       echo "**************************** process $i is done ********************************"
done
python3 find.py $Result


set -- $DataSet4
for i 
do 
       echo $err
       echo " ******************** $i is being processed with FG-MED  ***********************************"

       Path=$DataSource2$i".bin"
       Result=$ResultSoure"2D/Time_powerFG-MED.result"

       echo "power is processed with para (24 60 723)" >> $Result
       ./Main_FSM_FG $Path 24 60 723 F 2 >> $Result

       echo "power is processed with para (3 60 5784)" >> $Result
       ./Main_FSM_FG $Path 3 60 5784 F 2 >> $Result

       echo "power is processed with para (12 60 1446)" >> $Result
       ./Main_FSM_FG $Path 12 60 1446 F 2 >> $Result

       echo "**************************** process $i is done ********************************"

done
python3 find.py $Result


for i in $(seq 0 99)
do
    echo $err
    FileName=$(printf "%s%04d\n" "sequence-0" $i)
    echo " ******************** $FileName is being processed with FG-MED  ***********************************"
    
    Path=$DataSource2"MinstMoving/"$FileName".bin"
    Result=$ResultSoure"2D/Time_MinstMovingFG-MED.result"

    j=1
    while [ $j -le 1000 ]; 
    do
        echo " ***************************** $FileName (loop=$j) is being processed with FG-MED  **************************"
        ./Main_FSM_FG $Path 64 64 20 I 2 $j 2 >> $Result
        j=`expr $j \* 10`  
        
    done
    
    echo "**************************** process $FileName is done ********************************"
done
python3 find.py $Result


set -- $DataSet6
for i
do 
    echo $err
    echo " ******************** $i is being processed with FG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"2D/Time_lenaFG-MED.result"
    
    j=1
    while [ $j -le 1000 ]; 
    do
        echo " ***************************** $i (loop=$j) is being processed with FG-MED  **************************"
        ./Main_FSM_FG $Path 512 512 $j I 2 >> $Result
        j=`expr $j \* 10`
    done
    echo "**************************** process $i  is done ********************************"

done
python3 find.py $Result


i=1000
while [ $i -le 100000000 ]; 
do
    echo $err
    count=$(echo $i | grep -o 0 | wc -l)
    set -- $s
    for j 
    do
        echo "**************************** Zipf_$i"_"$j is being processed with FG-MED ***********************************"

        Path=$DataSource2"Zipf_"$j"_[1,10]_"$count".bin"
        Result=$ResultSoure"1D/Time_ZipfFG-MED.result"

        echo "Zipf_$i"_"$j is processed with para " >> $Result

        k=1
        while [ $k -le 100 ]; 
        do
            echo "**************************** Zipf_$i_$j(loop=$k) is being processed with FG-MED ***********************************"
            ./Main_FSM_FG $Path $k $i I 1 >> $Result
            k=`expr $k \* 10`
        done    
        echo "**************************** process Zipf_$i"_"$j  is done ********************************"
    done
    i=`expr $i \* 10`
    echo "**************************** process Zipf_$i  is done ********************************"
done
python3 find.py $Result


i=1000
while [ $i -le 100000000 ]; 
do
    echo $err
    count=$(echo $i | grep -o 0 | wc -l)
    echo "**************************** normal_10$count is being processed with FG-MED ***********************************"

    Path=$DataSource2"normal_10"$count".bin"
    Result=$ResultSoure"1D/Time_normalFG-MED.result"

    echo "normal_10$count is processed with para " >> $Result

    j=1
    while [ $j -le 100 ]; 
    do
        echo "**************************** normal_10$count(loop=$j) is being processed with FG-MED ***********************************"
        ./Main_FSM_FG $Path $j $i I 1 >>$Result
        j=`expr $j \* 10`
    done 
    i=`expr $i \* 10`
    echo "**************************** process normal_10$count is done ********************************"
done
python3 find.py $Result


i=1000
while [ $i -le 1000000 ]; 
do
    echo $err
    count=$(echo $i | grep -o 0 | wc -l)
    echo "**************************** constant_10$count is being processed with FG-MED ***********************************"

    Path=$DataSource2"constant_10"$count".bin"
    Result=$ResultSoure"1D/Time_constantFG-MED.result"

    echo "constant_10$count is processed with para " >> $Result

    j=1
    while [ $j -le 100 ]; 
    do
        echo "**************************** constant_10$count(loop=$j) is being processed with FG-MED ***********************************"
        ./Main_FSM_FG $Path $j $i I 1 >>$Result
        j=`expr $j \* 10`
    done 
    i=`expr $i \* 10`
    echo "**************************** process constant_10$count is done ********************************"
done
python3 find.py $Result