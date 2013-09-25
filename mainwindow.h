#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QStatusBar>
#include <QGridLayout>
#include <QPushButton>
#include <QListView>
#include <QListWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QTextEdit>
#include <pthread.h>
#include <QTextStream>
#include <QScrollBar>
#include <QFormLayout>
#include <QTimer>
#include <QTime>
#include <QComboBox>
#include <QTextEdit>
#include <QCheckBox>
#include "src_hex/qhexedit.h"

class QextSerialPort;
class QextSerialEnumerator;

#define CR 13
#define LF 10
#define PING_SEND_VALUE 0x01
#define PING_RECEIVE_VALUE 0x02
#define AUTOBAUDING_QUERY "\5"//\2\1"
#define AUTOBAUDING_QUERY_CHAR_ARRAY {0x05}//,0x02,0x01}
#define AUTOBAUDING_ANSWER "\6"//\1\2"
#define AUTOBAUDING_ANSWER_CHAR_ARRAY {0x06}//,0x01,0x02}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:

    void setOverwriteMode(bool mode);
    void loadFile(const QString &fileName);

    void fillControls();
    void fillStatusTab(bool status);
    void startConnection();
    void stopConnection();

    void dsrChangedHandle(bool status);
    void ctsChangedHandle(bool status);
    void setDTR();
    void setRTS();

    void onPortNameChanged(const QString &name);
    void onBaudRateChanged(int idx);
    void onFlowControlChanged(int idx);
    void onParityChanged(int idx);
    void onDataBitsChanged(int idx);
    void onStopBitsChanged(int idx);
    void onTimeoutChanged(int val);
    void onOpenCloseButtonClicked();
    void onTerminatorChanged(int index);
    void onChar1TerminatorChanged(int index);
    void onChar2TerminatorChanged(int index);

    void onStartAutoBauding();
    void autoBaudingTimerHandle();
    void onAutoBaudingCheckBoxEnable(int);
    void onPingCheckBoxEnable(int);

    void onSendButtonClicked();
    void onClearButtonClicked();

    void onSendHexButtonClicked();
    void onClearHexButtonClicked();
    void onOpenHexFileButtonCLicked();

    void onLineStatusCheck();
    void onReadyRead();

    void onPortAddedOrRemoved();

    void onPingButtonClicked();
    void onPingTimeout();
    void checkCounterAndSendNextPing();
private slots:

private:
    void createUI();
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QTimer *elapsedTimeTimer;

    QStatusBar *statusBar;

    //global
    QGroupBox *disableTab;
    QVBoxLayout* disableTabLayout;
    QGroupBox *teachingPanel;
    QLabel *autoBaudingInfoLabel;
    QLabel *busyMovieLabel;


    //starttab
    QWidget *startTab;
    QLabel *startLabel;
    QVBoxLayout *startTabLayout;

    //pingtab
    QWidget *pingTab;
    QVBoxLayout *pingTabLayout;
    QScrollBar *pingModeScrollBar;
    QTextEdit *pingConsole;
    QPushButton *pingButton;

    //consoleTab
    QWidget *textModeTab;
    QScrollBar *textModeScrollBar;
    QScrollBar *sendTextModeScrollBar;
    QTextEdit *textConsole;
    QTextEdit *sendTextConsole;
    QVBoxLayout* textModeTabLayout;
    QHBoxLayout* textModeButtonsLayout;
    QWidget *consoleButtons;
    QPushButton *sendButton;
    QPushButton *clearConsoleButton;
    QWidget *signalsPanel;
    QHBoxLayout* signalsPanelLayout;
    QPushButton *setDTRButton;
    QPushButton *setRTSButton;
    bool DTRstatus;
    bool RTSstatus;
    QPushButton *DSRButton;
    QPushButton *CTSButton;
    bool DSRstatus;
    bool CTSstatus;

    //OptionsTab
    QWidget *optionsTab;
    QLabel *portLabel;
    QComboBox *portBox;
    QLabel *baudRateLabel;
    QComboBox *baudRateBox;
    QLabel *characterFormatLabel;
    QComboBox *characterFormatBox;
    QLabel *stopBitsLabel;
    QComboBox *stopBitsBox;
    QLabel *flowControlLabel;
    QComboBox *flowControlBox;
    QLabel *terminatorLabel;
    QComboBox *terminatorBox;
    QLabel *ownTerminatorChar1Label;
    QComboBox *ownTerminatorChar1Box;
    QLabel *ownTerminatorChar2Label;
    QComboBox *ownTerminatorChar2Box;
    QString terminatorString;
    QLabel *parityLabel;
    QComboBox *parityBox;
    QLabel *timeoutEnableLabel;
    QComboBox *timeoutEnableBox;
    QLabel *timeoutLabel;
    QSpinBox *timeoutValue;

    QLabel *autoBaudingLabel;
    QPushButton *autoBaudingButton;

    QLabel *autoBaudingEnabledLabel;
    QCheckBox *autoBaudingEnabledCheckBox;
    QLabel *pingEnabledLabel;
    QCheckBox *pingEnabledCheckBox;
    bool pingEnabled;
    QTimer *autoBaudingTimeoutTimer;
    QLabel *openClosePortLabel;
    QPushButton *openClosePortButton;
    QFormLayout *optionsLayout;

    //Port info Tab
    QWidget *portinfoTab;
    QLabel *portEnabledLabel;
    QLabel *portEnabledValue;
    QLabel *setPortLabel;
    QLabel *setPortValue;
    QLabel *setBaudRateLabel;
    QLabel *setBaudRateValue;
    QLabel *setCharacterFormatLabel;
    QLabel *setCharacterFormatValue;
    QLabel *setStopBitsLabel;
    QLabel *setStopBitsValue;
    QLabel *setFlowControlLabel;
    QLabel *setFlowControlValue;

    QFormLayout *portinfoLayout;

    //binaryTab
    QWidget *binaryTab;

    QHexEdit *hexConsole;
    QHexEdit *sendHexConsole;
    QVBoxLayout* binModeTabLayout;
    QHBoxLayout* binModeButtonsLayout;
    QWidget *binButtons;
    QPushButton *sendHexButton;
    QPushButton *clearHexButton;
    QPushButton *openHexFileButton;
    QWidget *signalsHexPanel;
    QHBoxLayout* signalsHexPanelLayout;
    QPushButton *setHexDTRButton;
    QPushButton *setHexRTSButton;
    QPushButton *DSRHexButton;
    QPushButton *CTSHexButton;


    //Port
    QTimer *timer;
    QTimer *lineStatusTimer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;
    QString buferredDataFromPort;
    QString buferredData;

    //ping
    QTime pingTime;
    QTimer *pingTimeoutTimer;
    int pingCounter;
    int pingAvgTime;
    int pingAvgCounter;
    QString pingInfo;

};

#endif // MAINWINDOW_H
