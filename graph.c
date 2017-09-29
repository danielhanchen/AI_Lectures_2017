// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	Edge new = {v,w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->edges[w][v] = wt;
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int v, w;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; new->nE = 0;
	new->edges = malloc(nV*sizeof(int *));
	assert(new->edges != 0);
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc(nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	// not needed for this lab
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n",v,names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n",names[w],g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert(g != NULL);
	
	// Set the first item to be the start
	path[0] = src;
	
	// If same location, then terminate
	if (src == dest) return 1;

	Queue q = newQueue();
	QueueJoin(q, src);
	
	int city, x, d, route = 0;
	
	// Use malloc. Memset - buffer size == malloced size
	int *map = malloc(sizeof(int)*g->nV);
	memset(map, -1, sizeof(int)*g->nV);
	int *reverse = malloc(sizeof(int)*g->nV);
	
	// Reference the starting pos as -2
	map[src] = -2;
	
	// If queue not empty and route does NOT yet exist, continue
	while (!QueueIsEmpty(q) && route != 1) {
	
	    // Let the city you are at leave queue
	    city = QueueLeave(q);
	    
	    // Check each connection if it exists
	    for (x = 0; x < g->nV; x++) {
	    
	        // Get distance FROM city->x
	        d = g->edges[city][x];
	        
	        // If the cond met AND not going back (circular)
	        // AND the mapped array has NOT been visited
	        if (d < max && map[x] == -1 && x != city) {
                
	            map[x] = city;
	            
	            if (x == dest) {
	                route = 1;
	                break;
	            }
	            QueueJoin(q, x);
	        }
	    }
    }
    dropQueue(q);
    d = 0;
    
    if (route == 1) {
    
        // Go backwards in map and update reversed path
        for (x = dest; map[x] != -2; x = map[x]) 
            reverse[d++] = x;
            
        // Update the real path
        for (x = 0; x < d; x++) 
            path[x+1] = reverse[d-x-1];
        
    }
    free(map);
    free(reverse);
    
    if (route == 1) return d+1;
    return 0;
}



