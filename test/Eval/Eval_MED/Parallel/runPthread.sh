#/Eval/Eval_MED/Parallel/runPthread.sh
#Author: Fenfang Li
#Time: 7/2023

# Double Dataset
DataSet1="diffusivity pressure velocityx velocityy velocityz viscocity"

# Float Dataset  
DataSet2="CLOUDf48 Pf48 PRECIPf48 QCLOUDf48 QGRAUPf48
          QICEf48 QRAINf48 QSNOWf48 QVAPORf48 TCf48
          Uf48 Vf48 Wf48 CLOUDf48_lg PRECIPf48_lg
          QCLOUDf48_lg QGRAUPf48_lg QICEf48_lg QRAINf48_lg QSNOWf48_lg"

DataSet3="baryon_density dark_matter_density temperature velocity_x velocity_y velocity_z"


#Source set
DataSource="/path/to/inputData/"
ResultSoure="/path/to/result/"

#Run 
set -- $DataSet1
for i
do
        echo $err
        echo " ********************  $i is being processed  with Pthread ***********************************"

        Path=$DataSource"Miranda/"$i".d64"

        Result=$ResultSoure"Pth_MirandaMedianOdd"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with MedianOdd ////////////////////////////////"
                ./Main_pth_MedianOdd $Path 256 384 384 D $j >> $Result  
        done
        python3 find.py $Result

        Result=$ResultSoure"Pth_MirandaCG-MED"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with CG-MED ////////////////////////////////"
                ./Main_pth_FSM_ori $Path 256 384 384 D $j >> $Result  
        done
        python3 find.py $Result

        Result=$ResultSoure"Pth_MirandaFG-MED"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with FG-MED ////////////////////////////////"
                ./Main_pth_FSM_FG $Path 256 384 384 D $j >> $Result  
        done
        python3 find.py $Result

        echo "****************************  process $i is done  *****************************"
done 


set -- $DataSet2
for i
do
        echo $err
        echo " ******************** $i is being processed with Pthread  ***********************************"

        Path=$DataSource"Hurricane-ISABEL/100x500x500/"$i".bin.f32"

        Result=$ResultSoure"Pth_HurricaneMedianOdd"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with MedianOdd ////////////////////////////////"
                ./Main_pth_MedianOdd $Path 500 500 100 F $j >> $Result  
        done
        python3 find.py $Result
        
        Result=$ResultSoure"Pth_HurricaneCG-MED"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with CG-MED ////////////////////////////////"
                ./Main_pth_FSM_ori $Path 500 500 100 F $j >> $Result  
        done
        python3 find.py $Result

        Result=$ResultSoure"Pth_HurricaneFG-MED"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with CG-MED ////////////////////////////////"
                ./Main_pth_FSM_FG $Path 500 500 100 F $j >> $Result  
        done
        python3 find.py $Result

        echo "**************************** process $i is done ********************************"
done


set -- $DataSet3
for i
do
        echo $err
        echo " ******************** $i is being processed with CG-MED  ***********************************"

        Path=$DataSource"EXASKY-NYX/"$i".f32"

        Result=$ResultSoure"Pth_NYXMedianOdd"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with MedianOdd ////////////////////////////////"
                ./Main_pth_MedianOdd $Path 512 512 512 F $j >> $Result  
        done
        python3 find.py $Result

        Result=$ResultSoure"Pth_NYXCG-MED"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with MedianOdd ////////////////////////////////"
                ./Main_pth_FSM_ori $Path 512 512 512 F $j >> $Result  
        done
        python3 find.py $Result

        Result=$ResultSoure"Pth_NYXFG-MED"
        for j in $(seq 1 16) 
        do
                echo "////////////////////////////////  Pthread $j with MedianOdd ////////////////////////////////"
                ./Main_pth_FSM_FG $Path 512 512 512 F $j >> $Result  
        done
        python3 find.py $Result
       
        echo "**************************** process $i is done ********************************"
done

