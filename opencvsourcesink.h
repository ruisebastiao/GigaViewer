#ifndef OPENCVSOURCESINK_H
#define OPENCVSOURCESINK_H

#include "imagesourcesink.h"

class OpencvSourceSink: public ImageSourceSink
{
public:
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool RecordFrame(ImagePacket& source);
    bool StartRecording(QString recFold, QString codec, int fps, int cols, int rows);
    bool StopRecording();
    bool IsOpened();
    bool SkipFrames(bool forward);

private:
    cv::VideoCapture camera;
    cv::VideoWriter recFile;
    int nFrames;
    bool liveFeed;
};

#endif // OPENCVSOURCESINK_H
