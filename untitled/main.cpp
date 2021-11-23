#include "main.h"
#include <QCoreApplication>

int main()
{
    // очистка файла
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.close();

    //открытие файла для записи
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    Audio_Init(22050,1,8,"audio/pcm",QALittleEnd,QAUnInt);
    //предварительное создание сигналов
    signal_I_f1 = Create_Signal("sin", 1000.0, 1.0);
    signal_Q_f1 = Create_Signal("cos", 1000.0, 1.0);
    signal_I_f2 = Create_Signal("sin", 2100.0, 1.0);
    signal_Q_f2 = Create_Signal("cos", 2100.0, 1.0);
    sig_f1_size = signal_I_f1.size();
    sig_f2_size = signal_I_f2.size();
    //запуск работы устройтсва
    Audio_Start_Listen(1.0,100);
    //прекращение работы устройтсва
    Audio_Close();
    file.close();//закрытие файла txt для прекращения записи данных
    return 0;
}

void Audio_Init(int sample_rate,
                int channel_count,
                int sample_size,
                QString codec,
                QAudioFormat::Endian byte_order,
                QAudioFormat::SampleType sample_type) {
    /*
    SupportedByteOrder:  (LittleEndian)
    SupportedChannelCounts: (1, 2)
    SupportedSampleTypes: (SignedInt, UnSignedInt)
    SupportedSampleSizes: (8, 16)
    SupportedSampleRates: (11025, 22050, 44100, 48000, 96000)
    */
    format.setSampleRate(sample_rate);//частота дискретизации
    format.setChannelCount(channel_count);//количество каналов записи
    format.setSampleSize(sample_size);//разрядность АЦП
    format.setCodec(codec);//объявление имени использующего кодека
    format.setByteOrder(byte_order);//порядок чтения битов буфера(посмотреть в справке аудио формат)
    format.setSampleType(sample_type);//объявление типа представления считываемых данных
    //проверка соответствия параметров устройства заданным параметрам
    info = QAudioDeviceInfo::defaultInputDevice();//создание объета info класса QAudioDeviceInfo
    if (!info.isFormatSupported(format)) { //если параметры объекта формат не соответствуют фактическим параметрам устройства приема,
        qWarning() << "Format not supported, trying to use the nearest...";
        format = info.nearestFormat(format); //то система выберет ближайшие значенния параметров
    }
    //объявляется объект audio класса  QAudioInput обладающий параметрами объектов info и format
    audio = new QAudioInput(info,format);
}

void Audio_Start_Listen(qreal volume, int cycles_cnt) {
    //объявление значения громкости(уровень чувствительности)
    audio->QAudioInput::setVolume(volume);
    //активация записи
    Audio_Get_Info();
    cout << "Lessss go!" << endl;
    input = audio->start();
    int period_size = audio->QAudioInput::periodSize();
    qDebug() << "A State:" << audio->QAudioInput::state();
    qDebug() << "A PeriodSize:" << period_size;
    for (int cycle_num = 0; cycle_num < cycles_cnt; cycle_num++) {
        while (audio->bytesReady() != period_size) {}
         QByteArray buffer = input->read(period_size);//считываем данные записывая в буфер все доступные байты
         for(int i = 0; i < period_size; i++) {
           unsigned int input_value = static_cast<unsigned int>(buffer[i]) & 0xFF;
           //text_out << value << "\n";

           qreal output_value = qreal(input_value) * (signal_I_f1[sig_f1_cnt]
                                                      + signal_Q_f1[sig_f1_cnt]
                                                      + signal_I_f2[sig_f2_cnt]
                                                      + signal_Q_f2[sig_f2_cnt]);
           text_out << output_value << "\n";


           if (sig_f1_cnt < (sig_f1_size - 1)) {
               sig_f1_cnt++;
           }
           else {
              sig_f1_cnt = 0;
           }
           if (sig_f2_cnt < (sig_f2_size - 1)) {
               sig_f2_cnt++;
           }
           else {
              sig_f2_cnt = 0;
           }
         }
    }
}

void Audio_Close() {
    audio->stop();//закрытие файла для чтения
    while (audio->QAudioInput::state() != QAudio::StoppedState) {}
    delete audio;
}

void Audio_Get_Info() {  //выводит информацю об устройстве по факту его работы
    qDebug() << "F ByteOrder: " << format.byteOrder();
    qDebug() << "F ChannelCount:" << format.channelCount();
    qDebug() << "F SampleType:" << format.sampleType();
    qDebug() << "F SampleSize:" << format.sampleSize();
    qDebug() << "F SampleRate:" << format.sampleRate();
    qDebug() << "A BufferSize: " << audio->QAudioInput::bufferSize();
    qDebug() << "A BytesReady:" << audio->QAudioInput::bytesReady();
    qDebug() << "A PeriodSize:" << audio->QAudioInput::periodSize();
    qDebug() << "A State:" << audio->QAudioInput::state();
    qDebug() << "A Error:" << audio->QAudioInput::error();
}

QVector<qreal> Create_Signal(QString func, qreal freq, qreal amplitude) {
    QVector<qreal> new_signal;
    int signal_period = format.sampleRate() / int(freq); //кол-во отсчетов, необходимое для одного периода сигнала
    if (func == "sin") {
        for (int t = 0; t < signal_period; t++) {
            new_signal.append(amplitude * qSin(freq * 2.0 * M_PI * qreal(t) / qreal(format.sampleRate()))); // заполнение массива
        }
    }
    else {
        for (int t = 0; t < signal_period; t++) {
            new_signal.append(amplitude * qCos(freq * 2.0 * M_PI * qreal(t) / qreal(format.sampleRate())));
        }
    }
    return new_signal;
}
