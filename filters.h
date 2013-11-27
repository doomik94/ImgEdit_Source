#ifndef FILTERS_H
#define FILTERS_H
//#include <QMainWindow>
#include <QThread>
#include "imageedit.h"
#include "ui_imageedit.h"

class rgb
{
    int R, G, B;
public:

    rgb (int r, int g, int b):R(r), G(g), B(b){}
    rgb (QRgb x):R(qRed(x)), G(qGreen(x)), B(qBlue(x)){}
    rgb(){}
    rgb (const rgb& A)
    {
        R=A.getR();
        G=A.getG();
        B=A.getB();
    }

    int getR()const{return R;}
    int getG()const{return G;}
    int getB()const{return B;}
    rgb &operator+(rgb& x)const
    {
        rgb y(R+x.getR(), G+x.getG(), B+x.getB());
        return y;
    }
    rgb &operator-(rgb& x)const
    {
        rgb y(R-x.getR(), G-x.getG(), B-x.getB());
        return y;
    }
    rgb &operator+(const QRgb& x)const
    {
        rgb y(R+qRed(x), G+qGreen(x), B+qBlue(x));
        return y;
    }
    void Add(const unsigned int& xr, const unsigned int& xg, const unsigned int& xb)
    {
        R+=xr; G+=xg; B+=xb;
    }
    rgb &operator-(const QRgb& x)const
    {
        rgb y(R-qRed(x), G-qGreen(x), B-qBlue(x));
        return y;
    }
    rgb &operator+=(rgb& x)
    {
        return *this=*this+x;
    }
    rgb &operator-=(rgb& x)
    {
        return *this=*this-x;
    }
    rgb &operator+=(const QRgb& x)
    {
        return *this=*this+x;
    }
    rgb &operator-=(const QRgb& x)
    {
        return *this=*this-x;
    }
    rgb &operator=(const rgb& x)
    {
        if (this == &x) return *this;
        R=x.getR();
        G=x.getG();
        B=x.getB();
        return *this;
    }
    rgb &operator/(const unsigned int &x)const
    {
        rgb y;
        y = rgb(R/x, G/x, B/x);
        return y;
    }
    rgb &operator*(const unsigned int &x)const
    {
        rgb y;
        y = rgb(R*x, G*x, B*x);
        return y;
    }
    void alignment()
    {
        if (R>255) R=255;
        else if (R<0) R=0;
        if (G>255) G=255;
        else if (G<0) G=0;
        if (B>255) B=255;
        else if (B<0) B=0;
    }
    void SetToNull()
    {
        R=0; G=0; B=0;
    }
};

#include "imageedit.h"


class filterThread:public QThread
{
protected:
    int R;
    bool StopFlag;
    float Progress;
    QImage m_Image, m_Result;
public:
    void run()=0;
    void StartProcessing(QImage img, int r=3)
    {

        R=r;
        m_Image=m_Result=img;
        start();
    }
    void Stop()
    {
        StopFlag=true;
    }

    QImage getImage(){ return m_Image;}
    QImage getResult(){ return m_Result;}
    int getProgress(){ return (int)(Progress)%101;}

    bool isStoped(){return StopFlag;}
private slots:
    void on_spinBox_editingFinished();
};



class BlurThread: public filterThread
{

 public:

     void run()
     {
         Progress=0;
         if (R==0){return;}
         rgb RGB(0,0,0);
         StopFlag = false;
         int cnt=0;
         for(int i=0; i < m_Image.height(); ++i)
         {
             for (int j=0; j < m_Image.width(); ++j)
             {
                 for (int n=-R; n<=R; ++n)
                 {
                     if (j+n >=0 && j+n < m_Image.width())
                     {
                         RGB += m_Image.pixel(i, j+n);
                         ++cnt;
                     }
                     if (i+n >=0 && i+n < m_Image.height() && n!=0)
                     {
                         RGB += m_Image.pixel(i+n, j);
                         ++cnt;
                     }
                 }
                 m_Result.setPixel(i, j, qRgb(RGB.getR()/cnt, RGB.getG()/cnt, RGB.getB()/cnt));
                 RGB.SetToNull();
                 cnt=0;
             }
             Progress += (100.0f / ((float)m_Image.height()));
             if (StopFlag){ break; return;}
         }

//         for(int i=0; i < m_Image.width(); ++i)
//         {
//             for (int j=0; j < m_Image.height(); ++j)
//             {
//                 for (int n=-R; n<=R; ++n)
//                 {
//                     if (j+n >=0 && j+n < m_Image.height())
//                     {
//                         RGB += m_Image.pixel(j+n, i);
//                         ++cnt;
//                     }
//                 }
//                 m_Result.setPixel(j, i, qRgb(RGB.getR()/cnt, RGB.getG()/cnt, RGB.getB()/cnt));
//                 cnt=0;
//                 RGB.SetToNull();
//             }
//             Progress += (100.0f / (2*(float)m_Image.width()));
//             if (StopFlag){ break;}
//         }
//         Progress=0.0;
     }

 };

 class SharpenThread:public filterThread
 {
 public:
     void run()
     {
         Progress=0;
         /*if (R == 0)
         {
            Progress = 100;
            return;
         }*/
         R=1;
         StopFlag = false;
         //const int N = 4*R+1;
         rgb RGB(0, 0, 0);
         int cnt=0;
         for(int i=0; i < m_Image.height(); ++i)
         {
            for (int j=0; j < m_Image.width(); ++j)
            {
                 for (int n=-R; n<=R; ++n)
                 {
                    if (n==0)
                        continue;
                    if (j+n >= 0 && j+n < m_Image.width())
                    {
                        RGB-=m_Image.pixel(i, j+n);
                        ++cnt;
                    }
                        if (i+n >= 0 && i+n < m_Image.height())
                        {
                            RGB-=m_Image.pixel(i+n, j);
                            ++cnt;
                        }
                 }
                   rgb tmp ((cnt+1)*qRed(m_Image.pixel(i, j)),
                   (cnt+1)*qGreen(m_Image.pixel(i, j)),
                   (cnt+1)*qBlue(m_Image.pixel(i, j)));
                   RGB+=tmp;
                   RGB.alignment();
                   m_Result.setPixel(i, j, qRgb(RGB.getR(), RGB.getG(), RGB.getB()));
                   RGB.SetToNull();
                   cnt=0;
            }
            Progress+=(100.0f / ((float)m_Image.height()));
            if (StopFlag) {break; return;}
        }
 /*        for(int i=0; i < m_Image.width(); ++i)
         {
               for (int j=0; j < m_Image.height(); ++j)
               {
                    for (int n=-R; n<=R; ++n)
                    {
                       if (n==0)
                           continue;
                       if (j+n >= 0 && j+n < m_Image.width())
                       {
                           RGB-=tempImage.pixel(j+n, i);
                           ++cnt;
                       }
   //                        if (i+n >= 0 && i+n < m_Image.height())
   //                        {
   //                            RGB-=m_Image.pixel(i+n, j);
   //                            ++cnt;
   //                        }
                    }
                    rgb tmp ((cnt+1)*qRed(tempImage.pixel(j, i)),
                    (cnt+1)*qGreen(tempImage.pixel(j, i)),
                    (cnt+1)*qBlue(tempImage.pixel(j, i)));
                    RGB+=tmp;
                    RGB.alignment();
                    m_Result.setPixel(j, i, qRgb(RGB.getR(), RGB.getG(), RGB.getB()));
                    RGB.SetToNull();
                    cnt=0;
               }
               Progress+=(100.0f / (2*((float)m_Image.height())));
         }*/
     }

 };

class BlackWhiteThread: public filterThread
{
public:
    void run()
    {
        Progress = 0;
        StopFlag = false;
        int mean;
        for (int i=0; i<m_Image.height(); ++i)
        {
            for (int j=0; j<m_Image.width(); ++j)
            {
                mean = (qRed(m_Image.pixel(i, j))+qGreen(m_Image.pixel(i, j))+qBlue(m_Image.pixel(i, j)))/3;
                m_Result.setPixel(i, j, qRgb(mean, mean, mean));
            }
            Progress+=(100.0f / ((float)m_Image.height()));
            if (StopFlag) {break; return;}
        }
    }
};

class BrightnessThread: public filterThread
{
    void run ()
    {
        Progress = 0;
        StopFlag = false;
        rgb temp;
        if (R==0){Progress = 100; return;}
        for (int i=0; i<m_Image.height(); ++i)
        {
            for (int j=0; j<m_Image.width(); ++j)
            {
                temp=rgb(qRed(m_Image.pixel(i, j))+R, qGreen(m_Image.pixel(i, j))+R, qBlue(m_Image.pixel(i, j))+R);
                temp.alignment();
                m_Result.setPixel(i, j, qRgb(temp.getR(), temp.getG(), temp.getB()));
            }
            Progress+=(100.0f / ((float)m_Image.height()));
        }
    }
};

class ContrastThread: public filterThread
{
    void run ()
    {
        if (R == 0){Progress = 100; return;}
        int average_brightness = 0;
        Progress = 0;
        StopFlag = false;
        for (int i=0; i<m_Image.height(); ++i)
        {
            for (int j=0; j<m_Image.width(); ++j)
            {
                average_brightness+=(qRed(m_Image.pixel(i, j)) + qGreen(m_Image.pixel(i, j)) + qBlue(m_Image.pixel(i, j)))/3;
            }
            Progress+=(100.0f / (2*(float)m_Image.height()));
            if (StopFlag) {break; return;}

        }
        average_brightness/=m_Image.width()*m_Image.height();
        int B[256];
        for (int i=0, tmp, delta; i<256; ++i)
        {
            delta = i-average_brightness; //вычисление разности компоненты RGB от средней €ркости
            tmp = average_brightness + R*delta; //новое начение дл€ компонент RGB
            if (tmp < 0) tmp = 0; else
                if (tmp > 255) tmp = 255;
            B[i]=tmp;
        }
        for (int i=0; i<m_Image.height(); ++i)
        {
            for (int j=0; j<m_Image.width(); ++j)
            {
                m_Result.setPixel(i, j, qRgb(B[qRed(m_Image.pixel(i, j))], B[qGreen(m_Image.pixel(i, j))], B[qBlue(m_Image.pixel(i, j))]));
            }
            Progress+=(100.0f / (2*(float)m_Image.height()));
            if (StopFlag) {break; return;}
        }
    }
};

class GrayWorldThread: public filterThread
{
public:
    void run()
    {
        rgb averageRGB(0, 0, 0);
        unsigned int average_brightness = 0;
        Progress = 0;
        StopFlag = false;
        for (int i=0; i<m_Image.height(); ++i)
        {
            for (int j=0; j<m_Image.width(); ++j)
            {
                rgb tmp(qRed(m_Image.pixel(i, j)), qGreen(m_Image.pixel(i, j)), qBlue(m_Image.pixel(i, j)));
                averageRGB+=tmp;
            }
        }
        averageRGB=averageRGB/(m_Image.height()*m_Image.width());
        average_brightness = (averageRGB.getR()+averageRGB.getG()+averageRGB.getB())/3;
        //averageRGB = rgb(average_brightness/averageRGB.getR(), average_brightness/averageRGB.getG(), average_brightness/averageRGB.getB());
        for (int i=0; i<m_Image.height(); ++i)
        {
            for (int j=0; j<m_Image.width(); ++j)
            {
                rgb tmp((qRed (m_Image.pixel(i, j)))*average_brightness/averageRGB.getR(),
                        (qGreen (m_Image.pixel(i, j)))*average_brightness/averageRGB.getG(),
                        (qBlue (m_Image.pixel(i, j)))*average_brightness/averageRGB.getB());
                tmp.alignment();
                m_Result.setPixel(i, j, qRgb(tmp.getR(), tmp.getG(), tmp.getB()));
            }
            Progress+=(100.0f / ((float)m_Image.height()));
            if (StopFlag){break; return;}
        }
    }
};

class StampingThread: public filterThread
{
    void run()
    {
        Progress = 0;
        StopFlag = false;
        rgb NewCurrPix(0, 0, 0);
        for(int i=0; i < m_Image.height(); ++i)
        {
            for (int j=0; j < m_Image.width(); ++j)
            {
                rgb tmp;
                if (i+1 < m_Image.height())
                {
                    tmp = rgb(m_Image.pixel(i+1, j));
                    NewCurrPix+=tmp;
                }else
                {
                    tmp = rgb(m_Image.pixel(i-1, j));
                    NewCurrPix+=tmp;
                }
                if (j+1 < m_Image.width())
                {
                    tmp = rgb(m_Image.pixel(i, j+1));
                    NewCurrPix+=tmp;
                }else
                {
                    tmp = rgb(m_Image.pixel(i, j-1));
                    NewCurrPix+=tmp;
                }
                if (i-1>=0)
                {
                    tmp = rgb(m_Image.pixel(i-1, j));
                    NewCurrPix-=tmp;
                }else
                {
                    tmp = rgb(m_Image.pixel(i+1, j));
                    NewCurrPix-=tmp;
                }
                if (j-1>=0)
                {
                    tmp = rgb(m_Image.pixel(i, j-1));
                    NewCurrPix-=tmp;
                }else
                {
                    tmp = rgb(m_Image.pixel(i, j+1));
                    NewCurrPix-=tmp;
                }
                NewCurrPix.Add(128, 128, 128);
                NewCurrPix.alignment();
                m_Result.setPixel(i, j, qRgb(NewCurrPix.getR(), NewCurrPix.getG(), NewCurrPix.getB()));
                NewCurrPix.SetToNull();
            }
            Progress+=(100.0f / ((float)m_Image.height()));
            if (StopFlag){break; return;}
        }
    }
};

class WaterColorThread: public filterThread
{
    void run()
    {
        Progress = 0;
        StopFlag = false;
        rgb NewCurrPix(0, 0, 0);
        for(int i=0; i < m_Image.height(); ++i)
        {
            for (int j=0; j < m_Image.width(); ++j)
            {
                NewCurrPix = rgb(m_Image.pixel(i, j));
                NewCurrPix = NewCurrPix*9;
                rgb tmp;
                for (int m=-1; m<2; ++m)
                    for (int n=-1; n<2; ++n)
                    {
                        if (m==0 && n==0) continue;
                        int m1=m, n1=n;
                        if (i+m < 0 || i+m >= m_Image.height())
                            m1=-m;
                        if (j+n < 0 || j+n >= m_Image.width())
                            n1=-n;
                        tmp = rgb(m_Image.pixel(i+m1, j+n1));

                        NewCurrPix-=tmp;
                    }
                NewCurrPix.alignment();
                m_Result.setPixel(i, j, qRgb(NewCurrPix.getR(), NewCurrPix.getG(), NewCurrPix.getB()));

            }
            Progress+=(100.0f / ((float)m_Image.height()));
            if (StopFlag) {break; return;}
        }

    }
};

filterThread *filter;
#endif // FILTERS_H
