#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
/*****************AP设置项 *****************/
const char *STAssid = "ESP1418" ; //AP名字
const char *STApassword = "12345678"; //AP密码
/****************UDP设置项 *****************/
unsigned int read_Port = 20002;      //监听端口
unsigned int send_Port = 20001;      //发送端口
char readdata[512];                  //数据缓存
WiFiUDP Udp;                         //定义udp
IPAddress ap_ip(192, 168, 1, 1);    //AP的ip地址
/****************io设置项 *****************/
int num=4;
int state_led=2;                     //IO2（D4），板载led，作为判断WiFi连接状态，连上为亮
int led = 5;                         //IO5（D1），用来做控制灯亮灭的
int btn = 14;                         //IO4（D5）， 打开按钮
int btnA = D6;                       
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  init_IO(); //初始化需要的IO引脚
  init_STA();
  Udp.begin(read_Port);             //UDP设置监听指定端口
}

void loop()
{
  Btn_Event();
  Udp_Handler(Read_Udp());
}

//按钮事件
void Btn_Event()
{
  if (digitalRead(btn) == LOW&&digitalRead(btnA)==HIGH)
  {
   
    while(digitalRead(btnA)==HIGH)
    {
      String str=(String)num;
      myPrint(str, 0);
      delay(100);
      
      if(digitalRead(btn)==HIGH){
        return;}
    }
    num=num-1;
    String str=(String)num;
    myPrint(str, 0);
    delay(2000);
  }
  else if (digitalRead(btn) == HIGH&&digitalRead(btnA)==LOW)
  {
   
    while(digitalRead(btn)==HIGH)
    {
      String str=(String)num;
      myPrint(str, 0);
      delay(100);
      
      if(digitalRead(btnA)==HIGH){
        return;}
    }
    num=num+1;
    String str=(String)num;
    myPrint(str, 0);
    delay(2000  );
  }
  else{
    String str=(String)num;
    myPrint(str, 0);
    }
  //按钮为低电平触发
}

//初始化io
void init_IO()
{
//  pinMode(led, OUTPUT);             //设置指定io为输出模式
//  digitalWrite(led, 0);             //初始化为低电平，关闭灯状态
  pinMode(state_led, OUTPUT);       //设置指定io为输出模式
  digitalWrite(state_led, 1);       //初始化为高电平，关闭灯状态，板载led为低电平点亮
  pinMode(btn, INPUT);       
  pinMode(btnA,INPUT);
}

//初始化连接AP
void init_STA()
{
  IPAddress local_ip(192, 168, 1, 123);
  IPAddress arg1(192, 168, 1, 1);
  IPAddress arg2(255, 255, 255, 0);
  IPAddress arg3(192, 168, 1, 1);
  IPAddress dns2(192, 168, 1, 1);
  WiFi.disconnect();
  WiFi.config(local_ip, arg1, arg2, arg3, dns2);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STAssid, STApassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(state_led, !digitalRead(state_led)); //翻转WiFi连接状态指示灯
    Serial.print(".");
  }
  digitalWrite(state_led, 0);
}

//自定义函数,将接收的数据，以String类型返回，方便分析处理
String Read_Udp()
{
  String data = "";
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    memset(readdata, 0x00, sizeof(readdata));
    Udp.read(readdata, 511);
    Udp.flush();
    for (int i = 0; i < packetSize; i++)
    {
      data += readdata[i];
    }
    Serial.println(data);
  }
  return data;
}

/************自定义函数，对接收数据分析处理************/
void Udp_Handler(String data)
{
  if (data != "")
  {
    if (data == "打开")
    {
      digitalWrite(led, 1);
      //myPrint("打开", 1);
    }
    else if (data == "关闭")
    {
      digitalWrite(led, 0);
      //myPrint("关闭", 1);
    }

    else if (data == "请求")
    {
      //myPrint("收到", 1);
    }
  }
}

/*自定义函数
  封装数据发送功能，方便调用
  第一个参数，使用String类型作为发送内容
  第二个参数，a为真，指定发送，对发送方返回数据，a为假，给固定的AP地址发数据
*/
void myPrint(String data, bool a)
{
  if (a)
  {
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  }
  else
  {
    Udp.beginPacket(ap_ip, send_Port);
  }
  Udp.print(data);
  Udp.endPacket();
}
