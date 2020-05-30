#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* 
 * Huffman's Code with no use of Trees (at least not explicitly)
 * This is just to understand the principles behind Huffman's ideas, the code is not very efficient.
 * 
 * The code will then create a stringstream with the compressed sequence, which can be converted to binary and saved to a file
 * 
 * The output file has the following format:
 * First byte: number of bytes-1 for the Huffman's table, (e.g., if 255 means 256 entries) 
 * Next bytes: followed by the characters in probability order, e.g.
 * suppose table is ACDEB (first byte is 0x04), bytes would be: 0x41 0x43 0x44 0x45 0x42
 * The output file has to be appended with zeros, this is an issue because we don't know when the data finishes.
 * To avoid this problem, 4 bytes soon after the table are used to tell the size of the data in bits. Files of up to 2MB in size can be encoded this way.
 * Next 4 bytes: size (in bits) of the data.
 * Then the data (the Huffman's codes) follows.
 * 
 * The code can be simplified and improved. The binary readings into strigs are unnecessary, but the strings make it easier to visualise and print partial results.
 * 
 * Copyright Andre Barczak 2015
 */

using namespace std;

#define N 256 //1 byte
unsigned long int accumulator[N];//the histogram
unsigned long int indexsorting[N];//the order of the probabilities by index
string HuffmanCodes[N];//This is the dictionary, it needs to be saved in the compressed file, so we know how to decode it
stringstream compressedbits;
int number_of_entries=0;//this is the number of entries with accumulator != 0


void print_Huffman_Codes(){
  unsigned int bits[N][N];//level, bit
  int b=0;
  int level=0;
  bits[0][0]=1;
  int i=0; int j=0;
  for(i=1;i<N;i++){
    level++;
    b=0;
    for(j=0;j<=level;j++){
      bits[i][j]=b;
      if(b==1)b=0;
      else b=1;
    }
    if(bits[i][j-1]==1)bits[i][j-1]=0;
    else bits[i][j-1]=1;
  }
  level=-1;
  for(int i=0;i<N;i++){
    level++;
    stringstream tempss;
    for(int j=0;j<=level;j++){
      tempss << bits[i][j];
    }
    HuffmanCodes[i]=tempss.str();
  }
}


void init_indexsorting(){
  for(int a=0;a<N;a++){
    indexsorting[a]=a;
  }
}

void build_histogram(char * filename){
  memset(accumulator, 0, sizeof(accumulator));
  ifstream inputfile;
  inputfile.open(filename,ifstream::binary|ios::in);     // open file
  if(!inputfile.good()){printf("Can't open the file \n");exit(0);}
  char c;
  unsigned char unsignec;
  while(inputfile.read((char *)(&c), sizeof(c))){
    unsignec=(unsigned char)c;
    //use the unsigned char to build a histogram
    if(!inputfile.eof()){
      accumulator[unsignec]++;
    }
  }
  inputfile.close();
}

void swap(unsigned long int *a,unsigned long int *b){
	unsigned long int temp=*a;
	*a=*b;
	*b=temp;	
}

void Quicksort(int first, int last){//reversed
   int i = first+1, j = last;
   int pivot = accumulator[first];
   while (i < j) {
     while ( (accumulator[i] >= pivot) && (i<last) ) {
       i++; 
     }
     while ( (accumulator[j] < pivot && j>=0) ){ 
       j--; 
     }
     if (i < j) { 
       swap(&accumulator[i], &accumulator[j]);
       swap(&indexsorting[i], &indexsorting[j]);
     }
   }
   if(pivot<accumulator[j]){
     swap(&accumulator[first], &accumulator[j]);
     swap(&indexsorting[first], &indexsorting[j]);
   }
   if (first < j-1) { 
     Quicksort(first, j-1); 
   }
   if (j+1 < last) { 
     Quicksort(j+1, last); 
   }
}

void sort_probabilities(unsigned long int data[], unsigned long int indexsorting[]){
  init_indexsorting();
  Quicksort(0,N-1);
  number_of_entries=0;
  for(int a=0;a<N;a++){
    if(accumulator[a]!=0) number_of_entries++;
  }
}

void computeEntropy(unsigned long int accumulator[], char *filename){
  //print histogram, compute totalelem
  unsigned long int totalelem=0;
  for(int i=0;i<256;i++){
    totalelem=totalelem+accumulator[i];
  }
  printf("Total elem %ld \n",totalelem);
   //use non-zero elements of the accumulator to calculate entropy
  float entropy=0.0;
  for(int i=0;i<256;i++){
    if(accumulator[i]!=0){
      entropy=entropy-(((float)accumulator[i])/totalelem)*(log(((float)accumulator[i])/totalelem)/log(2));
    }
  }
  printf("Entropy for file %s is: %f\n",filename,entropy);
}

string Search_Huffmans_Code(unsigned char a){
  string s;
  int i;
  for(i=255;i>=0;i--){
    if( indexsorting[i]==a) break;
  }
  return HuffmanCodes[i];
}


void Compress(char * filename, char * filenameOut){
  ifstream inputfile;
  inputfile.open(filename,ifstream::binary|ios::in);     // open file
  if(!inputfile.good()){printf("Can't open the file \n");exit(0);}
  char c;
  unsigned char unsignec;
  int numberofbytes=0;
  while(inputfile.read((char *)(&c), sizeof(c))){
    unsignec=(unsigned char)c;
    //use the unsigned char to Encode (choose a Huffman's code) based on the histogram 
    if(!inputfile.eof()){
      compressedbits << Search_Huffmans_Code(unsignec);
      numberofbytes++;
    }
  }
  string printable_compressedbits=compressedbits.str();
  cout << "Number of bits: " << printable_compressedbits.size() << " Number of bytes: " << printable_compressedbits.size()/8.0 << endl;
  cout << "Compression ratio: " << numberofbytes/(printable_compressedbits.size()/8.0) << endl;
  inputfile.close();
  //now save the bits into an output file, with the extra information about the table and number of bits
   ofstream outputfile;
   outputfile.open(filenameOut, ofstream::binary|ios::out);
   if(!outputfile.good()){printf("Can't open the output file \n");exit(0);}
   unsigned char putbyte=0;
   putbyte=(unsigned char)number_of_entries-1;
   printf("Number of entries %d \n", number_of_entries);//write byte with number of entries
   outputfile.write((char *)&putbyte,sizeof(putbyte));
   //write entries (indexsorting)
   for(int a=0;a<N;a++){
     if(accumulator[a]!=0) {
       putbyte=(unsigned char)indexsorting[a];
       outputfile.write((char *)&putbyte,sizeof(putbyte));
     }
     else break;
   }
   //write the number of bits (using 4 bytes for that)
   unsigned int number_of_bits=printable_compressedbits.size();
   outputfile.write((char *)&number_of_bits,sizeof(number_of_bits));
   //Finally, write the bits from compressedbits
   string sputdata = compressedbits.str();
   int counter=7;
   putbyte=0;
   for(int i=0;i<sputdata.size();i++){
     unsigned char mask=1;
     mask=mask<<counter;
     counter--;
     if(sputdata[i]=='1') putbyte = putbyte | mask;
     if(counter==-1) {
       outputfile.write((char *)&putbyte,sizeof(putbyte));
       counter=7;
       putbyte=0;
     }
   }
   //last byte, possibly with fewer than 8 bits of data
   if(counter!=7) outputfile.write((char *)&putbyte,sizeof(putbyte));
   outputfile.close();
}

void Decompress(char * filename, char * filenameOut){
  ifstream inputfile;
  inputfile.open(filename,ifstream::binary|ios::in);     // open file
  if(!inputfile.good()){printf("Can't open the file \n");exit(0);}
  char c;
  unsigned char unsignec;
  //get number of entries in the table
  inputfile.read((char *)(&c), sizeof(c));
  unsignec= (unsigned char)c;
  unsigned int number_of_entries=unsignec+1;
  printf("Number of entries %d \n",number_of_entries);
  //write the table 
  for (int a=0;a<number_of_entries;a++){
    inputfile.read((char *)(&c), sizeof(c));
    unsignec=(unsigned char)c;
    indexsorting[a]=unsignec;
  }
  //read the number of bits in the file
  int number_of_bits=0;
  inputfile.read((char *)(&number_of_bits), sizeof(number_of_bits));
  printf("Number of bits %d \n", number_of_bits);
  int bitcounter=0;//control the number of bits
  compressedbits.str("");//clean up stringstream to receive data
  while(inputfile.read((char *)(&c), sizeof(c))){
    unsignec=(unsigned char)c;
    for(int a=7;a>=0;a--){
      unsigned char mask=1;
      mask = mask << a;
      if( (c & mask)!=0) compressedbits << "1";
      else compressedbits << "0";
      bitcounter++;
      if(bitcounter>number_of_bits) break;
    }
  }
  //search for HuffmanCodes until it matches a character
  string decode = compressedbits.str();
  string findchar="";
  stringstream temps;
  stringstream decodedss;
  string decodedstring;
  int a=0;
  cout << endl << "Starting decompression " << endl;
  for(int i=0;i<decode.size();i++){
    temps << decode[i];
    findchar=temps.str();
    for(a=0;a<N;a++){
      if(findchar.compare(HuffmanCodes[a])==0){
        findchar="";
        temps.str("");
        break;
      }
    }
    if(a < number_of_entries){//if a>=number_of_entries, didn't find a match
      decodedss<<(unsigned char)indexsorting[a];
    }
  }
  decodedstring=decodedss.str();
  inputfile.close();
  //now right the output file
  ofstream outputfile;
  outputfile.open(filenameOut, ofstream::binary|ios::out);
  if(!outputfile.good()){printf("Can't open the output file \n");exit(0);}
  unsigned char putbyte=0;
  for(int i=0;i<decodedstring.size();i++){
    putbyte=decodedstring[i];
    outputfile.write((char *)&putbyte,sizeof(putbyte));
  }
  outputfile.close();
}


int main(int argc, char ** argv){
    if(argc!=4){printf("huffman filein fileout 1/0 (1 compress, 0 decompress)\n"); exit(0);}
    FILE *input, *output;
    input = fopen(argv[1], "r");
    output = fopen(argv[2],"w");
    
    print_Huffman_Codes();//actually create the code onto HuffmanCodes[] without using a Tree
    
    if (atoi(argv[3])==1){//Compress
      build_histogram(argv[1]);
      computeEntropy(accumulator, argv[1]);//measure entropy
      sort_probabilities(accumulator, indexsorting);
      Compress(argv[1],argv[2]);
    }
    else{//Decompress
      Decompress(argv[1],argv[2]);
    }
    return 0;
}
