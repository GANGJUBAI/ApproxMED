#runCGMED.sh
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
        Result=$ResultSoure"Err_FG/RE_"$i
        ./Err_FSM_ori $Path 256 384 384 D 3 >> $Result
       
       python3 find.py $Result
       python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED"

       echo "****************************  process $i is done  *****************************"
done



set -- $DataSet2
for i
do
       echo $err
       echo " ******************** $i is being processed with FG-MED  ***********************************"

       Path=$DataSource"Hurricane-ISABEL/100x500x500/"$i".bin.f32"
       Result=$ResultSoure"Err_FG/RE_"$i
       ./Err_FSM_ori $Path 500 500 100 F 3 >> $Result
       
       python3 find.py $Result
       python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED"

       echo "**************************** process $i is done ********************************"
done


set -- $DataSet3
for i
do
       echo $err
       echo " ******************** $i is being processed with FG-MED  ***********************************"

       Path=$DataSource"EXASKY-NYX/"$i".f32"
       Result=$ResultSoure"Err_FG/RE_"$i
       ./Err_FSM_ori $Path 512 512 512 F 3 >> $Result
       
       python3 find.py $Result
       python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED"
       
       echo "**************************** process $i is done ********************************"
done 
 

set -- $DataSet4
for i 
do 
        echo $err
        echo "******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Scalibility/RE_"$i"_maxmin"

        j=1
        while [ $j -le 1000 ]; 
        do
            echo " ***************************** $i (loop=$j) is being processed with CG-MED  **************************"
            ./Err_FSM_ori $Path 512 512 $j I 2 > $Result

            python3 find.py $Result
            python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Scalibility/Scalibility_CG-MED_lena_maxmin"

            j=`expr $j \* 10`
        done

        echo "**************************** process $i is done ********************************"
done
python3 find.py $ResultSoure"Scalibility/Scalibility_CG-MED_lena"



set -- $DataSet5
for i 
do 
        echo $err
        echo "******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource2"MinstMoving/"$i".bin"
        Result=$ResultSoure"Scalibility/RE_"$i"_maxmin"
        
        j=1
        while [ $j -le 1000 ]; 
        do
            echo " ***************************** $i (loop=$j) is being processed with CG-MED  **************************"
            ./Err_FSM_ori $Path 64 64 20 I 2 $j 2 > $Result

            python3 find.py $Result
            python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Scalibility/Scalibility_CG-MED_Minst_maxmin"

            j=`expr $j \* 10`
        done

        echo "**************************** process $i is done ********************************"
done
python3 find.py $ResultSoure"Scalibility/Scalibility_CG-MED_Minst"


set -- $DataSet6
for i 
do 
        echo $err
        echo "******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Scalibility/RE_"$i"_maxmin"

        j=1
        while [ $j -le 100 ]; 
        do
            echo " ***************************** $i (loop=$j) is being processed with CG-MED  **************************"
            ./Err_FSM_ori $Path $j 1000000 I 1 > $Result

            python3 find.py $Result
            python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Scalibility/Scalibility_CG-MED_zipf_maxmin"

            j=`expr $j \* 10`
        done

        echo "**************************** process $i is done ********************************"
done
python3 find.py $ResultSoure"Scalibility/Scalibility_CG-MED_zipf"


set -- $DataSet7
for i 
do 
        echo $err
        echo "******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Scalibility/RE_"$i"_maxmin"

        j=1
        while [ $j -le 100 ]; 
        do
            echo " ***************************** $i (loop=$j) is being processed with CG-MED  **************************"
            ./Err_FSM_ori $Path $j 10000000 F 1 > $Result

            python3 find.py $Result
            python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Scalibility/Scalibility_CG-MED_norm_maxmin"

            j=`expr $j \* 10`
        done 

        echo "**************************** process $i is done ********************************"
done
python3 find.py $ResultSoure"Scalibility/Scalibility_CG-MED_norm"


set -- $DataSet9
for i 
do 
        echo $err
        echo "******************** $i is being processed with FG-MED  ***********************************"

        Path=$DataSource2$i".bin"
        Result=$ResultSoure"Err_FG/RE_"$i
        ./Err_FSM_FG $Path 3 60 5784 F 2 >> $Result

        python3 find.py $Result
        python3 $ResultSoure"accuracy_Count.py" $Result >> $ResultSoure"Err_FG-MED"

        echo "**************************** process $i is done ********************************"
done