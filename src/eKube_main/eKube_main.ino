

#include <SPI.h>
const long start_frame = 0x00;
const long end_frame = 0xFF;
const byte total_leds = 54;
const byte num_of_faces = 6;

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
//const byte B_face[8]  = {36, 37, 38, 39, 44, 43, 42, 41};

//const byte RM_plane[12] = {50, 49, 48, 34, 31, 28, 12, 13, 14, 1, 4, 7}; //Turning the face that is 1 back from the front, clockwise
//const byte FM_plane[12] = {19, 22, 25, 46, 49, 52, 43, 40, 37, 10, 13, 16}; //Plane 1 from right



const byte planes[8][12] = {{8, 7, 6, 44, 43, 42, 35, 34, 33, 26, 25, 24}, {0, 1, 2, 18, 19, 20, 27, 28, 29, 36, 37, 38},
  {24, 23, 18, 15, 14, 9, 38, 39, 44, 51, 50, 45}, {20, 21, 26, 47, 48, 53, 42, 41, 36, 11, 12, 17},
  {2, 3, 8, 45, 46, 47, 33, 32, 27, 17, 16, 15}, {6, 5, 0, 9, 10, 11, 29, 30, 35, 53, 52, 51},
  {50, 49, 48, 34, 31, 28, 12, 13, 14, 1, 4, 7}, {19, 22, 25, 46, 49, 52, 43, 40, 37, 10, 13, 16}
};

const byte faces[6][8] = {{51, 52, 53, 48, 47, 46, 45, 50}, {15, 16, 17, 12, 11, 10, 9, 14}, {6, 7, 8, 3, 2, 1, 0, 5},
  {33, 34, 35, 30, 29, 28, 27, 32}, {24, 25, 26, 21, 20, 19, 18, 23}, {36, 37, 38, 39, 44, 43, 42, 41}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int i = 0;

//Arrayy
byte led_frame[total_leds][4];

const int NUM_LEDS = 54;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INITIALIZATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0)); //TODO check max frequency
  SPI.begin();
  //Initialize cube
  SetFace(0, 0x07, 0x00, 0xFF, 0x00); //green
  delay(100);
  SetFace(1, 0x07, 0, 255, 255); //yellow
  delay(100);
  SetFace(2, 0x07, 0x00, 0x00, 0xFF); //red
  delay(100);
  SetFace(3, 0x07, 0xFF, 0x00, 0x00); //blue
  delay(100);
  SetFace(4, 0x07, 0, 69, 255); //orange
  delay(100);
  SetFace(5, 0x07, 0xFF, 0xFF, 0xFF); //white
  delay(100);
  SendData();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MAIN LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  for(int i=0;i<255;i++){
  SetFace(0, 0x07, 0, 0, i); //green
  SetFace(1, 0x07, 0,i, 0); //yellow
  SetFace(2, 0x07, 0, i, i); //red
  SetFace(3, 0x07, i, 0, 0); //blue
  SetFace(4, 0x07, i, 0, i); //orange
  SetFace(5, 0x07, i, i, 0); //white
  
  SendData();
  delay(50);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TURN OFF ALL LEDS FUNCTION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearCube() {
  for (i = 0; i < 4; i++)
  {
    SPI.transfer(start_frame);

  }
  delayMicroseconds(1);
  for (i = 0; i < NUM_LEDS; i++)
  {
    SPI.transfer(0xE7);
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
//Determine IF A SWIPE OCCURRED OR IF A BUTTON WAS JUST PRESSED, determine plane swiped and direction.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SwipeDetect() {


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Move the colors to the correct positions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SwipeReact(byte plane_num, byte swipe_direction) {
  byte b;
//Update the LEDs on the perimeter of the plane being rotated
  for (byte a = 0; a < 12; a++) {
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
      b = 12 - b;
    }
    led_frame[planes[plane_num][a]][0] = led_frame[planes[plane_num][b]][0];
    led_frame[planes[plane_num][a]][1] = led_frame[planes[plane_num][b]][1];
    led_frame[planes[plane_num][a]][2] = led_frame[planes[plane_num][b]][2];
    led_frame[planes[plane_num][a]][3] = led_frame[planes[plane_num][b]][3];



  }

  //Update the LEDs on the face of the plane being rotated
  if (plane_num < 6) {

    for (byte a = 0; a < 8; a++) {
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
        b = 8 - b;
      }
      led_frame[planes[plane_num][a]][0] = led_frame[planes[plane_num][b]][0];
      led_frame[planes[plane_num][a]][1] = led_frame[planes[plane_num][b]][1];
      led_frame[planes[plane_num][a]][2] = led_frame[planes[plane_num][b]][2];
      led_frame[planes[plane_num][a]][3] = led_frame[planes[plane_num][b]][3];

    }

  }
}
