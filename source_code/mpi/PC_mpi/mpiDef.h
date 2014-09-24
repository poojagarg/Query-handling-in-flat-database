#ifndef VECTOR
#define VECTOR
#include<vector>
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#endif
#include<math.h>
#ifndef IOSTREAM
#define IOSTREAM
#include<iostream>
#endif
#ifndef CSTDIO
#define CSTDIO
#include<cstdio>
#endif
#define N 24
#define P 4
#define E 0.000001
#define CONV 0.85

#if(N%P==0)
#define DIV N/P 
#else
#define DIV N/P + 1
#endif

#define FOR(I,T) for(int I=0;I<T;I++)
using namespace std;
double residual;
vector<double> source(N,1.0/N);
double dest[N];
double subDest[DIV];
vector<vector<int> > AdjList(N);
int AdvAdjList[N*P][DIV+2];
int subAdvAdjList[N][DIV+2];
/*void input(){
    FOR(s,N)
    {
        int degree;
        cin>>degree;
        FOR(j,degree)
        {
            int in;
            cin>>in;
            AdjList[s].push_back(in);
        }
   }
}*/
void random2()
{
 FILE* fp = fopen("input.txt","w");
    if(fp==NULL){
	cout<<"file not found\n";}
 	srand ( time(NULL) );
    for(int i=0;i<N;i++)
    {
        for (int t=0;t<N;t++)
        {
            int temp;
            temp = rand() % 100;
            if(temp%2==0) fprintf(fp,"%d ",1);
            else fprintf(fp,"%d ",0);
        }
        fprintf(fp,"\n");
    }
	fclose(fp);
	
}
void input()
{
	random2();
    FILE* fp = fopen("input.txt","r");
    if(fp==NULL){
	cout<<"file not found\n";}

    FOR(s,N)
    {
        FOR(i,N)
        {
            int b;
            fscanf(fp,"%d",&b);
            if(b==1) AdjList[s].push_back(i);
        }
    }
	fclose(fp);
}



void initSubDest(){
	FOR(i,DIV){
		subDest[i]=0;
	}
}
void copySourceDestination(){
	int n=source.size();	
	FOR(i,n){
		source[i]=dest[i];
	}
}
void convert()
{
        vector<int>  list[P];
        pair<int,vector<int> > elem;
        int div;
        if(N%P==0) div = N/P;
        else div = N/P + 1;
	int global;	

        FOR(s,N)
        {
		//printf("source: %d->",s);
                int n  = AdjList[s].size();
                //elem.first = n;
		global = n;

                FOR(d,n)
                {
		    
                    	int index = AdjList[s][d] / div;
			//printf("<%d, %d>",AdjList[s][d],index);
                        list[index].push_back(AdjList[s][d]);
                }
			//printf("\n");

                FOR(p,P)
                {
                    //elem.second = list[p];
                    AdvAdjList[N*p+s][0] = global;
                    AdvAdjList[N*p+s][1] = list[p].size();
                    int size = list[p].size();  
                    FOR(i,size)
                    {
                        AdvAdjList[N*p+s][i+2] = list[p][i];
                    }
                    list[p].clear();
                }
        }

}

double calResidual(vector<double> two,double* one)
{
    double sum = 0;
    FOR(i,N) sum += (one[i]-two[i])*(one[i]-two[i]);
    return sqrt(sum);
}


