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
#include <QThread>
#include <QFormLayout>
#include <QTimer>
#include <QTime>
#include <QComboBox>
#include <QTextEdit>

class QextSerialPort;
class QextSerialEnumerator;

#define CR 13
#define LF 10
#define PING_SEND_VALUE 0x01
#define PING_RECEIVE_VALUE 0x02
#define AUTOBAUDING_QUERY "\1\2\2\1"
#define AUTOBAUDING_QUERY_CHAR_ARRAY {0x01,0x02,0x02,0x01}
#define AUTOBAUDING_ANSWER "\2\1\1\2"
#define AUTOBAUDING_ANSWER_CHAR_ARRAY {0x02,0x01,0x01,0x02}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void fillControls();
    void fillStatusTab(bool status);
    void dsrChangedHandle(bool status);
    void setDTR();
    void setRTS();

    void onPortNameChanged(const QString &name);
    void onBaudRateChanged(int idx);
    void onParityChanged(int idx);
    void onDataBitsChanged(int idx);
    void onStopBitsChanged(int idx);
    void onTimeoutChanged(int val);
    void onOpenCloseButtonClicked();
    void onStartAutoBauding();
    void autoBaudingTimerHandle();

    void onSendButtonClicked();

    void onReadyRead();

    void onPortAddedOrRemoved();

    void onPingButtonClicked();
    void onPingTimeout();
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
    QWidget *signalsPanel;
    QHBoxLayout* signalsPanelLayout;
    QPushButton *setDTRButton;
    QPushButton *setRTSButton;
    bool DTRstatus=false;
    bool RTSstatus=false;
    QPushButton *DSRButton;
    QPushButton *CTSButton;

    void teachConsoleAddText(QString text);
    void teachConsoleAddOnlyPlainText(QString text);

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
    QLabel *ownTerminatorLabel;
    QTextEdit *ownTerminatorValue;
    QString terminatorString="";
    QLabel *parityLabel;
    QComboBox *parityBox;
    QLabel *timeoutEnableLabel;
    QComboBox *timeoutEnableBox;
    QLabel *timeoutLabel;
    QSpinBox *timeoutValue;

    QLabel *autoBaudingLabel;
    QPushButton *autoBaudingButton;
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
    /*QLabel *terminatorLabel;
    QComboBox *terminatorBox;
    QLabel *ownTerminatorLabel;
    QTextEdit *ownTerminatorValue;
    QLabel *parityLabel;
    QComboBox *parityBox;
    QLabel *timeoutEnableLabel;
    QComboBox *timeoutEnableBox;
    QLabel *timeoutLabel;
    QSpinBox *timeoutValue;

    QLabel *autoBaudingLabel;
    QPushButton *autoBaudingButton;
    QLabel *openClosePortLabel;
    QPushButton *openClosePortButton;*/
    QFormLayout *portinfoLayout;

    //starttab
    QWidget *binaryTab;

    //Port
    QTimer *timer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;

    //ping
    QTime pingTime;
    QTimer *pingTimeoutTimer;
    int pingCounter;
    /*
time.start();
time.elapsed();
*/

};

#endif // MAINWINDOW_H
