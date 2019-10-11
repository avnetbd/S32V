#include <xml_input.h>
#include "data/common/headers/lbpcascade_frontalface.h"

#include <iostream>
#include <sstream>

using namespace tinyxml2;
using namespace std;

XMLReader::XMLReader() : windowHeight(1), windowWidth(1)
{
}

XMLReader::~XMLReader()
{
  delete apexLBPFeatures;
  delete apexLBPStages;
}

void XMLReader::Init()
{
#ifdef __STANDALONE__
  XMLDocument cascades;
  if(cascades.LoadString(lbpcascade_frontalface) != XML_SUCCESS)
  {
    cout << "can't open the cascades file" << endl;
    return;
  }
#else
  XMLDocument cascades;
  if(cascades.LoadFile("data/common/lbpcascade_frontalface.xml") != XML_SUCCESS)
  {
    cout << "can't open the cascades file" << endl;
    return;
  }
#endif

  //start at the size node
  XMLNode* node =
      cascades.FirstChild()->NextSibling()->NextSibling()->FirstChild()->FirstChild()->NextSibling()->NextSibling();

  {
    stringstream ss(node->ToElement()->GetText());
    ss >> windowHeight;
  }

  node = node->NextSibling();

  {
    stringstream ss(node->ToElement()->GetText());
    ss >> windowWidth;
  }

  //TODO: stage params?

  //stages
  node = node->NextSibling()->NextSibling()->NextSibling()->NextSibling();

  //int windowViewScale = 20;

  XMLNode* stageNode = node->FirstChild();

  while(stageNode)
  {
    stageNode = stageNode->NextSibling();
    lbpStage lbps;
    lbps.firstFeatureIndex = (int)lbpFeatures.size();

    XMLNode*     stageSumNode = stageNode->FirstChild()->NextSibling();
    stringstream sss(stageSumNode->ToElement()->GetText());

    double dThr;
    sss >> dThr;
    lbps.thresholdFixed = STAGE_FIXED_POINT_TYPE(dThr * STAGE_FIXED_POINT_MULTIPLIER);

    XMLNode* featureNode = stageSumNode->NextSibling()->FirstChild();
    while(featureNode)
    {
      featureNode = featureNode->NextSibling();
      lbpFeature lbpf;

      XMLNode*     innerNode = featureNode->FirstChild();
      stringstream fss0(innerNode->ToElement()->GetText());
      string       temp;
      fss0 >> temp >> temp >> lbpf.rectangleIndex >> lbpf.values[0] >> lbpf.values[1] >> lbpf.values[2] >>
          lbpf.values[3] >> lbpf.values[4] >> lbpf.values[5] >> lbpf.values[6] >> lbpf.values[7];

      innerNode = innerNode->NextSibling();

      stringstream fss1(innerNode->ToElement()->GetText());

      double dLV0, dLV1;
      fss1 >> dLV0 >> dLV1;
      lbpf.leafValuesFixed[0] = STAGE_FIXED_POINT_TYPE(dLV0 * STAGE_FIXED_POINT_MULTIPLIER);
      lbpf.leafValuesFixed[1] = STAGE_FIXED_POINT_TYPE(dLV1 * STAGE_FIXED_POINT_MULTIPLIER);

      lbpFeatures.push_back(lbpf);

      featureNode = featureNode->NextSibling();
    }

    lbps.featureCount = (int)lbpFeatures.size() - lbps.firstFeatureIndex;
    lbpStages.push_back(lbps);

    stageNode = stageNode->NextSibling();
  }

  node                   = node->NextSibling();
  XMLNode* rectangleNode = node->FirstChild();
  while(rectangleNode)
  {
    lbpRectangle lbpr;
    stringstream rss(rectangleNode->FirstChild()->ToElement()->GetText());
    rss >> lbpr.x >> lbpr.y >> lbpr.width >> lbpr.height;

    lbpRectangles.push_back(lbpr);

    rectangleNode = rectangleNode->NextSibling();
  }

  //apex structs
  apexLBPFeatures     = new APEX_lbpFeature[lbpFeatures.size()];
  apexLBPStages       = new APEX_lbpStage[lbpStages.size()];
  apexLBPFeaturesSize = lbpFeatures.size() * sizeof(APEX_lbpFeature);
  apexLBPStagesSize   = lbpStages.size() * sizeof(APEX_lbpStage);

  for(unsigned int i = 0; i < lbpFeatures.size(); ++i)
  {
    lbpFeature   lbpf = lbpFeatures[i];
    lbpRectangle lbpr = lbpRectangles[lbpf.rectangleIndex];
    for(int j = 0; j < 8; ++j)
    {
      apexLBPFeatures[i].values[j] = lbpf.values[j];
    }
    apexLBPFeatures[i].leafValues[0] = lbpf.leafValuesFixed[0];
    apexLBPFeatures[i].leafValues[1] = lbpf.leafValuesFixed[1];
    apexLBPFeatures[i].x             = lbpr.x;
    apexLBPFeatures[i].y             = lbpr.y;
    apexLBPFeatures[i].width         = lbpr.width;
    apexLBPFeatures[i].height        = lbpr.height;
  }

  for(unsigned int i = 0; i < lbpStages.size(); ++i)
  {
    apexLBPStages[i].threshold    = lbpStages[i].thresholdFixed;
    apexLBPStages[i].featureCount = lbpStages[i].featureCount;
  }
}