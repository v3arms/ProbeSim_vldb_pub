from openpyxl import load_workbook
import scipy as sp
import scipy.sparse as scsp
import numpy as np
from scipy.io import savemat
import json
from scipy.io import loadmat
import networkx as nx
import sys
from os.path import splitext
import os


# input  - .xlsx file in format like nppro.lxlsx
# output - folder with graph matrix representation, can be used as input to the simrank program


if len(sys.argv) < 2 :
    print('Specify .xlsx')
    sys.exit(0)
data = load_workbook(filename=sys.argv[1], read_only=True)


id2node    = {}
node2id    = {}
i = []
j = []
v = []


cur_node = 0
datas = []
for rown, vals in enumerate(data.active.values) :
    if (rown == 0) :
        continue
    if (rown % 50000 == 0) :
        print(rown, " processed")

    if vals[0] not in id2node :
        id2node[vals[0]] = cur_node
        node2id[cur_node] = vals[0]
        cur_node = cur_node + 1
    
    if vals[2] not in id2node :
        id2node[vals[2]] = cur_node
        node2id[cur_node] = vals[2]
        cur_node = cur_node + 1
    
    datas.append(str(id2node[vals[0]]) + " " + str(id2node[vals[2]]))
    datas.append(str(id2node[vals[2]]) + " " + str(id2node[vals[0]]))

    i.append(id2node[vals[0]])
    j.append(id2node[vals[2]])
    i.append(id2node[vals[2]])
    j.append(id2node[vals[0]])
    v.append(1)
    v.append(1)


A = scsp.coo_matrix((v, (i, j))).tocsr()
A.sum_duplicates()
A.data[:] = 1


output_dir  = splitext(sys.argv[1])[0]

try :
    os.mkdir(output_dir)
except OSError :
    pass

output_file = "graph.edgelist"
G = nx.from_scipy_sparse_matrix(A, create_using=nx.DiGraph)

print('Adj matrix size :', A.shape)

nx.write_edgelist(G, output_dir + '/' + output_file, data=False)


f = open(output_dir + '/' + output_file, 'r')
s = f.read()
s = str(cur_node) + '\n' + s
f.close()
f = open(output_dir + '/' + output_file, 'w')
f.write(s)

f.flush()
f.close()

print("dump mappings..")
with open(output_dir + '/node2id.json', 'w') as f :
    json.dump(node2id, f)
with open(output_dir + '/id2node.json', 'w') as f :
    json.dump(id2node, f)
