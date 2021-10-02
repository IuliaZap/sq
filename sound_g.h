#pragma once

#include <QObject>
#include<QByteArray>
#include <QBuffer>
#include<QAudioFormat>
#include <QAudioOutput>
#include <QtEndian>
#include <QDebug>
#include <qmath.h>



class sound_g:public QObject
{
    Q_OBJECT
public:
    explicit sound_g(QObject* parent = 0, int sampleRate = 8000, int channelCount = 1, int sampleSize = 16, const QString& codec = "audio/pcm");
    virtual ~sound_g();

    void appendSound(qreal, quint32, quint32);
    //void appendPause(quint32);

    /**
      * \brief Clears the sound buffer.
      */
    void clear();

    void play();
    void stop();

    static void playSound(qreal, quint32, quint32);

private:
    static sound_g* generatorHelper;

    QByteArray*   soundBuffer;
    QBuffer*      outputBuffer;
    QAudioFormat* format;
    QAudioOutput* output;


};



