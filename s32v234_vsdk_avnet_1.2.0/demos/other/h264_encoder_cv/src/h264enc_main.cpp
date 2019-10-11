#include "utils_h264encoder.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define LINES_NUMBER 16
#define WIDTH 640
#define HEIGHT 480

#define INPUT_ROOT "./data/common/"

//int32_t main(int32_t argc, char *argv[])
int32_t main(void)
{
  H264Encoder::H264Encoder_setup(WIDTH, HEIGHT, LINES_NUMBER); /* create H264Encoder class instance*/

  H264Encoder::getInstance()->Initialize(); /* Inialiaize encoder and SRAM, DDR buffers*/

  FILE* pEncFile; /* file where to write output*/
  pEncFile = fopen("encoded_file.h264", "wb");
  if(pEncFile == NULL)
  {
    printf("\n fopen() pEncFile Error!!!\n");
    return 1;
  }

  cv::VideoCapture capture; /* open video */
  capture.open(INPUT_ROOT "Megamind.avi");

  if(!capture.isOpened()) // if not success, exit program
  {
    printf("Cannot open the video file: \n");
    return -1;
  } // if video open failed

  cv::Mat DefaultFrame = cv::Mat::zeros(cv::Size(720, 528), CV_8UC3);

  cv::Mat        Frame      = cv::Mat::zeros(cv::Size(WIDTH, HEIGHT), CV_8UC3); //Mat RGB image
  unsigned char* pFrame     = (unsigned char*)(Frame.data);
  unsigned long  lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0, lTimeComplete = 0;
  printf("Encoding frame: 000");
  for(int k = 0; k < 200; k++) /* encoding loop, 45 frames now */
  {
    capture >> DefaultFrame; /* capture opencv frame */

    if(DefaultFrame.data == NULL)
    {
      break;
    }

    cv::resize(DefaultFrame, Frame, Frame.size(), 0, 0, 0); /* resize for testing size 256x256 */

    GETTIME(&lTimeStart);

    H264Encoder::getInstance()->EncoderFrame((unsigned char*)DefaultFrame.data, pFrame, true); /* Encoder 1 frame*/
    int final_length = H264Encoder::getInstance()->encoded_data_length;

    GETTIME(&lTimeEnd);
    lTimeDiff = lTimeEnd - lTimeStart;
    lTimeComplete += lTimeDiff;
    fwrite((unsigned char*)DefaultFrame.data, sizeof(uint8_t), final_length / sizeof(uint8_t), pEncFile);

    printf("\b\b\b%03u : %lu us,\t %d bytes \n", k, lTimeDiff, final_length); /* print frame number */
    fflush(stdout);
  }

  int final_length = H264Encoder::getInstance()->EncoderLastFrame(
      (unsigned char*)DefaultFrame.data); /* encoder lase frame, necessary for last macroblock of the image */
  fwrite((unsigned char*)DefaultFrame.data, sizeof(uint8_t), final_length / sizeof(uint8_t), pEncFile);
  H264Encoder::getInstance()->Close();

  printf("\n\n time for frame    %lu us %d Sram \n", lTimeComplete / 200, LINES_NUMBER);

  fclose(pEncFile);

  return 0;
} // main()l
