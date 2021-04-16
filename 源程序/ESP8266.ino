#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>

char auth[] = "你的授权码";
char ssid[] = "WiFi的ssid";
char pswd[] = "wifi密码";

bool ledState;

BlinkerButton Button1("btn-01");  //注意修改按钮的数据名"btn-01"为自己的


void button1_callback(const String & state) {
    Open(); //按钮被点击后执行
}

void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}
void Open()
{
  //发送开门的信号
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(1, HIGH);
  delay(400);
  digitalWrite(1, LOW);
  digitalWrite(LED_BUILTIN, LOW);
//    digitalWrite(LED_BUILTIN, HIGH);
//    digitalWrite(1, HIGH);
//    delay(1000);
//    digitalWrite(1, LOW);
//    digitalWrite(LED_BUILTIN, LOW);
}
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    //        digitalWrite(LED_BUILTIN, LOW);
    Open();
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    ledState = true;
  }
  else if (state == BLINKER_CMD_OFF) {
    //    digitalWrite(LED_BUILTIN, HIGH);
    Open();
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
    ledState = false;
  }
}

void miotQuery(int32_t queryCode)
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(ledState ? "on" : "off");
      BlinkerMIOT.print();
      break;

    default :
      BlinkerMIOT.powerState(ledState ? "on" : "off");
      BlinkerMIOT.print();
      break;
  }
}
void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  BLINKER_DEBUG.debugAll();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(1,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Blinker.begin(auth, ssid, pswd);
  Button1.attach(button1_callback);
  Blinker.attachData(dataRead);
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachQuery(miotQuery);
}

void loop()
{
  Blinker.run();
}
