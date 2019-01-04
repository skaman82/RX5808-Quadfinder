#include "state_search.h"

#include "receiver.h"
#include "channels.h"
#include "ui.h"
#include "pstr_helper.h"
#include "settings_eeprom.h"

int ping;
int alarms = 0;
int AlarmState = LOW;
long alarminterval = 500;
unsigned long spendtime = 0;
unsigned long beaconspendtime = 0;
unsigned long pingspendtime = 0;
unsigned long updatetime = 0;
int8_t counter = 0;
int32_t oldvoltage = 0;
int32_t samplevoltage = 0;

uint8_t rssiA = 0;
uint16_t rssiARaw = 0;

    
#define BORDER_GRAPH_L_X 59

#define CHANNEL_TEXT_SIZE 5
#define CHANNEL_TEXT_X 0
#define CHANENL_TEXT_Y 15
#define CHANNEL_TEXT_H (CHAR_HEIGHT * CHANNEL_TEXT_SIZE)

#define FREQUENCY_TEXT_SIZE 1
#define FREQUENCY_TEXT_X 0
#define FREQUENCY_TEXT_Y 0
#define FREQUENCY_TEXT_H (CHAR_HEIGHT * FREQUENCY_TEXT_SIZE)

#define SCANBAR_BORDER_X 0
#define SCANBAR_BORDER_Y (CHANNEL_TEXT_H + 21)
#define SCANBAR_BORDER_W (BORDER_GRAPH_L_X - 3)
#define SCANBAR_BORDER_H 7

#define SCANBAR_X (SCANBAR_BORDER_X + 2)
#define SCANBAR_Y (SCANBAR_BORDER_Y + 2)
#define SCANBAR_W (SCANBAR_BORDER_W - 4)
#define SCANBAR_H (SCANBAR_BORDER_H - 4)

#define GRAPH_SEPERATOR_Y SCREEN_HEIGHT_MID
#define GRAPH_SEPERATOR_W (SCREEN_WIDTH - BORDER_GRAPH_L_X)
#define GRAPH_SEPERATOR_STEP 3

#define GRAPH_X (BORDER_GRAPH_L_X + 4)
#define GRAPH_W (SCREEN_WIDTH - BORDER_GRAPH_L_X)
#ifdef USE_DIVERSITY
    #define GRAPH_H (GRAPH_SEPERATOR_Y - 2)
    #define GRAPH_A_Y 0
    #define GRAPH_B_Y (SCREEN_HEIGHT - GRAPH_H - 1)

    #define RX_TEXT_SIZE 1
    #define RX_TEXT_X (BORDER_GRAPH_L_X + 4)
    #define RX_TEXT_H (CHAR_HEIGHT * RX_TEXT_SIZE)
    #define RX_TEXT_A_Y ((GRAPH_A_Y + GRAPH_H / 2) - (RX_TEXT_H / 2))
    #define RX_TEXT_B_Y ((GRAPH_B_Y + GRAPH_H / 2) - (RX_TEXT_H / 2))
#else
    #define GRAPH_H (SCREEN_HEIGHT - 16) //was -1
    #define GRAPH_Y 15 // was 0
    #define GRAPH_B_Y 0
#endif

using Ui::display;


void alarmcheck() {
  if (alarms == 1)
  {
    //Buzzer Stuff
    unsigned long currenttime = millis();

    if (currenttime - spendtime >= alarminterval) {
      spendtime = currenttime;

      if (AlarmState == LOW) {
        AlarmState = HIGH;
      }
      else {
        AlarmState = LOW;
      }

      // digitalWrite(6, AlarmState);
      digitalWrite(8, AlarmState);
    }

  }
  else alarms = 0;
}

void beacon() {
    if (ping == 1) {

    
    unsigned long beaconcurrenttime = millis();
    
    int delaytime = ((103 - rssiA) * 50);
    //Serial.println (delaytime);
    
    if (beaconcurrenttime - beaconspendtime >= delaytime) {
      beaconspendtime = beaconcurrenttime;
      
          digitalWrite(13, HIGH); //LED green
          if (alarms == 0) { 
          digitalWrite(6, LOW); //buzzerer
          }
      }
     else {
            digitalWrite(13, LOW);
            if (alarms == 0) { 
            digitalWrite(6, HIGH);
            }
           }  
  }
}



  void StateMachine::SearchStateHandler::onInitialDraw() {

    Ui::clear();

    drawBorders();

    drawChannelText();
    drawFrequencyText();
    drawScanBar();
    drawRssiGraph();

    Ui::needDisplay();
  }

  void StateMachine::SearchStateHandler::onUpdateDraw() {
    Ui::clearRect(
      0,
      15,
      BORDER_GRAPH_L_X,
      CHANNEL_TEXT_H
    );

    Ui::clearRect(
      0,
      FREQUENCY_TEXT_Y,
      BORDER_GRAPH_L_X,
      CHAR_HEIGHT * 2
    );

    Ui::clearRect(
      SCANBAR_X,
      SCANBAR_Y,
      SCANBAR_W,
      SCANBAR_H
    );

        Ui::clearRect( // update the rssi text
      60,
      0,
      64,
      15
    );

    drawChannelText();
    drawFrequencyText();
    drawScanBar();
    drawRssiGraph();
    menu.draw();

    Ui::needDisplay();
  }

  void StateMachine::SearchStateHandler::drawBorders() {
    display.drawRoundRect(
      SCANBAR_BORDER_X,
      SCANBAR_BORDER_Y,
      SCANBAR_BORDER_W,
      SCANBAR_BORDER_H,
      2,
      WHITE
    );

    Ui::drawDashedVLine(
      BORDER_GRAPH_L_X,
      0,
      SCREEN_HEIGHT,
      GRAPH_SEPERATOR_STEP
    );
  }

  void StateMachine::SearchStateHandler::drawChannelText() {
    display.setTextSize(5);
    display.setTextColor(WHITE);
    display.setCursor(0, 15);

    display.print(Channels::getName(Receiver::activeChannel));




    

    
  }

  void StateMachine::SearchStateHandler::drawFrequencyText() {
    display.setTextSize(FREQUENCY_TEXT_SIZE);
    display.setTextColor(WHITE);
    display.setCursor(FREQUENCY_TEXT_X, FREQUENCY_TEXT_Y);

    display.print(Channels::getFrequency(Receiver::activeChannel));
  }

  void StateMachine::SearchStateHandler::drawScanBar() {
    uint8_t scanWidth = orderedChanelIndex * SCANBAR_W / CHANNELS_SIZE;

    display.fillRect(
      SCANBAR_X,
      SCANBAR_Y,
      scanWidth,
      SCANBAR_H,
      WHITE
    );
  }

  void StateMachine::SearchStateHandler::drawRssiGraph() {

    Ui::drawGraph(
      Receiver::rssiALast,
      RECEIVER_LAST_DATA_SIZE,
      100,
      GRAPH_X,
      GRAPH_Y,
      GRAPH_W,
      GRAPH_H
    );

  
    analogRead(A6); // Fake read to let ADC settle.
    rssiARaw = analogRead(A6);
    
  

     rssiA = constrain(
            map(
                rssiARaw,
                EepromSettings.rssiAMin,
                EepromSettings.rssiAMax,
                0,
                100
            ),
            0,
            100
        );

    display.setCursor(75, 0);
    display.print("RSSI ");
    display.print(rssiA);
    
    
    beacon();




    float voltage;
    int sensorValue = analogRead(A2); // read the input on analog pin 2:

    unsigned long currentrunthtime = millis();
    if (currentrunthtime - updatetime >= 100) {
      updatetime = currentrunthtime;
      counter += 1;
      samplevoltage += sensorValue; 
    }
    
    if (counter >= 10) {   // if we have 10 samples            
          oldvoltage = (samplevoltage / 10);
          samplevoltage = 0;
          counter = 0;
    }

    voltage = oldvoltage * (5.25 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage:

    
   display.drawRect(40, 0, 15, 7, WHITE);
   display.drawRect(39, 2, 1, 3, WHITE);
   
   if (voltage > 4.25) {
      voltage = 0;
      alarms = 0;
   }
   
   else if (voltage >= 4.00) {
   display.drawRect(42, 2, 2, 3, WHITE);
   display.drawRect(45, 2, 2, 3, WHITE);
   display.drawRect(48, 2, 2, 3, WHITE);
   display.drawRect(51, 2, 2, 3, WHITE);
   alarms = 0;
   }

   else if (voltage >= 3.80) {
   display.drawRect(42, 2, 2, 3, WHITE);
   display.drawRect(45, 2, 2, 3, WHITE);
   display.drawRect(48, 2, 2, 3, WHITE);
   alarms = 0;
   }

   else if (voltage >= 3.60) {
   display.drawRect(42, 2, 2, 3, WHITE);
   display.drawRect(45, 2, 2, 3, WHITE);
   alarms = 0;
   }

   else if (voltage >= 3.40) {
   display.drawRect(42, 2, 2, 3, WHITE);
   alarms = 0;
   }

   else if (voltage <= 3.35) {
   alarms = 1;



if (AlarmState == HIGH) {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(85, 20);
      display.print("LOW");
      display.setCursor(75, 32);
      display.print("BATTERY");
      display.drawRect(42, 2, 2, 3, WHITE);
      }      
      
   }
   
   else {
     alarms = 0;
     }
  

    alarmcheck();

   
    // voltage on oled for debugging
    // display.setTextSize(1);
    // display.setTextColor(WHITE);
    // display.setCursor(80, 20);
    // display.println(voltage);
    

  }



  void StateMachine::SearchStateHandler::drawMenu() {
    this->menu.draw();
  }
