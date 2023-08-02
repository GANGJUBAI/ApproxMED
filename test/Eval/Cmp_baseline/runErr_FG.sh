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
DataSet4="lena"
DataSet5="sequence-00001"
DataSet6="Zipf_1.2_[1,10]_6"
DataSet7="normal_107"
DataSet8="constant_103"
DataSet9="power_0708"

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
        Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
        ./Err_FSM_FG_maxmin $Path 256 384 384 D 3 >> $Result
       
       python3 find.py $Result
       python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"


       echo "****************************  process $i is done  *****************************"
done



set -- $DataSet2
for i
do
       echo $err
       echo " ******************** $i is being processed with FG-MED  ***********************************"

       Path=$DataSource"Hurricane-ISABEL/100x500x500/"$i".bin.f32"
       Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
       ./Err_FSM_FG_maxmin $Path 500 500 100 F 3 >> $Result
       
       python3 find.py $Result
       python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"

       echo "**************************** process $i is done ********************************"
done


set -- $DataSet3
for i
do
       echo $err
       echo " ******************** $i is being processed with FG-MED  ***********************************"

       Path=$DataSource"EXASKY-NYX/"$i".f32"
       Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
       ./Err_FSM_FG_maxmin $Path 512 512 512 F 3 >> $Result
       
       python3 find.py $Result
       python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"
       
       echo "**************************** process $i is done ********************************"
done 
 

set -- $DataSet4
for i 
do 
        echo $err
        echo "******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
        ./Err_FSM_FG_maxmin $Path 512 512 1 I 2 >> $Result

        python3 find.py $Result
        python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"

        echo "**************************** process $i is done ********************************"
done


set -- $DataSet5
for i 
do 
        echo $err
        echo "******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2"MinstMoving/"$i".bin"
        Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
        ./Err_FSM_FG_maxmin $Path 64 64 20 I 2 100 2 >> $Result

        python3 find.py $Result
        python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"

        echo "**************************** process $i is done ********************************"
done

set -- $DataSet6
for i 
do 
        echo $err
        echo "******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
        ./Err_FSM_FG_maxmin $Path 10 1000000 I 1 >> $Result

        python3 find.py $Result
        python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"

        echo "**************************** process $i is done ********************************"
done


set -- $DataSet7
for i 
do 
        echo $err
        echo "******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
        ./Err_FSM_FG_maxmin $Path 10 1000000 F 1 >> $Result

        python3 find.py $Result
        python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"

        echo "**************************** process $i is done ********************************"
done


set -- $DataSet9
for i 
do 
        echo $err
        echo "******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Err_FG/RE_"$i"_maxmin"
        ./Err_FSM_FG_maxmin $Path 3 60 5784 F 2 >> $Result

        python3 find.py $Result
        python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED_maxmin"

        echo "**************************** process $i is done ********************************"
done