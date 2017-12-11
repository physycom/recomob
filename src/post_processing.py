# Copyright 2017 Andrea Checcoli
# 
# ***************************************************************************
# This file is part of recomob.
# 
# recomob is free software : you can redistribute it and / or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# recomob is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with recomob. If not, see <http://www.gnu.org/licenses/>.
# ***************************************************************************


import scipy.linalg
import numpy as np
import pandas as pd
import matplotlib.pylab as plt
import matplotlib.ticker as ticker
from matplotlib.ticker import MultipleLocator
import seaborn

data_file = "check_8002350.txt"
tot_file = "totali_8002350.txt"

n = np.asarray(pd.read_csv(data_file, sep = "\t", index_col=0, header=-1))
N = np.asarray(pd.read_csv(tot_file, sep="\n", header=-1))
#
#tot = np.asarray([np.sum(n, 0)])
#print("tot shape",tot.shape)
#print("tot check",np.sum(N-tot))

C = np.dot(n[:-1].transpose(), n[:-1])
Ct = np.dot(n[1:].transpose(), n[:-1])
b = (N[1:]-N[:-1])/N[:-1] 
T = np.dot(n[:-1].transpose(), b * n[:-1])

Pi = np.dot( Ct-T, np.linalg.inv(C))

a = np.sum(Pi,0)

temp = np.asarray([np.dot(a, n[1:].transpose())]).transpose()
temp = temp - N[1:]
temp = np.dot(n[1:].transpose(), temp )


dati_pi_c = []
dati_solo_c = []

for i in range (0,len(n)-1):
    dati_pi_c.append(np.asarray((np.dot(Pi,n[i,:]) + b[i]*n[i])))
    dati_solo_c.append(n[i]+b[i]*n[i])

pi_c = np.asarray(dati_pi_c) 
solo_c = np.asarray(dati_solo_c)    
conf_pi_mis = np.sqrt(np.sum(((dati_pi_c-n[1:,:])*(dati_pi_c-n[1:,:])),0))/np.sqrt(np.sum((n[1:,:]*n[1:,:]),0))
conf_uno_mis = np.sqrt(np.sum(((dati_solo_c-n[1:,:])*(dati_solo_c-n[1:,:])),0))/np.sqrt(np.sum((n[1:,:]*n[1:,:]),0))        
pi_mis = np.asarray(conf_pi_mis)
uno_mis = np.asarray(conf_uno_mis)

print("(alpha * n - N) * n", temp)


labels=['Cannaregio','Castello','San Marco','San Polo','Dorsoduro','Santa Croce','Nowhere']
ore=np.array(['08.00-08.10',
'08.10-08.20',
'08.20-08.30',
'08.30-08.40',
'08.40-08.50',
'08.50-09.00',
'09.00-09.10',
'09.10-09.20',
'09.20-09.30',
'09.30-09.40',
'09.40-09.50',
'09.50-10.00',
'10.00-10.10',
'10.10-10.20',
'10.20-10.30',
'10.30-10.40',
'10.40-10.50',
'10.50-11.00',
'11.00-11.10',
'11.10-11.20',
'11.20-11.30',
'11.30-11.40',
'11.40-11.50',
'11.50-12.00',
'12.00-12.10',
'12.10-12.20',
'12.20-12.30',
'12.30-12.40',
'12.40-12.50',
'12.50-13.00',
'13.00-13.10',
'13.10-13.20',
'13.20-13.30',
'13.30-13.40',
'13.40-13.50',
'13.50-14.00',
'14.00-14.10',
'14.10-14.20',
'14.20-14.30',
'14.30-14.40',
'14.40-14.50',
'14.50-15.00',
'15.00-15.10',
'15.10-15.20',
'15.20-15.30',
'15.30-15.40',
'15.40-15.50',
'15.50-16.00',
'16.00-16.10',
'16.10-16.20',
'16.20-16.30',
'16.30-16.40',
'16.40-16.50',
'16.50-17.00',
'17.00-17.10',
'17.10-17.20',
'17.20-17.30',
'17.30-17.40',
'17.40-17.50',
'17.50-18.00',
'18.00-18.10',
'18.10-18.20',
'18.20-18.30',
'18.30-18.40',
'18.40-18.50',
'18.50-19.00',
'19.00-19.10',
'19.10-19.20',
'19.20-19.30',
'19.30-19.40',
'19.40-19.50',
'19.50-20.00',
'20.00-20.10',
'20.10-20.20',
'20.20-20.30',
'20.30-20.40',
'20.40-20.50',
'20.50-21.00',
'21.00-21.10',
'21.10-21.20',
'21.20-21.30',
'21.30-21.40',
'21.40-21.50',
'21.50-22.00',
'22.00-22.10',
'22.10-22.20',
'22.20-22.30',
'22.30-22.40',
'22.40-22.50',
'22.50-23.00',
'23.00-23.10',
'23.10-23.20',
'23.20-23.30',
'23.30-23.40',
'23.40-23.50'])

#grafico densità di Geid

colori=['navy','limegreen','darkorange']
aree=np.array([1.57,2.38,0.523,0.341,1.84,2.56,8.78])
plt.figure()
plt.subplots_adjust(top=1.0)#left=0.075, right=0.98, top=0.95,  bottom=0.22)
ax=[plt.plot(n[:,i]/aree[i], label=labels[i], linewidth=2.0, color=colori[i]) for i in range(0,3)]
plt.legend(fontsize=22)
plt.xlim(0,95)
plt.grid(axis='y', alpha=0.5)
plt.xticks(np.arange(0,95,12),ore[np.arange(0,95,12)], fontsize=18, rotation=25)
ml = MultipleLocator(1)
plt.axes().xaxis.set_minor_locator(ml)
plt.yticks(fontsize=22)
plt.ylabel('GeID/$km^2$',fontsize=26)
plt.legend(fontsize=26)
[plt.legend(label=labels[r]) for r in range(len(n[0]))]
plt.show()

#grafico confronto nodo castello vs others

plt.figure()
[plt.plot(Pi[r,1]*n.T[r], label = labels[r]) for r in range(len(n.T))if r!=1]
plt.legend(fontsize=26)
plt.xlim(0,94)
plt.title("Correnti GeID uscenti da 'Castello'", fontsize=25)
plt.xticks(np.arange(0,95,12),ore[np.arange(0,95,12)], fontsize=20, rotation=25)
plt.yticks(fontsize=20)
ml = MultipleLocator(1)
plt.axes().xaxis.set_minor_locator(ml)
plt.ylabel("corrente GeID", fontsize=23)
plt.grid(axis='y', alpha=.5)
plt.show()


#colorplot andamento totale
#plt.figure()
#ax = plt.imshow(np.log10(n.T), aspect='auto', cmap='plasma')
#plt.xticks(np.arange(0,95,12),ore[np.arange(0,95,12)],fontsize=20, rotation=25)
#ml = MultipleLocator(1)
#plt.axes().xaxis.set_minor_locator(ml)
#plt.yticks(np.arange(0,7),labels,fontsize=22)
#cbar = plt.colorbar(ax, ticks=[0, 0.5, 1, 1.5, 2, 2.4])
#cbar.ax.set_yticklabels(['1', '4', '10', '30', '100', '280'], fontsize=22)
#plt.show()


###grafico errore id vs pi
#plt.figure()
#plt.plot(pi_mis*100, color='blue' , label="[$\Pi_{ij}$ + c(t)] - Measure" )
#plt.plot(uno_mis*100, color='red', label="[Id +c(t)] - Measure")
#plt.xlim(0,6)
#plt.xticks(np.arange(0,7),labels, fontsize=26)
#plt.yticks(fontsize=20)
#plt.ylabel("Error percentage %", fontsize=26)
#plt.legend(loc ='best', fontsize=26)

##grafico presenze totali
plt.figure()
plt.subplots_adjust(top=1.0)#left=0.075, right=0.98, top=0.95,  bottom=0.15)
plt.xlim(0,95)
ml = MultipleLocator(1)
plt.axes().xaxis.set_minor_locator(ml)
plt.xticks(np.arange(0,95,12),ore[np.arange(0,95,12)], fontsize=20, rotation=25)
plt.yticks(fontsize=22)
plt.grid(axis='y', alpha=0.5)
plt.ylabel('GeID rilevati',fontsize=26)
plt.plot(N,'-o')

##matrice binarizzata per coeff negativi di Pi
#plt.figure()
#q = []
#q = np.where(Pi>0,1,-1)
#plt.yticks(np.arange(0,7),labels,fontsize=16)
#plt.xticks(np.arange(0,7),labels,fontsize=16)
#plt.imshow(q, aspect='auto', cmap='spring')
