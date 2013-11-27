#include "imageedit.h"
#include "ui_imageedit.h"
#include "filters.h"
ImageEdit::ImageEdit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageEdit)
{

    ui->setupUi(this);
    QPixmap i;
    i.load("../ImgEdit/lena.jpg");
    //i.load("../ImgEdit/q.jpg");
    ui->label->setPixmap(i);
    ui->label->setScaledContents(true);
    ui->label_2->setPixmap(i);
    ui->label_2->setScaledContents(true);
    ui->Brightnesslvl->setVisible(false);
    ui->Contrastlvl->setVisible(false);
    ui->StopButton->setEnabled(false);
}

void ImageEdit:: SetEnabledBrightness (bool x)
{
    ui->BrightnessButton->setEnabled(!x);
    ui->Brightnesslvl->setVisible(x);
}

void ImageEdit:: SetEnabledContrast (bool x)
{
    ui->ContrastButton->setEnabled(!x);
    ui->Contrastlvl->setVisible(x);
}

void ImageEdit::setEnabledButtons(bool x)
{
    ui->StopButton->setEnabled(!x);
    ui->BlackWhiteButton->setEnabled(x);
    ui->BlurButton->setEnabled(x);
    ui->BrightnessButton->setEnabled(x);
    ui->ContrastButton->setEnabled(x);
    ui->NegativeButton->setEnabled(x);
    ui->SharpenButton->setEnabled(x);
    ui->Apply->setEnabled(x);
    ui->GWButton->setEnabled(x);
    ui->StampingButton->setEnabled(x);
    ui->WaterColorButton->setEnabled(x);
}

ImageEdit::~ImageEdit()
{
    delete ui;
}

void ImageEdit::on_Apply_clicked()
{
    ui->label->setPixmap(*(ui->label_2->pixmap()));
}

void ImageEdit::on_BlurButton_clicked()
{
    SetEnabledContrast(false);
    SetEnabledBrightness(false);
    setEnabledButtons(false);
    filter=new BlurThread();
    filter->StartProcessing(ui->label->pixmap()->toImage());
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        //qApp->processEvents();
    }
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}

void ImageEdit::on_SharpenButton_clicked()
{
    SetEnabledContrast(false);
    SetEnabledBrightness(false);
    setEnabledButtons(false);
    filter=new SharpenThread();
    filter->StartProcessing(ui->label->pixmap()->toImage());
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}

void ImageEdit::on_BlackWhiteButton_clicked()
{
    SetEnabledContrast(false);
    SetEnabledBrightness(false);
    setEnabledButtons(false);
    filter=new BlackWhiteThread();
    filter->StartProcessing(ui->label->pixmap()->toImage());
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    ui->progressBar->setValue(filter->getProgress());
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}



void ImageEdit::on_BrightnessButton_clicked()
{
    SetEnabledContrast(false);
    ui->Brightnesslvl->setValue(0);
    SetEnabledBrightness(true);
}





void ImageEdit::on_Brightnesslvl_valueChanged(int arg1)
{
    setEnabledButtons(false);
    filter=new BrightnessThread();
    filter->StartProcessing(ui->label->pixmap()->toImage(), arg1);
    ui->Brightnesslvl->setEnabled(false);
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    ui->progressBar->setValue(filter->getProgress());
    ui->Brightnesslvl->setEnabled(true);
    ui->Brightnesslvl->setFocus();
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}

void ImageEdit::on_ContrastButton_clicked()
{
    SetEnabledBrightness(false);
    ui->Contrastlvl->setValue(1);
    SetEnabledContrast(true);
}

void ImageEdit::on_Contrastlvl_valueChanged(int arg1)
{

    if (arg1 == 1)
    {
        ui->label_2->setPixmap(*(ui->label->pixmap()));
        return;
    }
    setEnabledButtons(false);
    filter = new ContrastThread();
    filter->StartProcessing(ui->label->pixmap()->toImage(), arg1);
    ui->Contrastlvl->setEnabled(false);
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    ui->Contrastlvl->setEnabled(true);
    ui->Contrastlvl->setFocus();
    ui->progressBar->setValue(filter->getProgress());
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}

void ImageEdit::on_NegativeButton_clicked()
{
    SetEnabledContrast(false);
    SetEnabledBrightness(false);
    setEnabledButtons(false);
    filter = new ContrastThread();
    filter->StartProcessing(ui->label->pixmap()->toImage(), -1);
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    ui->progressBar->setValue(filter->getProgress());
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}

void ImageEdit::on_StopButton_clicked()
{
    filter->Stop();
}

void ImageEdit::on_GWButton_clicked()
{
    SetEnabledContrast(false);
    SetEnabledBrightness(false);
    setEnabledButtons(false);
    filter = new GrayWorldThread();
    filter->StartProcessing(ui->label->pixmap()->toImage());
    while(filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    ui->progressBar->setValue(filter->getProgress());
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}

void ImageEdit::on_StampingButton_clicked()
{
    SetEnabledContrast(false);
    SetEnabledBrightness(false);
    setEnabledButtons(false);
    filter = new StampingThread();
    filter->StartProcessing(ui->label->pixmap()->toImage());
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    ui->progressBar->setValue(filter->getProgress());
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}

void ImageEdit::on_WaterColorButton_clicked()
{
    SetEnabledContrast(false);
    SetEnabledBrightness(false);
    setEnabledButtons(false);
    filter = new WaterColorThread();
    filter->StartProcessing(ui->label->pixmap()->toImage());
    while (filter->isRunning())
    {
        ui->progressBar->setValue(filter->getProgress());
        qApp->processEvents();
    }
    ui->progressBar->setValue(filter->getProgress());
    if (!filter->isStoped())
        ui->label_2->setPixmap(QPixmap::fromImage(filter->getResult()));
    delete filter;
    filter = NULL;
    setEnabledButtons(true);
}
