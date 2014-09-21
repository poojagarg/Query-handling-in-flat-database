// Format for Select: select <list of field_names> from <table_name> where <cond> 
	/*displays all the specified columns of those records which satisfy the condition*/
// Format for INNER Join: join <table_name1> alias1 <table_name2> alias2 where <cond> # alias1 and alias2 are characters strictly
	/*displays the table formed after the join */
// Keywords: join, select, from, where
// <cond>: field <RelationalOp> <f> (Semantics: f.type=field.type) ab > bc,a < d; //take care of space
// <f>: field || constant  
// <list of field_names>: field <list> 
// <list>: , field <list> || NULL

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maxFieldNameLength 20
#define maxNumOfFields 20
#define maxNumOfColumns 20
#define maxTableNameLength 20
#define lengthString 20
#define queryLength 200
#define maxNumberOfCondition 20
#define maxLengthOfRecordColumn 20
#define hashTableSize 7
#define databaseSize 7


typedef char string[lengthString];
typedef char record[maxNumOfColumns][maxLengthOfRecordColumn];
typedef struct metadata{
	char fieldName[maxFieldNameLength];
	int type;
	}metadata;
typedef struct selectType
{
	char fieldName[maxNumOfFields][maxFieldNameLength]; //when allocating
	char tableName[maxTableNameLength]; 
	int numberOfFields;
	}selectType;

typedef struct joinType
{
	char tableName1[maxTableNameLength];
	char tableName2[maxTableNameLength]; 
	char alias1;
	char alias2;
	}joinType;

typedef union queryField{
	selectType tS;
	joinType tJ;
}queryField;

typedef union Constant{
	int number;
	string str;
}Constant;
typedef union field{
	Constant constant;
	char col[maxFieldNameLength];
}field;

typedef struct condition{
	char RelationalOp; // > < = !
	field f1, f2;
	int field1Type, field2Type;
	// 0:number , 1: string constant, 2: string
	}condition;

typedef struct query{
	int type; //type can be 0: select, 1: join, -1: wrong
	condition cond[maxNumberOfCondition];
	int numberOfConditions;
	queryField qF;
}query;
typedef struct node{
  string e; //name of the field 
  int columnNumber; //column number of the field
  int type; //type of field: 0-int , 1-string
  node* next;
}node;
typedef node** hashTable;
typedef struct table{
	string e;//table name
	hashTable h;//hash table of the table
	int numOfColumns;
	table* next;
}table;
typedef table** database;



query dummyQuery();
query extract(char *s);
void select(query q);
void join(query q);
query extractSelect(char* s, int startIndex);
queryField extractSelectFields(char *s, int *startIndex);
condition extractCondition(char *s,int *startIndex);
void consumeKeyword(char *s, int *startIndex);
void copyQuery(query *q,query original);
void copyQueryField(queryField *q, queryField original, int type);
void copyCondition(condition *c, condition original);
void copySelectType(selectType *s, selectType original);
void copyField(field *f, field original, int type);
void debug();
query extractJoin(char* s, int startIndex);
void copyJoinType(joinType *j,joinType original);
void execute(query q);
void filePathFromTableName(char* filePath, char* fileName);

hashTable createHashTable(){
  hashTable h=(hashTable)malloc(sizeof(node*)*hashTableSize );
  for(int i=0; i<hashTableSize; i++){
    h[i]=NULL;
  }
  return h;
}
database createDatabase(){
	database d=(database)malloc(sizeof(table*)*databaseSize);
	for(int i=0; i<databaseSize; i++){
		d[i]=NULL;
	}
	return d;
}
int hash(string e, int size){
  int len=strlen(e);
  int sum=0;
  for(int i=0; i<len; i++){
    sum+=e[i];
  }
  return sum%size;
}

void viewHashTable(hashTable h){
  for(int i=0; i<hashTableSize; i++){
    node* n=h[i];
    
    while(n){
      printf("<%d %s>",n->columnNumber,n->e);
      n=n->next;
    }
  }
}
void insertHashTable(string e,int type, int columnNumber, hashTable h){
  int key=hash(e, hashTableSize);
  node* n=(node*)malloc(sizeof(node));
  n->columnNumber=columnNumber;
  strcpy(n->e, e);
  n->type=type;
  n->next=h[key];
  h[key]=n;
}
void insertTable(string e,int numOfColumns, hashTable h, database d){
	int key=hash(e,databaseSize);
	table *t=(table*)malloc(sizeof(table));
	t->h=h;
	t->numOfColumns=numOfColumns;
	strcpy(t->e,e);
	t->next=d[key];
	d[key]=t;
}
node* findHashTable(string e, hashTable h){
  int key=hash(e, hashTableSize);
  node* n=h[key];
  while(n){
    if(strcmp(n->e,e)==0)
      break;
    n=n->next;
  }
  if(!n) return NULL;
  else return n;
}
hashTable findTable(string e, database d){
  int key=hash(e, databaseSize);
  table* n=d[key];
  while(n){
    if(strcmp(n->e,e)==0)
      break;
    n=n->next;
  }
  if(!n) return NULL;
  else return n->h;
}
table* findTableInDatabase(string e, database d){
  int key=hash(e, databaseSize);
  table* n=d[key];
  while(n){
    if(strcmp(n->e,e)==0)
      break;
    n=n->next;
  }
  if(!n) return NULL;
  else return n;
}

void filePathFromTableName(char* filePath, char* fileName){

	strcat(fileName,".txt");
	
	strcpy(filePath,"../../input/");
	
	strcat(filePath,fileName);
	
}
void computeConditionFields(record rec,condition cond,hashTable hT,char* s1,char *s2,int *i1, int *i2){
	
	int ind=0;
	
	if(cond.field1Type==2){
		node* n=findHashTable(cond.f1.col,hT);
		switch(n->type){
			case 0: *i1=0;
					ind=0;
					while(rec[n->columnNumber][ind]!='\0'){
						*i1+=(rec[n->columnNumber][ind]-'0')+(*i1)*10;
					}
					break;
			case 1:	strcpy(s1,rec[n->columnNumber]);
					break;
		}
	}
	else if(cond.field1Type==0){
		*i1=cond.f1.constant.number;
	}
	else{
		strcpy(s1,cond.f1.constant.str);
	}
	if(cond.field2Type==2){
		node* n=findHashTable(cond.f2.col,hT);
		switch(n->type){
			case 0: *i2=0;
					ind=0;
					while(rec[n->columnNumber][ind]!='\0'){
						*i2+=(rec[n->columnNumber][ind]-'0')+(*i2)*10;
					}
					break;
			case 1:	strcpy(s2,rec[n->columnNumber]);
					break;
		}
	}
	else if(cond.field2Type==0){
		*i2=cond.f2.constant.number;
	}
	else{
		strcpy(s2,cond.f2.constant.str);
	}

}
int isFalseCondition(condition cond, char* s1,char* s2,int i1,int i2){
	switch(cond.RelationalOp){
		case '<':	if(strcmp(s1,"~")==0){
						if(i1<i2)
							return 0;
						else
							return 1;
						}
					else{
						if(strcmp(s1,s2)<0)
							return 0;
						else 
							return 1;
					}
		case '>':	if(strcmp(s1,"~")==0){
						if(i1>i2)
							return 0;
						else
							return 1;
						}
					else{
						if(strcmp(s1,s2)>0)
							return 0;
						else 
							return 1;
					}
		case '!':	if(strcmp(s1,"~")==0){
						if(i1!=i2)
							return 0;
						else
							return 1;
						}
					else{
						if(strcmp(s1,s2)!=0)
							return 0;
						else 
							return 1;
					}
		case '=':	if(strcmp(s1,"~")==0){
						if(i1==i2)
							return 0;
						else
							return 1;
						}
					else{
						if(strcmp(s1,s2)==0)
							return 0;
						else 
							return 1;
					}				

	}
	return 1;
}
void displayRecord(query q,record rec,hashTable hT){
	switch(q.type){
		case 0:	for(int i=0; i<q.qF.tS.numberOfFields; i++){
					node* n=findHashTable(q.qF.tS.fieldName[i],hT);
					printf("%s\t",rec[n->columnNumber]);
					}
				printf("\n");
				break;
		case 1:	break;
	}
}
void execute(query q, database d){
	switch(q.type){
			case 0:
				string filePath;
				filePathFromTableName(filePath,q.qF.tS.tableName);
				FILE* fp=fopen(filePath,"r");
				printf("%s",filePath);
				
				if(!fp){
					printf("file not found");
					return;
				}
				hashTable hT=findTable(q.qF.tS.tableName,d);
				table* t=findTableInDatabase(q.qF.tS.tableName,d);
				int numOfColumns;
				if(hT!=NULL){//table is already hashed
					char c;
					do{
						fscanf(fp,"%c",&c);
					}while(c!='\n');

					do{
						fscanf(fp,"%c",&c);
					}while(c!='\n');
					numOfColumns=t->numOfColumns;
				}
				else{//create hashtable for the table and make an entry in database.
					hT= createHashTable();

						
					fscanf(fp,"%d\n",&numOfColumns);
					metadata m[maxNumOfColumns];
					
					for(int i=0; i<numOfColumns; i++){
						string type;
						fscanf(fp,"%s %s\t",m[i].fieldName,type);
						if(strcmp(type,"int")){
							m[i].type=0;
						}
						else 
							m[i].type=1;//string
						insertHashTable(m[i].fieldName,m[i].type, i, hT);
						//i gives the index number
					}
					insertTable(q.qF.tS.tableName,numOfColumns, hT,d);
				}
				record rec;
				while(!feof(fp)){
					for(int i=0; i<numOfColumns; i++){
						fscanf(fp,"%s",rec[i]);
						}
					int i;
					for(i=0; i<q.numberOfConditions; i++){
						int i1=-1, i2=-1; 
						string s1,s2;
						s1[0]=s2[0]='~';
						s1[1]=s2[1]='\0';
						computeConditionFields(rec,q.cond[i],hT,s1,s2,&i1,&i2);
						if(isFalseCondition(q.cond[i],s1,s2,i1,i2)){
							break;
						}
									
					}
					if(i==q.numberOfConditions){
						displayRecord(q, rec,hT);
					}
				}
				break;
			/*case 1:break;
			case 2:break;*/
		}
}
void copyQuery(query *q,query original){
	q->type=original.type;
	q->numberOfConditions=original.numberOfConditions;
	for(int i=0; i<q->numberOfConditions; i++){
		copyCondition(&q->cond[i],original.cond[i]);
	}
	copyQueryField(&q->qF,original.qF,q->type);
}
void copyQueryField(queryField *q, queryField original, int type){
	if(type==0){//select
		copySelectType(&q->tS, original.tS);
	}
	else if(type==1){
		copyJoinType(&q->tJ,original.tJ);
	}

}
void copyCondition(condition *c, condition original){
	c->RelationalOp=original.RelationalOp;
	c->field1Type=original.field1Type;
	c->field2Type=original.field2Type;
	copyField(&c->f1, original.f1,c->field1Type);
	copyField(&c->f2, original.f2, c->field2Type);
}
void copyField(field *f, field original, int type){
	switch(type){
		case 0: f->constant.number=original.constant.number;
				break;
		case 1: strcpy(f->constant.str, original.constant.str);
				break;
		case 2: strcpy(f->col, original.col);
				break;

	}
}
void copySelectType(selectType *s, selectType original){
	s->numberOfFields=original.numberOfFields;
	for(int i=0; i<s->numberOfFields; i++){
		strcpy(s->fieldName[i],original.fieldName[i]);
	}
	strcpy(s->tableName, original.tableName);
}
void copyJoinType(joinType *j,joinType original){
	j->alias1=original.alias1;
	j->alias2=original.alias2;
	strcpy(j->tableName1,original.tableName1);
	strcpy(j->tableName2,original.tableName2);
}
queryField extractJoinFields(char *s, int *startIndex){
	queryField qF;
	joinType tJ;
	int ind=0;
	while(s[*startIndex]!=' '){
		tJ.tableName1[ind++]=s[(*startIndex)++];
	}
	tJ.tableName1[ind]='\0';
	(*startIndex)++;
	tJ.alias1=s[(*startIndex)++];
	(*startIndex)++;
	ind=0;
	while(s[*startIndex]!=' '){
		tJ.tableName2[ind++]=s[(*startIndex)++];
	}
	tJ.tableName2[ind]='\0';
	(*startIndex)++;
	tJ.alias2=s[(*startIndex)++];
	(*startIndex)++;
	copyJoinType(&qF.tJ,tJ);
	return qF;
}
queryField extractSelectFields(char *s, int *startIndex){
	queryField qF;
	selectType tS;
	tS.numberOfFields=0;
	int ind=0;
	while(s[*startIndex]!=' '){
		tS.fieldName[tS.numberOfFields][ind++]=s[(*startIndex)++];

		if(s[*startIndex]==','){
			tS.fieldName[tS.numberOfFields][ind]='\0';
			(*startIndex)++;
			tS.numberOfFields++;
			ind=0;
		}
	}
	tS.fieldName[tS.numberOfFields][ind]='\0';
	tS.numberOfFields++;
	(*startIndex)++; 
	copySelectType(&qF.tS,tS);
	return qF;	
}
void consumeKeyword(char *s, int *startIndex){
	while(s[*startIndex]!=' '){
		(*startIndex)++;
	}
	(*startIndex)++;
}
condition extractCondition(char *s,int *startIndex){
	condition c;
	int ind=0;
	if(s[*startIndex]=='"'){//string constant
		c.field1Type=1;
		(*startIndex)++;
		while(s[*startIndex]!='"'){
			c.f1.constant.str[ind++]=s[(*startIndex)++];
		}
		c.f1.constant.str[ind]='\0';
		/*printf("%s",c.f1.constant.str);
				debug();*/
		(*startIndex)++;//expects a space now
	}
	else if(s[*startIndex]>='0'&&s[*startIndex]<='9'){//integer constant
		c.field1Type=0;
		c.f1.constant.number=0;
		while(s[*startIndex]!=' '){
			c.f1.constant.number=c.f1.constant.number*10+(s[*startIndex]-'0');
			(*startIndex)++;
		}
		/*printf("%d",c.f1.constant.number);*/
	}
			
		else{ 
			c.field1Type=2;
			while(s[*startIndex]!=' '){
				c.f1.col[ind++]=s[(*startIndex)++];
			}
				c.f1.col[ind]='\0';
				/*printf("%s",c.f1.col);
				debug();*/
		}
	(*startIndex)++;
	c.RelationalOp=s[(*startIndex)++];
	(*startIndex)++;
	ind=0;
	if(s[*startIndex]=='"'){//string constant
		c.field2Type=1;
		(*startIndex)++;
		while(s[*startIndex]!='"'){
			c.f2.constant.str[ind++]=s[(*startIndex)++];
		}
		c.f2.constant.str[ind]='\0';
		/*printf("%s",c.f2.constant.str);
		debug();*/
		(*startIndex)++;// , / ;
			}
	else if(s[*startIndex]>='0'&&s[*startIndex]<='9'){//integer constant
		c.field2Type=0;
		c.f2.constant.number=0;
		while(s[*startIndex]!=','&&s[*startIndex]!=';'){
			c.f2.constant.number=c.f2.constant.number*10+(s[*startIndex]-'0');
			(*startIndex)++;
		}
		/*printf("%d",c.f2.constant.number);
			();*/
	}
			
		else{ 
			c.field2Type=2;
			while(s[*startIndex]!=','&&s[*startIndex]!=';'){
				c.f2.col[ind++]=s[(*startIndex)++];
			}
			c.f2.col[ind]='\0';
			/*printf("%s",c.f2.col);
			debug();*/
		}
	return c;
}
query extractSelect(char* s, int startIndex){
	query q;
	q.type=0;
	//extractSelectFields(s, &startIndex);
	copyQueryField(&q.qF,extractSelectFields(s, &startIndex),q.type);//new startIndex would point towards:from' '<table_name>
	consumeKeyword(s,&startIndex);//from
	int ind=0;
	while(s[startIndex]!=' '){
		q.qF.tS.tableName[ind++]=s[startIndex++];
	}
		q.qF.tS.tableName[ind++]='\0';
	startIndex++;
	consumeKeyword(s,&startIndex);//where
	q.numberOfConditions=0;
	while(s[startIndex]!=';'){
		//extractCondition(s,&startIndex);
		copyCondition(&q.cond[q.numberOfConditions++],extractCondition(s,&startIndex));
		//debug();
		if(s[startIndex]==',')
			startIndex++;
		else if(s[startIndex]==';')
			break;
	}
	return q;
}

query extractJoin(char* s, int startIndex){
	query q;
	q.type=1;
	copyQueryField(&q.qF,extractJoinFields(s, &startIndex),q.type);
	consumeKeyword(s,&startIndex);//where
	q.numberOfConditions=0;
	while(s[startIndex]!=';'){
		//extractCondition(s,&startIndex);
		copyCondition(&q.cond[q.numberOfConditions++],extractCondition(s,&startIndex));
		//debug();
		if(s[startIndex]==',')
			startIndex++;
		else if(s[startIndex]==';')
			break;
	}
	return q;
}
query dummyQuery(){
	query q;
	q.type=-1;
	return q;
}
void printField(field f, int type){
	//0: number, 1: string constant, 2: a field name
	switch(type){
		case 0:	printf("field: %d\t",f.constant.number);
				break;
		case 1:	printf("field: %s\t",f.constant.str);
				break;
		case 2:	printf("field: %s\t",f.col);
				break;

	}
}
void display(query q){
	printf("type: %d\n",q.type);
	printf("numberOfConditions: %d",q.numberOfConditions);
	for(int i=0; i<q.numberOfConditions; i++){
		printField(q.cond[i].f1,q.cond[i].field1Type);
		printf(" %c ", q.cond[i].RelationalOp);
		printField(q.cond[i].f2,q.cond[i].field2Type);
		printf("\n");
	}
	if(q.type==0){//select type
		for(int i=0; i<q.qF.tS.numberOfFields; i++){
			printf("%s\t",q.qF.tS.fieldName[i]);
		}
		printf("\ntable name: %s \n", q.qF.tS.tableName);
	}
	else if(q.type==1){
		printf("Table 1: %s, %c\n", q.qF.tJ.tableName1,q.qF.tJ.alias1);
		printf("Table 2: %s, %c\n", q.qF.tJ.tableName2,q.qF.tJ.alias2);
	}
}
void debug(){
	int d;
	scanf("%d",& d);
}
query extract(char *st){
	
	int i=0;
	char type[30];
	
	while(st[i]!=' '&&st[i]!='\0'){
		type[i]=st[i];
		i++;
		}
		
		type[i++]='\0';
		//printf("<%s>",type);
		
	//i points to <list>	
	if(strcmp(type,"select")==0){
		
		return extractSelect(st, i);
	}

	if(strcmp(type,"join")==0){
		return extractJoin(st, i);
	}
	
	else
		return dummyQuery();

}