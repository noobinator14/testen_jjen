#include <stdio.h>
#include <stdlib.h>
#include "../index/index.h"
#include "../buffer/buffer.h"
#include "../strongly_connected_comp/scc.h"
#include "../grail/grail.h"
#include "bidir_grail.h"

extern long GLOBAL_GRAIL_VISIT;

int bidir_grail (uint32_t from, uint32_t to, Buffer *out_buffer, Buffer * in_buffer, NodeIndex * out_index, NodeIndex * in_index,
		int current_out_ind_size, int current_in_ind_size, info_deikti *next_nodes1, info_deikti *next_nodes2, long *visited, long visit_size, 
		GrailIndex *grail_index, scc *components) {


	int node1, node2, i, a, distance1=1, distance2=1, current1, offset1, current2, offset2, shortest;
	uint32_t last_expanded_node1=-1, last_expanded_node2=-1;
	list_node nod1, nod2, next1, next2;
	unsigned char *checked;
	GRAIL_ANSWER grail_answer;

	if (from==to)
		return 0;
	node1=getListHead(out_index,from,current_out_ind_size);
	if (node1==-1 || node1==-2)				// node1 doesn't exist
		return -1;
	getListNode((out_buffer->base_addr)+node1,&nod1);	// make sure node1 has at least 1 outgoing edge
	if (nod1.neighbor[0]==-1)
		return -1;
	node2=getListHead(in_index,to,current_in_ind_size);
	if (node2==-1 || node2==-2)				// node2 doesn't exist
		return -1;
	getListNode((in_buffer->base_addr)+node2,&nod2);	// make sure node2 has at least 1 ingoing edge
	if (nod2.neighbor[0]==-1)
		return -1;


	if(current_out_ind_size>current_in_ind_size) {
		checked = malloc(sizeof(unsigned char)*current_out_ind_size);
		if(checked == NULL) {
			printf("FAIL: malloc of 'checked'\n");
			return -2;
		}
		for(i=0; i<current_out_ind_size; i++)
			checked[i] = 0;
	}
	else {
		checked = malloc(sizeof(unsigned char)*current_in_ind_size);
		if(checked == NULL) {
			printf("FAIL: malloc of 'checked'\n");
			return -2;
		}
		for(i=0; i<current_in_ind_size; i++)
			checked[i] = 0;
	}

/* Elegxos se ba8os 1 apo thn afethria */
	checked[from]=1;
	for (i=0;i<NEIGHB;i++) {
		if (nod1.neighbor[i]==-1)
			break;
		else if (nod1.neighbor[i]==to) {
			free(checked);
			return 1;
		}
		else if (nod1.neighbor[i]>=0){
			checked[nod1.neighbor[i]] = 1;
			grail_answer=isReachableGrailIndex(grail_index,components,nod1.neighbor[i],to);
			if (grail_answer==YES) {
				free(checked);
				info_deikti next_nodes3=LIST_dimiourgia();
				info_deikti next_nodes4=LIST_dimiourgia();
				if (next_nodes3==NULL || next_nodes4==NULL)
					return -2;
				shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,in_buffer,out_index,in_index, 								current_out_ind_size,current_in_ind_size,nod1.neighbor[i],to,&next_nodes3,&next_nodes4);
				LIST_katastrofi(&next_nodes3);
				LIST_katastrofi(&next_nodes4);
				return (1+shortest);
			}
			else if (grail_answer==NO) {
				free(checked);
				return -1;
			}
			if ( (eisagogi_telos(next_nodes1,nod1.neighbor[i])) < 0) {
				free(checked);
				return -2;
			}
		}
	}
	if (nod1.nextListNode>=0) {
		getListNode((out_buffer->base_addr)+nod1.nextListNode,&next1);
		while (1) {
			for (i=0;i<NEIGHB;i++) {
				if (next1.neighbor[i]==-1)
					break;
				else if (next1.neighbor[i]==to) {
					free(checked);
					return 1;
				}
				else if (next1.neighbor[i]>=0) {
					checked[next1.neighbor[i]] = 1;
					grail_answer=isReachableGrailIndex(grail_index,components,next1.neighbor[i],to);
					if (grail_answer==YES) {
						free(checked);
						info_deikti next_nodes3=LIST_dimiourgia();
						info_deikti next_nodes4=LIST_dimiourgia();
						if (next_nodes3==NULL || next_nodes4==NULL)
							return -2;
						shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,in_buffer,out_index,in_index, 									current_out_ind_size,current_in_ind_size,next1.neighbor[i],to,&next_nodes3,&next_nodes4);
						LIST_katastrofi(&next_nodes3);
						LIST_katastrofi(&next_nodes4);
						return (1+shortest);
					}
					else if (grail_answer==NO) {
						free(checked);
						return -1;
					}
					if ( (eisagogi_telos (next_nodes1,next1.neighbor[i])) < 0) {
						free(checked);
						return -2;
					}
				}
			}
			a=next1.nextListNode;
			if (a>=0)
				getListNode((out_buffer->base_addr)+a,&next1);
			else
				break;
		}
	}
	if (LIST_keni(*next_nodes1)) {
		free(checked);
		return -1;
	}
	last_expanded_node1=from;

/* Elegxos se ba8os 1 apo ton kombo proorismou */
	checked[to]=2;
	for (i=0;i<NEIGHB;i++) {
		if (nod2.neighbor[i]==-1)
			break;
		else if (nod2.neighbor[i]>=0){
			if(checked[nod2.neighbor[i]] == 1) {
				free(checked);
				return 2;
			}
			else
				checked[nod2.neighbor[i]] = 2;
			grail_answer=isReachableGrailIndex(grail_index,components,from,nod2.neighbor[i]);
			if (grail_answer==YES) {
				free(checked);
				info_deikti next_nodes3=LIST_dimiourgia();
				info_deikti next_nodes4=LIST_dimiourgia();
				if (next_nodes3==NULL || next_nodes4==NULL)
					return -2;
				shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,in_buffer,out_index,in_index, 								current_out_ind_size,current_in_ind_size,from,nod2.neighbor[i],&next_nodes3,&next_nodes4);
				LIST_katastrofi(&next_nodes3);
				LIST_katastrofi(&next_nodes4);
				return (1+shortest);
			}
			else if (grail_answer==NO) {
				free(checked);
				return -1;
			}
			if ( (eisagogi_telos(next_nodes2,nod2.neighbor[i])) < 0) {
				free(checked);
				return -2;
			}
		}
	}
	if (nod2.nextListNode>=0) {
		getListNode((in_buffer->base_addr)+nod2.nextListNode,&next2);
		while (1) {
			for (i=0;i<NEIGHB;i++) {
				if (next2.neighbor[i]==-1)
					break;
				else if (next2.neighbor[i]>=0) {
					if(checked[next2.neighbor[i]] == 1) {
						free(checked);
						return 2;
					}
					else
						checked[next2.neighbor[i]] = 2;
					grail_answer=isReachableGrailIndex(grail_index,components,from,next2.neighbor[i]);
					if (grail_answer==YES) {
						free(checked);
						info_deikti next_nodes3=LIST_dimiourgia();
						info_deikti next_nodes4=LIST_dimiourgia();
						if (next_nodes3==NULL || next_nodes4==NULL)
							return -2;
						shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,in_buffer,out_index,in_index, 								current_out_ind_size,current_in_ind_size,from,next2.neighbor[i],&next_nodes3,&next_nodes4);
						LIST_katastrofi(&next_nodes3);
						LIST_katastrofi(&next_nodes4);
						return (1+shortest);
					}
					else if (grail_answer==NO) {
						free(checked);
						return -1;
					}
					if ( (eisagogi_telos (next_nodes2,next2.neighbor[i])) < 0) {
						free(checked);
						return -2;
					}
				}
			}
			a=next2.nextListNode;
			if (a>=0)
				getListNode((in_buffer->base_addr)+a,&next2);
			else
				break;
		}
	}
	if (LIST_keni(*next_nodes2)) {
		free(checked);
		return -1;	
	}
	last_expanded_node2=to;

	if (from<visit_size && to<visit_size) {			// mark as visited
		visited[from]=GLOBAL_GRAIL_VISIT;
		visited[to]=GLOBAL_GRAIL_VISIT;
	}
	else {
		printf("FAIL: bidir_grail out of 'visited' bounds\n");
		free(checked);
		return -2;
	}

/* shmadi gia au3hsh va8ous */
	if ( (eisagogi_telos(next_nodes1,-3) ) < 0 ) {
		free(checked);
		return -2;
	}
	if ( (eisagogi_telos(next_nodes2,-3) ) < 0 ) {
		free(checked);
		return -2;
	}

/* Elegxos se megalutero ba8os */
	while(1) {
		//Katebainoume epipedo apo A
		distance1++;
		current1=eksagogi_arxi(next_nodes1);
		while (current1!=-3) {
			if (visited[current1] < GLOBAL_GRAIL_VISIT) {
				offset1=getListHead(out_index,current1,current_out_ind_size);
				if (offset1>=0) {
					getListNode((out_buffer->base_addr)+offset1,&nod1);
					for (i=0;i<NEIGHB;i++) {
						if (nod1.neighbor[i]==-1)
							break;
						else if (nod1.neighbor[i]>=0) {
							if(checked[nod1.neighbor[i]] == 2) {
								free(checked);
								return distance1+distance2;
							}
							else
								checked[nod1.neighbor[i]] = 1;
							grail_answer=isReachableGrailIndex(grail_index,components,nod1.neighbor[i],last_expanded_node2);
							if (grail_answer==YES) {
								free(checked);
								info_deikti next_nodes3=LIST_dimiourgia();
								info_deikti next_nodes4=LIST_dimiourgia();
								if (next_nodes3==NULL || next_nodes4==NULL)
									return -2;
								shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,
										in_buffer,out_index,in_index,current_out_ind_size,current_in_ind_size,
										nod1.neighbor[i],last_expanded_node2,&next_nodes3,&next_nodes4);
								LIST_katastrofi(&next_nodes3);
								LIST_katastrofi(&next_nodes4);
								return (shortest+distance1+distance2-1);
							}
							else if (grail_answer==NO) {
								free(checked);
								return -1;
							}
							if ( (eisagogi_telos(next_nodes1,nod1.neighbor[i])) < 0) {
								free(checked);
								return -2;
							}
						}
					}
					if (nod1.nextListNode!=-1) {
						getListNode((out_buffer->base_addr)+nod1.nextListNode,&next1);
						while (1) {
							for (i=0;i<NEIGHB;i++) {
								if (next1.neighbor[i]==-1)
									break;
								else if (next1.neighbor[i]>=0) {
									if(checked[next1.neighbor[i]] == 2) {
										free(checked);
										return distance1+distance2;
									}
									else
										checked[next1.neighbor[i]] = 1;
									grail_answer=isReachableGrailIndex(grail_index,components,next1.neighbor[i],
													last_expanded_node2);
									if (grail_answer==YES) {
										free(checked);
										info_deikti next_nodes3=LIST_dimiourgia();
										info_deikti next_nodes4=LIST_dimiourgia();
										if (next_nodes3==NULL || next_nodes4==NULL)
											return -2;
										shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,
											in_buffer,out_index,in_index,current_out_ind_size,current_in_ind_size,
											next1.neighbor[i],last_expanded_node2,&next_nodes3,&next_nodes4);
										LIST_katastrofi(&next_nodes3);
										LIST_katastrofi(&next_nodes4);
										return (shortest+distance1+distance2-1);
									}
									else if (grail_answer==NO) {
										free(checked);
										return -1;
									}
									if ( (eisagogi_telos (next_nodes1,next1.neighbor[i])) < 0) {
										free(checked);
										return -2;
									}
								}
							}
							a=next1.nextListNode;
							if (a!=-1)
								getListNode((out_buffer->base_addr)+a,&next1);
							else
								break;
						}
					}
					last_expanded_node1=current1;
				}
				if (current1<visit_size)
					visited[current1]=GLOBAL_GRAIL_VISIT;
				else {
					printf("FAIL: bidir_grail out of 'visited' bounds\n");
					free(checked);
					return -2;
				}
			}

			if (LIST_keni(*next_nodes1)) {
				free(checked);
				return -1;
			}

			current1=eksagogi_arxi(next_nodes1);
		}

		if (LIST_keni(*next_nodes1)) {
			free(checked);
			return -1;
		}
		if ( (eisagogi_telos (next_nodes1,-3)) < 0) {
			free(checked);
			return -2;
		}

		//Anebainoume epipedo apo B
		distance2++;
		current2=eksagogi_arxi(next_nodes2);
		while (current2!=-3) {
			if (visited[current2] < GLOBAL_GRAIL_VISIT) {
				offset2=getListHead(in_index,current2,current_in_ind_size);
				if (offset2>=0) {
					getListNode((in_buffer->base_addr)+offset2,&nod2);
					for (i=0;i<NEIGHB;i++) {
						if (nod2.neighbor[i]==-1)
							break;
						else if (nod2.neighbor[i]>=0) {
							if(checked[nod2.neighbor[i]] == 1) {
								free(checked);
								return distance1+distance2;
							}
							else
								checked[nod2.neighbor[i]] = 2;
							grail_answer=isReachableGrailIndex(grail_index,components,nod2.neighbor[i],last_expanded_node1);
							if (grail_answer==YES) {
								free(checked);
								info_deikti next_nodes3=LIST_dimiourgia();
								info_deikti next_nodes4=LIST_dimiourgia();
								if (next_nodes3==NULL || next_nodes4==NULL)
									return -2;
								shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,
										in_buffer,out_index,in_index,current_out_ind_size,current_in_ind_size,
										nod2.neighbor[i],last_expanded_node1,&next_nodes3,&next_nodes4);
								LIST_katastrofi(&next_nodes3);
								LIST_katastrofi(&next_nodes4);
								return (shortest+distance1+distance2-1);
							}
							else if (grail_answer==NO) {
								free(checked);
								return -1;
							}
							if ( (eisagogi_telos(next_nodes2,nod2.neighbor[i])) < 0) {
								free(checked);
								return -2;
							}
						}
					}
					if (nod2.nextListNode!=-1) {
						getListNode((in_buffer->base_addr)+nod2.nextListNode,&next2);
						while (1) {
							for (i=0;i<NEIGHB;i++) {
								if (next2.neighbor[i]==-1)
									break;
								else if (next2.neighbor[i]>=0) {
									if(checked[next2.neighbor[i]] == 1) {
										free(checked);
										return distance1+distance2;
									}
									else
										checked[next2.neighbor[i]] = 2;
									grail_answer=isReachableGrailIndex(grail_index,components,next2.neighbor[i],
													last_expanded_node1);
									if (grail_answer==YES) {
										free(checked);
										info_deikti next_nodes3=LIST_dimiourgia();
										info_deikti next_nodes4=LIST_dimiourgia();
										if (next_nodes3==NULL || next_nodes4==NULL)
											return -2;
										shortest=estimateShortestPathStronglyConnectedComponents(components,out_buffer,
											in_buffer,out_index,in_index,current_out_ind_size,current_in_ind_size,
											next2.neighbor[i],last_expanded_node1,&next_nodes3,&next_nodes4);
										LIST_katastrofi(&next_nodes3);
										LIST_katastrofi(&next_nodes4);
										return (shortest+distance1+distance2-1);
									}
									else if (grail_answer==NO) {
										free(checked);
										return -1;
									}
									if ( (eisagogi_telos (next_nodes2,next2.neighbor[i])) < 0) {
										free(checked);
										return -2;
									}
								}
							}
							a=next2.nextListNode;
							if (a!=-1)
								getListNode((in_buffer->base_addr)+a,&next2);
							else
								break;
						}
					}
					last_expanded_node2=current2;
				}
				if (current2<visit_size)
					visited[current2]=GLOBAL_GRAIL_VISIT;
				else {
					printf("FAIL: bidir_grail out of 'visited' bounds\n");
					free(checked);
					return -2;
				}
			}

			if (LIST_keni(*next_nodes2)) {
				free(checked);
				return -1;
			}

			current2=eksagogi_arxi(next_nodes2);
		}

		if (LIST_keni(*next_nodes2)) {
			free(checked);
			return -1;
		}
		if ( (eisagogi_telos (next_nodes2,-3)) < 0) {
			free(checked);
			return -2;
		}

	}
	free(checked);
	return -1;						// path does not exist
}
