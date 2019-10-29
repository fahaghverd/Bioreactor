#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

const int stepPin = 14; 
const int dirPin = 15;

int c = 0;
boolean n = false;
int menuItem = 1;
int lastMenuItem = 1;
int programItem = 1;
int lastProgramItem = 1;
int setItem = 1;
int page = 1;
int frame = 1;
int menuFrame = 1;
boolean power = false;

boolean selectSetItem = false;
boolean selectPlanItem[8] = {false,false,false,false,false,false,false,false};

volatile boolean Buttons[6] = {false,false,false,false,false,false}; 

String menuItem1 = "Positioning";
String menuItem2 = "Planning";
String menuItem3 = "Setting";

int xp = 0.5;
int np = 50;
int fp = 100;

String programItem1 = "Frq: ";
String programItem2 = "Cycle: ";
String programItem3 = "Str C.: ";
String programItem4 = "Flx C.: ";
String programItem5 = "Dly S1: ";
String programItem6 = "Dly S2: ";
String programItem7 = "Dly F1: ";
String programItem8 = "Dly F2: ";
int Value[8] = {500,100,5,0,0,0,0,0};
int TotalTime = 0;

String setItem1 = "Contrast: ";
int contrast = 60;
String setItem2 = "Light: ON";
boolean backlight = true;

int ButtonState[6] = {0,0,0,0,0,0};         
int lastButtonState[6] = {0,0,0,0,0,0};

Adafruit_PCD8544 display = Adafruit_PCD8544( 8, 9,10);

void setup() 
{
  
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(5, OUTPUT); //Red LED
  pinMode(6, OUTPUT); //Green LED
  pinMode(7, INPUT_PULLUP); //Power
  pinMode(19, INPUT_PULLUP); //Auto
  pinMode(0, INPUT_PULLUP); //Select
  pinMode(4, INPUT_PULLUP); //Up
  pinMode(3, INPUT_PULLUP); //Down
  pinMode(1, INPUT_PULLUP); //menu
  pinMode(12, OUTPUT); //LCD LED

  Serial.begin(9600);

  TurnOnRedLed();

  display.begin();      
  display.setContrast(contrast); //Set contrast to 50
  display.clearDisplay(); 
  display.display();   

}

void loop() 
{  
  drawMenu();

  ButtonState[5] = digitalRead(7); //power
  ButtonState[4] = digitalRead(19); //auto
  ButtonState[3] = digitalRead(0); //select
  ButtonState[2] = digitalRead(4); //up
  ButtonState[1] = digitalRead(3); //down
  ButtonState[0] = digitalRead(1); //menu

  for(int i=0;i<6;i++)
  {
    CheckIfButtonIsPressed(i);   
  }
  
  if(Buttons[5])
  {
    Buttons[5] = false;
    if(power)
    {
      power=false; //TurnLcdOff
      TurnOnRedLed();
    }
    else if(!power)
    {
      power=true; //TurnLcdOn
      TurnOffLed();
      c = 0;
      menuItem = 1;
      lastMenuItem = 1;
      programItem = 1;
      lastProgramItem = 1;
      setItem = 1;
      page = 1;
      frame = 1;
      menuFrame = 1;
    }
  }
  
  if(power)
  {
    if( page == 1)
    {
      if (Buttons[0])
      {
        Buttons[0] = false;
        menuItem = 1;
        menuFrame = 1;
      }
      
      if (Buttons[1])
      {
        Buttons[1] = false;
        lastMenuItem = menuItem;
        menuItem++;  
        if (menuItem == 4) 
        {
          menuItem = 3;
        }
      }
    
      if (Buttons[2])
      {     
        Buttons[2] = false;
        lastMenuItem = menuItem;
        menuItem--;
        if (menuItem == 0)
        {
          menuItem = 1;
        } 
      } 

      if (Buttons[3])
      {
        Buttons[3] = false;
        page = 2;
      }
    }
  
    if(menuItem == 1 && page == 2 )
    {
      Serial.print("\n Positioning"); 
      if(Buttons[0])
      {
        Buttons[0] = false;
        page = 1;
      }                                                                                                                                                                                                                                                                                                             

      if(Buttons[1])
      {
        Serial.print("\n down"); 
        Buttons[1] = false;
        digitalWrite(dirPin,HIGH);
                Serial.print("\n" + np);
        for(int x = 0; x < np; x++)
        {
        digitalWrite(stepPin,HIGH);
        Serial.print("\n 1");  
        delayMicroseconds(1000000/(2*fp)); 
        digitalWrite(stepPin,LOW); 
        Serial.print("\n 0");  
        delayMicroseconds(1000000/(2*fp));     
        }
        delay(200);
      }

      if(Buttons[2])
      {
        Buttons[2] = false;
        digitalWrite(dirPin,LOW);
        for(int x = 0; x < np; x++)
        {
        digitalWrite(stepPin,HIGH);  
        delayMicroseconds(1000000/(2*fp)); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(1000000/(2*fp));     
        }
        delay(200);
      }
    }

    if(page == 2 && menuItem == 2)
    {
      if(Buttons[0])
      {
        Buttons[0]=false;
        if(c == 1)
        {
          c = 0;
        }
        else if(selectPlanItem[programItem-1])
        {
          selectPlanItem[programItem-1] = false;
        }
        else if(!selectPlanItem[programItem-1])
        {
          page = 1 ;
          programItem = 1;
          frame = 1;
        }            
      }
    
      if(Buttons[1] && c == 0)
      {
        Buttons[1] = false;
        if(selectPlanItem[programItem-1])
        {
          if(programItem == 3 || programItem == 4)
          {
            Value[programItem-1]--;
            if(Value[programItem-1] < 0)
            {
              Value[programItem-1] = 0;
            }
          }
          else
          {
            Value[programItem-1] = Value[programItem-1] - 100;
            if(Value[programItem-1] < 0)
            {
              Value[programItem-1] = 0;
            }
          }
        }
        else if(!selectPlanItem[programItem-1])
        {
          if(programItem >= 3 && programItem-frame == 2)
          {
            frame = frame + 1;
          }
          lastProgramItem = programItem;
          programItem++;  
          if (programItem == 9) 
          {
            programItem = 8;
          }
          if (frame == 7) 
          {
            frame = 6;
          }
        }
      }

      if(Buttons[2] && c == 0)
      {
        Buttons[2] = false;
        if(selectPlanItem[programItem-1])
        {
          if(programItem == 3 || programItem == 4 )
          {
            Value[programItem-1]++;
            if(Value[programItem-1] > 20)
            {
              Value[programItem-1] = 20;
            }
          }
        else
        {
          Value[programItem-1] = Value[programItem-1]+100;
          if(Value[programItem-1] < 0)
          {
            Value[programItem-1] = 0;
          }
        }
      }
      else if(!selectPlanItem[programItem-1])
      {
        if(programItem >= 2 && programItem == frame)
        {
         frame=frame-1;
        }
        lastProgramItem = programItem;
        programItem = programItem-1;
        if (programItem == 0)
        {
          programItem = 1;
        } 
      }
    }

    if(Buttons[3] && c == 0)
    {
      Buttons[3] = false;
      selectPlanItem[programItem-1] = !selectPlanItem[programItem-1];
    }

    if (Buttons[4])
    {
      Buttons[4] = false;
      c++;
      if(c == 1)
      {
        TotalTime = ((((360*Value[2])/(4*1.8))*2 + ((360*Value[3])/(4*1.8))*2)/Value[0] + Value[4]/1000 + Value[5]/1000 + Value[6]/1000 + Value[7]/1000)*Value[1];
      }
      
      if(c == 2)
      {
        drawMenu();
        TurnOnGreenLed();
        c = 0;
        Run(Value);
        TurnOffLed();
      }
    }
  }
  
  /* if(page == 2 && menuItem == 3)
  {
    if(Buttons[0])
    {
      Buttons[0] = false;
      
      if(selectSetItem)
      {
        selectSetItem = false;
      }
      else
      {
        setItem = 1;
        page--;
      }
    }

    else if(Buttons[1])
    {
      Buttons[1] = false;
      if(selectSetItem)
      {
        contrast=contrast-10;
          if(contrast < 0)
          {
            contrast = 0;
          }
        display.setContrast(contrast);
      }
      else
      {
        setItem++;
        if (setItem == 3)
        {
          setItem = 2;
        } 
      }   
    }

    else if(Buttons[2])
    {
      Buttons[2] = false;
      if(selectSetItem)
      {
        contrast=contrast+10;

        display.setContrast(contrast);
      }
      else
      {
        setItem--;
        if (setItem == 0)
        {
          setItem = 1;
        }  
      }   
    }

    else if(Buttons[3])
    {
      Buttons[3] = false;
      if(setItem == 1)
      {
          if(!selectSetItem)
         {
          selectSetItem = true;
         }       
      }

      if(setItem == 2)
      {
        if (backlight) 
        {
          backlight = false;
          TurnBacklightOff();
          setItem2="Light: OFF";
        }
        else 
        {
          backlight = true; 
          TurnBacklightOn();
          setItem2="Light: ON";
        }
      }
    }
  } */ 
 }
}


void drawMenu()
{
  if(!power)
  {
    display.clearDisplay();   
    display.display();
  }
  else
  {
    if (page == 1) 
    {    
      display.setTextSize(1);
      display.clearDisplay();
      display.setTextColor(BLACK, WHITE);
      display.setCursor(15, 0);
      display.print("MAIN MENU");
      display.drawFastHLine(0,10,83,BLACK);
      display.setCursor(0, 15);

      if(menuItem == 1 && menuFrame == 1)
      {    
        displayMenuItem(menuItem1, 15,true);
        displayMenuItem(menuItem2, 27,false);
        //displayMenuItem(menuItem3, 35,false);
      }
      else if(menuItem == 2 && menuFrame == 1)
      {
        displayMenuItem(menuItem1, 15,false);
        displayMenuItem(menuItem2, 25,true);
        //displayMenuItem(menuItem3, 35,false);
      }
     /* else if(menuItem == 3 && menuFrame == 1)
      {
        displayMenuItem(menuItem1, 15,false);
        displayMenuItem(menuItem2, 25,false);
        //displayMenuItem(menuItem3, 35,true);
      }
      */
      display.display();
    }
    else if (page==2 && menuItem == 1) 
    {    
     displayInsMenuTitle(menuItem1,10);
     display.setTextSize(1);
     display.setTextColor(BLACK, WHITE);
     display.setCursor(0,15);
     display.print("To Set Initial"); 
     display.setCursor(7,24);
     display.print("Position Use");
     display.setCursor(13,33);
     display.print("Down & Up!");   
     display.setCursor(0,30);
     display.display();
    }

    else if (page==2 && menuItem == 2)
    {
    
      if(c == 1)
      {
        display.setTextSize(1);
        display.clearDisplay();
        display.setTextColor(BLACK, WHITE);
        display.drawFastHLine(0,8,83,BLACK);
        display.setCursor(5,10);
        display.print("To Run Press");
        display.setCursor(10,18);
        display.print("Auto Again!");
        display.setCursor(7,30);
        display.print("Time = "+ String(TotalTime) + "s");
        display.drawFastHLine(0,40,83,BLACK);
        display.display();
      }
      else if(c == 2)
      {
        display.setTextSize(2);
        display.clearDisplay();
        display.setTextColor(BLACK, WHITE);
        display.setCursor(0, 15);
        display.drawFastHLine(0,10,83,BLACK);
        display.print("Running");
        display.drawFastHLine(0,35,83,BLACK);
        display.display();
      }
      else
      {
        displayInsMenuTitle(menuItem2,18);
        if(programItem == 1 && frame ==1)
        {   
          displayPlanningItem(programItem1, 15,true,Value[0],1);
          displayPlanningItem(programItem2, 25,false,Value[1],2);
          displayPlanningItem(programItem3, 35,false,Value[2],3);
        }
        else if(programItem == 2 && frame == 1)
        {
          displayPlanningItem(programItem1, 15,false,Value[0],1);
          displayPlanningItem(programItem2, 25,true,Value[1],2);
          displayPlanningItem(programItem3, 35,false,Value[2],3);
        }
        else if(programItem == 3 && frame == 1)
        {
          displayPlanningItem(programItem1, 15,false,Value[0],1);
          displayPlanningItem(programItem2, 25,false,Value[1],2);
          displayPlanningItem(programItem3, 35,true,Value[2],3);
        }
        else if(programItem == 4 && frame == 2)
        {
          displayPlanningItem(programItem2, 15,false,Value[1],2);
          displayPlanningItem(programItem3, 25,false,Value[2],3);
          displayPlanningItem(programItem4, 35,true,Value[3],4);
        }
        else if(programItem == 5 && frame == 3)
        {
          displayPlanningItem(programItem3, 15,false,Value[2],3);
          displayPlanningItem(programItem4, 25,false,Value[3],4);
          displayPlanningItem(programItem5, 35,true,Value[4],5);
        } 
        else if(programItem == 6 && frame == 4) 
        {
          displayPlanningItem(programItem4, 15,false,Value[3],4);
          displayPlanningItem(programItem5, 25,false,Value[4],5);
          displayPlanningItem(programItem6, 35,true,Value[5],6);
        }
        else if(programItem == 7 && frame == 5) 
        {
          displayPlanningItem(programItem5, 15,false,Value[4],5);
          displayPlanningItem(programItem6, 25,false,Value[5],6);
          displayPlanningItem(programItem7, 35,true,Value[6],7);
        }
        else if(programItem == 8 && frame == 6)
        {
          displayPlanningItem(programItem6, 15,false,Value[5],6);
          displayPlanningItem(programItem7, 25,false,Value[6],7);
          displayPlanningItem(programItem8, 35,true,Value[7],8);  
        }
        else if(programItem == 7 && frame == 6)
        {
          displayPlanningItem(programItem6, 15,false,Value[5],6);
          displayPlanningItem(programItem7, 25,true,Value[6],7);
          displayPlanningItem(programItem8, 35,false,Value[7],8);
        }
        else if(programItem == 6 && frame == 6)
        {
          displayPlanningItem(programItem6, 15,true,Value[5],6);
          displayPlanningItem(programItem7, 25,false,Value[6],7);
          displayPlanningItem(programItem8, 35,false,Value[7],8);
        }
        else if(programItem == 5 && frame == 5)
        {
          displayPlanningItem(programItem5, 15,true,Value[4],5);
          displayPlanningItem(programItem6, 25,false,Value[5],6);
          displayPlanningItem(programItem7, 35,false,Value[6],7);
        }
        else if(programItem == 4 && frame == 4)
        {
          displayPlanningItem(programItem4, 15,true,Value[3],4);
          displayPlanningItem(programItem5, 25,false,Value[4],5);
          displayPlanningItem(programItem6, 35,false,Value[5],6); 
        }
        else if(programItem == 3 && frame == 3)
        {
          displayPlanningItem(programItem3, 15,true,Value[2],3);
          displayPlanningItem(programItem4, 25,false,Value[3],4);
          displayPlanningItem(programItem5, 35,false,Value[4],5);
        }
        else if(programItem == 2 && frame == 2)
        {
          displayPlanningItem(programItem2, 15,true,Value[1],2);
          displayPlanningItem(programItem3, 25,false,Value[2],3);
          displayPlanningItem(programItem4, 35,false,Value[3],4);
        }
        else if(programItem == 3 && frame == 2)
        {
          displayPlanningItem(programItem2, 15,false,Value[1],2);
          displayPlanningItem(programItem3, 25,true,Value[2],3);
          displayPlanningItem(programItem4, 35,false,Value[3],4);
        }
        else if(programItem == 4 && frame == 3)
        {
          displayPlanningItem(programItem3, 15,false,Value[2],3);
          displayPlanningItem(programItem4, 25,true,Value[3],4);
          displayPlanningItem(programItem5, 35,false,Value[4],5);
        }
        else if(programItem == 5 && frame == 4)
        {
          displayPlanningItem(programItem4, 15,false,Value[3],4);
          displayPlanningItem(programItem5, 25,true,Value[4],5);
          displayPlanningItem(programItem6, 35,false,Value[5],6);
        }
        else if(programItem == 6 && frame == 5)
        {
          displayPlanningItem(programItem5, 15,false,Value[4],5);
          displayPlanningItem(programItem6, 25,true,Value[5],6);
          displayPlanningItem(programItem7, 35,false,Value[6],7);
        }
        display.display();
      }
  }
  
 /* else if(page==2 && menuItem == 3)
  {
    displayInsMenuTitle(menuItem3,21);
    if(setItem == 1)
    {
       displayPlanningItem(setItem1,15,true, contrast,10);
       displayMenuItem(setItem2,25,false);
    }
    if(setItem == 2)
    {
       displayPlanningItem(setItem1,15,false,contrast,10);
       displayMenuItem(setItem2,25,true);
    }
    display.display();
  } */
 }
}

    void displayMenuItem(String item, int position, boolean selected)
  {
    display.setTextSize(1.9);
    if(selected)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.setCursor(0, position);
    display.print(">"+item);
  }
  

    void TurnBacklightOn()
  {
    digitalWrite(12,HIGH);
  }

    void TurnBacklightOff()
  {
    digitalWrite(12,LOW);
  }

    void displayInsMenuTitle(String menuItem, int i)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(i, 0);
    display.print(menuItem);
    display.drawFastHLine(0,10,83,BLACK);  
  }

void displayPlanningItem(String item, int position, boolean selected, int value , int pItem)
  {
    display.setTextSize(1);
    if(selectPlanItem[pItem-1])
    {
      if(n)
      {
        display.setTextColor(WHITE, BLACK);
      }
      if(!n)
      {
        display.setTextColor(BLACK, WHITE);
      }
      n = !n;
      delay(200);
    }
    else if(!selectPlanItem[pItem-1])
    {
      if(selected)
      {
        display.setTextColor(WHITE, BLACK);
      }
      else
      {
        display.setTextColor(BLACK, WHITE);
      }
    }  
    display.setCursor(0, position);
    display.print(">"+item+String(value));
  } 
 

  void CheckIfButtonIsPressed(int i)
  {
    if (ButtonState[i] != lastButtonState[i]) 
    {
      if ( ButtonState[i] == 0) 
      {
        Buttons[i]=true;
      }
    delay(50);
    }
   lastButtonState[i]= ButtonState[i];
  }

  void Run(int value1[8])
  {
  int f=value1[0];
  int Cycle = value1[1];
  int Xs = value1[2];
  int Xf = value1[3];
  int delay_1 = value1[4];
  int delay_2 = value1[5];
  int delay_3 = value1[6];
  int delay_4 = value1[7];
  int ns=((360*Xs)/(4*1.8));
  int nf=((360*Xf)/(4*1.8));
    
  for(int i=1; i <= Cycle; i++)
  {
    digitalWrite(dirPin,HIGH);
    for(int x = 0; x < ns; x++ )
    {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(1000000/(2*f)); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1000000/(2*f));     
    }
    delay(delay_2);
    
    digitalWrite(dirPin,LOW);
    for(int x = 0; x < ns; x++)
    {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(1000000/(2*f)); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1000000/(2*f));     
    }
    delay(delay_3);
    
    digitalWrite(dirPin,LOW);
    for(int x = 0; x < nf; x++)
    {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(1000000/(2*f)); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1000000/(2*f));     
    }
    delay(delay_4);
    
    digitalWrite(dirPin,HIGH);
    for(int x = 0; x < nf; x++)
    {
      digitalWrite(stepPin,HIGH);  
      delayMicroseconds(1000000/(2*f)); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1000000/(2*f));     
    }
    delay(delay_1);
  }
  delay(2000);
  }

  void TurnOnRedLed()
  {
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
  }

  void TurnOnGreenLed()
  {
    digitalWrite(5,LOW);
    digitalWrite(6,HIGH);
  }

  void TurnOffLed()
  {
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
  }
