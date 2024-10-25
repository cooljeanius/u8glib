/*

  u8g_dev_uc1610_dogxl160.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  
*/

#include "u8g.h"

#define WIDTH 160
#define HEIGHT 104

static const uint8_t u8g_dev_uc1610_dogxl160_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip */
  0x0f1,	        /* set display height-1 */
  0x067,		/*  */
  0x0c0,		/* SEG & COM normal */
  0x040,		/* set display start line */
  0x050,		/* */
  0x02b,		/* set panelloading */
  0x0eb,		/* set bias 1/2 */
  0x081,		/* set contrast */
  0x05f,		/* */
  0x089,		/* set auto increment */
  0x0a6,		/* normal pixel mode */
  0x0d3,		/* 0xd3=40% RMS separation for gray levels */
  0x0af,		/* display on */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0a5,		                /* display all points, ST7565, UC1610 */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0a4,		                /* normal display */
  
  
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_uc1610_dogxl160_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x010,		/* set upper 4 bit of the col adr to 0 */
  0x000,		/* set lower 4 bit of the col adr to 0 */      
  U8G_ESC_END                /* end of sequence */
};

static uint8_t u8g_dev_1to2(uint8_t n)
{
  register uint8_t a,b,c;
  a = n;
  a &= 1;
  n <<= 1;
  b = n;
  b &= 4;  
  n <<= 1;
  c = n;
  c &= 16;
  n <<= 1;
  n &= 64;
  n |= a;
  n |= b;
  n |= c;
  n |= n << 1;
  return n;
}

uint8_t u8g_dev_uc1610_dogxl160_bw_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        int i;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*2) ); /* select current page 1/2 (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        for( i = 0; i < WIDTH; i++ )
        {
          u8g_WriteByte(u8g, dev, u8g_dev_1to2( ((uint8_t *)(pb->buf))[i] ) );          
        }

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*2+1) ); /* select current page 2/2 (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        for( i = 0; i < WIDTH; i++ )
        {
          u8g_WriteByte(u8g, dev, u8g_dev_1to2( ((uint8_t *)(pb->buf))[i] >> 4 ) );
        }
        
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_SetChipSelect(u8g, dev, 0);      
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_uc1610_dogxl160_gr_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page) ); /* select current page (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_SetChipSelect(u8g, dev, 0);      
      return 1;
  }
  return u8g_dev_pb8v2_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_uc1610_dogxl160_2x_bw_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        int i;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*4) ); /* select current page 1/2 (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        for( i = 0; i < WIDTH; i++ )
        {
          u8g_WriteByte(u8g, dev, u8g_dev_1to2( ((uint8_t *)(pb->buf))[i] ) );          
        }

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*4+1) ); /* select current page 2/2 (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        for( i = 0; i < WIDTH; i++ )
        {
          u8g_WriteByte(u8g, dev, u8g_dev_1to2( ((uint8_t *)(pb->buf))[i] >> 4 ) );          
        }

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*4+2) ); /* select current page 1/2 (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        for( i = 0; i < WIDTH; i++ )
        {
          u8g_WriteByte(u8g, dev, u8g_dev_1to2( ((uint8_t *)((uint8_t *)(pb->buf)+WIDTH))[i] ) );          
        }

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*4+3) ); /* select current page 2/2 (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        for( i = 0; i < WIDTH; i++ )
        {
          u8g_WriteByte(u8g, dev, u8g_dev_1to2( ((uint8_t *)((uint8_t *)(pb->buf)+WIDTH))[i] >> 4 ) );          
        }
        
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_SetChipSelect(u8g, dev, 0);      
      return 1;
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_uc1610_dogxl160_2x_gr_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*2) ); /* select current page (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_WriteSequence(u8g, dev, WIDTH, pb->buf) == 0 )
          return 0;

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1610_dogxl160_data_start);    
        u8g_WriteByte(u8g, dev, 0x060 | (pb->p.page*2+1) ); /* select current page (UC1610) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_WriteSequence(u8g, dev, WIDTH, (uint8_t *)(pb->buf)+WIDTH) == 0 )
          return 0;
        
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_SetChipSelect(u8g, dev, 0);      
      return 1;
  }
  return u8g_dev_pb16v2_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_uc1610_dogxl160_bw_sw_spi, WIDTH, HEIGHT, 8, u8g_dev_uc1610_dogxl160_bw_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_uc1610_dogxl160_bw_hw_spi, WIDTH, HEIGHT, 8, u8g_dev_uc1610_dogxl160_bw_fn, U8G_COM_HW_SPI);

U8G_PB_DEV(u8g_dev_uc1610_dogxl160_gr_sw_spi, WIDTH, HEIGHT, 4, u8g_dev_uc1610_dogxl160_gr_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_uc1610_dogxl160_gr_hw_spi, WIDTH, HEIGHT, 4, u8g_dev_uc1610_dogxl160_gr_fn, U8G_COM_HW_SPI);

uint8_t u8g_dev_uc1610_dogxl160_2x_bw_buf[WIDTH*2] U8G_NOCOMMON ; 
u8g_pb_t u8g_dev_uc1610_dogxl160_2x_bw_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_uc1610_dogxl160_2x_bw_buf}; 
u8g_dev_t u8g_dev_uc1610_dogxl160_2x_bw_sw_spi = { u8g_dev_uc1610_dogxl160_2x_bw_fn, &u8g_dev_uc1610_dogxl160_2x_bw_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_uc1610_dogxl160_2x_bw_hw_spi = { u8g_dev_uc1610_dogxl160_2x_bw_fn, &u8g_dev_uc1610_dogxl160_2x_bw_pb, U8G_COM_HW_SPI };

uint8_t u8g_dev_uc1610_dogxl160_2x_gr_buf[WIDTH*2] U8G_NOCOMMON ; 
u8g_pb_t u8g_dev_uc1610_dogxl160_2x_gr_pb = { {8, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_uc1610_dogxl160_2x_gr_buf}; 
u8g_dev_t u8g_dev_uc1610_dogxl160_2x_gr_sw_spi = { u8g_dev_uc1610_dogxl160_2x_gr_fn, &u8g_dev_uc1610_dogxl160_2x_gr_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_uc1610_dogxl160_2x_gr_hw_spi = { u8g_dev_uc1610_dogxl160_2x_gr_fn, &u8g_dev_uc1610_dogxl160_2x_gr_pb, U8G_COM_HW_SPI };
