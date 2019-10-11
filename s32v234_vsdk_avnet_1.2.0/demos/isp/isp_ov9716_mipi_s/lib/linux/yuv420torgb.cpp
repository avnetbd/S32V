/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2016 NX Semiconductors;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#include <iostream>

extern "C"
void Neon_Yuv420ToRgb(unsigned char* arRgb,
          unsigned char* arY,
          unsigned char* arUv,
          uint16_t arLength)
{
  int iterations = arLength >> 3;

  asm volatile
    (
     "movi v28.8H, #25\n"               //0.392 * 2^6 cca   uG const
     "movi v29.8H, #37\n"               //0.581 * 2^6 cca   vG const
     "movi v30.8H, #130\n"              //2.033 * 2^6 cca   B const
     "movi v31.8H, #72\n"               //1.14  * 2^6 cca   R const

     "movi v27.8H, #128\n"              //1.14  * 2^7 cca   R const

    "1:\n"
     "LD1 {v0.8B}, [%[arY]], #8\n"      // Y value

     "LD1 {v4.8B}, [%[arUv]], #8\n"
   
     "MOV v5.8B, v4.8B\n"

     "TRN1 v25.8B, v4.8B, v5.8B\n"      //v4 V0 V0 V1 V1 V2 V2 V3 V3
     "TRN2 v26.8B, v4.8B, v5.8B\n"      //v5 U0 U0 U1 U1 U2 U2 U3 U3

     //widening Y channels values to 16bits
     "UXTL   v0.8H, v0.8B\n"
     "UXTL   v23.4S, v0.4H\n"
     "UXTL2  v24.4S, v0.8H\n"
     "UQSHL  v23.4S, v23.4S, #6\n"
     "UQSHL  v24.4S, v24.4S, #6\n"
     
     
     //widening UV channel to 16bits
     "UXTL   v4.8H, v25.8B\n"           //put it into v4 and v5 registers
     "UXTL   v5.8H, v26.8B\n"

     "SUB v4.8H, v4.8H, v27.8H\n"
     "SUB v5.8H, v5.8H, v27.8H\n"
   
     
     //G channel
     "SMULL v6.4S, v4.4H, v28.4H\n"     //v4 je U, v28 je koeficient
     "SMULL2 v7.4S, v4.8H, v28.8H\n"

     "SMULL v8.4S, v5.4H, v29.4H\n"     //v4 je V, v28 je koeficient
     "SMULL2 v9.4S, v5.8H, v29.8H\n"

     "SQADD v6.4S, v6.4s, v8.4S\n"      // (U * const + V * const)
     "SQADD v7.4S, v7.4s, v9.4S\n"      // (V * const + V * const)
    
     "SQSUB v10.4S, v23.4S, v6.4S\n"    //Y - U (neco) nula to je
     "SQSUB v11.4S, v24.4S, v7.4S\n"    //Y - U (neco) nula to je 
     
     "SHRN v10.4H, v10.4S, #6\n"        //podelim 256
     "SHRN2 v10.8H, v11.4S, #6\n"

     "SQXTUN v21.8B, v10.8H\n"          // narrowing
    
    
    // R channel
     "SMULL  v12.4S, v5.4H, v31.4H\n"   //R channel: V * const
     "SMULL2 v13.4S, v5.8H, v31.8H\n"   //R channel: V * const(second part)

     "SQADD v15.4S, v23.4S, v12.4S\n"   // R channell (Y + V*neco)
     "SQADD v16.4S, v24.4S, v13.4S\n"   // R channell (Y + V*neco)
        
     "SHRN v15.4H, v15.4S, #6\n"        //podelim 256
     "SHRN2 v15.8H, v16.4S, #6\n"
     
     "SQXTUN v22.8B, v15.8H\n" // narrowing
     
     //B channel
     "SMULL v16.4S, v4.4H, v30.4H\n"    //B channel : U * const
     "SMULL2 v17.4S, v4.8H, v30.8H\n"   //B channel : U * const (second part)
    
     "SQADD v16.4S, v23.4S, v16.4S\n"   // R channell (Y + V*neco)
     "SQADD v17.4S, v24.4S, v17.4S\n"   // R channell (Y + V*neco)
    
     "SHRN v19.4H, v16.4S, #6\n"        //podelim 64
     "SHRN2 v19.8H, v17.4S, #6\n"
     
     "SQXTUN v20.8B, v19.8H\n"          // narrowing
   
//     B je v20
//     G je v21
//     R je v22

     "ST3 {v20.B, v21.B, v22.B}[0], [%[arRgb]], #3\n"
     "ST3 {v20.B, v21.B, v22.B}[1], [%[arRgb]], #3\n"
     "ST3 {v20.B, v21.B, v22.B}[2], [%[arRgb]], #3\n"
     "ST3 {v20.B, v21.B, v22.B}[3], [%[arRgb]], #3\n"

     "ST3 {v20.B, v21.B, v22.B}[4], [%[arRgb]], #3\n"
     "ST3 {v20.B, v21.B, v22.B}[5], [%[arRgb]], #3\n"
     "ST3 {v20.B, v21.B, v22.B}[6], [%[arRgb]], #3\n"
     "ST3 {v20.B, v21.B, v22.B}[7], [%[arRgb]], #3\n"

     "subs %[iterations],%[iterations],#1\n"
     "bne 1b\n"

      
      

     :
     : [arY]"r"(arY), [iterations]"r"(iterations), [arRgb]"r"(arRgb), [arUv]"r"(arUv)
     :

     );
}

#define UTOG (6475L)      // 0,392 * 2^14
#define VTOG (9518L)      // 0,581 * 2^14
#define UTOB (33309L)     // 2,033 * 2^14
#define VTOR (18678L)     // 1,14  * 2^14
#define FRACTION (14)

//#define UTOG (25L)      // 0,392 * 2^6
//#define VTOG (37L)      // 0,581 * 2^6
//#define UTOB (130L)     // 2,033 * 2^6
//#define VTOR (72L)      // 1,14  * 2^6
//#define FRACTION (6)

extern "C"
void A53_Yuv420ToRgb(unsigned char *arRgb,
                     unsigned char *arY,
                     unsigned char *arUv,
                     uint32_t arLength)
{
  // pixel values
  uint32_t lXPos;
  int32_t  lU,lV;
  int32_t  lR,lG,lB,lGB,lGR,lY;

  for (lXPos = 0;lXPos < arLength; lXPos++)
  {
    lU  =*(arUv++)-128;
    lGB = (int32_t)lU * UTOG;
    lV  =*(arUv++)-128;
    lGR = (int32_t)lV * VTOG;
    lY  =(*arY++)<<FRACTION;
    lG  =(lY - lGB - lGR)>>FRACTION;
    lB  =(lY + lU * UTOB)>>FRACTION;
    lR  =(lY + lV * VTOR)>>FRACTION;
    if (lR>255) lR=255; if (lR<0) lR=0;
    if (lG>255) lG=255; if (lG<0) lG=0;
    if (lB>255) lB=255; if (lB<0) lB=0;
    *(arRgb++)=(uint8_t)lB;
    *(arRgb++)=(uint8_t)lG;
    *(arRgb++)=(uint8_t)lR;
    lY  =(*arY++)<<FRACTION;
    lG  =(lY - lGB - lGR)>>FRACTION;
    lB  =(lY + lU * UTOB)>>FRACTION;
    lR  =(lY + lV * VTOR)>>FRACTION;
    if (lR>255) lR=255; if (lR<0) lR=0;
    if (lG>255) lG=255; if (lG<0) lG=0;
    if (lB>255) lB=255; if (lB<0) lB=0;
    *(arRgb++)=(uint8_t)lB;
    *(arRgb++)=(uint8_t)lG;
    *(arRgb++)=(uint8_t)lR;
  }
}
