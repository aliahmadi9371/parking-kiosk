#include "voiceplayerworker.h"

VoicePlayerWorker::VoicePlayerWorker(QObject *parent) : QObject(parent)
{
    ThreadStarted = false;

    connect(this,SIGNAL(Sig_StartThread()),this, SLOT(Slot_StartThread()), Qt::QueuedConnection);

}

VoicePlayerWorker::~VoicePlayerWorker()
{
    Slot_StopPlaying();
    delete myProccess;
    delete this;
}

void VoicePlayerWorker::Slot_play()
{
    myProccess->start( "/usr/bin/mpv", QStringList(FileName) );
}

void VoicePlayerWorker::Slot_StartThread()
{
    connect(this,SIGNAL(Sig_Play()),this, SLOT(Slot_play()), Qt::QueuedConnection);
    myProccess = new QProcess(this);

    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    qRegisterMetaType<QProcess::ProcessState>("QProcess::ProcessState");
    connect(myProccess,SIGNAL(finished(int, QProcess::ExitStatus )),
            this, SLOT(Slot_FinishVoice(int, QProcess::ExitStatus )),
            Qt::QueuedConnection);

    connect(myProccess,SIGNAL(started()),
            this, SLOT(Slot_StartVoice()),
            Qt::QueuedConnection);

    connect(myProccess,SIGNAL(stateChanged(QProcess::ProcessState)),
            this, SLOT(Slot_StateChangeVoice(QProcess::ProcessState)),
            Qt::QueuedConnection);

    connect(this,SIGNAL(Sig_StopPlaying()),
            this, SLOT(Slot_StopPlaying()),
            Qt::QueuedConnection);


    VoicePlaying = false;
    ThreadStarted = true;
}

void VoicePlayerWorker::SetFileName(QString fileName)
{
    FileName = fileName;
}

void VoicePlayerWorker::Slot_StartVoice()
{
    qDebug() << "start voice playing" << FileName;
    VoicePlaying = true;
}

void VoicePlayerWorker::Slot_FinishVoice(int exitCode, QProcess::ExitStatus exitStatus)
{
    VoicePlaying = false;
    qDebug() << "finish voice playing" << FileName;
}

void VoicePlayerWorker::Slot_StateChangeVoice(QProcess::ProcessState newState)
{
    //qDebug() << "newState = " << newState;
}

void VoicePlayerWorker::Slot_StopPlaying()
{
    if(VoicePlaying){
        myProccess->kill();
        VoicePlaying = false;
    }
    qDebug() << "Slot_StopPlaying()" << FileName;
}


