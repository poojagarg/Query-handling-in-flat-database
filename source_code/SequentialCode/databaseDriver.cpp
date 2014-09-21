#ifndef databaseDriverDef
	#define databaseDriverDef
	#include "databaseDriverDef.h"
#endif

int main(int argc, char *argv[]){//argv[1]= Path of database directory
	database d=createDatabase();
	do{
		printf("Input query, and type ; to quit");
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
		do{c=getchar();}while(c!='\n'); //for enter in the input
	}while(1);
}