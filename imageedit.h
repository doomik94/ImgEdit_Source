#ifndef IMAGEEDIT_H
#define IMAGEEDIT_H

#include <QMainWindow>

namespace Ui {
class ImageEdit;
}

class ImageEdit : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ImageEdit(QWidget *parent = 0);
    ~ImageEdit();
    
private slots:

    void on_Apply_clicked();

    void on_BlurButton_clicked();

    void on_SharpenButton_clicked();

    void on_BlackWhiteButton_clicked();

    void on_BrightnessButton_clicked();

    void on_Brightnesslvl_valueChanged(int arg1);

    void on_ContrastButton_clicked();

    void on_Contrastlvl_valueChanged(int arg1);

    void on_NegativeButton_clicked();

    void on_StopButton_clicked();

    void on_GWButton_clicked();

    void on_StampingButton_clicked();

    void on_WaterColorButton_clicked();

private:
    Ui::ImageEdit *ui;
    void SetEnabledBrightness (bool);
    void SetEnabledContrast (bool);
    void setEnabledButtons(bool);
};

#endif // IMAGEEDIT_H
