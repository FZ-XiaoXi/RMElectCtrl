#include "softiic.h"
#include "RM.h"
//-------------------题目没有给延时函数，按理应该在每次点评改变时等待一会，或者使用for执行一定次数的指令
void Start(void);
void SendByte(uint8_t Data);
void ReceiveAck(void);
void Stop(void);
void SendData(uint8_t Data);

void Start(void){
    SCL_High();
    SDA_High(); //初始化
    SDA_Low();  //拉低DATA开始
    SCL_Low();
}
void SendByte(uint8_t Data){//应该是高位在前
    for(uint8_t i=0;i<8;i++){
        bool D=Data>>(7-i)&1;
        if(D){
            SDA_High();
            SCL_High();
            SCL_Low();
        }else{
            SDA_Low();
            SCL_High();
            SCL_Low();
        }
    }
}
bool ReceiveAck(void){
    SDA_High();         //释放SDA使得从机可以下拉
    SCL_High();         //第九CLK
    if(SDA_Read()){
        //NACK
        SCL_Low();
        return 0;
    }else{
        //ACK
        SCL_Low();
        return 1;
    }
}
void Stop(void){
    SCL_Low();
    SCL_High();
    SDA_High();
}
void SendData(uint8_t Data){
    Start();
    SendByte(Data);
    if(ReceiveAck()){
        //ACK
        //照题意，应该不需要发送下一字节，无需Restart
        Stop();
    }else
    {
        Stop();
    }
}