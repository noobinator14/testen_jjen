#include <stdio.h>
#include <stlib.h>
#include <string.h>
#include <stdint.h>
#include "./index/index.h"
#include "./buffer/buffer.h"
#include "./Bidirectional_BFS_SCC/bydir_SCC.h"
#include "./list/Pointer_List.h"
#include "./strongly_connected_comp/scc.h"


GrailIndex * buildGrailIndex(NodeIndex* out_index,Buffer *out_buffer, int current_out_ind_size, scc* components) {
	int * onS = NULL;

	GrailIndex * index = NULL;

	uint32_t dfsCount = 1;						//Δίνει τιμές στο rank
	uint32_t cur_mr = 1;						//Δίνει τιμές στο min_rank
	uint32_t checkedSCC = 0;					//Νομίζω δεν χρησιμοποιείται... το τσεκάρω
	uint32_t curSCC;						//Το SCC Pου ελέγχουμε αυτή τη στιγμή
	uint32_t * par = NULL;						//Toyw de;iktew toyw ejhg;v parak;atv
	uint32_t * stack = NULL;

	char *visited = NULL;

	onS = malloc(sizeof(int)*components->compt_counts);		//Πίνακας που κρατάει αν υπάρχει μεσα στη stack το συγκεκριμένο component
	if(onS == NULL) {
		return NULL;
	}
	for(i=0; i<components->compt_counts; i++) {
		onS[i] = 0;						//0 NO, 1 YES
	}

	visited = malloc(sizeof(char)*components->compt_counts);	//Όμοια, πίνακας ο οποίος κρατάει τιμές για κάθε comp Που έχουμε επισκεφτεί
	if(visited == NULL) {
		free(onS);
		return NULL;
	}
	for(i=0; i<components->compt_counts; i++) {
		visited[i] = 48; 					//48 (χαρακτήρας 0) ΝΟ, 49 (χαρακτήρας 1) ΥΕΣ
	}

	par = malloc(sizeof(uint32_t)*components->compt_counts);	//Όμοις, πινακας που κρατάει τον πατέρα του συγκεκριμένου SCC
	if(par == NULL) {						//Από ποιο comp πήγαμε στο curSCC
		free(onS);
		free(visited);
		return NULL;
	}
	for(i=0; i<components->compt_counts; i++) {
		par[i] = -1;						//Αρχικοποίηση σε -1, λόγοι ασφαλείας
	}


	stack = malloc(sizeof(uint32_t)*components->compt_counts);	//Στοίβα που αποθηκεύει SCC
	if(stack == NULL) {						//Κρατάμε μεταβλητή για την πρώτη άδεια θέση
		free(onS);
		free(visited);
		free(par);
		return NULL;
	}
	for(i=0; i<components->compt_counts; i++) {
		stack[i] = -1;						//Αρχικοποίηση σε -1, λόγοι αφαλείας
	}

	index = malloc(malloc(sizeof(GrailIndex)*components->compt_counts););
	if (index == NULL) {
		fee(onS);
		free(par);
		free(visited);
		fee(stack);
		printf("FAIL: malloc Grail\n");
		return NULL;
	}

	//Αρχικοποίηση των HYPER_NODES (min_rank, rank, nodes in this HYPER_NODE)

	for(i=0; i<components->compt_counts; i++) {
		index[i].values.nodesHY = malloc(component->comp[i].nodes_count*sizeof(uint32_t));	//Δημιουργία
		index[i].values.min_rank = 0;								//Κ' αρχικοποιήση τιμών του HYPERNODE
		index[i].values.rank = 0;								//ΠΕΡΝΑΜ ΤΙΣ ΤΙΜΕΣ ΓΙΑ ΤΟ POST_ORDER
		if(index[i].values.nodeHY == NULL) {
			free(index);
			return NULL;
		}
		for(j=0; j<component->comp[i].nodes_count; j++) {					//Κ' στην συνέχεια
			index[i].values.nodeHY[j] = components->comp[j].node_id;			//Όλους τους κόμβους που περιέχονται στο συγκεκριμένο SCC
		}
		index[i].head = NULL;									//Λίστες με τις εξερχόμενες ακμές (π.χ. SCC1 -> SCC2)
		index[i].head = components->comp[i].head;						//Head την αρχή
		index[i].cur = index[i].head;								//Tο χρησιμοποιούμε για την προσπέλαση των στοιχείων
	}

	//Play that fucking music (post_order YEAH)
	//Ολόιδιο με του Tarjan... Με τη διαφορά ότι ο τελευταίος κόμβος παίρνει τις τιμές (1,1) (λογική -1%)

	uint32_t c = 1;
	curSCC = 0;		//Καλά μαλάκες
	visited[0] = 49;	//Μας βάζετε να κάνουμε τον Ταρζαν ΑΠΟ ΤΗΝ ΑΡΧΗ
	par[0] = 0;		//ΜΕ μόνη αλλαγή ότι οι τιμές πάνε ανάποδα;
	stack[0] = 0;		//Είστε σοβαροί;
	onS[0] = 1;		//....
	while(c>0) {
		if(components->comp[i].out_compts == 0) {		//Δεν έχει παιδιά
			index[curSCC].values.min_rank = dfsCount;	//Περνάμε απλά τιμές
			index[curSCC].values.rank = dfsCount;
			checkedSCC++;
			dfsCount++;					//Αυξάνυμε το βάθος
			curSCC = par[curSCC];				//Πάμε στον πατέρα
			stack[c-1] = -1;				//Εξαγωγή από τη λίστα
			c--;
		}
		else {							//Έχει compts να τσεκάρει ακόμα...
			found = 0;
			while(index[curSCC].cur != NULL) {				//Για κάθε γείτονα compt
				if(visite[index[curSCC].cur->next_SCC] == 48) {		//Υπάρχει έναςHYPERNODE που δεν έχουμε επισκεφτεί
					onS[index[curSCC].cur->next_SCC] = 1;		//Let's get it on
					found = 1;
					visited[index[curSCC].cur->next_SCC] = 49;
					par[index[curSCC].cur->next_SCC] = curSCC;
					index[curSCC].cur = index[curSCC].cur->next;
					curSCC = index[curSCC].cur->next_SCC;
					stack[c] = curSCC;
					c++;
					break;
				}
			}
			if(found == 1) {						//Αν βρεθεί ένας που δεν έχουμε μπει
				continue;						//Ε μπες, θες κ' σχόλιο εδώ...
			}
			else {								//Αλλιώς, σημαίνει ότι δεν έχει άλλους γείτονες να τσεκάρει
				onS[curSCC] = 0;					//Πέρνα τα στοιχεία που χρειαζόμαστε
				stack[c-1] = -1;
				c--;
				index[curSCC].values.min_rank = cur_mr;
				index[curSCC].values.rank = dfsCount;
				curSCC = par[curSCC];
				checkedSCC++;
				dfsCount++;
			}
		}

		if(c == 0) {								//Σε περίπτωση που η λίστα μας αδειασε, τσεκάρουμε
			for(i=0; i<components->compt_counts; i++) {			//Αν υπάρχει κάποιος που δεν έχουμε μπει
				if(visited[i] == 48) {					//Αν ναι, τα ίδια
					checkedSCC++;					//Αλλιώς, σημαίνει ότι έχουμε τσεκάρει τους πάντες
					par[i] = i;					//Άρα πούλο η επανάληψη
					stack[0] = i;					//Τελέιωσε η post_order
					c++;
					found = 1;
					cur_mr++;
					break;
				}
			}
		}
	}
	printf("Move to the music\n");
	printf("Master, Post_Order has finished\n");

	for(i=0; i<components->compt_counts; i++) {
		index[i].cur = index[i].head;	//Επαναφορά των cur, μπορεί να τα θέλουμε πιο μετά. ΑΝ δεν το σβήνω
	}

	return index;	//Got that MILF money
}

int isReachableGrailIndex(GrailIndex * index, scc *components, uint32_t source_node, uint32_t target_node) {
// vlepe ekfwnhsh sel. 9!!

	if ( components->id_belongs_to(source) == components->id_belongs_to(target) )
		return YES;
	else if ((components->inS[source].rank < components->inS[target].rank) || (components->inS[source].rank < components->inS[source].rank))
		return NO;
	else
		return MAYBE;
}

int destroyGrailIndex(GrailIndex* index) {
	if (index!=NULL) {
		for(i=0; i<components->compt_counts; i++) {
			if(index[i].nodeHY != NULL) {
				index[i].head = NULL;
				free(index[i].nodeHY);
			}
			else {
				printf("Master, this was not suppossed to happen. . .\n");
				printf("Master, what shall we do. . . ?\n");
			}
		}
		free(index);
		return OK_SUCCESS;
	}
	else	{
		//Einai adeio OG
		return FAILURE;
	}
}
