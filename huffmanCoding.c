#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>					// Windows
#include <malloc/malloc.h>  		// MAC OS X

#define MAX_VALUE	1000

#define	MAX_LEAF	27
#define	MAX_NODE	(2*MAX_LEAF-1)

typedef struct __HUFFMANNODE__ {
	char 		ch;					// char value of the node
    
	int			weight;				// weight of the node
	int			parent;				// number of the node
    
	int			lChild;				// number of the left child of the node
	int			rChild;				// number of the right child of the node
} huffmanNode;

huffmanNode HuffmanTree[MAX_NODE + 1];	// use array to store tree ADT

char *huffmanCode[MAX_NODE + 1]; // usw array to store ptr of the code

// Create huffman tree according to the input
// weight : an array which contains weight of each node
// ch  	  : an array which contains char of each node
// leaf   : contains the number of leaves
void CtrHuffmanTree(huffmanNode *ht, int *weight, char *ch, int leaf);

// Create huffman code according to the input
// hc     : an array which contains the pointer to the code addr
// leaf   : contains the number of leaves
void CtrHuffmanCode(huffmanNode *ht, char **hc, int leaf);

// Huffman Coding the input file
// fp	  : a file pointer
// leaf   : contains the number of leaves
void encode(FILE *ifp, FILE *ofp, int leaf);

// deHuffman Coding the input file
void decode(FILE *ifp, FILE *ofp, int leaf);

int main(int argc, char *argv[]) {
    
	// test start
    FILE *ifp = NULL;
    FILE *ofp = NULL;
    
	char string[6] = {' ', 'A', 'B', 'C', 'D', 'E'};
	int  weight[6] = {0, 5, 3, 4, 2, 1};
    
	CtrHuffmanTree(HuffmanTree, weight, string, 5);
    CtrHuffmanCode(HuffmanTree, huffmanCode, 5);
	int i;
	for (i = 1; i <= 10; ++i) {
		printf("NO.%d ch:%c we:%d parent:%d lc:%d rc:%d\n", \
               i, HuffmanTree[i].ch, HuffmanTree[i].weight,\
               HuffmanTree[i].parent, HuffmanTree[i].lChild, \
               HuffmanTree[i].rChild);
	}
    
    for (i = 1; i<= 5; i++) {
        printf("%s\n", huffmanCode[i]);
    }
    encode(ifp, ofp, 5);
	// test end
    
    
    
	return 0;
}

void CtrHuffmanTree(huffmanNode *ht, int *weight, char *ch, int leaf) {
	int i, j, nodes;
	int min1, node1;						// min1 < min2
	int min2, node2;
    
	nodes = 2*leaf-1;
    
	for (i = 1; i<=leaf; i++) {				// init ht[1-leaf]
		ht[i].ch 	 = ch[i];
		ht[i].weight = weight[i];
		ht[i].parent = ht[i].lChild = ht[i].rChild = 0;
	}
    
	for (i = leaf+1; i <= nodes; i++) {		// init he[leaf-last]
		ht[i].ch 	 = '\0';
		ht[i].weight = ht[i].parent = ht[i].lChild = ht[i].rChild = 0;
	}
    
	for (i = 1; i < leaf; i++) {
		min1  = min2  = MAX_VALUE;
		node1 = node2 = 0;
        
		for (j = 1; j < leaf+i; j++) {		// select 2 min nodes from the
            // unselectd nodes
			if (ht[j].parent == 0 && ht[j].weight < min1) {
				min2 = min1;
                node2 = node1;
				min1 = ht[j].weight;
				node1 = j;
			} else if (ht[j].parent == 0 && ht[j].weight < min2) {
				min2 = ht[j].weight;
				node2 = j;
			}
		}
        
		ht[node1].parent = ht[node2].parent = j;
		ht[j].weight = ht[node1].weight + ht[node2].weight;
        
        if (min1 != min2) {                 // make sure small or new value
            // appear on the left
            ht[j].lChild = node2;
            ht[j].rChild = node1;
        } else {
            ht[j].lChild = node1;
            ht[j].rChild = node2;
        }
        
	}
}

void CtrHuffmanCode(huffmanNode *ht, char **hc, int leaf) {
	int i, start, curnode, parent;
    
	char *cd;
	cd = (char *) malloc(leaf*sizeof(char));	// alloc memory to store code of
    // each char
	cd[leaf - 1] = '\0'; 						// set end of the code
    
	for (i = 1; i<=leaf; i++) {
		start   = leaf - 1;
		curnode = i;
		parent  = ht[curnode].parent;
        
		while (parent != 0) {
			start--;
			if (ht[parent].lChild == curnode) cd[start] = '0';
			else 							  cd[start] = '1';
            
			curnode = parent;
			parent  = ht[parent].parent;
		}
        
		hc[i] = (char *)malloc((leaf - start)*sizeof(char));
		strcpy(hc[i], &cd[start]);
	}
    
	free(cd);
}

void encode(FILE *ifp, FILE *ofp, int leaf){
	if ((ifp = fopen("ToBeTrans.txt", "r")) == NULL) {
		fprintf(stderr, "Can't open input file.\n");		// back to menu
	} else if ((ofp = fopen("CodeFile.txt", "w")) == NULL) {
        fprintf(stderr, "Can't open output file.\n");		// back to menu
    } else {
		char ch;
		int  i;
        
		while((ch = fgetc(ifp)) != EOF) { 				// coding to the end of
            // file
			for(i = 1; i <= leaf; i++) {
				if (HuffmanTree[i].ch == ch) {
					fprintf(ofp, "%s\n", huffmanCode[i]);
                    break;                              // skip this for loop
				}
                
                if (i == leaf) {
                    printf("Warning: %c is not in the charset, ignored.\n", ch);
                }
			}
		}
        
		fclose(ifp);
        fclose(ofp);
	}
    
    
}

void decode(FILE *ifp, FILE *ofp, int leaf){
		if ((ifp = fopen("CodeFile.txt", "r")) == NULL) {
		fprintf(stderr, "Can't open input file.\n");		// back to menu
	} else if ((ofp = fopen("TextFile.txt", "w")) == NULL) {
        fprintf(stderr, "Can't open output file.\n");		// back to menu
    } else {
		char str[leaf];
		int  i;
        
		while((fgets(str, leaf, ifp)) != EOF) { 			// decoding to the end
		 													// of file
			for(i = 1; i <= leaf; i++) {
				if (!strcmp(huffmanCode[i], str)) {
					fprintf(ofp, "%c\n", HuffmanTree[i].ch);
                    break;                              // skip this for loop
				}
                
                if (i == leaf) {
                    printf("Warning: %s is not in the code set, ignored.\n",\
                    	 	str);
                }
			}
		}
        
		fclose(ifp);
        fclose(ofp);
	}
}




