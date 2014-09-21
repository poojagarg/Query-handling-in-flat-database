
#include<malloc.h>

typedef struct node{
  string e; 
  int columnNumber;
  node* next;
}node;
typedef node** hashTable;
hashTable createHashTable(int size){
  hashTable h=(hashTable)malloc(sizeof(node*)*size);
  for(int i=0; i<size; i++){
    h[i]=NULL;
  }
  return h;
}
int hash(string e, int size){
  int len=strlen(e);
  int sum=0;
  for(int i=0; i<len; i++){
    sum+=e[i];
  }
  return sum%size;
}

void viewHashTable(hashTable h, int size){
  for(int i=0; i<size; i++){
    node* n=h[i];
    
    while(n){
      printf("<%d %s>",n->columnNumber,n->e);
      n=n->next;
    }
  }
}
void insertHashTable(string e, int columnNumber, hashTable h, int size){
  int key=hash(e, size);
  node* n=(node*)malloc(sizeof(node));
  n->columnNumber=columnNumber;
  strcpy(n->e, e);
  n->next=h[key];
  h[key]=n;
}
int findHashTable(string e, hashTable h, int size){
  int key=hash(e, size);
  node* n=h[key];
  while(n){
    if(strcmp(n->e,e)==0)
      break;
    n=n->next;
  }
  if(!n) return 0;
  else return n->columnNumber;
}
int main(){
  int n;
  cin>>n;
  int A[n];
  hashTable h=createHashTable(n*n);
  for(int i=0; i<n; i++){
    cin>>A[i];
    insertHashTable(A[i],h, n);
  }
  for(int i=0; i<n; i++){
    if(findHashTable(i,h,n)){
      cout<<"Present "<<i<<'\t';
    }
  }
  viewHashTable(h,n);
  findThreeElements(h, n, 10, A);
}

