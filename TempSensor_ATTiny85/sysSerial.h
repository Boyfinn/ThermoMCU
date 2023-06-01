//Constants
#define MAX_MESSAGE_LENGTH 12
#define SAVE 0x41 //Example: A12312345yz
#define READ 0x42 //Example: B
#define LOAD 0x43  //Example: C
#define PURGE 0x44  //Example: D

//SoftwareSerial SoftSerial(PB0, PB1); //ATtiny85 (RX,TX)
SoftwareSerial SoftSerial(2,3);

struct config
{
  uint8_t HR, HG, HB;
  uint8_t LR, LG, LB;
  int8_t TL, TH;
  uint16_t DC;
};
struct config _c;

void SerialSetup();
void GetSerial();
void RunCommand(char*);
void SaveEEPROM(char*);
void LoadEEPROM();
void ApplyBytes(char*);


void SerialSetup()
{
  SoftSerial.begin(9600);
  SoftSerial.println("\n");
}

void RunCommand(char* c){
  switch(c[0])
  {
    case SAVE:
      ApplyBytes(c);
      SaveEEPROM(c);
      SoftSerial.println("SAVE");
      break;

    case READ:
      SoftSerial.print("LowCol:(");
      SoftSerial.print(_c.LR, HEX);
      SoftSerial.print(_c.LG, HEX);
      SoftSerial.print(_c.LB, HEX);
      SoftSerial.println(")");

      SoftSerial.print("HighCol:(");
      SoftSerial.print(_c.HR, HEX);
      SoftSerial.print(_c.HG, HEX);
      SoftSerial.print(_c.HB, HEX);
      SoftSerial.println(")");

      SoftSerial.print("Temps:(");
      SoftSerial.print(_c.TL);
      SoftSerial.print(_c.TH);
      SoftSerial.println(")");

      SoftSerial.print("Diodes:(");
      SoftSerial.print(_c.DC);
      SoftSerial.println(")");
      break;
    case LOAD:
      LoadEEPROM();
      break;
    case PURGE:
      break;
  }
  SoftSerial.println(c);
}

void SaveEEPROM(char* data)
{
  SoftSerial.print("Saving:");
  SoftSerial.println(data);
  EEPROM.put(1, _c);
}

void LoadEEPROM()
{

    EEPROM.get(1, _c);
  //SoftSerial.println(_c);
}

void ApplyBytes(char* c)
{
  //LowColors
  _c.LR = c[1];
  _c.LG = c[2];
  _c.LB = c[3];
  //HighColors
  _c.HR = c[4];
  _c.HG = c[5];
  _c.HB = c[6];
  //Temps
  _c.TL = c[7];
  _c.TH = c[8];
  //Diodes & flags
  _c.DC = word(c[9],c[10]);
}

void GetSerial()
{
  while (SoftSerial.available() > 0)
    {
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;
    char inByte = SoftSerial.read();
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      message[message_pos] = inByte;
      message_pos++;
    }
    else
    {
      message[message_pos] = '\0';
      RunCommand(message);
      message_pos = 0;
    }
  }
}