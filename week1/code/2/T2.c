#include <stdio.h>
int main(){
	while(1){
		int i;
		scanf("%d",&i);
		if(i==-1){
			return 0;
		}else if(i==0){
			printf("helloworld\n");
		}else if(i==1){
			printf("HELLOWORLD\n");
		}
	}
	return 0;
} 
