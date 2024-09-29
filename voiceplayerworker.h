#ifndef VOICEPLAYERWORKER_H
#define VOICEPLAYERWORKER_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class VoicePlayerWorker : public QObject
{
    Q_OBJECT
public:
    explicit VoicePlayerWorker(QObject *parent = 0);
    ~VoicePlayerWorker();
    QString FileName;
    QProcess *myProccess;
    bool ThreadStarted;

    bool VoicePlaying;

    void SetFileName(QString fileName);

signals:
    void Sig_Play();
    void Sig_StartThread();
    void Sig_StopPlaying();

public slots:
    void Slot_play();
    void Slot_StartThread();
    void Slot_StopPlaying();

    void Slot_StartVoice();
    void Slot_FinishVoice(int exitCode, QProcess::ExitStatus exitStatus);
    void Slot_StateChangeVoice(QProcess::ProcessState newState);
};

#endif // VOICEPLAYERWORKER_H
