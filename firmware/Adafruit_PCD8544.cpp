/*********************************************************************
This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

Adapted for Spark Core by Paul Kourany, April 2014
*********************************************************************/


#include "Adafruit_GFX/Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

#if !defined(PLATFORM_ID)		// Core v0.3.4
#warning "CORE"
  #define pinSetFast(_pin)		PIN_MAP[_pin].gpio_peripheral->BSRR = PIN_MAP[_pin].gpio_pin
  #define pinResetFast(_pin)	PIN_MAP[_pin].gpio_peripheral->BRR = PIN_MAP[_pin].gpio_pin
  #define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#endif

// the memory buffer for the LCD
uint8_t pcd8544_buffer[LCDWIDTH * LCDHEIGHT / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x80, 0xC0, 0x60, 0x20, 0x10, 0x18, 0x08, 0x08, 0x0C, 0x0C, 0x04, 0x04, 0x06, 0x02, 0x02, 0x02,
0x0A, 0x0A, 0xE8, 0xA8, 0xF8, 0xEC, 0xFE, 0x18, 0x0C, 0x0C, 0x16, 0x06, 0x02, 0x04, 0x08, 0x08,
0x10, 0x60, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9E, 0x8A, 0x00, 0x9E, 0x8A, 0x00,
0x9E, 0x8A, 0x00, 0x9E, 0x8A, 0x00, 0x9E, 0x8A, 0x00, 0x9E, 0x8A, 0x00, 0x9E, 0x8A, 0x00, 0x9E,
0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x42, 0x22, 0x1A,
0x0E, 0x0E, 0x06, 0x07, 0x87, 0x84, 0x44, 0x4C, 0x4C, 0x58, 0x60, 0xC8, 0xC0, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3C, 0xCB, 0x7F, 0x0F, 0xC7, 0x63, 0x11, 0x08, 0x04, 0x04, 0x03, 0x01,
0x01, 0x31, 0x21, 0x02, 0x00, 0x0C, 0x28, 0x33, 0x67, 0xDB, 0x14, 0x70, 0xC0, 0x00, 0x07, 0x02,
0xF0, 0x57, 0x02, 0xF0, 0x57, 0x02, 0xF0, 0x57, 0x02, 0xF0, 0x57, 0x02, 0xF0, 0x57, 0x02, 0xF0,
0x57, 0x02, 0x00, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xE6, 0x1B,
0x07, 0x00, 0x00, 0x00, 0x10, 0x7E, 0x25, 0x05, 0x14, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x81, 0x47, 0x24, 0x1E, 0x80, 0x40, 0x40, 0xA0, 0x20, 0x20, 0x20, 0xE1, 0xA2, 0xA4, 0xA8,
0xB0, 0x10, 0x60, 0x60, 0xA0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x11, 0x0F, 0x00,
0x00, 0x07, 0x1B, 0x6C, 0x90, 0xC0, 0x00, 0x00, 0xDE, 0x0A, 0xC0, 0x1E, 0xCA, 0x00, 0xDE, 0x0A,
0xC0, 0x1E, 0xD0, 0x1E, 0xC0, 0x1E, 0xD0, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x04, 0x04, 0x04, 0x04,
0x04, 0x04, 0x04, 0x02, 0x01, 0xC0, 0x3C, 0x22, 0x23, 0x1C, 0x08, 0x08, 0x01, 0x0F, 0x00, 0x02,
0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x00, 0x40, 0x81, 0x81, 0x01, 0x00, 0x03, 0x01,
0x02, 0x04, 0x18, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x00, 0x00, 0x03, 0x02, 0x7B, 0x40,
0x7B, 0x02, 0x7B, 0x40, 0x7B, 0x02, 0x7B, 0x40, 0x7B, 0x02, 0x7B, 0x40, 0x78, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x39, 0x47, 0xF8, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x30, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x80, 0x90, 0x4C, 0x42, 0x41, 0x81, 0x01, 0xE1, 0x19, 0x08, 0x88, 0x88, 0x50, 0x58, 0x26, 0x33,
0x12, 0x1E, 0x17, 0x11, 0x11, 0x11, 0x12, 0x08, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0x00,
0x00, 0x0F, 0xE8, 0x0F, 0xE0, 0x0F, 0xE8, 0x0F, 0xE0, 0x0F, 0xE8, 0x0F, 0xE0, 0x0F, 0xE8, 0x0F,
0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x3E, 0xC4,
0x18, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x06, 0x0B, 0x08, 0x04, 0x02, 0x12, 0x10,
0x12, 0x16, 0x14, 0x17, 0x11, 0x08, 0x08, 0x04, 0x04, 0x04, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x70, 0x1C,
0x07, 0x01, 0x00, 0x3C, 0x20, 0x3C, 0x01, 0x3D, 0x21, 0x3C, 0x01, 0x3D, 0x21, 0x3C, 0x01, 0x3D,
0x21, 0x3C, 0x01, 0x11, 0x11, 0x00, 0x00, 0x00,
};


// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
//#define enablePartialUpdate

#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif



static void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}

Adafruit_PCD8544::Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC,
    int8_t CS, int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {            // SOFTWARE SPI
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = CS;
  hwSPI = false;
}

Adafruit_PCD8544::Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC,
    int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {                       // SOFTWARE SPI NO CS
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = -1;
  hwSPI = false;
}

Adafruit_PCD8544::Adafruit_PCD8544(int8_t CS, int8_t DC,
    int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {                       // HARDWARE SPI
  _din = A5;    // MOSI
  _sclk = A3;   // SCK
  _dc = DC;
  _rst = RST;
  _cs = CS;
  hwSPI = true;
}

Adafruit_PCD8544::Adafruit_PCD8544(int8_t DC,
    int8_t RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {                       // HARDWARE SPI NO CS
  _din = A5;    // MOSI
  _sclk = A3;   // SCK
  _dc = DC;
  _rst = RST;
  _cs = -1;
  hwSPI = true;
}


// the most basic function, set a single pixel
void Adafruit_PCD8544::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);  
  else
    pcd8544_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8); 

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
uint8_t Adafruit_PCD8544::getPixel(int8_t x, int8_t y) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return 0;

  return (pcd8544_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;  
}


void Adafruit_PCD8544::begin(uint8_t contrast) {
  // set pin directions
  pinMode(_din, OUTPUT);
  pinMode(_sclk, OUTPUT);
  pinMode(_dc, OUTPUT);
  if (_rst > 0)
    pinMode(_rst, OUTPUT);
  if (_cs > 0)
    pinMode(_cs, OUTPUT);

  // toggle RST low to reset
  if (_rst > 0) {
    digitalWrite(_rst, LOW);
    delay(500);
    digitalWrite(_rst, HIGH);
  }

  if (hwSPI){
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV16);	// 36MHz / 16 = 2.25Mhz (max is 4.00MHz)
    SPI.setDataMode(0);
    SPI.begin();	
  }


  // get into the EXTENDED mode!
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

  // LCD bias select (4 is optimal?)
  command(PCD8544_SETBIAS | 0x4);

  // set VOP
  if (contrast > 0x7f)
    contrast = 0x7f;

  command( PCD8544_SETVOP | contrast); // Experimentally determined


  // normal mode
  command(PCD8544_FUNCTIONSET);

  // Set display to Normal
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  // Push out pcd8544_buffer to the Display (will show the AFI logo)
  display();
}

//Hardware SPI
inline void Adafruit_PCD8544::fastSPIwrite(uint8_t d) {
  
  if (hwSPI) {
    SPI.transfer(d);
    return;
  }
  
  for (uint8_t bit = 0; bit < 8; bit++)  {
	//PIN_MAP[_sclk].gpio_peripheral->BRR = PIN_MAP[_sclk].gpio_pin; // Clock Low
	pinResetFast(_sclk);

	if (d & (1 << (7-bit)))		// walks down mask from bit 7 to bit 0
		//PIN_MAP[_din].gpio_peripheral->BSRR = PIN_MAP[_din].gpio_pin; // Data High
		pinSetFast(_din);
	else
		//PIN_MAP[_din].gpio_peripheral->BRR = PIN_MAP[_din].gpio_pin; // Data Low
		pinResetFast(_din);
			
	//PIN_MAP[_sclk].gpio_peripheral->BSRR = PIN_MAP[_sclk].gpio_pin; // Clock High
	pinSetFast(_sclk);
	}

}

// Software SPI
inline void Adafruit_PCD8544::slowSPIwrite(uint8_t c) {

  shiftOut(_din, _sclk, MSBFIRST, c);
  
}



void Adafruit_PCD8544::command(uint8_t c) {
  //PIN_MAP[_dc].gpio_peripheral->BRR = PIN_MAP[_dc].gpio_pin;  //DC LOW
  pinResetFast(_dc);
  if (_cs > 0)
    //PIN_MAP[_cs].gpio_peripheral->BRR = PIN_MAP[_cs].gpio_pin;  //CS LOW
	pinResetFast(_cs);
  fastSPIwrite(c);
  if (_cs > 0)
    //PIN_MAP[_cs].gpio_peripheral->BSRR = PIN_MAP[_cs].gpio_pin;  //CS HIGH
    pinSetFast(_cs);
}

void Adafruit_PCD8544::data(uint8_t c) {
  //PIN_MAP[_dc].gpio_peripheral->BSRR = PIN_MAP[_dc].gpio_pin;  //DC HIGH
  pinSetFast(_dc);
  if (_cs > 0)
    //PIN_MAP[_cs].gpio_peripheral->BRR = PIN_MAP[_cs].gpio_pin;  //CS LOW
	pinResetFast(_cs);
  fastSPIwrite(c);
  if (_cs > 0)
    //PIN_MAP[_cs].gpio_peripheral->BSRR = PIN_MAP[_cs].gpio_pin;  //CS HIGH
	pinSetFast(_cs);
}

void Adafruit_PCD8544::setContrast(uint8_t val) {
  if (val > 0x7f) {
    val = 0x7f;
  }
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  command( PCD8544_SETVOP | val); 
  command(PCD8544_FUNCTIONSET);
  
 }



void Adafruit_PCD8544::display(void) {
  uint8_t col, maxcol, p;
  
  for(p = 0; p < 6; p++) {
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    command(PCD8544_SETYADDR | p);


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

    command(PCD8544_SETXADDR | col);

    digitalWrite(_dc, HIGH);
    if (_cs > 0)
      //PIN_MAP[_cs].gpio_peripheral->BRR = PIN_MAP[_cs].gpio_pin;  //CS LOW
      pinResetFast(_cs);
    for(; col <= maxcol; col++) {
      //uart_putw_dec(col);
      //uart_putchar(' ');
      fastSPIwrite(pcd8544_buffer[(LCDWIDTH*p)+col]);
    }
    if (_cs > 0)
      //PIN_MAP[_cs].gpio_peripheral->BSRR = PIN_MAP[_cs].gpio_pin;  //CS HIGH
      pinSetFast(_cs);

  }

  command(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?
#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif

}

// clear everything
void Adafruit_PCD8544::clearDisplay(void) {
  memset(pcd8544_buffer, 0, LCDWIDTH*LCDHEIGHT/8);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
  cursor_y = cursor_x = 0;
}

/*
// this doesnt touch the buffer, just clears the display RAM - might be handy
void Adafruit_PCD8544::clearDisplay(void) {
  
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {

    st7565_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      //uart_putw_dec(c);
      //uart_putchar(' ');
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7565_data(0x0);
    }     
    }

}

*/
