import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics



list_offset = [100,200,300,400,500,600,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000]
avg_corr = []

for offset in list_offset:
    df = pd.read_csv("RNA_loci_pairs_{}_2.csv".format(offset))
    df.columns = ["chr1","loci1","chr2","loci2",'corr','freq']
    avg_corr.append(df["corr"].mean())

plt.plot(list_offset,avg_corr)
plt.xlabel('offset')
plt.ylabel('average correlation')
plt.show()