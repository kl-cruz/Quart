#include "mainwindow.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "iostream"
#include <QMessageBox>
#include <QMovie>

#include "src/qextserialport.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::fillControls()
{
#ifndef Q_OS_WIN
    baudRateBox->addItem("150",BAUD150);
#endif
    baudRateBox->addItem("300",BAUD300);
    baudRateBox->addItem("600",BAUD600);
    baudRateBox->addItem("1200",BAUD1200);
    baudRateBox->addItem("2400",BAUD2400);
    baudRateBox->addItem("4800",BAUD4800);
    baudRateBox->addItem("9600",BAUD9600);
    baudRateBox->addItem("19200",BAUD19200);
    baudRateBox->addItem("38400",BAUD38400);
    baudRateBox->addItem("57600",BAUD57600);
    baudRateBox->addItem("115200",BAUD115200);
#ifndef Q_OS_WIN
    baudRateBox->setCurrentIndex(6);
#else
    baudRateBox->setCurrentIndex(5);
#endif

     foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
     {

         portBox->addItem(info.portName);
     }
     //make sure user can input their own port name!
     portBox->setEditable(true);


     flowControlBox->addItem("Brak kontroli",FLOW_OFF);
     flowControlBox->addItem("Kontrola sprzętowa (handshake): DTR/DSR, RTS/CTS",FLOW_HARDWARE);
     flowControlBox->addItem("Kontrola programowa: XON/XOFF",FLOW_XONXOFF);


     ownTerminatorValue->setText("Wpisz tutaj własny terminator. Znaki zapisz szesnastkowo, oddziel przecinkiem.");

     characterFormatBox->addItem("5", DATA_5);
     characterFormatBox->addItem("6", DATA_6);
     characterFormatBox->addItem("7", DATA_7);
     characterFormatBox->addItem("8", DATA_8);
     characterFormatBox->setCurrentIndex(3);

     parityBox->addItem("NONE", PAR_NONE);
     parityBox->addItem("ODD", PAR_ODD);
     parityBox->addItem("EVEN", PAR_EVEN);

     stopBitsBox->addItem("1",STOP_1);
     stopBitsBox->addItem("2",STOP_2);

     timeoutEnableBox->addItem("Nieaktywny",false);
     timeoutEnableBox->addItem("Aktywny", true);

     terminatorBox->addItem("Brak","");
     terminatorBox->addItem("Standardowy CR","\r");
     terminatorBox->addItem("Standardowy LF","\n");
     terminatorBox->addItem("Standardowy CR-LF","\r\n");
     terminatorBox->addItem("Własny (wpisz poniżej)","OWN");

     DSRButton->setStyleSheet("background-color: red;color:black");
     setRTSButton->setStyleSheet("background-color:red;color:black");
     setDTRButton->setStyleSheet("background-color:red;color:black");

     fillStatusTab(false);
}

void MainWindow::fillStatusTab(bool status)
{
    if(status)
    {
        portEnabledValue->setText("aktywny");
        portEnabledValue->setStyleSheet("background-color:green;color:white");
        setPortValue->setText(portBox->currentText());
        setBaudRateValue->setText(QString("%1").arg(baudRateBox->itemData(baudRateBox->currentIndex()).toInt()));

    }
    else
    {
        portEnabledValue->setText("nieaktywny");
        portEnabledValue->setStyleSheet("background-color:red;color:black");
    }
}

void MainWindow::startConnection()
{
    if(autoBaudingEnabledCheckBox->checkState()!=Qt::Checked)
    {
        binaryTab->setEnabled(true);
    }
    textModeTab->setEnabled(true);
    portBox->setEnabled(false);
    baudRateBox->setEnabled(false);
    characterFormatBox->setEnabled(false);
    stopBitsBox->setEnabled(false);
    flowControlBox->setEnabled(false);
    terminatorBox->setEnabled(false);
    ownTerminatorValue->setEnabled(false);
    parityBox->setEnabled(false);
    timeoutEnableBox->setEnabled(false);
    timeoutValue->setEnabled(false);
    autoBaudingButton->setEnabled(false);
    autoBaudingEnabledCheckBox->setEnabled(false);


}

void MainWindow::stopConnection()
{
    if(autoBaudingEnabledCheckBox->checkState()!=Qt::Checked)
    {
        autoBaudingButton->setEnabled(false);
    }
    else
    {
        autoBaudingButton->setEnabled(true);
    }
    textModeTab->setEnabled(false);
    binaryTab->setEnabled(false);
    portBox->setEnabled(true);
    baudRateBox->setEnabled(true);
    characterFormatBox->setEnabled(true);
    stopBitsBox->setEnabled(true);
    flowControlBox->setEnabled(true);
    terminatorBox->setEnabled(true);
    ownTerminatorValue->setEnabled(true);
    parityBox->setEnabled(true);
    timeoutEnableBox->setEnabled(true);
    timeoutValue->setEnabled(true);

    autoBaudingEnabledCheckBox->setEnabled(true);
}

void MainWindow::dsrChangedHandle(bool status)
{
    if(status)
    {
        DSRButton->setStyleSheet("background-color: red;color:black");
    }
    else{
        DSRButton->setStyleSheet("background-color: green;color:white");
    }
}

void MainWindow::ctsChangedHandle(bool status)
{
    if(status)
    {
        CTSButton->setStyleSheet("background-color: red;color:black");
    }
    else{
        CTSButton->setStyleSheet("background-color: green;color:white");
    }
}

void MainWindow::setDTR()
{
    DTRstatus=!DTRstatus;
    port->setDtr(DTRstatus);
    if(DTRstatus)
        setDTRButton->setStyleSheet("background-color:green;color:white");
    else
        setDTRButton->setStyleSheet("background-color:red;color:black");
}

void MainWindow::setRTS()
{
    RTSstatus=!RTSstatus;
    port->setRts(RTSstatus);
    if(RTSstatus)
        setRTSButton->setStyleSheet("background-color:green;color:white");
    else
        setRTSButton->setStyleSheet("background-color:red;color:black");
}


void MainWindow::onReadyRead()
{
    if (port->bytesAvailable()) {
        QString fromPort=QString::fromLatin1(port->readAll());
        /*for(int i=0;i<fromPort.count();++i)
        {
            std::cout<<"odebrano: "<<static_cast<unsigned short>(static_cast<unsigned char>(fromPort.toStdString().c_str()[i]))<<std::endl;
        }*/
        if(port->isOpen()){
        if ((fromPort.toStdString().c_str()[0]==PING_SEND_VALUE) && (fromPort.count()==1)){
            char ping=PING_RECEIVE_VALUE;
            port->write(&ping,1);
        }
        else
            if ((fromPort.toStdString().c_str()[0]==PING_RECEIVE_VALUE)&& (fromPort.count()==1)){
                unsigned int elapsed = pingTime.elapsed();
                pingConsole->append("Odebrano odpowiedź w czasie "+QString("%1").arg(elapsed)+"ms");
                pingAvgCounter++;
                pingAvgTime+=elapsed;
                checkCounterAndSendNextPing();

        }
        else
                if((fromPort.toStdString()==AUTOBAUDING_QUERY) && (autoBaudingEnabledCheckBox->checkState()==Qt::Checked))
                {
                    std::cout<<"Autobauding query"<<std::endl;
                    char resp[]=AUTOBAUDING_ANSWER_CHAR_ARRAY;
                    port->write(resp,sizeof(resp));
                    return;

                }
                else if((fromPort.toStdString()==AUTOBAUDING_ANSWER) && (autoBaudingEnabledCheckBox->checkState()==Qt::Checked))
                {
                    std::cout<<"Autobauding answer"<<std::endl;
                    autoBaudingTimeoutTimer->stop();
                    disableTab->setVisible(false);
                    return;
                }

        textConsole->moveCursor(QTextCursor::End);
        textConsole->insertPlainText(fromPort);

        }
    }



}

void MainWindow::onPortAddedOrRemoved()
{
    QString current = portBox->currentText();

    portBox->blockSignals(true);
    portBox->clear();
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        portBox->addItem(info.portName);

    portBox->setCurrentIndex(portBox->findText(current));

    portBox->blockSignals(false);
}


void MainWindow::onPortNameChanged(const QString &name)
{
    if (port->isOpen()) {
        port->close();
    }
}

void MainWindow::onBaudRateChanged(int idx)
{
    port->setBaudRate((BaudRateType)baudRateBox->itemData(idx).toInt());
}

void MainWindow::onParityChanged(int idx)
{
    port->setParity((ParityType)parityBox->itemData(idx).toInt());
}

void MainWindow::onDataBitsChanged(int idx)
{
    port->setDataBits((DataBitsType)characterFormatBox->itemData(idx).toInt());
}

void MainWindow::onStopBitsChanged(int idx)
{
    port->setStopBits((StopBitsType)stopBitsBox->itemData(idx).toInt());
}

void MainWindow::onTimeoutChanged(int val)
{
    port->setTimeout(val);
}

void MainWindow::onOpenCloseButtonClicked()
{
    if (!port->isOpen()) {
        port->setPortName(portBox->currentText());
        port->open(QIODevice::ReadWrite);
        if (port->isOpen()) {
        statusBar->showMessage("Port jest otwarty");
        lineStatusTimer->start();
        openClosePortButton->setText("Zakończ transmisję");
        fillStatusTab(true);
        textModeTab->setEnabled(true);
        if(terminatorBox->itemData(terminatorBox->currentIndex()).toString()=="OWN")
        {
            //PARSE TERMINATOR
        }
        else
            terminatorString=terminatorBox->itemData(terminatorBox->currentIndex()).toString();
        tabWidget->setCurrentIndex(2);
        startConnection();
        }
        else
        {
            QMessageBox::information(NULL, "Ostrzeżenie!", "Port jest nieaktywny!");
        }
    }
    else {
        lineStatusTimer->stop();
        port->close();
        statusBar->showMessage("Port jest zamknięty");
        openClosePortButton->setText("Uruchom transmisję");
        fillStatusTab(false);
        textModeTab->setEnabled(false);
        stopConnection();
    }

    if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
        timer->start();
    else
        timer->stop();
}

void MainWindow::onStartAutoBauding()
{
    autoBaudingTimeoutTimer->start();
    baudRateBox->setCurrentIndex(0);
    disableTab->setVisible(true);
    if (port->isOpen())
        port->close();
    port->setPortName(portBox->currentText());
    port->open(QIODevice::ReadWrite);
    timer->start();


    if (port->isOpen()){
        char autoBaud[]=AUTOBAUDING_QUERY_CHAR_ARRAY;
        port->write(autoBaud,sizeof(autoBaud));
    }
    else
    {
        disableTab->setVisible(false);
        autoBaudingTimeoutTimer->stop();
        QMessageBox::information(NULL, "Ostrzeżenie!", "Port jest nieaktywny!");
        timer->stop();
    }
}

void MainWindow::autoBaudingTimerHandle()
{
    if (port->isOpen())
        port->close();
    if(baudRateBox->currentIndex()==baudRateBox->count()-1)
    {
        disableTab->setVisible(false);
        autoBaudingTimeoutTimer->stop();
        timer->stop();
        QMessageBox::information(NULL, "Ostrzeżenie!", "Autobauding zakończony niepowodzeniem!");
    }
    else{
    baudRateBox->setCurrentIndex(baudRateBox->currentIndex()+1);
    }
    onBaudRateChanged(baudRateBox->currentIndex());

    port->setPortName(portBox->currentText());
    port->open(QIODevice::ReadWrite);

    if (port->isOpen()){
        char autoBaud[]=AUTOBAUDING_QUERY_CHAR_ARRAY;
        port->write(autoBaud,sizeof(autoBaud));
    }
    else
    {
        disableTab->setVisible(false);
        autoBaudingTimeoutTimer->stop();
        QMessageBox::information(NULL, "Ostrzeżenie!", "Port jest nieaktywny!");
        timer->stop();
    }
}

void MainWindow::onAutoBaudingCheckBoxEnable(int)
{
    if(autoBaudingEnabledCheckBox->checkState()==Qt::Checked)
    {
        autoBaudingButton->setEnabled(true);
    }
    else
    {
        autoBaudingButton->setEnabled(false);
    }
}

void MainWindow::checkNextAutoBauding()
{

}

void MainWindow::onSendButtonClicked()
{
    if (port->isOpen() && !sendTextConsole->toPlainText().isEmpty()){
        QString toSend=sendTextConsole->toPlainText().toLatin1()+terminatorString;
        port->write(toSend.toStdString().c_str());
        sendTextConsole->clear();
    }
}

void MainWindow::onLineStatusCheck()
{
    /* LS_CTS      CTS
     LS_DSR      DSR
     LS_DCD      DCD
     LS_RI       RI
     LS_RTS      RTS (POSIX only)
     LS_DTR      DTR (POSIX only)*/
             ulong status=port->lineStatus();
             if(status&LS_DSR)
             {
                 dsrChangedHandle(true);
             }
             else
             {
                 dsrChangedHandle(false);
             }

             if(status&LS_CTS)
             {
                 ctsChangedHandle(true);
             }
             else
             {
                 ctsChangedHandle(false);
             }
}

void MainWindow::onPingButtonClicked()
{
    pingConsole->append("\n->PING rs232");
    if (port->isOpen()){
        char ping=PING_SEND_VALUE;
        port->write(&ping,1);
        pingTime.restart();
        pingTimeoutTimer->start();
        pingCounter=5;
        pingAvgCounter=0;
        pingAvgTime=0;
    }
    else
    {
        pingConsole->append("Port wyłączony.");
    }
}

void MainWindow::onPingTimeout()
{
    pingConsole->append("Klient jest niedostępny.");
    checkCounterAndSendNextPing();
    //pingTimeoutTimer->stop();
}

void MainWindow::checkCounterAndSendNextPing()
{
    pingTimeoutTimer->stop();

    if (port->isOpen() && (--pingCounter>0)){
            char ping=PING_SEND_VALUE;
            port->write(&ping,1);
            pingTime.restart();
            pingTimeoutTimer->start();
     }else
     if (!port->isOpen())
     {
        QMessageBox::information(NULL, "Ostrzeżenie!", "Port jest nieaktywny!");
     }else
         if (pingCounter<=0)
         {
             pingConsole->append("Srednia prędkość z poprawnych transferów: "+QString("%1").arg(pingAvgTime*1.0/(pingAvgCounter*1.0))+"ms (poprawnych pakietów: "+QString("%1").arg(pingAvgCounter)+")");

         }
}


void MainWindow::createUI()
{
    /*Create canvas form*/

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("MainWindow"));
    this->resize(640, 700);
    QString windowTitle="";
    this->setWindowTitle(QApplication::translate("MainWindow", windowTitle.toStdString().c_str(), 0));

    //panel during teaching

    autoBaudingInfoLabel=new QLabel("Proszę czekać trwa proces autobaudingu");
    autoBaudingInfoLabel->resize(200,30);
    autoBaudingInfoLabel->setStyleSheet("background-color:rgba(0, 0, 0, 0.0);");

    QMovie *movie = new QMovie(":/graphics/wait.gif");
    busyMovieLabel = new QLabel();
    busyMovieLabel->setMovie(movie);
    busyMovieLabel->setAlignment(Qt::AlignHCenter);

    disableTab=new QGroupBox(this);
    disableTab->resize(this->width(),this->height());
    disableTab->setStyleSheet("background-color:rgba(255, 255, 255, 0.9);");
    disableTab->setVisible(false);
    disableTabLayout= new QVBoxLayout(disableTab);
    disableTabLayout->setAlignment(Qt::AlignCenter);
    disableTabLayout->addWidget(autoBaudingInfoLabel);
    disableTabLayout->addWidget(busyMovieLabel);
    movie->start();

    disableTabLayout->setSpacing(5);
    disableTabLayout->setContentsMargins(50,50,50,50);


    //end

    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    centralWidget->stackUnder(disableTab);

    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));


    tabWidget = new QTabWidget(centralWidget);
    tabWidget->setObjectName(QStringLiteral("tabWidget"));

    //start tab
    startTab=new QWidget();
    startTab->setObjectName(QStringLiteral("startTab"));
    tabWidget->addTab(startTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(startTab), QApplication::translate("MainWindow", "Witaj!", 0));

    startLabel=new QLabel();
    startLabel->setWordWrap(true);
    startLabel->show();

    startTabLayout=new QVBoxLayout();
    startTabLayout->addWidget(startLabel);
    startTab->setLayout(startTabLayout);



    //options tab
    optionsTab=new QWidget();
    optionsTab->setObjectName(QStringLiteral("optionsTab"));
    tabWidget->addTab(optionsTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(optionsTab), QApplication::translate("MainWindow", "Opcje", 0));

    baudRateLabel=new QLabel("Baudrate połączenia");
    portLabel=new QLabel("Port podłączonego urządzenia");
    characterFormatLabel=new QLabel("Format znaku");
    stopBitsLabel=new QLabel("Ilość bitów stopu");
    flowControlLabel=new QLabel("Kontrola przepływu");
    terminatorLabel=new QLabel("Wybór terminatora");
    ownTerminatorLabel=new QLabel("Własny terminatora");
    parityLabel=new QLabel("Parzystość");
    timeoutEnableLabel=new QLabel("Aktywność ograniczenia czasowego transakcji");
    timeoutLabel=new QLabel("Ograniczenie czasowe transakcji");

    autoBaudingLabel=new QLabel("Uruchom autobauding");
    autoBaudingTimeoutTimer=new QTimer(this);
    autoBaudingTimeoutTimer->setInterval(1001);
    autoBaudingTimeoutTimer->stop();

    autoBaudingEnabledLabel=new QLabel("Aktywacja autobaudingu (deaktywuje wysyłanie binarne)");
    autoBaudingEnabledCheckBox=new QCheckBox();

    openClosePortLabel=new QLabel("Uruchamianie portu");



    baudRateBox= new QComboBox();

    portBox= new QComboBox();

    characterFormatBox = new QComboBox();

    stopBitsBox= new QComboBox();

    flowControlBox= new QComboBox();

    terminatorBox= new QComboBox();

    ownTerminatorValue=new QTextEdit();

    parityBox= new QComboBox();

    timeoutEnableBox= new QComboBox();

    timeoutValue = new QSpinBox();
    timeoutValue ->setMinimum(10);
    timeoutValue ->setValue(20);
    timeoutValue ->setMaximum(9999);

    autoBaudingButton = new QPushButton("Uruchom");
    autoBaudingButton->setEnabled(false);
    openClosePortButton = new QPushButton("Uruchom transmisję");

    optionsLayout=new QFormLayout;
    optionsLayout->addRow(new QLabel("Opcje połączenia RS232"));
    optionsLayout->addRow(portLabel,portBox);
    optionsLayout->addRow(baudRateLabel,baudRateBox);
    optionsLayout->addRow(characterFormatLabel,characterFormatBox);
    optionsLayout->addRow(stopBitsLabel,stopBitsBox);
    optionsLayout->addRow(flowControlLabel,flowControlBox);
    optionsLayout->addRow(terminatorLabel,terminatorBox);
    optionsLayout->addRow(ownTerminatorLabel,ownTerminatorValue);
    optionsLayout->addRow(parityLabel,parityBox);
    optionsLayout->addRow(timeoutEnableLabel,timeoutEnableBox);
    optionsLayout->addRow(timeoutLabel,timeoutValue);
    optionsLayout->addRow(autoBaudingEnabledLabel,autoBaudingEnabledCheckBox);
    optionsLayout->addRow(autoBaudingLabel,autoBaudingButton);
    optionsLayout->addRow(openClosePortLabel,openClosePortButton);
    optionsTab->setLayout(optionsLayout);


    //textmode
    textModeTab=new QWidget();
    textModeTab->setObjectName(QStringLiteral("textModeTab"));
    textModeTab->setEnabled(false);
    tabWidget->addTab(textModeTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(textModeTab), QApplication::translate("MainWindow", "Tryb tekstowy", 0));

    textModeTabLayout= new QVBoxLayout(textModeTab);
    textModeTabLayout->setAlignment(Qt::AlignCenter);

    textConsole=new QTextEdit(textModeTab);
    textModeScrollBar=new QScrollBar(textConsole);
    textConsole->setReadOnly(true);
    textConsole->setAlignment(Qt::AlignLeft);
    textConsole->setVerticalScrollBar(textModeScrollBar);
    //textConsole->setFont(font);

    signalsPanel=new QWidget();
    signalsPanelLayout = new QHBoxLayout(signalsPanel);
    setDTRButton=new QPushButton("Stan DTR");
    setRTSButton=new QPushButton("Stan RTS");

    DSRButton=new QPushButton("Stan DSR");
    DSRButton->setEnabled(false);
    CTSButton=new QPushButton("Stan CTS");
    CTSButton->setEnabled(false);
    signalsPanelLayout->addWidget(setDTRButton);
    signalsPanelLayout->addWidget(setRTSButton);
    signalsPanelLayout->addWidget(DSRButton);
    signalsPanelLayout->addWidget(CTSButton);

    sendTextConsole=new QTextEdit();
    sendTextModeScrollBar=new QScrollBar(sendTextConsole);
    sendTextConsole->setAlignment(Qt::AlignLeft);
    sendTextConsole->setVerticalScrollBar(sendTextModeScrollBar);
    sendTextConsole->setMaximumHeight(height()/4);

    //sendTextConsole->setFont(font);
    consoleButtons=new QWidget();
    textModeButtonsLayout = new QHBoxLayout(consoleButtons);
    sendButton=new QPushButton();
    sendButton->setText("Wyślij");
    textModeButtonsLayout->addWidget(sendButton);
    textModeTabLayout->addWidget(textConsole);
    textModeTabLayout->addWidget(signalsPanel);
    textModeTabLayout->addWidget(sendTextConsole);
    textModeTabLayout->addWidget(consoleButtons);






    //ping

    pingTimeoutTimer=new QTimer(this);
    pingTimeoutTimer->setInterval(1000);

    pingTime.start();


    pingTab=new QWidget();
    pingTab->setObjectName(QStringLiteral("pingTab"));
    tabWidget->addTab(pingTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(pingTab), QApplication::translate("MainWindow", "Ping", 0));

    pingConsole=new QTextEdit();
    pingModeScrollBar=new QScrollBar(pingConsole);
    pingButton=new QPushButton("Uruchom proces ping");

    pingConsole->setVerticalScrollBar(pingModeScrollBar);

    pingTabLayout=new QVBoxLayout();
    pingTabLayout->addWidget(pingConsole);
    pingTabLayout->addWidget(pingButton);
    pingTab->setLayout(pingTabLayout);

    //binary tab
    binaryTab=new QWidget();
    binaryTab->setObjectName(QStringLiteral("binaryTab"));
    tabWidget->addTab(binaryTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(binaryTab), QApplication::translate("MainWindow", "Tryb binarny", 0));

    binaryTab->setEnabled(false);
    //portinfo
    portinfoTab=new QWidget();
    portinfoTab->setObjectName(QStringLiteral("portinfoTab"));
    tabWidget->addTab(portinfoTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(portinfoTab), QApplication::translate("MainWindow", "Stan obecnego połączenia", 0));


    portEnabledLabel=new QLabel("Stan połączenia");
    portEnabledValue=new QLabel("nieaktywne");
    setBaudRateLabel=new QLabel("Baudrate połączenia");
    setPortLabel=new QLabel("Port podłączonego urządzenia");
    setCharacterFormatLabel=new QLabel("Format znaku");
    setStopBitsLabel=new QLabel("Ilość bitów stopu");
    setFlowControlLabel=new QLabel("Kontrola przepływu");

    setBaudRateValue=new QLabel("wartość");
    setPortValue=new QLabel("wartość");
    setCharacterFormatValue=new QLabel("wartość");
    setStopBitsValue=new QLabel("wartość");
    setFlowControlValue=new QLabel("wartość");
    /*terminatorLabel=new QLabel("Wybór terminatora");
    ownTerminatorLabel=new QLabel("Własny terminatora");
    parityLabel=new QLabel("Parzystość");
    timeoutEnableLabel=new QLabel("Aktywność ograniczenia czasowego transakcji");
    timeoutLabel=new QLabel("Ograniczenie czasowe transakcji");

    autoBaudingLabel=new QLabel("Uruchom autobauding");
    openClosePortLabel=new QLabel("Uruchamianie portu");*/


    portinfoLayout=new QFormLayout;
    portinfoLayout->addRow(new QLabel("Stan uruchomionego połączenia"));
    portinfoLayout->addRow(portEnabledLabel,portEnabledValue);
    portinfoLayout->addRow(setPortLabel,setPortValue);
    portinfoLayout->addRow(setBaudRateLabel,setBaudRateValue);
    portinfoLayout->addRow(setCharacterFormatLabel,setCharacterFormatValue);
    portinfoLayout->addRow(setStopBitsLabel,setStopBitsValue);
    portinfoLayout->addRow(setFlowControlLabel,setFlowControlValue);

    portinfoTab->setLayout(portinfoLayout);










    gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

    this->setCentralWidget(centralWidget);
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    this->setStatusBar(statusBar);
    statusBar->showMessage("Program gotowy do pracy");

    QMetaObject::connectSlotsByName(this);

    fillControls();

    timer = new QTimer(this);
    timer->setInterval(25);
    //! [1]
    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 20};
    port = new QextSerialPort(portBox->currentText(), settings, QextSerialPort::Polling);
    lineStatusTimer=new QTimer(this);
    lineStatusTimer->setInterval(100);
    //! [1]

    enumerator = new QextSerialEnumerator(this);
    enumerator->setUpNotifications();

    connect(baudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
    connect(parityBox, SIGNAL(currentIndexChanged(int)), SLOT(onParityChanged(int)));
    connect(characterFormatBox, SIGNAL(currentIndexChanged(int)), SLOT(onDataBitsChanged(int)));
    connect(stopBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onStopBitsChanged(int)));
    connect(timeoutValue, SIGNAL(valueChanged(int)), SLOT(onTimeoutChanged(int)));

    connect(portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
    connect(openClosePortButton, SIGNAL(clicked()), SLOT(onOpenCloseButtonClicked()));
    connect(sendButton, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
    connect(timer, SIGNAL(timeout()), SLOT(onReadyRead()));
    connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));
    connect(port, SIGNAL(dsrChanged(bool)), SLOT(dsrChangedHandle(bool)));
    connect(autoBaudingButton,SIGNAL(clicked()),SLOT(onStartAutoBauding()));
    connect(autoBaudingTimeoutTimer,SIGNAL(timeout()),SLOT(autoBaudingTimerHandle()));
    connect(autoBaudingEnabledCheckBox,SIGNAL(stateChanged(int)),SLOT(onAutoBaudingCheckBoxEnable(int)));

    connect(lineStatusTimer, SIGNAL(timeout()), SLOT(onLineStatusCheck()));
    connect(setDTRButton, SIGNAL(clicked()), SLOT(setDTR()));
    connect(setRTSButton, SIGNAL(clicked()), SLOT(setRTS()));

    connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
    connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortAddedOrRemoved()));

    connect(pingTimeoutTimer, SIGNAL(timeout()), SLOT(onPingTimeout()));
    connect(pingButton, SIGNAL(clicked()), SLOT(onPingButtonClicked()));

}

