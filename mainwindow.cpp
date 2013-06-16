#include "mainwindow.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"

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

    baudRateBox->addItem("150",BAUD150);
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

     DSRButton->setStyleSheet("background-color: red");
}

void MainWindow::dsrChangedHandle(bool status)
{
    if(status)
    {
        DSRButton->setStyleSheet("background-color: red");
    }
    else{
        DSRButton->setStyleSheet("background-color: green");
    }
}


void MainWindow::onReadyRead()
{
    if (port->bytesAvailable()) {
        textConsole->moveCursor(QTextCursor::End);
        textConsole->insertPlainText(QString::fromLatin1(port->readAll()));
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

void MainWindow::onQueryModeChanged(int idx)
{
    //port->setQueryMode((QextSerialPort::QueryMode)ui->queryModeBox->itemData(idx).toInt());
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
    }
    else {
        port->close();
    }

    if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
        timer->start();
    else
        timer->stop();
}

void MainWindow::onSendButtonClicked()
{
    if (port->isOpen() && !sendTextConsole->toPlainText().isEmpty())
        port->write(sendTextConsole->toPlainText().toLatin1());
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

    teachingERMSLabel=new QLabel("Obecna wartość rms");
    teachingERMSLabel->resize(200,30);
    teachingERMSLabel->setStyleSheet("background-color:rgba(0, 0, 0, 0.0);");

    teachingTimeLabel=new QLabel("czas");
    teachingTimeLabel->resize(200,30);
    teachingTimeLabel->setStyleSheet("background-color:rgba(0, 0, 0, 0.0);");

    teachingEraLabel=new QLabel("epoka");
    teachingEraLabel->resize(200,30);
    teachingEraLabel->setStyleSheet("background-color:rgba(0, 0, 0, 0.0);");

    disableTab=new QGroupBox(this);
    disableTab->resize(this->width(),this->height());
    disableTab->setStyleSheet("background-color:rgba(255, 255, 255, 0.9);");
    disableTab->setVisible(false);
    disableTabLayout= new QVBoxLayout(disableTab);
    disableTabLayout->setAlignment(Qt::AlignCenter);
    disableTabLayout->addWidget(teachingERMSLabel);
    disableTabLayout->addWidget(teachingEraLabel);
    disableTabLayout->addWidget(teachingTimeLabel);

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

    textModeTab=new QWidget();
    textModeTab->setObjectName(QStringLiteral("textModeTab"));
    tabWidget->addTab(textModeTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(textModeTab), QApplication::translate("MainWindow", "Tryb tekstowy", 0));


    //textmode
    textModeTabLayout= new QVBoxLayout(textModeTab);
    textModeTabLayout->setAlignment(Qt::AlignCenter);

    textConsole=new QTextEdit(textModeTab);
    textModeScrollBar=new QScrollBar(textConsole);
    textConsole->setReadOnly(true);
    textConsole->setAlignment(Qt::AlignLeft);
    textConsole->setVerticalScrollBar(textModeScrollBar);

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
    consoleButtons=new QWidget();
    textModeButtonsLayout = new QHBoxLayout(consoleButtons);
    sendButton=new QPushButton();
    sendButton->setText("Wyślij");
    textModeButtonsLayout->addWidget(sendButton);
    textModeTabLayout->addWidget(textConsole);
    textModeTabLayout->addWidget(signalsPanel);
    textModeTabLayout->addWidget(sendTextConsole);
    textModeTabLayout->addWidget(consoleButtons);




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
    timeoutValue ->setMinimum(1);
    timeoutValue ->setValue(20);
    timeoutValue ->setMaximum(9999);

    autoBaudingButton = new QPushButton();
    openClosePortButton = new QPushButton();

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
    optionsLayout->addRow(autoBaudingLabel,autoBaudingButton);
    optionsLayout->addRow(openClosePortLabel,openClosePortButton);
    optionsTab->setLayout(optionsLayout);

    //about
    pingTab=new QWidget();
    pingTab->setObjectName(QStringLiteral("pingTab"));
    tabWidget->addTab(pingTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(pingTab), QApplication::translate("MainWindow", "Ping", 0));

    pingImageLabel=new QLabel("ABOUT_TEXT",pingTab);
    pingImageLabel->setWordWrap(true);
    pingImageLabel->show();

    pingTabLayout=new QVBoxLayout();
    pingTabLayout->addWidget(pingImageLabel);
    pingTab->setLayout(pingTabLayout);

    //binary tab
    binaryTab=new QWidget();
    binaryTab->setObjectName(QStringLiteral("binaryTab"));
    tabWidget->addTab(binaryTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(binaryTab), QApplication::translate("MainWindow", "Tryb binarny", 0));


    gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

    this->setCentralWidget(centralWidget);
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    this->setStatusBar(statusBar);
    statusBar->showMessage("Program gotowy do pracy");

    QMetaObject::connectSlotsByName(this);

    fillControls();

    timer = new QTimer(this);
    timer->setInterval(40);
    //! [1]
    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort(portBox->currentText(), settings, QextSerialPort::Polling);
    //! [1]

    enumerator = new QextSerialEnumerator(this);
    enumerator->setUpNotifications();

    connect(baudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
    connect(parityBox, SIGNAL(currentIndexChanged(int)), SLOT(onParityChanged(int)));
    connect(characterFormatBox, SIGNAL(currentIndexChanged(int)), SLOT(onDataBitsChanged(int)));
    connect(stopBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onStopBitsChanged(int)));
    connect(timeoutValue, SIGNAL(valueChanged(int)), SLOT(onTimeoutChanged(int)));
    /*connect(ui->queryModeBox, SIGNAL(currentIndexChanged(int)), SLOT(onQueryModeChanged(int)));*/

    connect(portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
    connect(openClosePortButton, SIGNAL(clicked()), SLOT(onOpenCloseButtonClicked()));
    connect(sendButton, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
    connect(timer, SIGNAL(timeout()), SLOT(onReadyRead()));
    connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));

   // connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));

    connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
    connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
}

