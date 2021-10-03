#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sound_g.h"
#include <vector>
#include<bitset>
#include <iostream>

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



void MainWindow::on_pushButton_clicked(){

    const uint f1 = 700;
    const uint f2 = 1100;
    const uint duration =20;
    const qreal amplitude   = 0.1;

        std::string str_massage = ui->plainTextEdit->toPlainText().toStdString();
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

                for(int j =0; j<8; ++j){

                    generator->appendSound(amplitude, (data_bit[i][j] ? f1 : f2), duration);
                    std::cout<<data_bit[i][j];

                }
                std::cout<<'\n';
            }
            generator->play();

        }

    //__
    //generator->appendSound(amplitude, f1, duration); //генерация синусоды при нажатии кнопки короткой равной длительности буфера soundBuffer
    //generator->play();


