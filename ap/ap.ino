#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char *ssid = "ESP1418";             //AP的SSID（WiFi名字）
const char *password = "12345678";            //AP的密码
int a=D3;
int b=D2;
int c=D7;
int d=D6;
int e=D5;
int f=D1;
int g=D0;
int dp=D8;
int pinArray[8]={a,b,c,d,e,f,g,dp};
//int led = 14;                                 //设置指示灯io，D5
int state_led=2;                              //IO2（D4），板载led，作为判断是否有设备连接
IPAddress sta_client;                        //保存sta设备的ip地址
/****************UDP设置项 *****************/
unsigned int read_Port = 20001;     //监听端口
unsigned int send_Port = 20002;     //发送端口
char readdata[512];                 //数据缓存
WiFiUDP Udp;                        //定义udp
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  init_IO(); //初始化需要的IO引脚
  init_AP();
  Udp.begin(read_Port);             //UDP设置监听指定端口
}
void loop()
{
  int gotoAP=WiFi.softAPgetStationNum();    //获取当前连接到AP的设备数量  
  if (gotoAP==0) 
  {
    digitalWrite(state_led, 1); //如果没有设备连接，熄灭WiFi状态LED
  }
  else
  {
    digitalWrite(state_led, 0); //如果有设备连接，点亮WiFi状态LED
  }
  Udp_Handler(Read_Udp());
}

//初始化io
void init_IO()
{
 for(int i=0;i<8;i++){
  pinMode(pinArray[i],OUTPUT);
  digitalWrite(pinArray[i],1);
  }
  pinMode(state_led, OUTPUT);       //设置指定io为输出模式
  digitalWrite(state_led, 1);       //初始化为高电平，关闭灯状态，板载led为低电平点亮
}

//初始化AP信息
void init_AP()
{
  WiFi.mode(WIFI_AP);                         //设置为AP模式
  WiFi.softAP(ssid, password);                //配置AP信息，并开启AP

  //默认IP为192.168.4.1 , 这里我自定义设置AP的ip段
  IPAddress softLocal(192, 168, 1, 1);        //IP地址，用以设置IP第4字段
  IPAddress softGateway(192, 168, 1, 1);      //IP网关，用以设置IP第3字段
  IPAddress softSubnet(255, 255, 255, 0);
  //配置自定义的IP信息
  WiFi.softAPConfig(softLocal, softGateway, softSubnet);
  IPAddress myIP = WiFi.softAPIP();           //用变量myIP接收AP当前的IP地址
  Serial.println(myIP);                       //打印输出myIP的IP地址
}

/*自定义函数
  将接收的数据，以String类型返回，方便分析处理
*/
String Read_Udp() {
  String data = "";
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    sta_client = Udp.remoteIP();
    memset(readdata, 0x00, sizeof(readdata));
    Udp.read(readdata, 511);
    Udp.flush();
    for (int i = 0; i < packetSize; i++) {
      data += readdata[i];
    }
    Serial.println(data);
  }
  return data;
}
/************自定义函数，对接收数据分析处理************/
void Udp_Handler(String data) {
  if (data != "") {
    if (data == "0") {
      show_0();
 //     myPrint("打开", 0);
    }
    else if (data == "1") {
      show_1();
 //     myPrint("关闭", 0);
    }
    else if (data == "2") {
      show_2();
 //     myPrint("关闭", 0);
    }
    else if (data == "3") {
      show_3();
 //     myPrint("关闭", 0);
    }
    else if (data == "4") {
      show_4();
 //     myPrint("关闭", 0);
    }
    else if (data == "5") {
      show_5();
 //     myPrint("关闭", 0);
    }
    else if (data == "6") {
      show_6();
 //     myPrint("关闭", 0);
    }
    else if (data == "7") {
      show_7();
 //     myPrint("关闭", 0);
    }
    else if (data == "8") {
      show_8();
 //     myPrint("关闭", 0);
    }
    else if (data == "9") {
      show_9();
 //     myPrint("关闭", 0);
    }
    else if (data == "请求") {
 //     myPrint("收到", 0);
    }
  }
}
/*自定义函数
  封装数据发送功能，方便调用
  第一个参数，使用String类型作为发送内容
  第二个参数，a为真，指定发送，对发送方返回数据，a为假，以保存的ip发送数据
*/
void myPrint(String data, bool a) {
  if (a) {
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  } else {
    Udp.beginPacket(sta_client, send_Port);
  }
  Udp.print(data);
  Udp.endPacket();
}

void show_0(){
  unsigned char j;
  for(int i=0;i<6;i++){
    digitalWrite(pinArray[i],LOW);
    }
    digitalWrite(g,HIGH);
    digitalWrite(dp,HIGH);
 }
void show_1(){
  digitalWrite(a,HIGH);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
  digitalWrite(dp,HIGH);
 }
void show_2(){
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,HIGH);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,HIGH);
  digitalWrite(g,LOW);
  digitalWrite(dp,HIGH);
 }
void show_3(){
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,LOW);
  digitalWrite(dp,HIGH);
 }
 void show_4(){
  digitalWrite(a,HIGH);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
  digitalWrite(dp,HIGH);
 }
 void show_5(){
  digitalWrite(a,LOW);
  digitalWrite(b,HIGH);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,HIGH);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
  digitalWrite(dp,HIGH);
 }
 void show_6(){
  digitalWrite(a,LOW);
  digitalWrite(b,HIGH);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
  digitalWrite(dp,HIGH);
 }
 void show_7(){
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
  digitalWrite(dp,HIGH);
 }
 void show_8(){
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
  digitalWrite(dp,HIGH);
 }
 void show_9(){
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,HIGH);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
  digitalWrite(dp,HIGH);
 }
