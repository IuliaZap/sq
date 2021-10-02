#include "sound_g.h"

//стандартные настройки для работы API qt для звуковой карты
sound_g::sound_g(QObject* parent,
                 int sampleRate,
                 int channelCount,
                 int sampleSize,
                 const QString& codec) : QObject(parent){

    soundBuffer = new QByteArray();

    format = new QAudioFormat();
    format->setSampleRate(sampleRate);
    format->setChannelCount(channelCount);
    format->setSampleSize(sampleSize);
    format->setCodec(codec);
    format->setByteOrder(QAudioFormat::LittleEndian);
    format->setSampleType(QAudioFormat::UnSignedInt);

    output = new QAudioOutput(*format, this);
    connect(output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(playbackFinished()));

    outputBuffer = new QBuffer(soundBuffer);
    if (outputBuffer->open(QIODevice::ReadOnly) == false) {
        qCritical() << "Invalid operation while opening QBuffer. audio/pcm";
        return;
    }
};
//деструктор - освобождение памяти
sound_g::~sound_g(){
    delete soundBuffer;
    delete format;
    delete output;
    delete outputBuffer;
};

/**
  * \brief Adds a specific tone to the playlist.
  *
  * \param amplitude The amplitude of this tone from 0.0 to 1.0
  * \param frequency The tone's frequency in Hz
  * \param msecs     The tone's duration. Minimum is 50 msecs, every value below that will be set to 50 msecs.
  */

//добавление в soundBuffer одного отрезка передаваемой информации
void sound_g::appendSound(qreal amplitude, quint32 frequency, quint32 msecs){
    msecs = (msecs < 50) ? 50 : msecs;

    qreal singleWaveTime = 1.0 / frequency;
    qreal samplesPerWave = qCeil(format->sampleRate() * singleWaveTime);
    quint32 waveCount = qCeil(msecs / (singleWaveTime * 1000.0));
    quint32 sampleSize = static_cast<quint32>(format->sampleSize() / 8.0);

    QByteArray data(waveCount * samplesPerWave * sampleSize * format->channelCount(), '\0');
    unsigned char* dataPointer = reinterpret_cast<unsigned char*>(data.data());

    for (quint32 currentWave = 0; currentWave < waveCount; currentWave++) {
        for (int currentSample = 0; currentSample < samplesPerWave; currentSample++) {
            double nextRadStep = (currentSample / static_cast<double>(samplesPerWave)) * (2 * M_PI);
            quint16 sampleValue = static_cast<quint16>(amplitude*(qSin(nextRadStep) + 1.0) * 16383.0);

            for (int channel = 0; channel < format->channelCount(); channel++) {
                qToLittleEndian(sampleValue, dataPointer);
                dataPointer += sampleSize;
            }
        }
    }

    soundBuffer->append(data);
}


/*void sound_g::appendPause(quint32 msecs){
    QByteArray data(format->sampleRate() * (msecs / 1000.0) * format->channelCount(), '\0');
    unsigned char* dataPointer = reinterpret_cast<unsigned char*>(data.data());
    for (int i = 0; i < data.size(); i += 2) {
        qToLittleEndian(16383, dataPointer);
        dataPointer += 2;
    }

    soundBuffer->append(data);
}*/

void sound_g::clear(){
    soundBuffer->clear();
}


void sound_g::play(){
    output->start(outputBuffer);
}
void sound_g::stop(){
    output->stop();
}

sound_g * sound_g::generatorHelper = new  sound_g();

void sound_g::playSound(qreal amplitude, quint32 frequency, quint32 msecs){
    generatorHelper->clear();
    generatorHelper->appendSound(amplitude, frequency, msecs);
    generatorHelper->play();
}
