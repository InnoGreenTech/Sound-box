#include "DFRobotDFPlayerMini.h"

#include "SoftwareSerial.h"
#define RX 4
#define TX 5
SoftwareSerial mp3Serial(RX, TX); // RX, TX

DFRobotDFPlayerMini DFPlayer;

byte lecture; //Indique qu'une lecture est en cours
int  number_of_tracks;
int index_play=1;
int last_play=1;
int play_button;
int play_light;
#define LUMINOSITY A4
#define PLAY        3
#define RANDOM_PIN A0

int  luminosity;                   // value of luminosity
byte light;                       // light one= 1 light off=0
byte read_done;
#define LIGHT_ONE  500
#define LIGHT_OFF  400


void setup() {
  
  mp3Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("Version: V1");
  Serial.println("Numero de serie: MP3 07/2020 01");

  pinMode(PLAY,INPUT_PULLUP);
  randomSeed(analogRead(RANDOM_PIN));
       

   if (!DFPlayer.begin(mp3Serial)) {  //Use softwareSerial to communicate with mp3.
   Serial.println(F("Unable to begin:"));
   Serial.println(F("1.Please recheck the connection!"));
   Serial.println(F("2.Please insert the SD card!"));
   while(true);
   }
  Serial.println(F("DFPlayer Mini online."));

  number_of_tracks=DFPlayer.readFileCounts();
  Serial.println(number_of_tracks);
  
//  DFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
    DFPlayer.outputSetting(0, 0);
  delay(200);
  DFPlayer.volume(30); 
  delay(200);
  DFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  delay(200);
  DFPlayer.EQ(DFPLAYER_EQ_BASS);

}

void loop() {
  luminosity_detect();

  /*play button*/
  
  if((digitalRead(PLAY))and play_button==1)
  {play_button=0;delay(2);}
  if ((!digitalRead(PLAY))and play_button==0){
    random_play();
    play_button=1; 
    delay(2); // filter for push buttom
  }

  /*play luminosity*/
  
  if((light==0)and play_light==1)
  {play_light=0;delay(2);}
  if ((light==1)and play_light==0){
    random_play();
    play_light=1; 
    delay(2); 
  }
      
}

void luminosity_detect(){  
  luminosity=analogRead(A4); 
  if (luminosity<LIGHT_OFF and light==1){light=0;}
  else if (luminosity>LIGHT_ONE and light==0){light=1;}
  //Serial.println(luminosity);
}

void random_play(){
    pinMode(TX,OUTPUT);
    while (index_play==last_play){index_play=random(number_of_tracks);}
    last_play=index_play;
    DFPlayer.play(index_play);
    pinMode(TX,INPUT_PULLUP);    // to protect noise
}
