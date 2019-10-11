#include <visualisation.h>

namespace visualisation
{
static int left_warning  = -10;
static int right_warning = -10;
////////////////////////////////////////////////////////////////////////////////
// Function draws a left lane cross warning into the mat image
////////////////////////////////////////////////////////////////////////////////
void drawLeftWarning(cv::Mat mat)
{
  cv::Point triangle[1][3];

  triangle[0][0] = cv::Point((webserver_getIntValueByName("tran_up_left_scene_x", CONFIG_SCENE_POINT_0_0) +
                              webserver_getIntValueByName("tran_down_left_scene_x", CONFIG_SCENE_POINT_2_0) - 50),
                             webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                                 webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1));
  triangle[0][1] = cv::Point((webserver_getIntValueByName("tran_up_left_scene_x", CONFIG_SCENE_POINT_0_0) +
                              webserver_getIntValueByName("tran_down_left_scene_x", CONFIG_SCENE_POINT_2_0) + 50),
                             webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                                 webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1));
  triangle[0][2] = cv::Point((webserver_getIntValueByName("tran_up_left_scene_x", CONFIG_SCENE_POINT_0_0) +
                              webserver_getIntValueByName("tran_down_left_scene_x", CONFIG_SCENE_POINT_2_0)),
                             webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                                 webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1) - 100);

  const cv::Point* ppt[1] = {triangle[0]};
  int              npt[]  = {3};

  cv::fillPoly(mat, ppt, npt, 1, cv::Scalar(0, 255, 255), 8);
  cv::polylines(mat, ppt, npt, 1, true, cv::Scalar(0, 0, 0), 8);
  cv::putText(mat, "!",
              cv::Point((webserver_getIntValueByName("tran_up_left_scene_x", CONFIG_SCENE_POINT_0_0) +
                         webserver_getIntValueByName("tran_down_left_scene_x", CONFIG_SCENE_POINT_2_0) - 12),
                        webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                            webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1) - 25),
              2, 2, cv::Scalar(0, 0, 0), 2);
}

////////////////////////////////////////////////////////////////////////////////
// Function draws a right lane cross warning into the mat image
////////////////////////////////////////////////////////////////////////////////
void drawRightWarning(cv::Mat mat)
{
  cv::Point triangle[1][3];

  triangle[0][0] = cv::Point((webserver_getIntValueByName("tran_up_right_scene_x", CONFIG_SCENE_POINT_1_0) +
                              webserver_getIntValueByName("tran_down_right_scene_x", CONFIG_SCENE_POINT_3_0) - 50),
                             webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                                 webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1));
  triangle[0][1] = cv::Point((webserver_getIntValueByName("tran_up_right_scene_x", CONFIG_SCENE_POINT_1_0) +
                              webserver_getIntValueByName("tran_down_right_scene_x", CONFIG_SCENE_POINT_3_0) + 50),
                             webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                                 webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1));
  triangle[0][2] = cv::Point((webserver_getIntValueByName("tran_up_right_scene_x", CONFIG_SCENE_POINT_1_0) +
                              webserver_getIntValueByName("tran_down_right_scene_x", CONFIG_SCENE_POINT_3_0)),
                             webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                                 webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1) - 100);

  const cv::Point* ppt[1] = {triangle[0]};
  int              npt[]  = {3};

  cv::fillPoly(mat, ppt, npt, 1, cv::Scalar(0, 255, 255), 8);
  cv::polylines(mat, ppt, npt, 1, true, cv::Scalar(0, 0, 0), 8);
  cv::putText(mat, "!",
              cv::Point((webserver_getIntValueByName("tran_up_right_scene_x", CONFIG_SCENE_POINT_1_0) +
                         webserver_getIntValueByName("tran_down_right_scene_x", CONFIG_SCENE_POINT_3_0) - 12),
                        webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) +
                            webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1) - 25),
              2, 2, cv::Scalar(0, 0, 0), 2);
}

////////////////////////////////////////////////////////////////////////////////
// Function draws a right lane cross warning into the mat image
////////////////////////////////////////////////////////////////////////////////
void drawLineHealthy(cv::Mat mat, LaneDetector* detector)
{
  if(detector->m_lane_healthy)
  {
    cv::line(mat, cv::Point(640 - 5, 20), cv::Point(640 - 7, 26), cv::Scalar(0, 150, 0), 3);
    cv::line(mat, cv::Point(640 + 5, 20), cv::Point(640 + 7, 26), cv::Scalar(0, 150, 0), 3);
    cv::line(mat, cv::Point(640 - 8, 30), cv::Point(640 - 10, 36), cv::Scalar(0, 150, 0), 3);
    cv::line(mat, cv::Point(640 + 8, 30), cv::Point(640 + 10, 36), cv::Scalar(0, 150, 0), 3);
    cv::line(mat, cv::Point(640 - 11, 40), cv::Point(640 - 13, 46), cv::Scalar(0, 150, 0), 3);
    cv::line(mat, cv::Point(640 + 11, 40), cv::Point(640 + 13, 46), cv::Scalar(0, 150, 0), 3);
    cv::circle(mat, cv::Point(640, 34), 25, cv::Scalar(0, 150, 0), 3);
  }
  else
  {
    cv::line(mat, cv::Point(640 + 8, 34 + 8), cv::Point(640 - 8, 34 - 8), cv::Scalar(0, 0, 150), 3);
    cv::line(mat, cv::Point(640 + 8, 34 - 8), cv::Point(640 - 8, 34 + 8), cv::Scalar(0, 0, 150), 3);
    cv::circle(mat, cv::Point(640, 34), 25, cv::Scalar(0, 0, 150), 3);
  }

  // reset counters for flashing warnings
  left_warning  = -10;
  right_warning = -10;
}

void drawLaneQuadrangle(cv::Mat mat, LaneDetector* detector)
{
  Lane lane = detector->GetCurrentLane();

  // Compute lane quadrangle points
  cv::Vec2i vec_left  = lane.left[1] - lane.left[0];
  cv::Vec2i vec_right = lane.right[1] - lane.right[0];

  lane.left[0].x  = (lane.left[0].x) * 2;
  lane.left[0].y  = (lane.left[0].y) * 2;
  lane.right[0].x = (lane.right[0].x) * 2;
  lane.right[0].y = (lane.right[0].y) * 2;
  lane.left[1].x += vec_left.val[0];
  lane.left[1].y += vec_left.val[1];
  lane.right[1].x += vec_right.val[0];
  lane.right[1].y += vec_right.val[1];

  lane.left[1].x  = (lane.left[1].x) * 2;
  lane.left[1].y  = (lane.left[1].y) * 2;
  lane.right[1].x = (lane.right[1].x) * 2;
  lane.right[1].y = (lane.right[1].y) * 2;

  /*cv::circle(mat, cv::Point(1240, 40), 10, cv::Scalar(0, 0, 255), 2);
	cv::circle(mat, cv::Point(1240, 680), 10, cv::Scalar(0, 0, 255), 2);
	cv::circle(mat, cv::Point(40,   40), 10, cv::Scalar(0, 0, 255), 2);
	cv::circle(mat, cv::Point(40,   680), 10, cv::Scalar(0, 0, 255), 2);*/

  if(webserver_getBoolValueByName("draw_bird", false))
  {
    cv::Point leftUp(webserver_getIntValueByName("tran_up_left_scene_x", CONFIG_SCENE_POINT_0_0) * 2,
                     webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) * 2);
    cv::Point rightUp(webserver_getIntValueByName("tran_up_right_scene_x", CONFIG_SCENE_POINT_1_0) * 2,
                      webserver_getIntValueByName("tran_up_right_scene_y", CONFIG_SCENE_POINT_1_1) * 2);
    cv::Point leftDown(webserver_getIntValueByName("tran_down_left_scene_x", CONFIG_SCENE_POINT_2_0) * 2,
                       webserver_getIntValueByName("tran_down_left_scene_y", CONFIG_SCENE_POINT_2_1) * 2);
    cv::Point rightDown(webserver_getIntValueByName("tran_down_right_scene_x", CONFIG_SCENE_POINT_3_0) * 2,
                        webserver_getIntValueByName("tran_down_right_scene_y", CONFIG_SCENE_POINT_3_1) * 2);

    cv::circle(mat, leftUp, 5, cv::Scalar(255, 0, 0), 2);
    cv::circle(mat, rightUp, 5, cv::Scalar(255, 0, 0), 2);
    cv::circle(mat, leftDown, 5, cv::Scalar(255, 0, 0), 2);
    cv::circle(mat, rightDown, 5, cv::Scalar(255, 0, 0), 2);

    //line(mat, leftUp, rightUp, cv::Scalar(255, 0, 0), 1);
    line(mat, rightUp, rightDown, cv::Scalar(255, 0, 0), 1);
    //line(mat, rightDown, leftDown, cv::Scalar(255, 0, 0), 1);
    line(mat, leftDown, leftUp, cv::Scalar(255, 0, 0), 1);
  }
  char text[255];

  if(webserver_getBoolValueByName("draw_lane_positions", false))
  {
    sprintf(text, "L: %d", (int)((lane.left_bird[0].x + lane.left_bird[1].x) / 2 * 100 / CONFIG_BIRD_WIDTH));
    cv::putText(mat, text,
                cv::Point(webserver_getIntValueByName("tran_up_left_scene_x", CONFIG_SCENE_POINT_0_0) * 2 - 50,
                          webserver_getIntValueByName("tran_up_left_scene_y", CONFIG_SCENE_POINT_0_1) * 2 - 30),
                2, 1.3, cv::Scalar(255, 0, 0), 2);

    sprintf(text, "R: %d", (int)((lane.right_bird[0].x + lane.right_bird[1].x) / 2 * 100 / CONFIG_BIRD_WIDTH));
    cv::putText(mat, text,
                cv::Point(webserver_getIntValueByName("tran_up_right_scene_x", CONFIG_SCENE_POINT_0_0) * 2 - 50,
                          webserver_getIntValueByName("tran_up_right_scene_y", CONFIG_SCENE_POINT_0_1) * 2 - 30),
                2, 1.3, cv::Scalar(255, 0, 0), 2);
  }

  if(detector->m_lane_healthy)
  {
    line(mat, lane.left[0], lane.left[1], cv::Scalar(0, 255, 0), 3);
    line(mat, lane.right[0], lane.right[1], cv::Scalar(0, 255, 0), 3);
  }
  else
  {
    line(mat, lane.left[0], lane.left[1], cv::Scalar(0, 0, 255), 3);
    line(mat, lane.right[0], lane.right[1], cv::Scalar(0, 0, 255), 3);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Function draws fps and analysys time to image
////////////////////////////////////////////////////////////////////////////////
void drawFPSandAnalytime(cv::Mat mat, float fps, float analyTime)
{
  char fps_text[255];
  sprintf(fps_text, "FPS: %d", (int)fps);
  cv::putText(mat, fps_text, cv::Point(40, 60), 2, 1.3, cv::Scalar(0, 0, 255), 2);

  sprintf(fps_text, "Fr.Time: %d [ms]", (int)(analyTime * 0.001));
  cv::putText(mat, fps_text, cv::Point(40, 120), 2, 1.3, cv::Scalar(0, 0, 255), 2);
}
}