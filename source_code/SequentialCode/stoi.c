#include<stdio.h>
#include<string.h>
int main(){
	char a[20];
	int num=0;
	do{
		scanf("%s",a);
		num=0;
		int n=strlen(a);
		for(int i=0; i<n; i++){
			printf("%c",a[i]);
			num=(a[i]-'0')+num*10;
		}
		printf("num:%d\n",num);
	}while(num>4);

}