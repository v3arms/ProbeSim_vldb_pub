### Simrank estimation program


#### Compile
In project directory:
```
bash compile.sh
```
#### Usage
```
./ssrank data_dir query_file [-o res_dir] [-e abs error] [-k topk_results]
```
Arguments:
 - `data_dir` - data directory, structure must be following :
 ```
 data_dir/
    graph.edgelist -- graph in edge list format(first line - number of nodes)
    id2node.json -- mappings real ids <---> matrix indexes
    node2id.json
 ```

  - `query_file` - file with ids to calculate singlesource simrank with. Structure :
```
id1
id2
...
idN
```
 - `-e` (optional) - float, max absolute error, `e = 0.005` if not specified
 - `-o` (optional) - path to directory to put result in (will be created if not exist). `path_to_data_dir_output`, if not specified
 - `-k` (optional) - number of pairs to put in result file in descending order by similarity value (all pairs will be calculated anyway). If not specified, all results will be placed to the file.
