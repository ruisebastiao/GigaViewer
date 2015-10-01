#ifndef IDSSOURCESINK_H
#define IDSSOURCESINK_H

#include <uEye.h>

#include <QThread>
#include "imagepacket.h"
#include "imagesourcesink.h"
#include <opencv2/opencv.hpp>

class IdsSourceSink : public ImageSourceSink
{
public:
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool IsOpened();
    int SetInterval(int msec);
    bool SetShutter(int shutTime);
    int SetAutoShutter(bool fitRange);

private:
    HIDS hCam;
    char* imgMem;
    int memId;
    int flagIDS;
    UEYEIMAGEINFO ImageInfo;
    HANDLE hEvent;

    int Index;
    unsigned long Last;
    int rows,cols;
    double camTimeStep;
    double camTimeOffset;
    cv::Mat buffer;
//    QVector<cv::Mat> matFrames;

};

#endif // IDSSOURCESINK_H