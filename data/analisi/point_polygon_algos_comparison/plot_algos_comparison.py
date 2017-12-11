import numpy as np
import pandas as pd
import seaborn
import matplotlib.pylab as plt
import matplotlib.ticker as mtick
from matplotlib.ticker import FormatStrFormatter


n = 10
step = 25000
data = pd.read_table("performances_n" + str(n) + "_step" + str(step) + ".txt", sep="\t")
data_1 = np.asarray(data)
data_2 = data_1[:,1:]

idx = np.array(np.argsort(data['#Points']))

norm=[]
norm.append(np.sum(data_1[:,1]))
norm.append(np.sum(data_1[:,2]))
norm.append(np.sum(data_1[:,3]))

norm =  np.asarray(norm)
ind = np.arange(len(data_1))
width = 2
algos_name= ["Winding Number","Crossing Number", "Winding Number Inclusion"]
fig, ax1=plt.subplots(figsize=(14,8))
fig.canvas.set_window_title("Running time comparison")
plt.subplots_adjust(left=0.075, right=0.95,top=0.9,bottom=0.25)
colors = ['mediumpurple','gold','limegreen']

for i in range(len(data_1)):
    for j in range(1,len(data_1[0])):
        if (i==0):
            ax1.bar(10*ind[i] + width*j, data_1[idx[i],j], width, color = colors[j-1], label=algos_name[j-1])          
        else:
            ax1.bar(10*ind[i] + width*j, data_1[idx[i],j], width, color = colors[j-1])
ax1.legend(loc='best', fontsize=26)
ax1.set_xlabel("Number of points computed", fontsize=25)
ax1.set_ylabel("Running time (ms)", fontsize=25)
ax1.set_title("Performances Comparisons of Point in Polygon algorithms", fontsize=26)
ax1.set_xlim(-5+width, len(data_1) + 90+ width)
ax1.set_xticks(ind+width)
plt.ticklabel_format(useOffset = True, style = 'sci', axis = 'y')
plt.xticks(np.linspace(0,90, num=10)+ 4.1, np.linspace(2500,25000, num=10).astype(int), rotation=0, fontsize=20)
plt.yticks(rotation=0, fontsize=20)
plt.ticklabel_format(axis='y',style='sci', scilimits=(0,1))
fig.savefig("Grafico_belloWI_CN.png")