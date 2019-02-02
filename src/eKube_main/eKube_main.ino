//USPI is a library I created to run the USART Port in SPI Master mode, so now we can use 2 SPI ports
//This was usefull since the APA102 LEDs don't have a CS line, so I couldn't use the same bus for 
//other devices
#include <USPI.h>
#include <SPI.h>
#include <EEPROM.h>
#include <avr/sleep.h>

const long start_frame = 0x00;
const long end_frame = 0xFF;
const byte total_leds = 54;
const byte num_of_faces = 6;

const byte brightness_select = 0x04;


//PINS:
//USART_SPI : 0:MISO 1:MOSI 4:SCLK
//SPI       : 11:MOSI 13:SCLK




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LED PLANE CONSTANTS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The following arrays represent the plane being turned clockwise.
//const byte U_plane[12]  = {8, 7, 6, 44, 43, 42, 35, 34, 33, 26, 25, 24}; //Turning the Upper face clockwise [0]-[11]
//const byte U_face[8]  = {51, 52, 53, 48, 47, 46, 45, 50};

//const byte D_plane[12]  = {0, 1, 2, 18, 19, 20, 27, 28, 29, 36, 37, 38}; //Turning lower face clockwise
//const byte D_face[8]    = {15, 16, 17, 12, 11, 10, 9, 14};

//const byte L_plane[12]  = {24, 23, 18, 15, 14, 9, 38, 39, 44, 51, 50, 45}; //Turning left face clockwise
//const byte L_face[8]  = {6, 7, 8, 3, 2, 1, 0, 5};

//const byte R_plane[12]  = {20, 21, 26, 47, 48, 53, 42, 41, 36, 11, 12, 17}; //Right face
//const byte R_face[8]  = {33, 34, 35, 30, 29, 28, 27, 32};

//const byte F_plane[12]  = {2, 3, 8, 45, 46, 47, 33, 32, 27, 17, 16, 15}; // front plane clockwise
//const byte F_face[8]  = {24, 25, 26, 21, 20, 19, 18, 23};

//const byte B_plane[12]  = {6, 5, 0, 9, 10, 11, 29, 30, 35, 53, 52, 51}; //back
//const byte B_face[8]  = {36, 41, 42, 43, 44, 39, 38, 37};

//const byte RV_plane[12] = {50, 49, 48, 34, 31, 28, 12, 13, 14, 1, 4, 7}; //Turning the face that is 1 back from the front, clockwise
//const byte FV_plane[12] = {19, 22, 25, 46, 49, 52, 43, 40, 37, 10, 13, 16}; //Plane 1 from right
//const byte FH_plane[12] = {5,4,3,23,22,21,32,31,30,41,40,44}; //Turning the face that is 1 from the top, clockwise


//Plane numbers... 0:Upper 1:Down/Lower 2:Left 3:Right 4:Front 5:Back 6:Right Vertical 7:Front Vertical 8:Front Horizontal
const byte planes[9][12] = {{6, 7, 8, 24, 25, 26, 33, 34, 35, 42, 43, 44}, {0, 1, 2, 18, 19, 20, 27, 28, 29, 36, 37, 38},
  {9, 14, 15, 18, 23, 24, 45, 50, 51, 44, 39, 38}, {11, 12, 17, 20, 21, 26, 47, 48, 53, 42, 41, 36},
  {2, 3, 8, 45, 46, 47, 33, 32, 27, 17, 16, 15}, {0, 6, 5, 51, 52, 53, 35, 30, 29, 11, 10, 9},
  {1, 4, 7, 50, 49, 48, 34, 31, 28, 12, 13, 14}, {10, 13, 16, 19, 22, 25, 46, 49, 52, 43, 40, 37},
  {5, 4, 3, 23, 22, 21, 32, 31, 30, 41, 40, 39}
};

const byte faces[6][8] = {{51, 52, 53, 48, 47, 46, 45, 50}, {15, 16, 17, 12, 11, 10, 9, 14}, {6, 7, 8, 3, 2, 1, 0, 5},
  {33, 34, 35, 30, 29, 28, 27, 32}, {24, 25, 26, 21, 20, 19, 18, 23}, {36, 41, 42, 43, 44, 39, 38, 37}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int i = 0;

//Arrayy where all the LED data is stored.
byte led_frame[total_leds][4];

const int NUM_LEDS = 54;

//USPI uspi();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INITIALIZATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0)); //TODO check max frequency
  SPI.begin();
  //Init Spi to use 4MHz
  USPI_start(4);
  //Serial.begin(9600);
  //AutoLoad();
  delay(1000);
  //attachInterrupt(0,WakeUp,CHANGE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MAIN LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  SetFace(0,1,255,0,0);
  SetFace(1,1,255,0,0);
  SetFace(2,1,0,255,0);
  SetFace(3,1,0,255,0);
  SetFace(4,1,0,0,255);
  SetFace(5,1,0,0,255);
  SendData();
  delay(2000);
  ClearCube2();
  delay(2000);
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TURN OFF ALL LEDS FUNCTION, w/o clearing array
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Right now this just sends data to turn off all the leds, but does not replace data in the array of LED values
void ClearCube() {
  for (i = 0; i < 4; i++)
  {
    SPI.transfer(start_frame);

  }
  delayMicroseconds(1);
  for (i = 0; i < NUM_LEDS; i++)
  {
    SPI.transfer(0xE0);
    delayMicroseconds(1);
    SPI.transfer(0x00);
    delayMicroseconds(1);
    SPI.transfer(0x00);
    delayMicroseconds(1);
    SPI.transfer(0x00);
    delayMicroseconds(1);
  }
  for (i = 0; i < 4; i++)
  {
    SPI.transfer(end_frame);
  }
  delayMicroseconds(1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TURN OFF ALL LEDS FUNCTION, w/ clearing array
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This updates the array to be 0 then sends the data.
void ClearCube2() {
  for (i = 0; i < 54; i++)
  {
    SetLed(i, 0xE0, 0, 0, 0);
  }
  SendData();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SEND THE DATA IN THE ARRAY TO THE LEDS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendData() {
  for (i = 0; i < 4; i++)
  {
    SPI.transfer(start_frame);
  }
  delayMicroseconds(1);
  for (i = 0; i < NUM_LEDS; i++)
  {
    SPI.transfer(led_frame[i][0]);
    delayMicroseconds(1);
    SPI.transfer(led_frame[i][1]);
    delayMicroseconds(1);
    SPI.transfer(led_frame[i][2]);
    delayMicroseconds(1);
    SPI.transfer(led_frame[i][3]);
    delayMicroseconds(1);
  }

  for ( i = 0; i < 4; i++)
  {
    SPI.transfer(end_frame);
  }
  delayMicroseconds(1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SEND DATA THRU THE USART PORT IN SPI MASTER MODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendDataUSPI()
{
   for (i = 0; i < 4; i++)
  {
    USPI_transmit(start_frame);
  }
  delayMicroseconds(1);
  for (i = 0; i < NUM_LEDS; i++)
  {
    USPI_transmit(led_frame[i][0]);
    delayMicroseconds(1);
    USPI_transmit(led_frame[i][1]);
    delayMicroseconds(1);
    USPI_transmit(led_frame[i][2]);
    delayMicroseconds(1);
    USPI_transmit(led_frame[i][3]);
    delayMicroseconds(1);
  }

  for ( i = 0; i < 4; i++)
  {
    USPI_transmit(end_frame);
  }
  delayMicroseconds(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SET A SINGLE LED IN THE ARRAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetLed(int led_num, byte brightness, byte blue, byte green, byte red) {


  led_frame[led_num][0] = brightness | 0b11100000;
  led_frame[led_num][1] = blue;
  led_frame[led_num][2] = green;
  led_frame[led_num][3] = red;


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SET A WHOLE FACE ON THE CUBE TO A SPECIFIC COLOR AND BRIGHTNESS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetFace(int face_num, byte brightness, byte blue, byte green, byte red) {

  //Set all the LEDs on one face the same color, brightness, etc.
  for (i = 0; i < 9; i++) {
    led_frame[face_num * 9 + i][0] = brightness | 0b11100000;
    led_frame[face_num * 9 + i][1] = blue;
    led_frame[face_num * 9 + i][2] = green;
    led_frame[face_num * 9 + i][3] = red;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SET A WHOLE PLANE ON THE CUBE TO A SPECIFIC COLOR AND BRIGHTNESS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetPlane(int plane_num, byte brightness, byte blue, byte green, byte red) {
  byte z;
  //Set all the LEDs on one face the same color, brightness, etc.
  for (z = 0; z < 12; z++) {
    led_frame[planes[plane_num][z]][0] = brightness | 0b11100000;
    led_frame[planes[plane_num][z]][1] = blue;
    led_frame[planes[plane_num][z]][2] = green;
    led_frame[planes[plane_num][z]][3] = red;
  }
  SendData();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Determine IF A SWIPE OCCURRED OR IF A BUTTON WAS JUST PRESSED, determine plane swiped and direction.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SwipeDetect() {


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Move the colors to the correct positions...Swipe direction 0:CW 1: CCW
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SwipeReact(byte plane_num, byte swipe_direction) {
  byte led_plane_buffer[12][4];
  byte led_face_buffer[8][4];
  short a;
  short b;

  //Buffer the plane LED data
  //Copy the LED data in the specific plane into this buffer array
  for ( a = 0; a < 12; a++) {
    led_plane_buffer[a][0] = led_frame[planes[plane_num][a]][0];
    led_plane_buffer[a][1] = led_frame[planes[plane_num][a]][1];
    led_plane_buffer[a][2] = led_frame[planes[plane_num][a]][2];
    led_plane_buffer[a][3] = led_frame[planes[plane_num][a]][3];
  }
  //Update the LEDs on the perimeter of the plane being rotated
  for ( a = 0; a < 12; a++) {
    if (swipe_direction == 1) {
      b = a - 3;
    }
    else {
      b = a + 3;
    }
    if (b < 0) {
      b = 12 + b;
    }
    else if (b > 11) {
      b = b - 12;
    }
    led_frame[planes[plane_num][a]][0] = led_plane_buffer[b][0];
//    Serial.print("led_frame[");
//    Serial.print(planes[plane_num][a], DEC);
//    Serial.print("][0] <= led_plane_buffer[");
//    Serial.print(b, DEC);
//    Serial.print("][0]");
//    Serial.println();
    led_frame[planes[plane_num][a]][1] = led_plane_buffer[b][1];
    led_frame[planes[plane_num][a]][2] = led_plane_buffer[b][2];
    led_frame[planes[plane_num][a]][3] = led_plane_buffer[b][3];

  }
  //
  //Buffer the plane LED data
  //Copy the LED data in the specific plane into this buffer array
  for ( a = 0; a < 8; a++) {
    led_face_buffer[a][0] = led_frame[faces[plane_num][a]][0];
    led_face_buffer[a][1] = led_frame[faces[plane_num][a]][1];
    led_face_buffer[a][2] = led_frame[faces[plane_num][a]][2];
    led_face_buffer[a][3] = led_frame[faces[plane_num][a]][3];
  }

  //Update the LEDs on the face of the plane being rotated
  if (plane_num < 6) {

    for ( a = 0; a < 8; a++) {
      if (swipe_direction == 1) {
        b = a - 2;
      }
      else {
        b = a + 2;
      }
      if (b < 0) {
        b = 8 + b;
      }
      else if (b > 7) {
        b = b - 8;
      }
      led_frame[faces[plane_num][a]][0] = led_face_buffer[b][0];
      led_frame[faces[plane_num][a]][1] = led_face_buffer[b][1];
      led_frame[faces[plane_num][a]][2] = led_face_buffer[b][2];
      led_frame[faces[plane_num][a]][3] = led_face_buffer[b][3];
    }

  }
  SendData();
}
void SolvedCube() {
  SetFace(0, brightness_select, 0x00, 0xFF, 0x00); //green
  SetFace(1, brightness_select, 0, 255, 255); //yellow
  SetFace(2, brightness_select, 0x00, 0x00, 0xFF); //red
  SetFace(3, brightness_select, 0xFF, 0x00, 0x00); //blue
  SetFace(4, brightness_select, 0, 69, 255); //orange
  SetFace(5, brightness_select, 0xFF, 0xFF, 0xFF); //white
  SendData();

}
//Plane numbers... 0:Upper 1:Down/Lower 2:Left 3:Right 4:Front 5:Back 6:Right Vertical 7:Front Vertical 8:Front Horizontal
//direction 0:CW 1:CCW
void Test() {

  SwipeReact(0, 1);
  delay(2000);
  SwipeReact(0, 1);
  delay(2000);
  SwipeReact(0, 1);
  delay(2000);
  SwipeReact(0, 1);
  delay(2000);

  SwipeReact(1, 1);
  delay(2000);
  SwipeReact(1, 1);
  delay(2000);
  SwipeReact(1, 1);
  delay(2000);
  SwipeReact(1, 1);
  delay(2000);

  AutoSave();


  delay(100000);


}
void SlowlyLight() {

  for (int c = 0; c < 54; c++) {

    SetLed(c, brightness_select, 255, 0, 0);
    SendData();
    delay(500);
  }


}
void CheckPlanes() {
  SetPlane(0, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(1, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(2, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(3, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(4, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(5, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(6, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(7, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);
  SetPlane(8, brightness_select, 255, 0, 0);
  delay(2000);
  ClearCube2();
  delay(2000);


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Autosave the state of the cube before it dies, or once the user turns it off and before complete shutdown.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AutoSave() {

  for (int i = 0; i < (54 * 4 - 1); i++)
  {
    for (int j = 0; j < 4; j++)
    {
      EEPROM.write((i * 4 + j), led_frame[i][j]);
      delay(10);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Autoload the state of the cube from the last saved data in EEPROM
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AutoLoad() {

  for (int i = 0; i < (54 * 4 - 1); i++)
  {
    for (int j = 0; j < 4; j++)
    {
      led_frame[i][j] = EEPROM.read((i * 4 + j));
      delay(10);//TODO see what delay is needed
    }
  }
  SendData();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to enter sleep mode, execute this if the user has turned off the cube or has not used it in a while
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GoToSleep() {

  //Use the lowest power consumtion mode..
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 

  //Asserts the sleep bit in the mcucr register
  sleep_enable();

  //Attach interrupt to pin 2, int 0. When the pin changes it will run the WakeUp function.
  attachInterrupt(0,WakeUp,CHANGE);

  //The device enters sleep mode, stays here until woken up.
  sleep_mode();
  
  //Disable sleep mode
  sleep_disable();

  //Dettach the interrupt
  detachInterrupt(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to wake up mcu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WakeUp() {
//Put anything here that you might want the device to do right after waking up.

//Enable load switches

//Load previously stored data (might already jump to setup after?? TODO
//AutoLoad();
 
}
