#ifdef ACF_KERNEL_IMPLEMENTATION

/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
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

#include "apu_segmentation_impl.h"

/*****************************************************************************
* functions
*****************************************************************************/

/*****************************************************************************
* local k-means algorithm 
*****************************************************************************/
void k_means_kernel_impl(vec16u* klabels, vec08u* m_Yvec, vec16u* kseedsY, vec16u* index, int bw, int bh, int pw, int /*ph*/)
{
   // superpixel 20 * 20 pixels
   for (int by = 0; by<bh; ++by)  // y data row
   {          
          for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
          {
            vec16u MinDist = 65535;  
            vec16u Y =  (vec16u) m_Yvec[bx];
            vec16u x =  (vec16u) (index[4] & 63) * 20 + bx;
            vec16u y =  (vec16u) (index[4] >> 6) * 20 + by;            
            vec16u* p_index = index;
            vec16u* p_kseedsY = kseedsY;
            
            vec16u t_klabels = 65535;
            for (int i = 0; i < 9; ++i)
            {             
              vec16u i_index = *p_index;
              vec16u i_kseedsY = *p_kseedsY;
              vec16u kx = (vec16u) (i_index & 63) * 20 + 10;
              vec16u ky = (vec16u) (i_index >> 6) * 20 + 10;    
              vec16u dY  = (Y - i_kseedsY)*(Y - i_kseedsY);              // euclidean distance of luminance
              vec16u dxy = (x - kx)*(x - kx) + (y - ky)*(y - ky);        // euclidean distance of x,y 
              vif (i_index == 65535 ) 
              {
                dxy = 5000;     
              }vendif;              
              uint8_t param = 3; // weight parameter for luminance
              vec16u dist = (dY >> param) + dxy;
              // find the closest cluster center
              vif (MinDist > dist)
              {                               
                  t_klabels = i_index;
                  MinDist =  dist;
              }vendif;
              p_index++;
              p_kseedsY++;
            }
            klabels[bx] = t_klabels;
          }
          klabels += bw;
          m_Yvec += bw;
   }
}

/*****************************************************************************
* draw contours on grayscale image
*****************************************************************************/
void draw_contours_around_segments_kernel_impl(vec16u* klabels, vec08u* inimg, vec08u* outimg, int bw, int bh, int klabelstride, int instride,  int outstride)
{
  for (int y = 0; y < bh; ++y)
  {
    for (int x = 0; x < bw; ++x)
    {
      vec08u tempimg = inimg[x];
      vec16u tempklabels = klabels[x];
      vec16u v = 0;
                 
      vif (klabels[x - klabelstride - 1] != tempklabels) v++; vendif;
      vif (klabels[x - klabelstride    ] != tempklabels) v++; vendif;
      vif (klabels[x - klabelstride + 1] != tempklabels) v++; vendif;
      
      vif (klabels[x                - 1] != tempklabels) v++; vendif;
      vif (klabels[x                   ] != tempklabels) v++; vendif;
      vif (klabels[x                + 1] != tempklabels) v++; vendif;
      
      vif (klabels[x + klabelstride - 1] != tempklabels) v++; vendif;
      vif (klabels[x + klabelstride    ] != tempklabels) v++; vendif;
      vif (klabels[x + klabelstride + 1] != tempklabels) v++; vendif;
      
            
      vif (v > 1)      
      {
          tempimg = 240;          
      }vendif;
      
      vif (tempklabels == 65535)      
      {
          tempimg = 0;          
      }vendif;
      

      //div 16
      outimg[x] = (vec08u)tempimg;
    }

    klabels += klabelstride;
    inimg += instride;
    outimg += outstride;
  }
}

/*****************************************************************************
* draw contours on RGB image
*****************************************************************************/
void draw_contours_around_segments_RGB_kernel_impl(vec16u* klabels, vec08u* inimg, vec08u* outimg, int bw, int bh, int klabelstride, int instride,  int outstride)
{
  for (int y = 0; y < bh; ++y)
  {
    int x3 = 0;
    for (int x = 0; x < bw; ++x)
    {
      vec08u tempimg[3];
      tempimg[0] = inimg[x3];
      tempimg[1] = inimg[x3 + 1];
      tempimg[2] = inimg[x3 + 2];
      vec16u tempklabels = klabels[x];
      vec16u v = 0;
                 
      vif (klabels[x - klabelstride - 1] != tempklabels) v++; vendif;
      vif (klabels[x - klabelstride    ] != tempklabels) v++; vendif;
      vif (klabels[x - klabelstride + 1] != tempklabels) v++; vendif;
      
      vif (klabels[x                - 1] != tempklabels) v++; vendif;
      vif (klabels[x                   ] != tempklabels) v++; vendif;
      vif (klabels[x                + 1] != tempklabels) v++; vendif;
      
      vif (klabels[x + klabelstride - 1] != tempklabels) v++; vendif;
      vif (klabels[x + klabelstride    ] != tempklabels) v++; vendif;
      vif (klabels[x + klabelstride + 1] != tempklabels) v++; vendif;
      
            
      vif (v > 1)      
      {
          tempimg[0] = 240;
          tempimg[1] = 240;
          tempimg[2] = 240;
      }vendif;   
     

      outimg[x3] = (vec08u)tempimg[0];
      outimg[x3 + 1] = (vec08u)tempimg[1];
      outimg[x3 + 2] = (vec08u)tempimg[2];
      x3 += 3;
    }

    klabels += klabelstride;
    inimg += instride;
    outimg += outstride;
  }
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
