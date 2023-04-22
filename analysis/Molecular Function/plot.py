import matplotlib.pyplot as plt
import numpy as np
import math
from scipy.special import expit
import pandas as pd


def convert_scale(value,heatmap_size):
    new_value = value * (heatmap_size[1] - 1)
    return new_value

def find_color(x,y,heatmap_colors,heatmap_size):
    column_index = int(convert_scale(x,heatmap_size))
    row_index = 4-int((heatmap_size[0] - 1) * (1 - y))

    color = heatmap_colors[row_index][column_index]
    return color


def calculate(i,j,heatmap_colors,heatmap_size):
    file = ['number_links_5_F.csv','number_links_6_F.csv','number_links_7_F.csv','number_links_8_F.csv','number_links_9_F.csv','number_links_10_F.csv']
    df = pd.read_csv(file[j])
    df.columns = ["Go_term","num_genes","exp_link","sim_link","stan_div"]
    exp_links = df['exp_link'].iloc[i]
    sim_links = df['sim_link'].iloc[i]
    stan_div = df['stan_div'].iloc[i]
    
    if exp_links > 1 and abs(exp_links-sim_links) > abs(stan_div):
        ratio =  exp_links / sim_links
        z_value = (exp_links - sim_links)/stan_div
    else:
        ratio = 1
        z_value = 0

    ratio = expit(ratio)
    z_value = 1.0 - math.exp(-0.2 * z_value * z_value)

    color = find_color(ratio,z_value,heatmap_colors,heatmap_size)
    return color

def histo():

	# Define the size of the heatmap
	heatmap_size = (5,10)

	# Create an empty matrix to hold the color values for each pixel
	heatmap_colors = np.zeros((heatmap_size[0], heatmap_size[1], 3))

	# Define some colors
	red = [[255,51,51],[255,102,102],[255,153,153],[255,204,204],[255,255,255]]
	orange = [[255,153,51],[255,178,102],[255,204,153],[255,229,204],[255,255,255]]
	yellow = [[255,255,51],[255,255,102],[255,255,153],[255,255,204],[255,255,255]]
	green1 = [[153,255,51],[178,255,102],[204,255,153],[229,255,204],[255,255,255]]
	green2 = [[51,255,51],[102,255,102],[153,255,153],[204,255,204],[255,255,255]]
	green3 = [[0,255,57],[63,255,107],[126,255,155],[191,255,205],[255,255,255]]
	light = [[0,255,169],[62,255,190],[126,255,212],[191,255,233],[255,255,255]]
	blue1 = [[51,255,255],[102,255,255],[153,255,255],[204,255,255],[255,255,255]]
	blue2 = [[52,153,255],[102,178,255],[153,204,255],[204,229,255],[255,255,255]]
	blue3 = [[52,52,255],[102,102,255],[153,153,255],[204,204,255],[255,255,255]]

	color = [red,orange,yellow,green1,green2,green3,light,blue1,blue2,blue3]

	for i in range(len(color)):
		color[i] = list(reversed(color[i]))

	# Loop through each pixel in the heatmap and assign a color
	for i in range(heatmap_size[0]):
		for j in range(heatmap_size[1]):
			# Set the color of each pixel based on its position in the heatmap
			heatmap_colors[i,j] = [value / 255 for value in color[j][i]] # red

	heatmap_data = (22,5)

	# Create an empty matrix to hold the color values for each pixel
	heatmap_data_color = np.zeros((heatmap_data[0], heatmap_data[1], 3))

	for i in range(heatmap_data[0]):
		for j in range(heatmap_data[1]):
			# Set the color of each pixel based on its position in the heatmap
			heatmap_data_color[i,j] = calculate(i,j,heatmap_colors,heatmap_size) # red



	# plt.imshow(heatmap_data_color)
	df = pd.read_csv('terms.csv')
	df.columns = ['name','ope']
	row_labels = df.iloc[:, 1].values
	fig, ax = plt.subplots()
	im = ax.imshow(heatmap_data_color)
	# ax.set_xticks(np.arange(len(row_labels)))
	ax.set_yticks(np.arange(len(row_labels)))
	# ax.set_xticklabels(row_labels)
	ax.set_yticklabels(row_labels)
	fig = plt.gcf()
	plt.title("Molecular Function")
	fig.set_size_inches(20, 20)
	plt.xlabel("Frequancy")

	plt.show()

	# Plot the heatmap
	fig, ax = plt.subplots()
	im = ax.imshow(heatmap_colors)

    # Set the x-axis ticks to show only the first and last values
	x_ticks = np.linspace(0.5, 2, 10)
	ax.set_xticks([0, 9])
	ax.set_xticklabels([x_ticks[0], x_ticks[-1]])
	plt.xlabel("ratio",size = 15)
	plt.ylabel("Z-score",size = 15)
	
	y_ticks = [0.0,1.2,1.9,2.6,5]
	ax.set_yticks([0,1,2,3,4],[0.0,1.2,1.9,2.6,5])
	
	plt.show()

histo()
