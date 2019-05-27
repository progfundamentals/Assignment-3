
  /* The following outlines the process for RSA encryption and decyrption :

   1.Two prime numbers are selected as p and q
   2. n = pq;  the modulus of both the keys
   3.the totent is calculated  = (p-1)(q-1)
   4. Choose e such that e > 1 and gcd (e, totient) must be equal to 1, making e is the public key
   5. Choose d such that it satisfies the equation de = 1 + k (totient), d is the private key.
   6. Cipher text is calculated using the equation c = m^e mod n where m is an unencrpted string
   7. using c and d, the message is decytped using equation m = c^d mod n, d being the private key.*/


/*preprocessor directives*/
#include <stdio.h>    
#include <stdlib.h>
#include <math.h>

/*global variables used by most functions*/
	int e, d, n; 

/*Finds greatest common divisor of two integers,
ensuring safer encryption*/
int gcd(int a, int b){
	int q, r1, r2, r;

	if (a>b) {
	r1=a;
	r2=b;
	}
	else {
	r1=b;
	r2=a;
	}
	while (r2>0) {
	q = r1/r2;
	r = r1 - q * r2;
	r1 = r2;
	r2 = r;
	}
	return r1;
}

int FastExponention(int bit, int n, int* y, int* a){
	if (bit == 1)
	*y = (*y * (*a)) % n;
	*a = (*a) * (*a) % n;
}
/*finds the carmicheal totent*/
int FindT(int a, int m, int n)
{
	int r;
	int y = 1;

	while (m > 0) {
		r = m % 2;
		FastExponention(r, n, &y, &a);
		m = m / 2;
	}
	return y;
}
/*this function tests if the two prime numbers selected
are in fact primary numbers*/
int PrimarityTest(int a, int i){
	int n = i - 1;
	int k = 0;
	int j, m, T;

	while (n % 2 == 0){
		k++;
		n = n / 2;
	}
	m = n;
	T = FindT(a, m,i);

	if (T == 1 || T == i - 1)
		return 1;

	for (j = 0; j < k; j++) {
		T = FindT(T, 2, i);
		if (T == 1)
			return 0;
		if (T == i - 1)
			return 1;
	}

	return 0;
} 

int inverse(int a, int b){

	int inv;
	int q, r, r1 = a, r2 = b, t, t1 = 0, t2 = 1;

	while (r2 > 0) {
		q = r1 / r2;
		r = r1 - q * r2;
		r1 = r2;
		r2 = r;
		t = t1 - q * t2;
		t1 = t2;
		t2 = t;
	}

	if (r1 == 1)
		inv = t1;

	if (inv < 0)
		inv = inv + a;
	return inv;
}
/*finds the encyption keys, randomly selects 2 prime integers */
int KeyGeneration()
{
	int q,p; 
	int phi_n;
	
do {
	do
	/*picks random int for p, tests whether it is a prime*/
		p = rand(); 
    while (p % 2 == 0);

} while (!PrimarityTest(2, p));

/*picks random integer for q, then tests it*/
	do {
		do
			q = rand(); 
		while (q % 2 == 0);
	} while (!PrimarityTest(2, q));
/*modulus*/
	n = p * q;
	/*totent */
	phi_n = (p - 1) * (q - 1);

	do
	/*ensures the gcd(e,totent) is equal to 1, then finds e accordingly*/
		e = rand() % (phi_n - 2) + 2; /*1 < e < phi_n*/
	while (gcd(e, phi_n) != 1);

/*d is the private key*/
	d = inverse(phi_n, e);
}

/*encryption code */
int Encryption(int value, FILE* out)
{
	int ciph;
	ciph= FindT(value, e, n);
	fprintf(out, "%d ", ciph);
}
/*decyrption code*/
int Decryption(int value, FILE* out)
{
	int deciph;
	deciph = FindT(value, d,n);
	fprintf(out,"%c",deciph);
}
int main(void)
{
	FILE *inp, *out;

	// destroy contents of these files (from previous runs, if any)
out=fopen("cipher.txt","w+");
fclose(out);
out=fopen("decipher.txt","w+");
fclose(out);

	KeyGeneration();

	inp=fopen("plain.txt","r+");
	if (inp == NULL) {
		printf("error accessing source file.\n");
		exit(1);
	}

	out=fopen("cipher.txt","w+");
	if (out == NULL) {
		printf("Error opening Destination File.\n");
		exit(1);
	}

	/*encryption starts here*/
	while (1) {
		char ch = getc(inp);
		if (ch == -1)
			break;
	/*changes value to ascii*/
		int value = ch;                         
		Encryption(value, out);
	}
	fclose(inp);
	fclose(out);

	/*decrpytion starts here*/
	inp = fopen("cipher.txt","r");
	if (inp == NULL) {
		printf("error opening cipher file\n");
		exit(1);
	}
	out = fopen("decipher.txt","w+");
	if (out == NULL) {
		printf("error opening files\n");
		exit(1);
	}
/*activates decryption code with new input*/
	while (1) {
		int cip;
		if (fscanf(inp,"%d",&cip) == -1)
			break;
		Decryption(cip, out);
	}
	fclose(out);
	return 0;
}
