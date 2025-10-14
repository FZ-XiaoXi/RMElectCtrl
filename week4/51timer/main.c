#include "reg52.h"
sbit LED1=P2^1;
sbit LED2=P2^0;
void main(){
	int i,a=0;
	while(1){
		LED1=0;
		LED2=1;
		for(i=0;i<30000;i++){a=1;}
		LED1=1;
		LED2=0;
		for(i=0;i<10000;i++){a=1;}
	}
}	