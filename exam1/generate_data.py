#!/usr/bin/env python3

import matplotlib.pyplot as plt
import os
import re


cuda_data = []
omp_data = []
serial_data = []
run_rng = 3001
step = 500

for i in range(0, run_rng, step):
    stream =  os.popen("./run_exam_1 -c -r " + str(i))
    cuda_data.append(int(re.findall("[0-9]+", stream.read())[1]))
    stream =  os.popen("./run_exam_1 -o -r " + str(i))
    omp_data.append(int(re.findall("[0-9]+", stream.read())[1]))
    stream =  os.popen("./run_exam_1 -s -r " + str(i))
    serial_data.append(int(re.findall("[0-9]+", stream.read())[1]))

plt.plot(range(0, run_rng, step), cuda_data,'o', color='green', label="Cuda")
plt.plot(range(0, run_rng, step), omp_data,'o', color='red', label="OpenMP")
plt.plot(range(0, run_rng, step), serial_data,'o', color='black', label="Serial")
plt.legend()
plt.xlabel("Number of Nodes")
plt.ylabel("Run Time (ms)")

plt.show()
