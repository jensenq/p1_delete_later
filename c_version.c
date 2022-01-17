/* libraries:
limits and stdlib are just some macros, stdio needed for I/O
USE standard
*/
#include<limits.h>
#include<stdlib.h>
#include<stdio.h>

//mode edge = struct (int sourceVertex, destVertex, edgeWeight);
typedef struct{
    int sourceVertex, destVertex;
    int edgeWeight;
}edge;

//mode graph = struct (int vertices, edges, flex [1]edge edgematrix);
typedef struct{
    int vertices, edges;
    edge* edgeMatrix;
}graph;

/*
proc loadGraph = (string filename) graph:
begin
  file f;
  open (f, filename, stand in channel);

  graph G;
  get(f, (vertices of G, edges of G, new line));

  int srcV;
  int dstV;
  int weight;
  for i from 0 to edges of G do
    get(f, (srcV, dstV, weight, new line))
    sourceVertex of edgeMatrix[i] of G := srcV;
    destVertex of edgeMatrix[i] of G := dstV;
    edgeWeight of edgeMatrix[i] of G := weight;
  od

  close(f);
  G;

end;
*/
graph loadGraph(char* fileName){
    FILE* fp = fopen(fileName,"r");
 
    graph G;
    int i;
 
    // read 1st line, two ints. "4 5"
    fscanf(fp,"%d%d",&G.vertices,&G.edges);
 
    G.edgeMatrix = (edge*)malloc(G.edges*sizeof(edge));
 
    for(i=0;i<G.edges;i++)
        // read each line, 3 ints, "3 4 2"
        fscanf(fp,"%d%d%d",&G.edgeMatrix[i].sourceVertex,&G.edgeMatrix[i].destVertex,&G.edgeMatrix[i].edgeWeight);
 
    fclose(fp);
 
    return G;
}

/*
proc floydWarshall = (graph g) void:
begin
end;
*/
void floydWarshall(graph g){
    // [0: vertices of g, 0: vertices of g ] int processWeights, processedVertices
    // int i,j,k;
    int processWeights[g.vertices][g.vertices], processedVertices[g.vertices][g.vertices];
    int i,j,k;
 
    //  FOR i FROM 0 WHILE i< vertices of g DO
    //    FOR j FROM 0 WHILE j< vertices of g DO
    //      processWeights[i,j] := 65535 // hard-coding max short lol.
    //      processedVertices[i,j] := if i!=j then j+1 else 0 fi
    //    OD
    //  OD
    //    
    for(i=0;i<g.vertices;i++)
        for(j=0;j<g.vertices;j++){
            processWeights[i][j] = SHRT_MAX;
            processedVertices[i][j] = (i!=j)?j+1:0;
        }
 
    //  FOR i FROM 0 WHILE i< edges of g DO
    //    int x := sourceVertex of (edgeMatrix of g [i]) -1
    //    int y := destVertex of (edgeMatrix of g [i]) -1
    //    processWeights[x,y] := edgeWeight of (edgeMatrix of g [i])
    //  OD
    for(i=0;i<g.edges;i++)
        processWeights[g.edgeMatrix[i].sourceVertex-1][g.edgeMatrix[i].destVertex-1] = g.edgeMatrix[i].edgeWeight;
 

    //  FOR i FROM 0 WHILE i< vertices of g DO
    //    FOR j FROM 0 WHILE j< vertices of g DO
    //      FOR k FROM 0 WHILE k< vertices of g DO
    //        if processWeights[j,i] + processWeights[i,k] < processWeights[j,k] then
    //          processWeights[j,k] = processWeights[j,i] + processWeights[i,k];
    //          processedVertices[j,k] = processedVertices[j,i];
    //        fi
    //      OD
    //    OD
    //  OD
    //        
    for(i=0;i<g.vertices;i++)
        for(j=0;j<g.vertices;j++)
            for(k=0;k<g.vertices;k++){
                if(processWeights[j][i] + processWeights[i][k] < processWeights[j][k]){
                    processWeights[j][k] = processWeights[j][i] + processWeights[i][k];
                    processedVertices[j][k] = processedVertices[j][i];
                }
            }
    //  printf("pair    dist   path");
    //  FOR i FROM 0 WHILE i< vertices of g DO
    //    FOR j FROM 0 WHILE j< vertices of g DO
    //      if i!=j then
    //        printf()
    //        k := i+1
    //        WHILE j!=k+1  DO
    //          k := processedVertices[k-1,j]
    //          // algol68 is 1 based not 0 based - On your print inside the while loop do k-1
    //          printf()
    //        OD
    //      fi
    //    OD
    //  OD
    //  printf("\n")
    printf("pair    dist   path");
    for(i=0;i<g.vertices;i++)
        for(j=0;j<g.vertices;j++){
            if(i!=j){
                printf("\n%d -> %d %3d %5d",i+1,j+1,processWeights[i][j],i+1);
                k = i+1;
                do{
                    k = processedVertices[k-1][j];
                    printf("->%d",k);
                }while(k!=j+1);
            }
        }
    printf("\n");
}


/*
main:(
  argv(#)
  if argc!=2 then 
    printf()
  else
    floydWarshall(loadGraph(argv(1)));
    0
)
*/
int main(int argC,char* argV[]){
    if(argC!=2)
      printf("Usage : %s <name of file containing graph data>\n",argV[0]);
    else
        floydWarshall(loadGraph(argV[1]));
    return 0;
}
 
