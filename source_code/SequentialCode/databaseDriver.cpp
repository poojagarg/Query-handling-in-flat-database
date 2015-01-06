/*
Format of Query: As given in query.txt
*/
#ifndef databaseDriverDef
	#define databaseDriverDef
	#include "databaseDriverDef.h"
#endif

int main(int argc, char *argv[]){//argv[1]= Path of database directory
	database d=createDatabase();
	do{
		printf("\nInput query, and type ^ in the end of query if that is the last query.\n");
		char s[queryLength];
		int ind=0;
		char c;
		do{
			c=getchar();
			s[ind++]=c;
		}while(c!=';');
			
		s[ind++]='\0';
		printf("%s\n",s);
		if(strlen(s)<4)
			break;
		
		query q;
		
		copyQuery(&q,extract(s));
		execute(q,d);
		//display(q);
		/*if(q.type==-1)
			break;
		if(q.type==0){
			select(q);
		}
		else{
			join(q);
		}*/
		do{c=getchar();}while(c!='\n'&&c!='^'); //for enter in the input
		if(c=='^')
			break;
	}while(1);
}