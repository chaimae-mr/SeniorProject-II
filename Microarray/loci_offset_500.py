import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics

files = ["Microarray_loci_pairs_500_test"]
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

    plt.plot(uniqe_values,avg_corr)
    plt.xlabel('freq - offset500')
    plt.ylabel('average correlation')
    plt.xlim(0, 50)  # set the limits of the x-axis
    plt.show()

    print(df2["corr"].mean())
