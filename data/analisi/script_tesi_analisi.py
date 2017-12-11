import matplotlib.pylab as plt
import scipy.linalg
import numpy as np
import pandas as pd
import os
from os import listdir
import math

sep = os.sep
local = os.path.abspath(".")
dirs = [f for f in listdir(local) if os.path.isdir(f)]
files = [f for f in listdir(local) if os.path.isfile(f)]
file_dir = local + sep + dirs[0] + sep

pi = []
c_dt_mk = file_dir + "C_Dt_mk_8002350.txt"
c_zero = file_dir + "C_zero_8002350.txt"
m_ki = file_dir + "Mk_8002350.txt"
registro = file_dir + "check_8002350.txt"
fattore = file_dir + "vettore_ct.txt"

data_all = np.asarray(pd.read_csv(registro, sep = "\t", index_col=0, header=-1))
data_cross = np.asarray(pd.read_csv(c_dt_mk, sep ="\t", header=-1).drop(7,1))
data_c_zero = np.asarray(pd.read_csv(c_zero, sep="\t", header=-1).drop(7,1))
data_m = np.asarray(pd.read_csv(m_ki, sep="\t", header=-1))
vettore_c = np.asarray(pd.read_csv(fattore, sep="\t", header=-1))
eps=np.diag(1.e-2*np.ones(7))
data_covaeps = data_c_zero + eps
pi = np.dot(data_cross,np.linalg.inv(np.transpose(data_c_zero)))
np.savetxt("Pij_8002350.txt", pi, fmt='%4f', delimiter="\t")


torevet = np.transpose(vettore_c)
vet_c= np.delete(torevet, len(torevet)-1)

dati_pi_c = []
dati_solo_c = []
r= len(data_all)
for i in range (0,r):
    dati_pi_c.append(np.asarray((np.dot(pi,data_all[i][:]) + vet_c[i]*data_all[i])))
    dati_solo_c.append(data_all[i]+vet_c[i]*data_all[i])

effect = np.asarray(dati_pi_c)
identity = np.asarray(dati_solo_c)

##a questo punto ho creato i registri calcolati per le popolazioni considerando sia le trasformazioni
#pij*n_j(t) + c(t)n_j(t) sia quelle con 1*n_j(t)+c(t)*n_j(t): le chiamo dati_pi_c e dati_solo_c

conf_pi_mis = []
conf_uno_mis = []
for i in range (0,r-1):
    conf_pi_mis.append(((dati_pi_c[i]-data_all[i+1])*(dati_pi_c[i]-data_all[i+1]))/(data_all[i+1]*data_all[i+1]))
    conf_uno_mis.append(((dati_solo_c[i]-data_all[i+1])*(dati_solo_c[i]-data_all[i+1]))/(data_all[i+1]*data_all[i+1]))        
pi_mis = np.asarray(conf_pi_mis)
uno_mis = np.asarray(conf_uno_mis)

pi_c = []
uno_c = []
k = len(data_all[0])
for i in range(0,k):
        pi_c.append(np.sqrt(np.sum(pi_mis[:][i])))
        uno_c.append(np.sqrt(np.sum(uno_mis[:][i])))

plt.plot(pi_c, color='blue' )
plt.plot(uno_c, color='red')
