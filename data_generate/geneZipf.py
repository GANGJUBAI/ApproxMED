import random 
from random import randint
import sys

if len(sys.argv) < 1 :
    print('usage: python %s <file' % sys.argv[0])
    sys.exit(0)


# set the size of dataset
dataset_size = int(sys.argv[1]) #10000

# set the paramwter of Zipf distribution 
s = float(sys.argv[2])   #1.2

# claculate the Zipf distribution 
probabilities = []
for rank in range(1, dataset_size+1):
    probability = (1.0 / pow(rank, s))
    probabilities.append(probability)

# normalized probability distribution 
probabilities_sum = sum(probabilities)
probabilities = [probability/probabilities_sum for probability in probabilities]

# generate dataset 
words = [randint(1,10) for i in range(1, dataset_size+1)]
dataset = random.choices(words, probabilities, k=dataset_size)

import struct
# Save the generated data to a file in binary form
foutname= sys.argv[3] #'Zipf_1.2_[1,10].bin'
fp = open(foutname,'wb+')
data = struct.pack( ('%di' % len(dataset)), *dataset)
fp.write(data)
fp.close()

