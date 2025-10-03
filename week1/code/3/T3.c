//#include <stdio.h>
typedef enum {
	GPIO_Speed_2MHz,GPIO_Speed_10MHz,GPIO_Speed_50MHz
}GPIO_Speed_ENUM;
struct GPIO_Speed_Str{
	GPIO_Speed_ENUM GPIO_Speed;
};
void GPIO_StructureInit(struct GPIO_Speed_Str *GPIO_Speed_Str_Pt);

int main(){
	struct GPIO_Speed_Str GPIO_Speed_Example;
	GPIO_StructureInit(&GPIO_Speed_Example);
	//printf("GPIO Speed: %d\n", GPIO_Speed_Example.GPIO_Speed);
	return 0;
} 

void GPIO_StructureInit(struct GPIO_Speed_Str *GPIO_Speed_Str_Pt){
	GPIO_Speed_Str_Pt->GPIO_Speed= GPIO_Speed_2MHz;
}