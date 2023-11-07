The median (MED) is a crucial statistic for measuring the central tendency of a set of quantitative elements. However, computing the exact MED is costly, and even stateof-the-art (SOTA) algorithms struggle to meet the demands of modern scientific data. Therefore, approximate MED is often considered a more feasible solution.
We propose a novel, simple and efficient approximate MED finding algorithm CG-MED and a fine-grained implementation of CG-MED, called FG-MED.

# Get Started
## Dataset
| Dataset | Description         | Dimensions   | Characteristics | Type   |
| :-----: | :----------------:  | :--------:    |:------:| :------:|
| Miranda |Turbulence simulations | 256x384x384 | multivariate | double |
| Hurricane ISABEL | Weather simulation | 100x500x500 | multivariate | float |
| NYX| Hydrodynamics and cosmological simulation | 512x512x512 | multivariate | float |
| _power_ | Gloabal Active Power Measurements | 1041120 | time-series | float |
| Moving MNIST | Computer vision | 64x64 | video | integer |
| Lena | Computer vision | 512x512 | image | integer |
| _zipf_ | Synthetic | - |zipf(1.2, 10000)|integer |
| _norm_ | Synthetic | - | N (0, 1) |integer |
| _constant_ | Synthetic | - |value=1|integer |

The first five datasets can be found at Scientific Data Reduction Benchmarks suite (https://sdrbench.github.io/) and the other can be found at Datasets for Benchmarking Floating-Point Compressors suite (https://dps.uibk.ac.at/~fabian/datasets/)
