#include "mainwidget.h"
#include "ui_mainwidget.h"

static MainWidget* main = NULL;
static bool readi1 = false;
static bool readi2 = false;

//State: -2 paper or net not ok
//State: -1 init
//State: 0 start
//State: 1 received loop input and send webservice 1
//State: 2 received reply 1
//state: 3 send webservice 2
//state: 4 received reply 2
//state: 5 print receipt
//state: 6 open gate
//state: 7 start card
//state: 8 finish card


void MainWidget::goToStateInit()
{
    PlayerButton->Sig_StopPlaying();
    PlayerCardTrue->Sig_StopPlaying();
    PlayerCardFalse->Sig_StopPlaying();
    bPaperResp = false;
    iState = -1;
    qDebug() << "State = " << iState;
    DrawDisplaye(iState,bPaperOK,bNetOk);
    connectInputs();
}

void MainWidget::connectInputs()
{
    ConnectInp1();
    ConnectInp2();
}

void MainWidget::ConnectInp1()
{
    readi1 = false;
}

void MainWidget::ConnectInp2()
{
    readi2 = false;
}

static void readiput1(){
//    qDebug() << "read input 1";
    if(main && !readi1){
        readi1 = true;
        emit main->InputDetected1();
    }
}

void MainWidget::recieveInput1()
{
//    timerInput.start(1500);

    if(!bLoopEnabled && iState == -1){
        iState = 0;

        qDebug() << "Read Input 1";
        bCardBot = false;
        qDebug() << bLoopEnabled;
        qDebug() << iState;

        qDebug() << "Start bot";

        bPaperOK = GetPrinterStaus();
        if(!bPaperOK){
            iState = -2;
            DrawDisplaye(iState,bPaperOK,bNetOk);
            connectInputs();
            return;
        }
        qDebug() << "State = " << iState;

        emit PlayerButton->Sig_Play();
        SendWebService1();
        return;
    }
    else if(bLoopEnabled && iState==2){
        qDebug() << "Read Input 1";
        bCardBot = false;
        qDebug() << "bot input for print";
        SendWebService2(iCurrentPlateId);
        return;
    }
    return;
}

static void readiput2(){
//    qDebug() << "read input 2";
    if(main && !readi2){
        readi2 = true;
        emit main->InputDetected2();
    }
}

void MainWidget::recieveInput2()
{
//    timerInput.start(1500);
    if(bLoopEnabled && iState == -1){
        iState = 0;
        qDebug() << "Read Input 2";
        qDebug() << iState;
        qDebug() << bLoopEnabled;

        qDebug() << "Start";
        bPaperOK = GetPrinterStaus();
        if(!bPaperOK){
            qDebug() << "printer not ok";
            iState = -2;
            DrawDisplaye(iState,bPaperOK,bNetOk);
            connectInputs();
            return;
        }
        //bPaperOK = false;

        qDebug() << "loop input";

        qDebug() << "State = " << iState;

        tProc.stop();
        bTimeout = false;
        tProc.start(iProcTime);
        inProc = true;

        SendWebService1();
        return;
    }
    return;
}

void MainWidget::SendWebService1()
{
    iState = 1;
    qDebug() << "State = " << iState;
    QString Url = sBaseURL1 + sKeyDevice + "=" + sDevice_Id;
    qDebug() << Url;
    QUrl serviceUrl = QUrl(Url);
    QNetworkRequest request(serviceUrl);
    bNetAborted1 = false;
    tReply1.start(iReplyTimeout1);
    myReply1 = myNetManager1->get(request);
}

void MainWidget::ReplyTimeout1()
{
    qDebug() << "reply timeout 1";
    bNetAborted1 = true;
    myReply1->abort();
}

void MainWidget::ReplyFinished1(QNetworkReply* Reply)
{
    tReply1.stop();
    iState = 2;
    qDebug() << "reply 1";
    if(bNetAborted1 && bLoopEnabled){
    qDebug() << "aborted reply 1";
        if(bTimeout){
            qDebug() << "proc time out";

            goToStateInit();
            return;
        }
        if(digitalRead(IIN2) == 0){
            SendWebService1();
            return;
            //lcd to state1
        }
        qDebug() << "no loop state";
        goToStateInit();
        return;
    }

    if(bNetAborted1 && !bLoopEnabled){
        qDebug() << "aborted reply 1";
        goToStateInit();
        //lcd to state-1
        return;
    }

    QByteArray tmp = Reply->readAll();
    QJsonDocument json_doc;
    QJsonParseError json_err;
    json_doc = QJsonDocument::fromJson(tmp,&json_err);
    QJsonObject json_object = json_doc.object();
    if(json_doc.isNull()){
        qDebug() << "reply 1 is null";
        if(bLoopEnabled){ 
            if(bTimeout){
                qDebug() << "proc time out";
                goToStateInit();
                return;
            }
            if(digitalRead(IIN2) == 0){
                qDebug() << "try ... service 1";
                SendWebService1();
                //lcd to state1
                return;
            }
            qDebug() << "no loop state";
            goToStateInit();
            return;
        }
        else{
            qDebug() << "no loop state";
            goToStateInit();
            return;
        }
    }
    qDebug() << "reply 1 is correct";

    QString ssMessage = json_object.value("Message").toString();
    qDebug() << "Message = "  << ssMessage;
    scMessage = ssMessage;

    QString sP1 = json_object.value("P1").toString();
    qDebug() << "P1 = " << sP1;

    QString sP2 = json_object.value("P2").toString();
    qDebug() << "P2 = " << sP2;

    QString sP3 = json_object.value("P3").toString();
    qDebug() << "P3 = " << sP3;

    QString sP4 = json_object.value("P4").toString();
    qDebug() << "P4 = " << sP4;

    int iPlateId = json_object.value("PlateId").toInt();
    qDebug() << "PlateId = " << iPlateId;
    iCurrentPlateId = iPlateId;

    scMessage == ssMessage;

    if(sP1.count()==0 || sP2.count()==0 || sP3.count()==0 || sP4.count()==0){
        //lcd state 2 without pelak
        bPelakk = false;
        DrawDisplaye(iState,bPaperOK,bNetOk);
    }
    else{

        bPelakk = true;
        ssP1 = sP1;
        ssP2 = sP2;
        ssP3 = sP3;
        ssP4 = sP4;
        DrawDisplaye(iState,true,true);
        //lcd state 2 with pelak
    }
    if(!bLoopEnabled){
        SendWebService2(iCurrentPlateId);
        return;
    }
}

void MainWidget::SendWebService2(int iPlate)
{
    iState = 3;
    qDebug() << "State = " << iState;
    QString Url = sBaseURL2 + sKeyDevice + "=" + sDevice_Id + "&" + sKeyPlate + "=" + QString::number(iPlate);
    qDebug() << Url;
    QUrl serviceUrl = QUrl(Url);
    QNetworkRequest request(serviceUrl);
    bNetAborted2 = false;
    tReply2.start(iReplyTimeout2);
    myReply2 = myNetManager2->get(request);
}

void MainWidget::ReplyTimeout2()
{
    qDebug() << "reply timeout 2";
    bNetAborted2 = true;
    myReply2->abort();
}

void MainWidget::ReplyFinished2(QNetworkReply* Reply)
{

    tReply2.stop();
    iState = 4;
    qDebug() << "reply 2";

    if(bNetAborted2 && bLoopEnabled){
    qDebug() << "aborted reply 2";
    bNetAborted1 = false;
        if(bTimeout){
            qDebug() << "proc time out";
            return;
        }
        if(digitalRead(IIN2) == 0){
            SendWebService1();
            return;
            //lcd to state1
        }
        qDebug() << "no loop state";
        goToStateInit();
        return;
    }

    if(bNetAborted2 && !bLoopEnabled){
        qDebug() << "aborted reply";
        goToStateInit();
        //lcd to state-1
        return;
    }
    qDebug() << "Reply 2 finished";

    QByteArray tmp = Reply->readAll();
    QJsonDocument json_doc;
    QJsonParseError json_err;
    json_doc = QJsonDocument::fromJson(tmp,&json_err);
    QJsonObject json_object = json_doc.object();
    if(json_doc.isNull()){
        if(bLoopEnabled){
            qDebug() << "reply 2 is null";
            if(bTimeout){
                qDebug() << "proc time out";
                return;
            }
            if(digitalRead(IIN2) == 0){
                SendWebService1();
                qDebug() << "retry webservice 1";
                return;
            }
            qDebug() << "no loop state";
            goToStateInit();
            return;
        }
        if(!bLoopEnabled){
            qDebug() << "no loop state";
            goToStateInit();
            return;
        }
        return;
    }

    qDebug() << "reply 2 is correct";



    tProc.stop();
    bTimeout = false;
    inProc = false;


    QString sId = QString::number(json_object.value("ID").toInt());
    qDebug() << "Id = "  << sId;

    QString sBarcode = json_object.value("BarcodeString").toString();
    qDebug() << "BarcodeString = " << sBarcode;

    QString sCarType = json_object.value("CarType").toString();
    qDebug() << "CarType = " << sCarType;

    QString sGateName = json_object.value("GateName").toString();
    qDebug() << "GateName = " << sGateName;

    QString sEntryDate = json_object.value("EntryDate").toString();
    qDebug() << "EntryDate = " << sEntryDate;

    QString sEntryTime = json_object.value("EntryTime").toString();
    qDebug() << "EntryTime = " << sEntryTime;

    QString sFooter = json_object.value("Footer").toString();
    qDebug() << "Footer = " << sFooter;

    QString sLogo = json_object.value("Logo").toString();
    QByteArray array64 = sLogo.toUtf8();
    QImage image;
    image.loadFromData(QByteArray::fromBase64(array64));
    qDebug() << "size of received image = " << image.size();


    QString datetime = sEntryDate + "   " +  sEntryTime;
    QString info =  sGateName + " - " + sCarType;

    CreateReceipt(datetime,sBarcode,info,sFooter, image);
    printBitmap();
    DrawDisplaye(iState,bPaperOK,bNetOk);


//    QCoreApplication::processEvents(QEventLoop::AllEvents);
    tOpen.start(tLcdTime);
    digitalWrite(OOUT1,1);
    digitalWrite(OOUT2,1);




}

void MainWidget::getCard(QString sCardNumber)
{
    if((!bLoopEnabled && iState == -1) || (bLoopEnabled && iState==2) || (bLoopEnabled && iState ==-1)){
        bCardBot = true;
        qDebug() << "Start Card";
        bPaperOK = GetPrinterStaus();
        if(!bPaperOK){
            iState = -2;
            DrawDisplaye(iState,bPaperOK,bNetOk);
            connectInputs();
        }
        else{
            qDebug() << "State = " << iState;
//            tProc.stop();
//            bTimeout = false;
//            tProc.start(iProcTime);
//            inProc = true;
            SendWebService3(sCardNumber);
        }
    }
    return;
}

void MainWidget::SendWebService3(QString sCSN)
{
    iState = 7;
    qDebug() << "State = " << iState;
    QString Url = sBaseURL3 + sKeyDevice + "=" + sDevice_Id + "&" + sKeyTag + "=" + sCSN;
    qDebug() << Url;
    QUrl serviceUrl = QUrl(Url);
    QNetworkRequest request(serviceUrl);
    bNetAborted3 = false;
    tReply3.start(iReplyTimeout1);
    myReply3 = myNetManager3->get(request);
}

void MainWidget::ReplyTimeout3()
{
    qDebug() << "reply timeout 3";
    bNetAborted3 = true;
    myReply3->abort();
}

void MainWidget::ReplyFinished3(QNetworkReply* Reply)
{
    tReply3.stop();
    iState = 8;
    qDebug() << "reply 3";

    if(bNetAborted3){
        qDebug() << "aborted reply 3";
        if(bTimeout){
            qDebug() << "proc time out";
            return;
        }
        goToStateInit();
        return;
    }

    qDebug() << "Reply 3 finished";

    QByteArray tmp = Reply->readAll();
    QJsonDocument json_doc;
    QJsonParseError json_err;
    json_doc = QJsonDocument::fromJson(tmp,&json_err);
    QJsonObject json_object = json_doc.object();
    if(json_doc.isNull()){
            goToStateInit();
            return;
    }
    qDebug() << "reply 3 is correct";

    tProc.stop();
    bTimeout = false;

    iCurrentId = json_object.value("CustomerID").toInt();
    qDebug() << "Id = "  << iCurrentId;

    sCurrentName = json_object.value("Name").toString();
    qDebug() << "Name = " << sCurrentName;

    isAuth = json_object.value("IsAuthorize").toInt();
    qDebug() << "isAuth = " << isAuth;

    bStartPrint = false;

    if(isAuth == 1){
        emit PlayerCardTrue->Sig_Play();
        tOpen.start(tLcdTime);
        digitalWrite(OOUT1,1);
        digitalWrite(OOUT2,1);
    }
    else{
        emit PlayerCardFalse->Sig_Play();
    }
    iState = 9;
    qDebug() << "State = " << iState;
    tOpen.start(tLcdTime);
    DrawDisplaye(iState,bPaperOK,bNetOk);
}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    qDebug() << "Start main Class";

    InitProgram();
    main = this;

}

bool MainWidget::printBitmap()
{
    bool receipt = false;
    if(GetPrinterStaus()){
        if(printerp->Connect(CMP_PORT_USB, sPrinterName.toStdString()) == CMP_STS_NORMAL){
            if(printerp->PrintBitmap( "img.png",CMP_BM_ASIS, CMP_ALIGNMENT_CENTER) == CMP_STS_NORMAL){
                receipt = true;
                printerp->CutPaper(CMP_CUT_PARTIAL_PREFEED);
            }
            printerp->Disconnect();
        }
    }
    return receipt;
}


void MainWidget::SetIniParams()
{

    qDebug() << "Get ini Parameters";
    QSettings setting("Saman_config.ini",QSettings::IniFormat);
    setting.setIniCodec("UTF-8");

    bLoopEnabled = setting.value("app_config/loop_enable",false).toBool();
    sBaseURL1 = setting.value("app_config/webservice1","").toString();
    sBaseURL2 = setting.value("app_config/webservice2","").toString();
    sBaseURL3 = setting.value("app_config/webservice3","").toString();
    sDevice_Id = setting.value("app_config/device_id","").toString();

    sLoopDisMessage = setting.value("app_config/disabled_loop_meesage","").toString();
    sLoopDisMessage = sLoopDisMessage.insert(9,'\n');
    sLoopDisMessage = sLoopDisMessage.insert(9,'\n');

    sMessage = setting.value("app_config/message","").toString();

    sPaperMessage = setting.value("app_config/paperNKmessage","").toString();
    sPaperMessage = sPaperMessage.insert(sPaperMessage.count()/2,'\n');
    sPaperMessage = sPaperMessage.insert(sPaperMessage.count()/2,'\n');
    sPNMessage = setting.value("app_config/pnNKmessage","").toString();
    sPNMessage = sPNMessage.insert(sPNMessage.count()/2,'\n');
    sPNMessage = sPNMessage.insert(sPNMessage.count()/2,'\n');
    sNetMessage = setting.value("app_config/netNKmessage","").toString();

    sWaitingMessage = setting.value("app_config/waitingMessage","").toString();
    sReceiptMessage = setting.value("app_config/receiptMessage","").toString();
    sCardMessage = setting.value("app_config/cardMessage","").toString();
    sCardMessageNot = setting.value("app_config/cardMessageNot","").toString();

    check_url = setting.value("app_config/baseurl","").toString();

    sPrinterName = setting.value("app_config/printer_name","").toString();

    feed = setting.value("keys/feed",true).toBool();
    time = setting.value("keys/time",5000).toInt();

    tLcdTime = setting.value("app_config/tLCD",4000).toInt();
    sKeyDevice = setting.value("others/key_device","").toString();
    sKeyPlate = setting.value("others/key_plate","").toString();
    sKeyPelak = setting.value("others/key_pelak","").toString();
    iReplyTimeout1 = setting.value("others/tReply1",4000).toInt();
    iReplyTimeout2 = setting.value("others/tReply2",4000).toInt();
    iProcTime = setting.value("others/tProc",8000).toInt();
    sKeyTag = setting.value("others/key_tag","").toString();

    iNetCheckTimeout = setting.value("app_config/net_check_timeout",2000).toInt();
    iNetCheckTrigerTime = setting.value("app_config/net_check_triger",5000).toInt();

}

void MainWidget::InitProgram()
{
    qDebug() << "Start initialize Application";
    bLoopEnabled = false;
    bPaperOK = false;
    bNetOk = false;
    bTimeout = false;
    bNetResp = false;

    inProc = false;

    sBaseURL1 = "";
    sBaseURL2 = "";
    sBaseURL3 = "";
    sDevice_Id = "";
    sLoopDisMessage = "";
    sPaperMessage = "";
    sWaitingMessage = "";
    sReceiptMessage = "";
    sCardMessage = "";
    check_url = "";
    tLcdTime = 0;

    bNetAborted1 = false;
    bNetAborted2 = false;

    bPaperResp = false;
    bPaperOK = false;

    scanner = "";

    //bPortOpened = false;

    timerInput.setSingleShot(true);
    connect(&timerInput,SIGNAL(timeout()),this,SLOT(connectInputs()),Qt::QueuedConnection);

    tReply1.setSingleShot(true);
    connect(&tReply1,SIGNAL(timeout()),this,SLOT(ReplyTimeout1()),Qt::QueuedConnection);

    tReply2.setSingleShot(true);
    connect(&tReply2,SIGNAL(timeout()),this,SLOT(ReplyTimeout2()),Qt::QueuedConnection);

    tReply3.setSingleShot(true);
    connect(&tReply3,SIGNAL(timeout()),this,SLOT(ReplyTimeout3()),Qt::QueuedConnection);

    tProc.setSingleShot(true);
    connect(&tProc,SIGNAL(timeout()),this,SLOT(ProcTimeout()),Qt::QueuedConnection);

    tOpen.setSingleShot(true);
    connect(&tOpen,SIGNAL(timeout()),this,SLOT(OpenTimeout()),Qt::QueuedConnection);

    tStartPrint.setSingleShot(true);
    connect(&tStartPrint,SIGNAL(timeout()),this,SLOT(onStartPrinttimeout()),Qt::QueuedConnection);


    myNetManager1 = new QNetworkAccessManager(this);
    connect(myNetManager1,SIGNAL(finished(QNetworkReply*)),this,SLOT(ReplyFinished1(QNetworkReply*)),Qt::QueuedConnection);

    myNetManager2 = new QNetworkAccessManager(this);
    connect(myNetManager2,SIGNAL(finished(QNetworkReply*)),this,SLOT(ReplyFinished2(QNetworkReply*)),Qt::QueuedConnection);

    myNetManager3 = new QNetworkAccessManager(this);
    connect(myNetManager3,SIGNAL(finished(QNetworkReply*)),this,SLOT(ReplyFinished3(QNetworkReply*)),Qt::QueuedConnection);



    ThreadButtonPalyer = new QThread;
    PlayerButton = new VoicePlayerWorker();
    PlayerButton->moveToThread(ThreadButtonPalyer);
    ThreadButtonPalyer->start();
    emit PlayerButton->Sig_StartThread();
    PlayerButton->SetFileName("/var/miladtower/voices/ReceiptReceiving.wav");



    ThreadCardPalyerTrue = new QThread;
    PlayerCardTrue = new VoicePlayerWorker();
    PlayerCardTrue->moveToThread(ThreadCardPalyerTrue);
    ThreadCardPalyerTrue->start();
    emit PlayerCardTrue->Sig_StartThread();
    PlayerCardTrue->SetFileName("/var/miladtower/voices/CardLocatingTrue.wav");

    ThreadCardPalyerFalse = new QThread;
    PlayerCardFalse = new VoicePlayerWorker();
    PlayerCardFalse->moveToThread(ThreadCardPalyerFalse);
    ThreadCardPalyerFalse->start();
    emit PlayerCardFalse->Sig_StartThread();
    PlayerCardFalse->SetFileName("/var/miladtower/voices/CardLocatingFalse.wav");


    SetIniParams();
    setWiringOPI();

    printerp = new CSJPOSLib::ESCPOSPrinter();

    PrepareUI();

    tNetCheckTimeout.setSingleShot(true);
    connect(&tNetCheckTimeout, SIGNAL(timeout()), this, SLOT(NetChecktimeout()),Qt::QueuedConnection);

    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(ReplyFinishedCheck(QNetworkReply*)),Qt::QueuedConnection);

    tNetStatusCheck.setSingleShot(false);
    connect(&tNetStatusCheck,SIGNAL(timeout()),this,SLOT(CallSystemStatus()),Qt::QueuedConnection);
    tNetStatusCheck.start(iNetCheckTrigerTime);

    iState = -1;
    CallSystemStatus();
}

void MainWidget::CallSystemStatus()
{
    if(iState == -1 || iState == -2){
        qDebug() << "net and printer status";
        bNetOk = GetNetStatus();
        bPaperOK = GetPrinterStaus();
        if(bPaperOK && bNetOk){
            iState = -1;
            DrawDisplaye(-1,bPaperOK,bNetOk);
        }
        else{
            iState = -2;
            DrawDisplaye(-2,bPaperOK,bNetOk);
            connectInputs();
        }
        bNetResp = false;
    }
    return;
}

bool MainWidget::GetPrinterStaus()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    bool s = false;
    if(printerp->Connect(CMP_PORT_USB, sPrinterName.toStdString()) == CMP_STS_NORMAL){
        qDebug() << "connect to printer";
        if(printerp->PrinterCheck() == CMP_STS_NORMAL){
            if(printerp->Status() == CMP_STS_NORMAL){
             s = true;
            }
            else{
                qDebug() << "printer error";
            }
        }
        else{
            qDebug() << "printer error";
        }
        printerp->Disconnect();
        qDebug() << "disconnect from printer";
    }
    return s;
}

bool MainWidget::GetNetStatus()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    bool s = false;
    QUrl u(check_url);
    QNetworkRequest req(u);
    bNetResp = false;
    tNetCheckTimeout.start(iNetCheckTimeout);
    reply = nam.get(req);
    while(!bNetResp){
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
    qDebug() << "resp net " << bNetOk;
    s = bNetOk;
    return s;
}

void MainWidget::ReplyFinishedCheck(QNetworkReply* Reply)
{
    tNetCheckTimeout.stop();
    reply = Reply;
    if (reply->bytesAvailable()){
        qDebug() <<  "You are connected to the network :)";
        bNetOk = true;
    }
    else{
        qDebug() <<  "You are not connected to the network :(";
        bNetOk = false;
    }
    bNetResp = true;
    return;
}

void MainWidget::NetChecktimeout()
{
    reply->abort();
    return;
}


void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift){
        event->ignore();
        return;
    }
    if(event->text() == "\r" || event->text() == "\r\n" || event->text() == "\n"){
        qDebug() << "full Scanner length" << scanner.length();
        if(scanner.length() == 10)
        scanner = scanner.remove(scanner.length()-2,2);
        qDebug() << "main Scanner: "  << scanner;
        getCard(scanner);
        scanner = "";
        return;

    }
    else{
        scanner+=event->text();
        return;
    }
}



void MainWidget::setWiringOPI()
{
    qDebug() << "set Wiring input and output pins";
    wiringPiSetupPhys();
    pinMode(OOUT1,OUTPUT);
    pinMode(OOUT2,OUTPUT);
    pinMode(IIN1,INPUT);
    pinMode(IIN2,INPUT);
    pullUpDnControl(IIN1, PUD_DOWN);
    pullUpDnControl(IIN2, PUD_UP);

    wiringPiISR(IIN1, INT_EDGE_FALLING, &readiput1);
    wiringPiISR(IIN2, INT_EDGE_FALLING, &readiput2);
    connect(this,SIGNAL(InputDetected1()), this, SLOT(recieveInput1()),Qt::QueuedConnection);
    connect(this,SIGNAL(InputDetected2()), this, SLOT(recieveInput2()),Qt::QueuedConnection);
}


QPixmap MainWidget::CreateFooterList(QString footer, int paperWidth, QFont font)
{
    QString str = footer;

    QFontMetrics fontMetrics(font);
    //qDebug() << fontMetrics.width(str);
    //qDebug() << fontMetrics.height();
    QRect messageContentRect = fontMetrics.boundingRect(QRect(0,0,paperWidth,0),Qt::AlignCenter,str);

    //qDebug() << messageContentRect.width();
    //qDebug() << messageContentRect.height();


    //split space and enter
    QStringList list1 = str.split('\n');
    QStringList list2;
    QStringList list3;
    //qDebug() << "list1.length = " << list1.length();
    //for(int i = 0; i<list1.length(); i++)
        //qDebug() << "list1["  << i << "]" << list1[i];
    int m = 0 ;
    int paperLength = paperWidth;
    for (int i = 0 ; i < list1.length(); i++){
        QStringList tmp = list1[i].split(' ');

        for(int j=0; j < tmp.length() ;j++)
            list2.append(tmp[j]);
    }
    //for(int i = 0 ; i < list2.length(); i++)
        //qDebug() << "list2["  << i << "]" << list2[i];

    list3.clear();
    int wordCnt = 0;
    while (wordCnt < list2.length())
    {
        QString tmpLine = "";
        QString line = "";
        QString final="";
        m = 0 ;
        while (m < paperLength && wordCnt < list2.length())
        {

            if (list2[wordCnt] == "" ){
                if (m>0)
                     list3.append(tmpLine);
                final = "";

                wordCnt++;
                break;
            }
            else {
                //qDebug() << "list2[" << wordCnt << "]" << list2[wordCnt];
                line = tmpLine;
                //qDebug() << "line" << line ;
                tmpLine = tmpLine + " " + list2[wordCnt];
                //qDebug() << "tmpLine" << tmpLine ;
                m = fontMetrics.width(tmpLine);
                //qDebug() << paperLength << m;
                if(m > paperLength){
                    final=line;
                    wordCnt--;
                    //qDebug() << "m > ";
                }
                else if(m == paperLength){
                    final=tmpLine;
                    wordCnt--;
                    //qDebug() << "m < ";
                }
            }
            wordCnt++;
        }
        //qDebug() << "final" << final << "wordCnt" << wordCnt << "tmpLine" << tmpLine  ;
        if ( wordCnt == list2.length())
            final = tmpLine;
        list3.append(final);

    }

    QPixmap pix(paperLength,messageContentRect.height()*list3.length());
    QPainter painter(&pix);
    pix.fill(Qt::white);
    painter.setPen(QPen(Qt::black));
    painter.setFont(font);

    for (int i = 0 ; i < list3.length() ; i++){
        painter.drawText(QRect(0,fontMetrics.height() * i ,paperLength,fontMetrics.height() * (i+1)), Qt::AlignRight | Qt::AlignJustify ,list3[i]);
        //qDebug() << "list3 : " << i << ":" <<list3[i] ;
    }

    qDebug() << "pix.size() =" << pix.size();
    painter.end();
    return pix;
}


void MainWidget::CreateReceipt(QString datetime, QString barcode, QString info, QString footer, QImage Logo)
{
    QString ffooter;
    ffooter.clear();
    ffooter = footer;

    //qDebug() << ffooter.count();
//    if(ffooter.count() > 1100){
//       ffooter = ffooter.remove(1100,ffooter.count() - 1100);
//    }
    //qDebug() << ffooter.count();


    //create barcode
    QPixmap pix1(384*1.25,32*1.25);
    pix1.fill(Qt::white);
    QPainter p1(&pix1);
    p1.setPen(Qt::SolidLine);
    p1.setPen(QColor(Qt::black));
    QString BarcodeText = barcode;
    QString Barcode;
    Barcode = encodeBarcode(BarcodeText);
    //qDebug() << "font id = "  << id1;
    QString Code128 = QFontDatabase::applicationFontFamilies(id1).at(0);
    QFont FontCode128(Code128,60*1.25);
    FontCode128.setLetterSpacing(QFont::AbsoluteSpacing,0);
    p1.setFont(FontCode128);
    p1.drawText(QRect(0,0,384*1.25,32*1.25),Qt::AlignCenter,Barcode);
    p1.end();
    pix1 = pix1.scaled(384*1.25,64*1.25,Qt::IgnoreAspectRatio,Qt::FastTransformation);

//    ui->label->setGeometry(400 - pix2.width()/2,pix2.height() + 100 ,pix1.width(),pix1.height());
//    ui->label->clear();
//    ui->label->setPixmap(pix1);


    QString BMitra = QFontDatabase::applicationFontFamilies(id2).at(0);
    QFont font(BMitra,19*1.25);
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.width(ffooter);
    int height = fontMetrics.height();
    qDebug() << "width =" << width;
    qDebug() << "height =" << height;



    int t = 0;
    t = width/1925;

//    for(int z=0; z<ffooter.count(); z++){
//        if(ffooter.at(z) == '\n')
//            t++;
//    }
    qDebug() << "enter count = " << t;
//    if(t>4)
//        t = 4 ;


    QPixmap PixFooter = CreateFooterList(ffooter,384*1.25,font);

//    int hdynamic = (width/(384*1.25) + 1)*height + height*t*2;
    int hdynamic = PixFooter.height();
    qDebug() << "hdynamic = " << hdynamic ;


    //qDebug() << "font id = "  << id;
    QPixmap pix(384*1.25,(216 + 120 + 120 - 48 - (48 + 16) - 30 + hdynamic)*1.25);
    qDebug() << "pix.size() =" << pix.size();
    pix.fill(Qt::white);
    QPainter p(&pix);
    p.setPen(Qt::SolidLine);
    p.setPen(QColor(Qt::black));

    p.drawImage(0 + (384/2)*1.25 - (Logo.width()/2),5,Logo);

    QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);


    p.setFont(QFont(BTitr,16*1.25));
    p.drawText(QRect(0,(184 - 30)*1.25,384*1.25,(40 + 2)*1.25),Qt::AlignCenter | Qt::AlignTop, info);

    QRect rtextBarcode1(0,(280-30)*1.25,192*1.25,32*1.25);
    QRect rtextBarcode2(192*1.25,(280-30)*1.25,192*1.25,32*1.25);


    p.setFont(QFont(BTitr,12*1.25));
    p.drawText(rtextBarcode2,Qt::AlignCenter, QString("شناسه قبض:    "));
    p.setFont(QFont("PT Sans",12*1.25));
    p.drawText(rtextBarcode1,Qt::AlignCenter, BarcodeText);
    p.drawImage(0,(220 - 30)*1.25,pix1.toImage());



    //qDebug() << "font id = "  << id2;

    p.setFont(QFont(BMitra,19*1.25));

    p.drawImage(0,(216 + 120 + 120 - 48 - (48 + 16) - 30)*1.25,PixFooter.toImage());
//    p.drawText(QRect(0, (216 + 120 + 120 - 48 - (48 + 16) - 30)*1.25, 384*1.25, hdynamic),Qt::AlignRight | Qt::TextWordWrap, ffooter);

    p.setPen(QPen(QColor(Qt::black),3));
    p.drawRect(0,0,(384*1.25) - 3,305*1.25);
    p.drawRect(31*1.25,150*1.25,320*1.25,38*1.25 );


    p.setPen(QColor(Qt::black));
    p.setFont(QFont(BTitr,20*1.25));
    p.drawText(QRect(0,100*1.25,384*1.25,32*1.25),Qt::AlignCenter,datetime);

    p.end();

    QImage img = pix.toImage();
    img.save("img.png","PNG");
    qDebug() << "receipte created";
}


QString MainWidget::encodeBarcode(QString code)
{
    QString encoded;

    encoded.prepend(QChar(codeToChar(CODE128_B_START))); //Start set with B Code 104
    encoded.append(code);
    encoded.append(QChar(calculateCheckCharacter(code)));
    encoded.append(QChar(codeToChar(CODE128_STOP))); //End set with Stop Code 106

    return encoded;
}

int MainWidget::calculateCheckCharacter(QString code)
{
    QByteArray encapBarcode(code.toUtf8()); //Convert code to utf8

    //Calculate check character
    long long sum = CODE128_B_START; //The sum starts with the B Code start character value
    int weight = 1; //Initial weight is 1

    foreach(char ch, encapBarcode) {
        int code_char = charToCode((int)ch); //Calculate character code
        sum += code_char*weight; //add weighted code to sum
        weight++; //increment weight
    }

    int remain = sum%103; //The check character is the modulo 103 of the sum

    //Calculate the font integer from the code integer
    if(remain >= 95)
        remain += 105;
    else
        remain += 32;

    return remain;
}

int MainWidget::codeToChar(int code)
{
    return code + 105;
}

int MainWidget::charToCode(int ch)
{
    return ch - 32;
}

void MainWidget::ProcTimeout()
{
    goToStateInit();
    bTimeout = true;
}

void MainWidget::OpenTimeout()
{
    digitalWrite(OOUT1,0);
    digitalWrite(OOUT2,0);
    while(PlayerButton->VoicePlaying || PlayerCardTrue->VoicePlaying || PlayerCardFalse->VoicePlaying)
        QApplication::processEvents(QEventLoop::AllEvents);
    goToStateInit();
}

void MainWidget::onStartPrinttimeout()
{
    bStartPrint = true;
}




QString MainWidget::getcurrenyIP()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        iNst = 1;
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            return address.toString().replace(".","A");
    }
    return "";
}

void MainWidget::DrawDisplaye(int iiState, bool paper, bool net)
{
    qDebug() << "******draw display******";
    qDebug() << "loop enable: " << bLoopEnabled;
    qDebug() << "State: " << iiState;
    qDebug() << "paper: " << paper;
    qDebug() << "net: "   << net;
    qDebug() << "************************";

    if(iiState == 9){
        if(isAuth == 1){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            lPelak.setHidden(true);
            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,25);
            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");

            QString Text = sCurrentName;
            Text = Text.append('\n');
            Text = Text.append('\n');
            Text = Text + sCardMessage;
            lblText1.setText(Text);
            lp1.setHidden(true);
            lp2.setHidden(true);
            lp3.setHidden(true);
            lp4.setHidden(true);
            return;
        }
        else{
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            lPelak.setHidden(true);
            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,35);
            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : red; }");
            lblText1.setText(sCardMessageNot);
            lp1.setHidden(true);
            lp2.setHidden(true);
            lp3.setHidden(true);
            lp4.setHidden(true);
            return;
        }
    }

    if(bLoopEnabled){
        if(iiState == -2){
            if(!paper && !net){
                lbgLable.setPixmap(pBK4);
                lbgLable.setHidden(false);
                lPelak.setHidden(true);
                QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
                QFont FontVazir_Bold(BTitr,35);
                lblText1.setHidden(false);
                lblText1.setFont(FontVazir_Bold);
                lblText1.setWordWrap(true);
                lblText1.setAlignment(Qt::AlignCenter);
                lblText1.setStyleSheet("QLabel { background-color : white; color : red; }");
                lblText1.setText(sPNMessage);

                lp1.setHidden(true);
                lp2.setHidden(true);
                lp3.setHidden(true);
                lp4.setHidden(true);
                return;
            }
            else if(!paper && net){
                lbgLable.setPixmap(pBK2);
                lbgLable.setHidden(false);
                lPelak.setHidden(true);
                QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
                QFont FontVazir_Bold(BTitr,35);
                lblText1.setHidden(false);
                lblText1.setFont(FontVazir_Bold);
                lblText1.setWordWrap(true);
                lblText1.setAlignment(Qt::AlignCenter);
                lblText1.setStyleSheet("QLabel { background-color : white; color : red; }");
                lblText1.setText(sPaperMessage);
                lp1.setHidden(true);
                lp2.setHidden(true);
                lp3.setHidden(true);
                lp4.setHidden(true);
                return;
            }
            else if(paper && !net){
                lbgLable.setPixmap(pBK3);
                lbgLable.setHidden(false);
                lPelak.setHidden(true);
                QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
                QFont FontVazir_Bold(BTitr,35);
                lblText1.setHidden(false);
                lblText1.setFont(FontVazir_Bold);
                lblText1.setWordWrap(true);
                lblText1.setAlignment(Qt::AlignCenter);
                lblText1.setStyleSheet("QLabel { background-color : white; color : red; }");
                lblText1.setText(sNetMessage);
                lp1.setHidden(true);
                lp2.setHidden(true);
                lp3.setHidden(true);
                lp4.setHidden(true);
                return;
            }
        }
        else if(iiState == -1){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            lPelak.setHidden(true);
            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,35);
            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");
            lblText1.setText(sMessage);
            lp1.setHidden(true);
            lp2.setHidden(true);
            lp3.setHidden(true);
            lp4.setHidden(true);
            return;
        }
        else if(iiState == 0){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            lPelak.setHidden(true);
            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,35);
            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");
            qDebug() << sWaitingMessage;
            lblText1.setText(sWaitingMessage);
            lp1.setHidden(true);
            lp2.setHidden(true);
            lp3.setHidden(true);
            lp4.setHidden(true);
            return;
        }
        else if(iiState == 2){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            if(bPelakk){
                lPelak.setPixmap(pPelak);
                lPelak.setHidden(false);
                lp1.setHidden(false);
                lp2.setHidden(false);
                lp3.setHidden(false);
                lp4.setHidden(false);

                lp4.setText(ssP4);
                lp3.setText(ssP3);
                lp2.setText(ssP2);
                lp1.setText(ssP1);
            }

            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,35);

            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");

            scMessage = scMessage.insert(scMessage.count()/2,'\n');
            scMessage = scMessage.insert(scMessage.count()/2,'\n');
            lblText1.setText(scMessage);

            return;
        }
        else if(iiState == 4){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            lPelak.setHidden(true);
            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,35);
            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");
            lblText1.setText(sReceiptMessage);
            lp1.setHidden(true);
            lp2.setHidden(true);
            lp3.setHidden(true);
            lp4.setHidden(true);
            return;
        }
    }
    else{
        if(iiState == -2){
            if(!paper && !net){
                lbgLable.setPixmap(pBK4);
                lbgLable.setHidden(false);
                lPelak.setHidden(true);
                QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
                QFont FontVazir_Bold(BTitr,35);
                lblText1.setHidden(false);
                lblText1.setFont(FontVazir_Bold);
                lblText1.setWordWrap(true);
                lblText1.setAlignment(Qt::AlignCenter);
                lblText1.setStyleSheet("QLabel { background-color : white; color : red; }");
                lblText1.setText(sPNMessage);
                lp1.setHidden(true);
                lp2.setHidden(true);
                lp3.setHidden(true);
                lp4.setHidden(true);
                return;
            }
            else if(!paper && net){
                lbgLable.setPixmap(pBK2);
                lbgLable.setHidden(false);
                lPelak.setHidden(true);
                QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
                QFont FontVazir_Bold(BTitr,35);
                lblText1.setHidden(false);
                lblText1.setFont(FontVazir_Bold);
                lblText1.setWordWrap(true);
                lblText1.setAlignment(Qt::AlignCenter);
                lblText1.setStyleSheet("QLabel { background-color : white; color : red; }");
                lblText1.setText(sPaperMessage);
                lp1.setHidden(true);
                lp2.setHidden(true);
                lp3.setHidden(true);
                lp4.setHidden(true);
                return;
            }
            else if(paper && !net){
                lbgLable.setPixmap(pBK3);
                lbgLable.setHidden(false);
                lPelak.setHidden(true);
                QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
                QFont FontVazir_Bold(BTitr,35);
                lblText1.setHidden(false);
                lblText1.setFont(FontVazir_Bold);
                lblText1.setWordWrap(true);
                lblText1.setAlignment(Qt::AlignCenter);
                lblText1.setStyleSheet("QLabel { background-color : white; color : red; }");
                lblText1.setText(sNetMessage);
                lp1.setHidden(true);
                lp2.setHidden(true);
                lp3.setHidden(true);
                lp4.setHidden(true);
            }
        }
        else if(iiState == -1){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            lPelak.setHidden(true);
            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,25);
            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");
            //qDebug() << sLoopDisMessage;
            lblText1.setText(sLoopDisMessage);
            lp1.setHidden(true);
            lp2.setHidden(true);
            lp3.setHidden(true);
            lp4.setHidden(true);
            return;
        }
        else if(iiState == 0){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            lPelak.setHidden(true);
            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,35);
            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");
            lblText1.setText(sWaitingMessage);
            lp1.setHidden(true);
            lp2.setHidden(true);
            lp3.setHidden(true);
            lp4.setHidden(true);
            return;
        }
        else if(iiState == 4){
            lbgLable.setPixmap(pBK1);
            lbgLable.setHidden(false);
            if(bPelakk){
                lPelak.setPixmap(pPelak);
                lPelak.setHidden(false);
                lp1.setHidden(false);
                lp2.setHidden(false);
                lp3.setHidden(false);
                lp4.setHidden(false);

                lp4.setText(ssP4);
                lp3.setText(ssP3);
                lp2.setText(ssP2);
                lp1.setText(ssP1);
            }

            QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont FontVazir_Bold(BTitr,35);

            lblText1.setHidden(false);
            lblText1.setFont(FontVazir_Bold);
            lblText1.setWordWrap(true);
            lblText1.setAlignment(Qt::AlignCenter);
            lblText1.setStyleSheet("QLabel { background-color : white; color : green; }");
            lblText1.setText(sReceiptMessage);
            return;
        }
    }
}


void MainWidget::PrepareUI()
{

    id = QFontDatabase::addApplicationFont("/var/miladtower/.fonts/BTitr.ttf");
    id1 = QFontDatabase::addApplicationFont("/var/miladtower/.fonts/code128.ttf");;
    id2 = QFontDatabase::addApplicationFont("/var/miladtower/.fonts/BMitra.ttf");
    id3 = QFontDatabase::addApplicationFont("/var/miladtower/.fonts/Vazir-Bold.ttf");


    pBK1.load("/var/miladtower/res/BK1.png","PNG");
    pBK1 = pBK1.scaled(800,480,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    pBK2.load("/var/miladtower/res/BK2.png","PNG");
    pBK2 = pBK2.scaled(800,480,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    pBK3.load("/var/miladtower/res/BK3.png","PNG");
    pBK3= pBK3.scaled(800,480,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    pBK4.load("/var/miladtower/res/BK4.png","PNG");
    pBK4 = pBK4.scaled(800,480,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    pPelak.load("/var/miladtower/res/pelak.png","PNG");
    pPelak = pPelak.scaled(270,64,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    QRect rbgPix(0,0,800,480);
    lbgLable.setHidden(true);
    lbgLable.setGeometry(rbgPix);
    lbgLable.setParent(this);

    QRect rmText(80,146,634,158);
    lblText1.setHidden(true);
    lblText1.setParent(this);
    lblText1.setGeometry(rmText);

    QRect rPelak(264,313,270,69);
    lPelak.setHidden(true);
    lPelak.setParent(this);
    lPelak.setGeometry(rPelak);



    //p1
    lp1.setParent(this);
    lp1.setHidden(true);
    lp1.setGeometry(264 + 40 + 13,313 + 5,40,55);
    QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont FontVazir_Bold1(BTitr,25);
    //lp1.setHidden(false);
    lp1.setFont(FontVazir_Bold1);
    lp1.setWordWrap(true);
    lp1.setAlignment(Qt::AlignCenter);
    lp1.setStyleSheet("QLabel { background-color : white; color : black; }");
    lp1.setText(sNetMessage);

    //p2
    lp2.setParent(this);
    lp2.setHidden(true);
    lp2.setGeometry(264 + 40 + 40 + 15 ,313 + 5 ,40,55);
    //QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont FontVazir_Bold2(BTitr,25);
    //lp1.setHidden(false);
    lp2.setFont(FontVazir_Bold2);
    lp2.setWordWrap(true);
    lp2.setAlignment(Qt::AlignCenter);
    lp2.setStyleSheet("QLabel { background-color : white; color : black; }");
    lp2.setText(sNetMessage);


    //p3
    lp3.setParent(this);
    lp3.setHidden(true);
    lp3.setGeometry(264 + 40 + 40 + 40 + 40 -5 -13 ,313 + 5 ,40 + 30,55);
    //QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont FontVazir_Bold3(BTitr,25);
    //lp1.setHidden(false);
    lp3.setFont(FontVazir_Bold3);
    lp3.setWordWrap(true);
    lp3.setAlignment(Qt::AlignCenter);
    lp3.setStyleSheet("QLabel { background-color : white; color : black; }");
    lp3.setText(sNetMessage);



    //p4
    lp4.setParent(this);
    lp4.setHidden(true);
    lp4.setGeometry(264 + 40 + 40 + 40 + 40 + 40 + 40 - 13,313 + 24 + 10 ,30 - 3,30 - 5);
    //QString BTitr = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont FontVazir_Bold4(BTitr,17);
    //lp1.setHidden(false);
    lp4.setFont(FontVazir_Bold4);
    lp4.setWordWrap(true);
    lp4.setAlignment(Qt::AlignCenter);
    lp4.setStyleSheet("QLabel { background-color : white; color : black; }");
    lp4.setText(sNetMessage);
}

MainWidget::~MainWidget()
{
    qDebug() << "~MainWidget";
    digitalWrite(OOUT1,0);
    digitalWrite(OOUT2,0);

    delete PlayerButton;
    delete PlayerCardTrue;
    delete PlayerCardFalse;
    ThreadButtonPalyer->terminate();
    ThreadCardPalyerTrue->terminate();
    ThreadCardPalyerFalse->terminate();
    delete ThreadButtonPalyer;
    delete ThreadCardPalyerTrue;
    delete ThreadCardPalyerFalse;
    delete ui;

    QApplication::quit();
}
