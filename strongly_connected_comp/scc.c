#include <stdio.h>
#include <stdlib.h>
#include "../buffer/buffer.h"
#include "../index/index.h"
#include "../list/Pointer_List.h"
#include "../Bidirectional_BFS_SCC/bydir_SCC.h"

#include "scc.h"

scc * estimatedStronglyConnectComp(Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index, int current_out_ind_size, int current_in_ind_size, uint32_t bNode) {
	int i, a, j, c=0;	//Snoop Dogg
	int node1;		//What

	uint32_t curN;
	uint32_t dfsCount = 1;
	uint32_t *stack;	//What

	char *visited;		//I don't know
	

	list_node nod1, next1;	//What you've heard about me

	scc *SCC;		//But a bitch
	//infoSCC *inS;		//Can't get a dollar out of me

	printf("bNode = %u\n", bNode);

	visited = malloc((bNode+1)*sizeof(char));
	if(visited == NULL) {
		return NULL;
	}
	for(i=0; i<bNode+1; i++) {
		visited[i] = 48;	//ACII code for 0
	}

	stack = malloc((bNode+1)*sizeof(uint32_t));
	if(stack == NULL) {
		free(visited);
		return NULL;
	}
	for(j=0; j<bNode+1; j++) {
		stack[j] = -1;
	}

	//Algo TARZAN

	int found = 0;
	int k=0;

	SCC = malloc(sizeof(scc));
	//printf("What?\n");
	if(SCC == NULL) {
		//printf("What?\n");
		return NULL;
	}
	SCC->inS = malloc((bNode+1)*sizeof(infoSCC));
	if(SCC->inS == NULL) {
		free(stack);
		free(visited);
		return NULL;
	}

	uint32_t *par = malloc((bNode+1)*sizeof(uint32_t));
	if(par == NULL) {
		printf("Obzwlht\n");
		return NULL;
	}

	for(i=0; i<bNode+1; i++) {
		par[i] = -1;
		SCC->inS[i].lowLink = -1;
		SCC->inS[i].rank = -1;
		SCC->inS[i].onS = 0;
	}
	for(j=0; j<bNode+1; j++) {						//Finding first node that has at least ONE neighbour
		node1 = getListHead(out_index,j,current_out_ind_size);		//And starting the TARZAN algorithm from that MOFO
		if(node1>=0) {
			curN = j;						
			stack[0] = j;
			SCC->inS[j].rank = 0;	
			SCC->inS[j].onS = 1;
			SCC->inS[j].lowLink = 0;
			break;
		}
	}

	c=1;

	//printf("Starting from node = %d\n", c);

	SCC->comp_counts = 0;
	SCC->comp = NULL;
	SCC->id_belongs_to_comp = malloc((bNode+1)*sizeof(uint32_t));
	if(SCC->id_belongs_to_comp == NULL) {
		free(SCC);
		free(stack);
		free(visited);
		return NULL;
	}
	//printf("c= %u\n", SCC->comp_counts);

	for(i=0; i<bNode+1; i++) {
		SCC->id_belongs_to_comp[i] = -1;
	}

	printf("Master, starting TARZAN right now\n");

	//stack[c-1] = bNode;
	curN = stack[0];
	par[curN] = curN;
	int curP = 0;

	//int pos = -1;
	int polo = 0;
	visited[curN] = 49;

	while(c>0) {
		//Tarjan
		printf("stack[%d] = %u\n",c-1, stack[c-1]);
		//printf("with ll: %d and rank %d\n", SCC->inS[curN].lowLink, SCC->inS[curN].rank);
		found = 0;
		node1 = getListHead(out_index,curN,current_out_ind_size);
		if(node1>=0) {
			getListNode((out_buffer->base_addr)+node1,&nod1);
			for(j=0; j<NEIGHB; j++) {
				//printf("checking neighb: %u\n", nod1.neighbor[j]);
				if(nod1.neighbor[j] == -1) {
					break;
				}
				else if(visited[nod1.neighbor[j]] == 48) {
					printf("Not visisted before\n");
					visited[nod1.neighbor[j]] = 49;
					par[nod1.neighbor[j]] = curN;
					curN = nod1.neighbor[j];
					stack[c] = curN;
					c++;
					curP = c -1;
					SCC->inS[curN].rank = dfsCount;
					SCC->inS[curN].lowLink = dfsCount;
					SCC->inS[curN].onS = 1;
					dfsCount++;
					found = 1;
					break;
				}
				else if(SCC->inS[nod1.neighbor[j]].onS == 1) {
					//printf("Bro...\n");
					if(SCC->inS[nod1.neighbor[j]].lowLink < SCC->inS[curN].rank) {
						//printf("And you've got a better lowLink than us\n");
						SCC->inS[curN].lowLink = SCC->inS[nod1.neighbor[j]].lowLink;
					}
				}
			}
			if(found == 1) {
				continue;
			}
			else if(nod1.nextListNode>=0) {
				getListNode((out_buffer->base_addr)+nod1.nextListNode,&next1);
				while(1) {
					for (j=0;j<NEIGHB;j++) {
						//printf("checking neighb: %u\n", next1.neighbor[j]);
						if (next1.neighbor[j]==-1) {
							break;
						}
						else if(visited[next1.neighbor[j]] == 48) {
							//printf("undefined\n");
							par[next1.neighbor[j]] = curN;
							curN = next1.neighbor[j];
							visited[curN] = 49;
							stack[c] = curN;
							curP=c;
							c++;
							found = 1;
							SCC->inS[curN].rank = dfsCount;
							SCC->inS[curN].lowLink = dfsCount;
							SCC->inS[curN].onS = 1;
							dfsCount++;
							break;
						}
						else if(SCC->inS[next1.neighbor[j]].onS == 1) {
							//printf("Bro...\n");
							if(SCC->inS[next1.neighbor[j]].lowLink < SCC->inS[curN].rank) {
								//printf("Really bro?\n");
								SCC->inS[curN].lowLink = SCC->inS[next1.neighbor[j]].lowLink;
							}
						}
					}
					if(found == 1) {
						break;
					}
					a=next1.nextListNode;
					if (a>=0)
						getListNode((out_buffer->base_addr)+a,&next1);
					else
						break;
				}
			}
			if(found == 1) {
				continue;
			}
		}
		else {
			polo++;
			SCC->inS[curN].onS = 0;
			SCC->id_belongs_to_comp[curN] = SCC->comp_counts;
			SCC->comp_counts++;
			for(i=0; i<c; i++) {	
				if(stack[i] == par[curN]) {
					curN = stack[i];
					curP = i;
					break;
				}
			}
			stack[c-1] = -1;
			c--;
			continue;
		}

		if(SCC->inS[curN].lowLink == SCC->inS[curN].rank) {
			printf("Node %u has lowLink == rank\n", curN);
			while(1) {
				SCC->inS[stack[c-1]].onS = 0;
				SCC->id_belongs_to_comp[stack[c-1]] = SCC->comp_counts;
				if(stack[c-1] == curN) {
					stack[c-1] = -1;
					curN = par[curN];
					c--;
					SCC->comp_counts++;
					break;
				}
				stack[c-1] = -1;
				c--;
			}
			/*
			while(1) {
				SCC->inS[stack[c-1]].onS = 0;
				SCC->id_belongs_to_comp[stack[c-1]] = SCC->comp_counts;
				if(stack[c-1] == curN) {
					if(c-1 == 0) {
						stack[c-1] = -1;
						c--;
						SCC->comp_counts++;
						break;
					}
					for(i=0; i<c; i++) {	
						if(stack[i] == par[curN]) {
							curN = stack[i];
							curP = i;
							break;
						}
					}
					stack[c-1] = -1;
					c--;
					SCC->comp_counts++;
					break;
				}
				stack[c-1] = -1;
				c--;
			}
			*/
		}
		else {
			for(i=0; i<c; i++) {	
				if(stack[i] == par[curN]) {
					/*if(SCC->inS[par[curN]].lowLink > SCC->inS[curN].lowLink) {
						SCC->inS[par[curN]].lowLink = SCC->inS[curN].lowLink;
					}
					*/
					curN = stack[i];
					curP = i;
					//printf("Got back to %u\n", curN);
					break;
				}
			}	
		}

		if(c == 0) {
			for(i=k; i<bNode+1; i++) {
				if(visited[i] == 48) {
					par[i] = i;
					SCC->inS[i].lowLink = dfsCount;
					SCC->inS[i].rank = dfsCount;
					SCC->inS[i].onS = 1;
					visited[i] = 49;
					k=i;
					stack[0] = i;
					c++;
					curN = i;
					curP=0;
					dfsCount++;
					break;
				}
			}
		}
	}
	//////////////////////////////////s0//////////////////////dab/////////////////////////////////////////////////////////////////
	/*
	*/
	//Algo TARZAN done
	printf("bNode: %u\n", bNode);
	printf("polo=%d, c=%d\n", polo, c);
	printf("Master, TARZAN is done\n");
	
	//SCC->comp_counts--;
	printf("SCC counts: %u\n", SCC->comp_counts);

	getchar();

	for(j=0; j<bNode+1; j++) {
		printf("node %u belongs to %d SCC\n", j, SCC->id_belongs_to_comp[j]);
	}

	getchar();

	SCC->comp = malloc((SCC->comp_counts)*sizeof(comp));
	if(SCC->comp == NULL) {
		free(visited);
		free(stack);
		//free(inS);
		free(SCC);
		printf("Error: Couldn't create S.C.C. struct\n");
		return NULL;
	}

	printf("Malloc ok\n");

	for(j=0; j<bNode+1; j++) {
		SCC->comp[SCC->id_belongs_to_comp[j]].nodes_count++;
		//SCC->comp[SCC->inS[j].lowLink-1].component_id = SCC->inS[j].lowLink;
		//printf("id %d belongs to %d\n", j, SCC->id_belongs_to_comp[j]);
	}

	printf("1st\n");

	for(j=0; j<SCC->comp_counts; j++) {
		SCC->comp[j].head = NULL;
		SCC->comp[j].nc = NULL;
		SCC->comp[j].out_comps = 0;
		SCC->comp[j].nodes_count = 0;
		SCC->comp[j].component_id = j;
	}

	printf("2nd\n");

	for(j=0; j<bNode+1; j++) {
		SCC->comp[SCC->id_belongs_to_comp[j]].nodes_count++;
	}

	printf("Done 1\n");

	c=0;
	for(j=0; j<SCC->comp_counts; j++) {
		//printf("%u nodes at %u comp\n", SCC->comp[j].nodes_count, SCC->comp[j].component_id);
		SCC->comp[j].node_id = malloc((SCC->comp[j].nodes_count)*sizeof(uint32_t));
		//printf("What...\n");
		if(SCC->comp[j].node_id == NULL) {
			return NULL;
		}
		//printf("Master, Malloc OK\n");
		for(i=0; i<bNode+1; i++) {
			//printf("%d node belongs to %u comp\n", i, SCC->id_belongs_to_comp[i]);
			if(SCC->id_belongs_to_comp[i] == SCC->comp[j].component_id) {
				SCC->comp[j].node_id[c] = i;
				c++;
			}
		}
		c=0;
	}
	
	printf("Done 2\n");

	for(i=0; i<SCC->comp_counts; i++) {								//Gia ka8e comp
		//printf("%d Nodes on %d SCC\n", SCC->comp[i].nodes_count, i);
		for(j=0; j<SCC->comp[i].nodes_count; j++) {						//K' gia ka8e kombo mesa se auto to comp
			node1 = getListHead(out_index,SCC->comp[i].node_id[j],current_out_ind_size);
			if(node1>=0) {
				getListNode((out_buffer->base_addr)+node1,&nod1);
				for(c=0; c<NEIGHB; c++) {						//Pairnoume olous tous geitones tou
					if(nod1.neighbor[c] == -1) {
						break;
					}
					else if(SCC->id_belongs_to_comp[nod1.neighbor[c]] != i) {	//K' elegxoume to SCC sto opoio anhkoun
						//printf("Found a fo\n");
						//printf();
						if(SCC->comp[i].out_comps == 0) {
							SCC->comp[i].head = malloc(sizeof(neighb_comp));
							if(SCC->comp[i].head == NULL) {
								return NULL;
							}
							SCC->comp[i].head->next_SCC = SCC->id_belongs_to_comp[nod1.neighbor[c]];
							SCC->comp[i].head->next = NULL;
							SCC->comp[i].nc = SCC->comp[i].head;
						}
						else {
							SCC->comp[i].nc->next = malloc(sizeof(neighb_comp));
							if(SCC->comp[i].nc->next == NULL) {
								return NULL;
							}
							SCC->comp[i].nc = SCC->comp[i].nc->next;
							SCC->comp[i].nc->next = NULL;
							SCC->comp[i].nc->next_SCC = SCC->id_belongs_to_comp[nod1.neighbor[c]];
						}
						SCC->comp[i].out_comps++;
					}
				}
				if(nod1.nextListNode>=0) {
					getListNode((out_buffer->base_addr)+nod1.nextListNode,&next1);
					while(1) {
						for (c=0; c<NEIGHB; c++) {
							if (next1.neighbor[j]==-1) {
								break;
							}
							else if(SCC->id_belongs_to_comp[next1.neighbor[c]] != i) {
								printf("Shoudln't have happened\n");
								if(SCC->comp[i].out_comps == 0) {
									SCC->comp[i].head = malloc(sizeof(neighb_comp));
									if(SCC->comp[i].head == NULL) {
										return NULL;
									}
									SCC->comp[i].head->next_SCC = SCC->id_belongs_to_comp[next1.neighbor[c]];
									SCC->comp[i].head->next = NULL;
									SCC->comp[i].nc = SCC->comp[i].head;
								}
								else {
									SCC->comp[i].nc->next = malloc(sizeof(neighb_comp));
									if(SCC->comp[i].nc->next == NULL) {
										return NULL;
									}
									SCC->comp[i].nc = SCC->comp[i].nc->next;
									SCC->comp[i].nc->next = NULL;
									SCC->comp[i].nc->next_SCC = SCC->id_belongs_to_comp[next1.neighbor[c]];
								}
								SCC->comp[i].out_comps++;
							}
						}
						a=next1.nextListNode;
						if (a>=0)
							getListNode((out_buffer->base_addr)+a,&next1);
						else
							break;
					}
				}
			}
		}
	}

	//checking SCC data
	/*
	for(i=0; i<SCC->comp_counts; i++) {
		printf("SCC %d with\n", i);
		printf("and %d nodes inside the HYPER_NODE\n", SCC->comp[i].nodes_count);
		printf("Nodes: \n");
		for(j=0; j<SCC->comp[i].nodes_count; j++) {
			printf("Node %d inside this SCC\n", SCC->comp[i].node_id[j]);
		}
		printf("Total Neighbor SCC: %d\n", SCC->comp[i].out_comps);
		printf("NEXT\n");
	}
	*/

	free(visited);
	free(stack);
	free(par);

	printf("Master, dfs completed\n");

	return SCC;
}

int findNodeStronglyConnectedComponetID(scc * compts, uint32_t nodeId) {
	//printf("LoL: %d\n", compts->id_belongs_to_comp[nodeId]);
	return compts->id_belongs_to_comp[nodeId];
}

/*

OK_SUCCESS iterateStronglyConnectComponentID(scc * comps,  ComponentCursor * cursor) {
	return 0;
}

bool nextStronglyConnectedComponentID(scc * compts, ComponentCursor * cursor) {
	return 0;
}

*/

int estimateShortestPathStronglyConnectedComponents(scc * compts, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index, int current_out_ind_size, int current_in_ind_size, uint32_t source_node, uint32_t target_node, info_deikti *next_nodes1, info_deikti *next_nodes2) {
	int c1;
	int c2;

	c1 = findNodeStronglyConnectedComponetID(compts, source_node);
	c2 = findNodeStronglyConnectedComponetID(compts, target_node);

	printf("Source: %u at: %d compt\n", source_node, c1);
	printf("Target: %u at: %d compt\n", target_node, c2);

	if(c1 == -1) {
		printf("Den sas blepw lew oute kan\n");
		return -1;
	}
	else if(c2 == -1) {
		printf("Den sas blepw lew oute kan\n");
		return -1;
	}
	else if(c1 != c2) {
		printf("Master... Turn down what?!?\n");
		return -1;
	}
	else {
		int dist = mybfs_SCC(source_node, target_node, out_buffer, in_buffer, out_index, in_index, 
			 current_out_ind_size, current_in_ind_size, next_nodes1, next_nodes2, compts);
		return dist;
	}
}

int destroyStronglyConnectedComponents(scc * SCC) {
	int i;

	if(SCC != NULL) {
		if(SCC->comp != NULL) {
			for(i=0; i<SCC->comp_counts; i++) {
				if(SCC->comp[i].node_id != NULL) {
					free(SCC->comp[i].node_id);
				}
			}
			free(SCC->comp);
		}
		free(SCC->inS);
		free(SCC->id_belongs_to_comp);
		free(SCC);
	}

	return OK_SUCCESS;
}

