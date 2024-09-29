#ifndef MAINWIDGET_H
#define MAINWIDGET_H


#define OOUT1 31
#define OOUT2 33

#define IIN1 35
#define IIN2 37


#define CODE128_B_START 104
#define CODE128_STOP 106


#include <QWidget>
#include <QTimer>
#include <QSettings>
#include <QDebug>
#include <QKeyEvent>
//#include <QSerialPort>
//#include <QSerialPortInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QLabel>
#include <QFontDatabase>
#include <QPainter>
#include <QThread>
#include <QNetworkInterface>
#include <QLabel>
#include <QTextCodec>
#include <QPainter>

#include <QBoxLayout>
#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QStyle>

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "CSJPOSLib.h"


#include <QThread>
#include "voiceplayerworker.h"

using namespace std;



namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    QString check_url;
     int mywidth;

     QThread *ThreadButtonPalyer;
     VoicePlayerWorker* PlayerButton;

     QThread *ThreadCardPalyerTrue;
     VoicePlayerWorker* PlayerCardTrue;

     QThread *ThreadCardPalyerFalse;
     VoicePlayerWorker* PlayerCardFalse;


private:
    Ui::MainWidget *ui;

    bool GetPrinterStaus();
    bool GetNetStatus();


    bool feed;
    int time;


    int iNst;

    int iCurrentPlateId;

    //ini params:
    bool bLoopEnabled;
    QString sBaseURL1;
    QString sBaseURL2;
    QString sDevice_Id;
    QString sLoopDisMessage;
    QString sMessage;
    QString sPaperMessage;
    QString sPNMessage;
    QString sNetMessage;
    QString sWaitingMessage;
    QString sReceiptMessage;
    int tLcdTime;
    QString sKeyDevice;
    QString sKeyPlate;
    QString sKeyPelak;
    int iReplyTimeout1;
    int iReplyTimeout2;


    CSJPOSLib::ESCPOSPrinter *printerp;
    QString sPrinterName;


    QLabel lbgLable;
    QPixmap pBK1;
    QPixmap pBK2;
    QPixmap pBK3;
    QPixmap pBK4;


    QPixmap pPelak;
    QLabel lPelak;
    QLabel lp1;
    QLabel lp2;
    QLabel lp3;
    QLabel lp4;

    QLabel lblText1;

    int id;
    int id1;
    int id2;
    int id3;


    QString ssP1;
    QString ssP2;
    QString ssP3;
    QString ssP4;
    QString scMessage;
    bool bPelakk;

    void PrepareUI();




    void SendWebService1();
    QNetworkAccessManager *myNetManager1;
    QNetworkReply *myReply1;
    QTimer tReply1;
    bool bNetAborted1;

    void SendWebService2(int iPlate);
    QNetworkAccessManager *myNetManager2;
    QNetworkReply *myReply2;
    QTimer tReply2;
    bool bNetAborted2;


    void SendWebService3(QString sCSN);
    QNetworkAccessManager *myNetManager3;
    QNetworkReply *myReply3;
    QTimer tReply3;
    bool bNetAborted3;
    QString sKeyTag;
    QString sBaseURL3;

    QString sCurrentCard;
    int isAuth;
    int iCurrentId;
    QString sCurrentName;
    QString sCardMessage;
    QString sCardMessageNot;


    void DrawDisplaye(int iiState, bool paper, bool net);

    bool bPaperResp;
    bool bPaperOK;
    bool bNetOk;
    int iState;


    bool inProc;


    QTimer timerInput;



    QString scanner;
    //QSerialPort serialPort;


    //bool bPortOpened;

    QTimer tProc;
    int iProcTime;
    bool bTimeout;

    QTimer tOpen;

    QTimer tStartPrint;
    bool bStartPrint;

    void InitProgram();
    void SetIniParams();
    void setWiringOPI();

    bool bCardBot;




//    void OpenSerial();
//    void CloseSerial();
//    void StartPrinter();
//    void reinitPrinter();
//    void InitPrinter(int dot, int heattime, int heatinterval);
//    void Font_w2_h2_b();
//    void SetPrinterDensity();
//    void alingment(int align);
//    void linespace(int space);
//    void lineFeeds(int line);
//    void ASB_AutoStatus(bool enable);
//    void getPaperStatus();
//    void PartialCut();
//    void PanelButtonsEnDis(bool bEnabled);

    void goToStateInit();

    QString getcurrenyIP();

    //void PrintBarcode(QString barcode);
    bool printBitmap();
    void CreateReceipt(QString datetime, QString barcode,QString info,QString footer, QImage Logo);
    QPixmap CreateFooterList(QString footer, int paperWidth, QFont font);


    QString encodeBarcode(QString code);
    int calculateCheckCharacter(QString code);
    int codeToChar(int code);
    int charToCode(int ch);


    QNetworkAccessManager nam;
    bool bNetResp;
    QTimer tNetStatusCheck;
    QTimer tNetCheckTimeout;
    QNetworkReply *reply;
    int iNetCheckTimeout;
    int iNetCheckTrigerTime;


protected:
    void keyPressEvent(QKeyEvent *event);
    void getCard(QString sCardNumber);

signals:
    void InputDetected1();
    void InputDetected2();
private slots:
    void recieveInput1();
    void recieveInput2();


    void connectInputs();
    void ConnectInp1();
    void ConnectInp2();


    void ReplyTimeout1();
    void ReplyTimeout2();
    void ReplyTimeout3();

    void ProcTimeout();

    //void ReadSerial();

    void ReplyFinished1(QNetworkReply* Reply);
    void ReplyFinished2(QNetworkReply* Reply);
    void ReplyFinished3(QNetworkReply* Reply);

    void OpenTimeout();

    void onStartPrinttimeout();

    void CallSystemStatus();
    void ReplyFinishedCheck(QNetworkReply* Reply);
    void NetChecktimeout();

};

#endif // MAINWIDGET_H
