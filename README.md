The median (MED) is a crucial statistic for measuring the central tendency of a set of quantitative elements. However, computing the exact MED is costly, and even state-of-the-art (SOTA) algorithms struggle to meet the demands of modern scientific data. Therefore, approximate MED is often considered a more feasible solution.
We propose a novel, simple and efficient approximate MED finding algorithm CG-MED and a fine-grained implementation of CG-MED, called FG-MED.

# Get Started
* Clone the repository:
  
    `git clone https://github.com/GANGJUBAI/ApproxMED.git`
  
* Get the dataset:
  
## Dataset
| Dataset | Description         | Dimensions   | Characteristics | Type   |
| :-----: | :----------------:  | :--------:    |:------:| :------:|
| Miranda |Turbulence simulations | 256x384x384 | multivariate | double |
| Hurricane ISABEL | Weather simulation | 100x500x500 | multivariate | float |
| NYX| Hydrodynamics and cosmological simulation | 512x512x512 | multivariate | float |
| _power_ | Gloabal Active Power Measurements | 1041120 | time-series | float |
| Moving MNIST | Computer vision | 64x64 | video | integer |
| Lena | Computer vision | 512x512 | image | integer |
| _zipf_ | Synthetic | - |zipf(1.2, 10000)| integer |
| _norm_ | Synthetic | - | N (0, 1) | integer |
| _constant_ | Synthetic | - |value=1| integer |

---- The first three datasets can be found at Scientific Data Reduction Benchmarks suite (https://sdrbench.github.io/).

---- The _power_ dataset consists of Global Active Power measurements from the UCI Individual household electric power consumption dataset.

---- The Moving MNIST and Lena datasets are significant datasets for computer version from Kaggle public dataset.

---- The last three datasets are synthetic, they are treated as 1D dataset, the size can be selected randomly.

## Data generation
  1. Read Image/vedio
     If you want to test the dataset Lena or Moving MNIST, you could get it from ./data_generate/readImage.py or ./data_generate/readVedio.py.
     
  2. Generate the synthetic datasets
     If you want to test three synthetic dataset, you could get it from ./data_generate/gene Constant.c or geneNormal.c or geneZipf.c.

## Inout Format
Since the input parameters used by algorithms vary with the dimension, the input format is included in the repository (runXXX.sh). 

Here's a small example with 3-dimensional double-precision floating-point dataset:

` ./Main_FSM_ori $DatasetPath 256 384 384 D 3 `

## Application
We implement our approximate median finding algorithms in data compression, in which we use the common compressor FPC and FPZIP.

### 1.FPC

  **FPC** is a fast and effective lossless compressor/decompressor for IEEE 754 64-bit double-precision floating-point data. 
  
  The raw source code can be found at https://userweb.cs.txstate.edu/~burtscher/research/FPC/, and adapted FPC could be found at test/Eval/Application/fpc.h
  
### 2.FPZIP

  **FPZIP** is a library and command-line utility for lossless and optionally lossy compression of 2D and 3D floating-point arrays.
  
  The source code and installation can be found at https://github.com/llnl/fpzip. It should be installed to test/Compressor.
### 3.SZ

  **SZ** is error-bounded lossy compressor for HPC data.
  
   Although SZ is not used in paper, the function for reading 3D data is used. The source code and installation can be found at https://github.com/szcompressor/SZ. Similarly, it should be installed to test/Compressor.
