/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
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
#include "warp.h"
#include "config_ldw.h"
using namespace std;
using namespace cv;

////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////
Warp::Warp()// : apex_remap(APEX_APEX0)
{
	warp_init = false;
}

////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////
Warp::Warp(int apuldx)
{
        warp_init = false;
        m_apuldx = apuldx;
        
        new (&apex_remap) LDWS_REMAP_SOBEL_MEDIAN(apuldx);
}

void Warp::set_warp_init(bool value)
{
    warp_init = value;
}
////////////////////////////////////////////////////////////////////////////////
// Function warps the input image into the output using pre-computed 
// transformation.
// Uses APEX subsystem.
// @param in Input image
// @param out Output bird eye image
// @param transform Transformation LUTs to be used 
////////////////////////////////////////////////////////////////////////////////
void Warp::Warp_BirdEye(vsdk::UMat in, vsdk::UMat out, Transform_LUT &transform)
{
  // If not initialised, do it
  if (not warp_init)
  {
    // Allocate all contiguously allocated blocks
//     src = vsdk::UMat(CONFIG_SCENE_SIZE_H, 
// 	     CONFIG_SCENE_SIZE_W, 
// 	     VSDK_CV_8UC1);
//     
//     dst = vsdk::UMat(CONFIG_BIRD_SIZE_H, 
// 	     CONFIG_BIRD_SIZE_W, 
// 	     VSDK_CV_8UC1);
    
    blocks = vsdk::UMat(CONFIG_BIRD_SIZE_H / CONFIG_BIRD_CHUNK_SIZE_H, 
        CONFIG_BIRD_SIZE_W / CONFIG_BIRD_CHUNK_SIZE_W,
		VSDK_CV_32SC1);
    
    offsets = vsdk::UMat( 
         CONFIG_BIRD_SIZE_H, 
         CONFIG_BIRD_SIZE_W,
		 VSDK_CV_16UC1);
    
    deltas = vsdk::UMat(CONFIG_BIRD_SIZE_H, 
      CONFIG_BIRD_SIZE_W, 
	  VSDK_CV_8UC2);
    
    warp_init = true;
    
    // Copy LUTs into allocated memory on accessible to APEX
    memcpy(blocks.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 
	   (char *)transform.GetBlockPtr(), 
	   (CONFIG_BIRD_SIZE_W / CONFIG_BIRD_CHUNK_SIZE_W) * 
	   (CONFIG_BIRD_SIZE_H / CONFIG_BIRD_CHUNK_SIZE_H) * 4);
    
    memcpy(offsets.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 
	   (char *)transform.GetLocalPtr(), 
	   CONFIG_BIRD_SIZE_W * CONFIG_BIRD_SIZE_H * 2);
    
    memcpy(deltas.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 
	   (char *)transform.GetDeltaPtr(), 
	   CONFIG_BIRD_SIZE_W * CONFIG_BIRD_SIZE_H * 2);
    
    
    
    // Initialise APEX remap function
    apex_remap.Initialize();
    apex_remap.ConnectIO("LOCAL_OFFSET", offsets);
    apex_remap.ConnectIO("DELTA", deltas);
    //apex_remap.Start_Prepare();
    
  }
  
  apex_remap.ConnectIndirectInput("SRC_IMG", in, blocks);
  apex_remap.ConnectIO("DST_IMG", out);
  
  // Run APEX process
  apex_remap.Start();
  /*LDWS_REMAP_SOBEL_MEDIAN apex_remap2(APEX_APEX0);
  apex_remap2.Initialize();
  apex_remap2.ConnectIndirectInput("SRC_IMG", src, blocks);
  apex_remap2.ConnectIO("DST_IMG", dst);
  apex_remap2.ConnectIO("LOCAL_OFFSET", offsets);
  apex_remap2.ConnectIO("DELTA", deltas);
  apex_remap2.Start();*/
  // Wait till done
  //apex_remap.Wait_NoReset();
  apex_remap.Wait();

}
