#/Eval/Eval_MED/runHit_FG.sh
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
DataSet5="sequence-00001" #(sequence-0000x --- the number of sequence is from 0 to 9999) 
DataSet6="lena"
DataSet7="Zipf_1.2_[1,10]_6"
#DataSet7Size="1000 10000 100000 1000000 10000000 100000000"
s="1 1.2 1.5 1.8"
#DataSet8="normal_10m(m=magnitude)"
DataSet8="normal_107"
#DataSet8Size="1000 10000 100000 1000000"
#DataSet9="constant_10m(m=magnitude)" the size of dataset9 is same to dataset7, thus use the "DataSet7Size"
DataSet9="constant_103"

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
        Result=$ResultSoure"Hit_FG/Hit_MirandaFG-MED"

        ./Hit_FSM_FG $Path 256 384 384 D 3 47 > $Result  
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_MirandaFG-MED"
 
        echo "****************************  process $i is done  *****************************"
done 


set -- $DataSet2
for i
do
        echo $err
        echo " ******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource"Hurricane-ISABEL/100x500x500/"$i".bin.f32"
        Result=$ResultSoure"Hit_FG/Hit_HurricaneFG-MED"
       
      
        ./Hit_FSM_FG $Path 500 500 100 F 3 47 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_HurricaneFG-MED"

        echo "**************************** process $i is done ********************************"
done



set -- $DataSet3
for i
do
        echo $err
        echo " ******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource"EXASKY-NYX/"$i".f32"
        Result=$ResultSoure"Hit_FG/Hit_NYXFG-MED"
      
        ./Hit_FSM_FG $Path 512 512 512 F 3 47 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_NYXFG-MED"
       
        echo "**************************** process $i is done ********************************"
done


set -- $DataSet4
for i 
do 
        echo $err
        echo " ******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Hit_FG/Hit_powerFG-MED"

        ./Hit_FSM_FG $Path 3 60 5784 F 2 47 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_powerFG-MED"


        echo "**************************** process $i is done ********************************"
done


set -- $DataSet5
for i 
do
        echo $err
        echo " ******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2"MinstMoving/"$i".bin"
        Result=$ResultSoure"Hit_FG/Hit_MinstMovingFG-MED"

        ./Hit_FSM_FG $Path 64 64 20 I 2 100 2 47 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_MinstMovingFG-MED"

        echo "**************************** process $i is done ********************************"
done


set -- $DataSet6
for i
do 
    echo $err
    echo " ******************** $i is being processed with FG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_FG/Hit_lenaFG-MED"
    
    ./Hit_FSM_FG $Path 512 512 100 I 2 47 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_lenaFG-MED"

    echo "**************************** process $i  is done ********************************"
done


set -- $DataSet7
for i
do 
    echo $err
    echo " ******************** $i is being processed with FG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_FG/Hit_ZipfFG-MED.result"
    

    ./Hit_FSM_FG $Path 10 1000000 I 1 47 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_ZipfFG-MED"

    echo "**************************** process $i  is done ********************************"
done


set -- $DataSet8
for i
do 
    echo $err
    echo " ******************** $i is being processed with FG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_FG/Hit_normalFG-MED.result"
    
    ./Hit_FSM_FG $Path 1 10000000 F 1 47 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_normalFG-MED"

    echo "**************************** process $i  is done ********************************"
done


set -- $DataSet9
for i
do 
    echo $err
    echo " ******************** $i is being processed with FG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_FG/Hit_constantFG-MED"
    
    ./Hit_FSM_FG $Path 100 1000 I 1 47 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_constantFG-MED"

    echo "**************************** process $i  is done ********************************"
done