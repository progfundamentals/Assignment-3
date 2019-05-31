/*******************************************************************************
 * The folling program implements the huffman compression technique to compress
 * a string of characters, which can be entered in the program. It then 
 * displays the compressed string, then decompresses it. 
 * Author: Felicty Rodricks
*******************************************************************************/


/*******************************************************************************
 * Header files, only allowed headers are used 
*******************************************************************************/
/*Used for printf, scan functions*/
#include <stdio.h>
/*stcpy functions*/
#include <string.h>
 
/*******************************************************************************
 * List structs 
*******************************************************************************/
/*defines a node structure as part of the huffman tree */
typedef struct node_t {
	/*defines both left and right child of this node*/
	struct node_t *left, *right;
	/*occurences of character*/
	int freq;
	/*listed input character*/
	char c;
} *node;

 /*defines a pool, also known as a collection of many nodes, or heap*/
struct node_t pool[256] = {{0}};
node qqq[255], *q = qqq - 1;
int n_nodes = 0, qend = 1;
char *code[128] = {0}, buf[1024];

/*creates a new node as part of the huffman tree*/
node node_new(int freq, char c, node a, node b){
	/*stores a single character as a single huffman node.*/
	node n = pool + n_nodes++;
	/*stores character as a single node, */
	if (freq) n->c = c, n->freq = freq;
	/*starting from the front to the back of the list, node counts are 
	stored as a single node, and the two rarest characters stored as 
	its children*/
	else {
		n->left = a, n->right = b;
		n->freq = a->freq + b->freq;
	}
	return n;
}
 
/*******************************************************************************
 * This function creates a priority queue, where characters and their counts
 * are placed inside. 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void q_add(node n)
{   
	
	int j, i = qend++;
	/*organises the characters into ascending order */
	while ((j = i / 2)) {
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}
 /*removes node from the tree */
node qremove()
{
	int i, l;
	/*sets node n */
	node n = q[i = 1];
    /*removes the lowest value node from the heap*/
	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}
 
/*******************************************************************************
 * This function maps every possible character to an integer,uses one byte each. 
 * 
*******************************************************************************/
void make_tree(node n, char *s, int len)
{
	static char *out = buf;
	int temp = n->c;
    if (temp) {
		/*sets the length of the s array to 0*/
		s[len] = 0;
		strcpy(out, s);
		code[temp] = out;
		out += len + 1;
		return;
	}
    /*allocates each left node of the tree as a 0*/
	s[len] = '0'; make_tree(n->left,  s, len + 1);
	/*allocates each right node of the tree as a 1*/
	s[len] = '1'; make_tree(n->right, s, len + 1);
}
/*******************************************************************************
 * This function puts all the nodes in an open list, and ensures it remains
 * sorted at all times 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void init(const char *s)
{
	int i, freq[128] = {0};
	char c[16];
 
	while (*s) freq[(int)*s++]++;
    
	for (i = 0; i < 128; i++)
		if (freq[i]) q_add(node_new(freq[i], i, 0, 0));
 
	while (qend>2) 
		q_add(node_new(0, 0, qremove(), qremove()));
 
	make_tree(q[1], c, 0);
}
/*******************************************************************************
 * This function compresses the text file 
 * inputs:
 * - string
 * outputs:
 * - none
*******************************************************************************/
void compress(const char *s, char *out)
{
	int temp;
    while (*s) {
		/*copies the input string*/
		temp = *s;
        strcpy(out, code[temp]);
		/*finds the length of the input*/
		temp = *s++;
        out += strlen(code[temp]);
	}
}
/*******************************************************************************
 * This function decompresses the input string 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void decompress(const char *s, node t)
{
	node n = t;
	while (*s) {
		if (*s++ == '0') n = n->left;
		else n = n->right;
 
		if (n->c) putchar(n->c), n = t;
	}
 
	putchar('\n');
	if (t != n) printf("garbage input\n");
}
/*******************************************************************************
 * Main: tests the above functions as a driver function 
*******************************************************************************/
/*int main(void)
{ 

	const char *str = "this is an example fftfygftybygtfftfbtftubfff1 12345678 45385800514564768661 'brandnew   password  1968394565512475247 21  testuser   asad  9247727281445952125 12 	 4143   Circuit Analysis  3";		 
    char buf[1024];
 
	init(str);

	compress(str, buf);
	printf("compressed string: %s\n", buf);

	printf("decompressed string: ");

	decompress(buf, q[1]);
 
	return 0;
}*/