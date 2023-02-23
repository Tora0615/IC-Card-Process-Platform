#include "oledfont.h"
#include "Wire.h"
#define res A3  //RES
#define OLED_RES_Clr() digitalWrite(res,LOW)  //RES
#define OLED_RES_Set() digitalWrite(res,HIGH)
#define OLED_CMD  0  //写命令
#define OLED_DATA 1   //写数据
/*--------*/
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN   9     // Configurable, see typical pin layout above
#define SS_PIN    10    // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
//#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}
MFRC522::MIFARE_Key key;
byte tempUid[4];
byte targetUid[4];

#define modePin 2
#define readMode 0
#define writeMode 1

void setup()
{
  pinMode(modePin, INPUT);

  OLED_Init();
  OLED_ColorTurn(0);//0正常显示 1反色显示(白底)
  OLED_DisplayTurn(0);//0正常显示 1翻转180度显示
  /*---------*/
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  show_ready();
}

void loop()
{

  if (digitalRead(modePin) == LOW) {
    OLED_ShowString(2,0,"read mode",8); 
    OLED_ShowString(3,2," change?",8); 
  }else {
    OLED_ShowString(2,0,"write mode",8);
    OLED_ShowString(3,2," change?",8);  
  }
  delay(1000);
  OLED_Clear();
  wait_for_card();

  //Serial.println(digitalRead(modePin));
  if (digitalRead(modePin) == LOW) {
    wait_for_card();
    readmode_get_uid(readMode);  //ok read
  } else {
    wait_for_card();
    OLED_ShowString(0,0,"write mode",8); 
    delay(800);
    OLED_ShowString(0,0,"Target UID",8);
    writemode_get_uid();  //ok write
    OLED_Clear();



    OLED_ShowString(0,0,"Wait New ",8); 
    OLED_ShowString(38,2,"Card",8); 
    delay(2000);
    OLED_Clear();
    wait_for_card();
    write_from_target_uid(targetUid);
    OLED_Clear();



    OLED_ShowString(0,1,"Read New",8); 
    OLED_ShowString(39,3,"UID",8);
    delay(2000);
    OLED_Clear();
    wait_for_card();
    OLED_ShowString(10,0,"New UID",8);
    writemode_get_uid();  //ok read
  }

  

  





                
                                                //Serial.println(mfrc522.uid.size);  //4(組)
                                              //  Serial.println(mfrc522.uid.uidByte[0]);  //181
                                              //  Serial.println(mfrc522.uid.uidByte[1]);  //202
                                              //  Serial.println(mfrc522.uid.uidByte[2]);  //145
                                              //  Serial.println(mfrc522.uid.uidByte[3]);  //52
                                              //  
                                              //  Serial.println(mfrc522.uid.uidByte[0], HEX);  //B5
                                              //  Serial.println(mfrc522.uid.uidByte[1], HEX);  //CA
                                              //  Serial.println(mfrc522.uid.uidByte[2], HEX);  //91
                                              //  Serial.println(mfrc522.uid.uidByte[3], HEX);  //34
                                              //  delay(50000);






  

  
// Set new UID
//  byte newUid[] = NEW_UID; //{0xDE, 0xAD, 0xBE, 0xEF}  //byte 0~255
                                              //  //byte newUid[] = {123,132,213,111};  //OK，要花括號
                                              //  Serial.println(newUid[0]);  //222
                                              //  Serial.println(newUid[1]);  //173
                                              //  Serial.println(newUid[2]);  //190
                                              //  Serial.println(newUid[3]);  //239
                                              //  Serial.println(newUid[0],HEX);  //DE
                                              //  Serial.println(newUid[1],HEX);  //AD
                                              //  Serial.println(newUid[2],HEX);  //BE
                                              //  Serial.println(newUid[3],HEX);  //EF
                                              //  Serial.println(222,HEX); //DE
                                              //  delay(50000);

  
}


//最多四排
void show_ready(){
  OLED_ShowString(13,0,"READY",16);  
  delay(1000);
  OLED_Clear();
}

void show_uid(byte arr[]){
  String A_part = String(arr[0],HEX)+"-"+String(arr[1],HEX)+"- ";  //要多空一格
  A_part.toUpperCase();
  char A_part_arr[A_part.length()];
  A_part.toCharArray(A_part_arr, A_part.length());
  OLED_ShowString(0,2,A_part_arr,8);  

  String B_part = String(arr[2],HEX)+"-"+String(arr[3],HEX)+" ";  //要多空一格
  B_part.toUpperCase();
  char B_part_arr[B_part.length()];
  B_part.toCharArray(B_part_arr, B_part.length());
  OLED_ShowString(33,3,B_part_arr,8); 

  delay(3000);
  OLED_Clear();
}

//先 show mode 再 call showuid 
void show_mode_and_uid(int mode,byte arr[]){
  if(mode == 0){
    OLED_ShowString(3,0,"read mode",8); 
  }else{
    OLED_ShowString(3,0,"write mode",8); 
  }
  show_uid(arr);
}





void readmode_get_uid(int mode){
  for (byte i = 0; i < mfrc522.uid.size; i++) {
      tempUid[i] = mfrc522.uid.uidByte[i];
  } 
  show_mode_and_uid(mode,tempUid);
}


void writemode_get_uid(){
  for (byte i = 0; i < mfrc522.uid.size; i++) {
      targetUid[i] = mfrc522.uid.uidByte[i];
  } 
  //show_mode_and_uid(mode,targetUid);
  show_uid(targetUid);
}

void write_from_target_uid(byte uidArr[]){
  if ( mfrc522.MIFARE_SetUid(uidArr, (byte)4, true) ) {
    OLED_ShowString(3,0,"Write",8); 
    OLED_ShowString(0,2,"Success!",8); 
    delay(2000);
  }else{
    OLED_ShowString(3,0,"Write",8); 
    OLED_ShowString(3,2,"Fail...",8); 
    delay(2000);
  }
}

void wait_for_card(){
  while ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
  }
}







/*--------內建函式----------*/
//反显函数
void OLED_ColorTurn(u8 i)
{
  if(!i) OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
  else  OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
  if(i==0)
    {
      OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
      OLED_WR_Byte(0xA1,OLED_CMD);
    }
  if(i==1)
    {
      OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
      OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
  Wire.beginTransmission(0x3c);
  if(mode){Wire.write(0x40);}
  else{Wire.write(0x00);}
  Wire.write(dat); // sends one byte
  Wire.endTransmission();    // stop transmitting
}

//坐标设置
void OLED_Set_Pos(u8 x, u8 y) 
{ 
  x+=32;
  OLED_WR_Byte(0xb0+y,OLED_CMD);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f),OLED_CMD);
}       
//开启OLED显示    
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
  OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
  OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}            
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!   
void OLED_Clear(void)  
{  
  u8 i,n;       
  for(i=0;i<4;i++)  
  {  
    OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
    OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
    OLED_WR_Byte (0x12,OLED_CMD);      //设置显示位置—列高地址   
    for(n=0;n<64;n++)OLED_WR_Byte(0,OLED_DATA); 
  } //更新显示
}


//在指定位置显示一个字符
//x:0~127
//y:0~63         
//sizey:选择字体 6x8  8x16
void OLED_ShowChar(u8 x,u8 y,const u8 chr,u8 sizey)
{       
  u8 c=0,sizex=sizey/2,temp;
  u16 i=0,size1;
  if(sizey==8)size1=6;
  else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
  c=chr-' ';//得到偏移后的值
  OLED_Set_Pos(x,y);
  for(i=0;i<size1;i++)
  {
    if(i%sizex==0&&sizey!=8) OLED_Set_Pos(x,y++);
    if(sizey==8)
    {
      temp=pgm_read_byte(&asc2_0806[c][i]);
      OLED_WR_Byte(temp,OLED_DATA);//6X8字号
    }
    else if(sizey==16) 
    {
      temp=pgm_read_byte(&asc2_1608[c][i]);
      OLED_WR_Byte(temp,OLED_DATA);//8x16字号
    }
    else return;
  }
}
//显示一个字符号串 
void OLED_ShowString(u8 x,u8 y,const char *chr,u8 sizey)
{
  u8 j=0;
  while (chr[j]!='\0')
  {   
    OLED_ShowChar(x,y,chr[j++],sizey);
    if(sizey==8)x+=6;
    else x+=sizey/2;
  }
}

//OLED的初始化
void OLED_Init(void)
{
  pinMode(res,OUTPUT);//RES
  Wire.begin(0x3c); // join i2c bus (address optional for master)
  OLED_RES_Clr();
  delay(200);
  OLED_RES_Set();
  
  OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
  OLED_WR_Byte(0x00,OLED_CMD); /*set lower column address*/ 
  OLED_WR_Byte(0x12,OLED_CMD); /*set higher column address*/
  OLED_WR_Byte(0x00,OLED_CMD); /*set display start line*/ 
  OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/ 
  OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
  OLED_WR_Byte(0x4f,OLED_CMD); /*128*/ 
  OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
  OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/ 
  OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
  OLED_WR_Byte(0x1F,OLED_CMD); /*duty = 1/32*/ 
  OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/ 
  OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
  OLED_WR_Byte(0x00,OLED_CMD); 
  OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
  OLED_WR_Byte(0x80,OLED_CMD); 
  OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
  OLED_WR_Byte(0x1f,OLED_CMD); 
  OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
  OLED_WR_Byte(0x12,OLED_CMD); 
  OLED_WR_Byte(0xdb,OLED_CMD); /*set vcomh*/ 
  OLED_WR_Byte(0x40,OLED_CMD); 
  OLED_WR_Byte(0x8d,OLED_CMD); /*set charge pump enable*/ 
  OLED_WR_Byte(0x14,OLED_CMD);
  OLED_Clear();
  OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
}
