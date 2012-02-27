#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include "avtsourcesink.h"
#include <QDebug>

CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(0),currSource(0), recording(FALSE),timerInterval(100),reversePlay(FALSE),needTimer(TRUE),running(FALSE)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
}

void CamBackend::run()
{
    if (currSource->IsOpened()) {
        if (needTimer) {
            timer.setInterval(timerInterval);
            timer.start();
            exec(); //will go beyond this point when quit() is send from within this thread
            timer.stop();
        } else {  // the AVT backend will block itself when waiting for the next frame. No need for an extra timer
            running=TRUE;
            while (running) {
                GrabFrame();
            }
        }
    } else {
        qDebug()<<"Camera is not opened";
    }
}

void CamBackend::GrabFrame()
{
    int incr=1;
    if (reversePlay) incr=-1;

    if (!currSource->GrabFrame(currImage,incr)) {
        qDebug()<<"Some error occured while grabbing the frame";
        return;
    }
    if (currImage.image.rows==0) {
//        StopAcquisition();
        return;
    }
    if (recording && currSink) currSink->RecordFrame(currImage);
    emit NewImageReady(currImage);
}

bool CamBackend::StartAcquisition(QString dev)
{
    if (dev.contains(".fmf")) {
        currSource=new FmfSourceSink;
        needTimer=TRUE;
    } else if (dev=="AVT") {
        currSource=new AvtSourceSink;
        needTimer=FALSE;
    } else {
        currSource=new OpencvSourceSink;
        needTimer=TRUE;
    }
    currSource->Init();
    currSource->StartAcquisition(dev);
    return TRUE;
}

void CamBackend::StopAcquisition()
{
    running=FALSE;
    currSource->StopAcquisition();
    if (needTimer) quit();
}

void CamBackend::ReleaseCamera()
{
    currSource->ReleaseCamera();
    delete currSource;
    currSource=0;
}

void CamBackend::SetInterval(int newInt)
{
    reversePlay=newInt<0;
    if (needTimer) {
        timer.setInterval(abs(newInt));
    } else {  // the source handles the interval by itself
        currSource->SetInterval(abs(newInt));
    }
}

void CamBackend::StartRecording(bool startRec,QString recFold, QString codec)
{
    if (startRec) {
        if (codec=="FMF") {
            currSink=new FmfSourceSink;
        } else {
            currSink=new OpencvSourceSink;
        }
        int fps=timer.interval()/10;
        currSink->StartRecording(recFold,codec,fps,currImage.image.cols,currImage.image.rows);
    } else { // stopping recording
        currSink->StopRecording();
        delete currSink;
        currSink=0;
    }
    recording=startRec;
}


