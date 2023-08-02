# test/Eval/Application/runApplication.sh
# Author: Fenfang Li
# Time: 2023/7


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
       echo " ****************************  $i is being compressed **************************** "

       Path=$DataSource"Miranda/"$i".d64"
       
       echo " ****************************  $i is being compressed with socc ****************************"
       Source=$ResultSoure"Cmp_Miranda.result"
       ./Compress $Path 256 384 384 >> $Source
       python3 find.py $Source

       echo " ****************************  $i is being compressed with CG-MED ****************************"
       Source=$ResultSoure"Cmp_MirandaCG-MED.result"
       ./Compress_ori $Path 256 384 384 >> $Source
       python3 find.py $Source

       echo "****************************  $i is being compressed with FG-MED ****************************"
       Source=$ResultSoure"Cmp_MirandaFG-MED.result"
       ./Compress_FG $Path 256 384 384 >> $Source
       python3 find.py $Source

       echo "****************************  compress $i is done  **************************** "
done


set -- $DataSet2
for i
do     
       echo $err 
       echo " **************************** $i is being compressed ***********************************"
       
       Path=$DataSource"Hurricane-ISABEL/100x500x500/"$i".bin.f32" 

       echo " ****************************  $i is being compressed with socc ****************************"
       Source=$ResultSoure"Cmp_Hurricane.result"
       ./Compress_float $Path 500 500 100 >> $Source
       python3 find.py $Source

       echo " ****************************  $i is being compressed with CG-MED ****************************"
       Source=$ResultSoure"Cmp_HurricaneCG-MED.result"
       ./Compress_float_ori $Path 500 500 100 >> $Source
       python3 find.py $Source

       echo "****************************  $i is being compressed with FG-MED ****************************"
       Source=$ResultSoure"Cmp_HurricaneFG-MED.result"
       ./Compress_float_FG $Path 500 500 100 >> $Source
       python3 find.py $Source

       echo "**************************** compress $i is done *******************************"
done


set -- $DataSet3
for i
do     
       echo $err 
       echo " **************************** $i is being compressed ***********************************"
       
       Path=$DataSource"EXASKY-NYX/"$i".f32"

       echo " ****************************  $i is being compressed with socc ****************************"
       Source=$ResultSoure"Cmp_NYX.result"
       ./Compress_float $Path 512 512 512 >> $Source
       python3 find.py $Source

       echo " ****************************  $i is being compressed with CG-MED ****************************"
       Source=$ResultSoure"Cmp_NYXCG-MED.result"
       ./Compress_float_ori $Path 512 512 512 >> $Source
       python3 find.py $Source

       echo "****************************  $i is being compressed with FG-MED ****************************"
       Source=$ResultSoure"Cmp_NYXFG-MED.result"
       ./Compress_float_FG $Path 512 512 512 >> $Source
       python3 find.py $Source

       echo "**************************** compress $i is done *******************************"
done
