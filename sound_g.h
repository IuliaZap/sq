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
    explicit sound_g(QObject* parent = 0, int sampleRate = 8000, int channelCount = 1, int sampleSize = 16, const QString& codec = "audio/pcm"); //explicit - предотвращает неявное преобразование при инициализации
    virtual ~sound_g();// virtual - виртуальная функция  -  обычно такое определение нужно для базового(родительского) класса, чтобы при вызове дочернего деструктора не вызвался деструктор родителя

    void appendSound(qreal, quint32, quint32);// добавление символов информационной последовательности(кусочка синусоиды "0" или "1")
    //void appendPause(quint32);

    /**
      * \brief Clears the sound buffer.
      */
    void clear();//отчитка буфера

    void play();
    void stop();

    static void playSound(qreal, quint32, quint32); // static - это переменная или функция принадлежащая к классу а не к объекту класса

private:
    static sound_g* generatorHelper; //***Ё указатель на объект класса sound_g, служит вспомогательным объектом класса, чтобы выделить под него динамически память и скрыть его реализацию для остальных классов проекта

    QByteArray*   soundBuffer;//буфер для одного символа
    QBuffer*      outputBuffer; // буфер для строки символов
    QAudioFormat* format; // настроки требуемые для работы QAudioOutput
    QAudioOutput* output; // указатель на объект QAudioOutput


};



