import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics

files = ["RNA_loci_pairs_500_2"]
for fd in files:
    df2 = pd.read_csv("{}.csv".format(fd))
    df2.columns = ["chr1","loci1","chr2","loci2",'corr','freq']

    uniqe_values = df2["freq"].unique()
    uniqe_values.sort()
    avg_corr = []

    for i in uniqe_values:
        temp_corr = []

        for index, row in df2.iterrows():
            if row['freq'] >= i:
                temp_corr.append(row['corr'])
        avg_corr.append(statistics.mean(temp_corr))

    plt.plot(uniqe_values,avg_corr,color='red',linewidth=2)
    plt.title('The average correlation between linked \ngenes Vs. count frequency  ',color='darkblue',fontsize=15)
    plt.xlabel('4c count frequancy',fontsize=14)
    plt.ylabel('Average correlation',fontsize=14)
    plt.xlim(0, 27)  
    plt.show()
