#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sound_g.h"
#include <vector>
#include<bitset>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

struct MainWindow::bitss{
    bool b0 : 1;
    bool b1 : 1;
    bool b2 : 1;
    bool b3 : 1;
    bool b4 : 1;
    bool b5 : 1;
    bool b6 : 1;
    bool b7 : 1;
}ex;


void MainWindow::on_pushButton_clicked(){
    const bitss bin_digits2 ={0, 1 , 0 , 0 , 0 , 0 , 0 , 0} ;
    const uint f1 = 400;
    const uint f2 = 1000;
    const uint duration = 200;
    const qreal amplitude   = 0.1;
    //binary
    const char bin_digits[4] ="01";
    std::vector<bitss> *binary;
    //int i = sizeof (char)*8;
    std::string str_massage = ui->plainTextEdit->toPlainText().toStdString();
    int sizem = str_massage.size();

    binary->resize(sizem);

    for(int a=0; a<sizem;++a){
        for(int di=7; di>=0; --di){
            char tmp = str_massage[a];
            char d = (tmp&1);
            tmp >>= 1;

            binary->at(ex.b0) =  (bin_digits2>>d);
        }
    }

    for(int a=0; a<sizem;++a){
        for(int di=7; di>=0; --di){
            if(binary->at(a)[di]==1)
                generator->appendSound(amplitude, f1, duration);
            else if(binary->at(a)[di]==0)
                generator->appendSound(amplitude, f2, duration);
        }
    }
    generator->play();



}
    //__

        /*std::string str_massage = ui->plainTextEdit->toPlainText().toStdString();
        int size = str_massage.size();
        std::vector<char> data;
            std::vector<std::bitset<8>> data_bit;
            data.resize(size);
            data_bit.resize(size*8);
            for(int i=0;i<size; ++i){
                 data[i]= str_massage.c_str()[i];
            }
            for(int i=0;i<size; ++i){
                data_bit[i] = std::bitset<8>(data[i]);

                for(int i =0; i<size*8; ++i){
                    if(data_bit[i]==1)
                        generator->appendSound(amplitude, f1, duration);//

                    else if(data_bit[i]==0)
                        generator->appendSound(amplitude, f2, duration);
                }
            }

        }*/

    //__
    //generator->appendSound(amplitude, f1, duration); //генерация синусоды при нажатии кнопки короткой равной длительности буфера soundBuffer
    //generator->play();


