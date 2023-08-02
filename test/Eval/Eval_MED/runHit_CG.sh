#/Eval/Eval_MED/runHit_CG.sh
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
        echo " ********************  $i is being processed  with CG-MED ***********************************"

        Path=$DataSource"Miranda/"$i".d64"
        Result=$ResultSoure"Hit_CG/Hit_MirandaCG-MED"

        ./Hit_FSM_ori $Path 256 384 384 D 3 > $Result  
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_MirandaCG-MED"
 
        echo "****************************  process $i is done  *****************************"
done 


set -- $DataSet2
for i
do
        echo $err
        echo " ******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource"Hurricane-ISABEL/100x500x500/"$i".bin.f32"
        Result=$ResultSoure"Hit_CG/Hit_HurricaneCG-MED"
       
      
        ./Hit_FSM_ori $Path 500 500 100 F 3 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_HurricaneCG-MED"

        echo "**************************** process $i is done ********************************"
done



set -- $DataSet3
for i
do
        echo $err
        echo " ******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource"EXASKY-NYX/"$i".f32"
        Result=$ResultSoure"Hit_CG/Hit_NYXCG-MED"
      
        ./Hit_FSM_ori $Path 512 512 512 F 3 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_NYXCG-MED"
       
        echo "**************************** process $i is done ********************************"
done


set -- $DataSet4
for i 
do 
        echo $err
        echo " ******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Hit_CG/Hit_powerCG-MED"

        ./Hit_FSM_ori $Path 3 60 5784 F 2 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_powerCG-MED"


        echo "**************************** process $i is done ********************************"
done


set -- $DataSet5
for i 
do
        echo $err
        echo " ******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource2"MinstMoving/"$i".bin"
        Result=$ResultSoure"Hit_CG/Hit_MinstMovingCG-MED"

        ./Hit_FSM_ori $Path 64 64 20 I 2 100 2 > $Result
        python3 find.py $Result
        python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_MinstMovingCG-MED"

        echo "**************************** process $i is done ********************************"
done


set -- $DataSet6
for i
do 
    echo $err
    echo " ******************** $i is being processed with CG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_CG/Hit_lenaCG-MED"
    
    ./Hit_FSM_ori $Path 512 512 100 I 2 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_lenaCG-MED"

    echo "**************************** process $i  is done ********************************"
done


set -- $DataSet7
for i
do 
    echo $err
    echo " ******************** $i is being processed with CG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_CG/Hit_ZipfCG-MED.result"
    

    ./Hit_FSM_ori $Path 10 1000000 I 1 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_ZipfCG-MED"

    echo "**************************** process $i  is done ********************************"
done


set -- $DataSet8
for i
do 
    echo $err
    echo " ******************** $i is being processed with CG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_CG/Hit_normalCG-MED.result"
    
    ./Hit_FSM_ori $Path 1 10000000 F 1 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_normalCG-MED"

    echo "**************************** process $i  is done ********************************"
done


set -- $DataSet9
for i
do 
    echo $err
    echo " ******************** $i is being processed with CG-MED  ***********************************"

    Path=$DataSource2$i".bin"
    Result=$ResultSoure"Hit_CG/Hit_constantCG-MED"
    
    ./Hit_FSM_ori $Path 100 1000 I 1 > $Result
    python3 find.py $Result
    python3 $ResultSoure"countHitRate.py" $Result >> $ResultSoure"Hit_constantCG-MED"

    echo "**************************** process $i  is done ********************************"
done