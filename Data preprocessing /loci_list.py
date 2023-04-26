import pandas as pd
import numpy as np


#code to creat file with interchromosomal contacta 
def new_loci():
    df = pd.read_csv('List.csv')
    new_list = open('loci_list.csv','w+') 
    for index, row in df2.iterrows():
        if df2['chr1'].iloc[index] == df2['chr2'].iloc[index]:
            continue 
        else:
            new_list.write(str(df2['chr1'].iloc[index]))
            new_list.write(',')
            new_list.write(str(df2['loci1'].iloc[index]))
            new_list.write(',')
            new_list.write(str(df2['chr2'].iloc[index]))
            new_list.write(',')
            new_list.write(str(df2['loci2'].iloc[index]))
            new_list.write(',')
            new_list.write(str(df2['freq'].iloc[index]))
            new_list.write('\n')    
    new_list.close()
new_loci()