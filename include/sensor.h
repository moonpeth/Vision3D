#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "OpenNI.h"

//enum of twos video streams
enum EActiveStream {
    color_stream = 0,
    depth_stream = 1
};

class CSensor {
public:
    //declare of device
    openni :: Device m_device;
    //the two types of video stream
    openni :: VideoStream m_colorStream;
    openni :: VideoStream m_depthStream;
    //the video stream actuel
    EActiveStream active_stream;

public:
    //constructor and deconstructor
    CSensor();
    ~CSensor();

    bool OnInit(bool show_color_stream);
};

#endif
