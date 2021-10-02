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

void MainWindow::on_pushButton_clicked(){
    const uint f1 = 400;
    const uint f2 = 1000;
    const uint duration = 200;
    const qreal amplitude   = 0.1;
    //__
    std::string show_me_what_you_ve_got = ui->plainTextEdit->toPlainText().toStdString();
    int size = show_me_what_you_ve_got.size();
    std::vector<char> data;
        std::vector<std::bitset<8>> data_bit;
        data.resize(size);
        data_bit.resize(size*8);
        for(int i=0;i<size; ++i){
             data[i]= show_me_what_you_ve_got.c_str()[i];
        }
        for(int i=0;i<size; ++i){
            data_bit[i] = std::bitset<8>(data[i]);

            for(int i =0; i<size*8; ++i){
                if(data_bit[i]==1)
                    generator->appendSound(amplitude, f1, duration);

                else if(data_bit[i]==0)
                    generator->appendSound(amplitude, f2, duration);
            }
        }
                generator->play();
    }

    //__
    //generator->appendSound(amplitude, f1, duration);
    //generator->play();


