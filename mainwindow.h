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
#include <QComboBox>
#include <QTextEdit>

class QextSerialPort;
class QextSerialEnumerator;

#define CR 13
#define LF 10

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void fillControls();
    void dsrChangedHandle(bool status);

    void onPortNameChanged(const QString &name);
    void onBaudRateChanged(int idx);
    void onParityChanged(int idx);
    void onDataBitsChanged(int idx);
    void onStopBitsChanged(int idx);
    void onQueryModeChanged(int idx);
    void onTimeoutChanged(int val);
    void onOpenCloseButtonClicked();
    void onSendButtonClicked();
    void onReadyRead();

    void onPortAddedOrRemoved();
/*    void handleAddPixel();
    void setRMSOnWaitPanel();
    void setTimeOnWaitPanel();
    void resetDisablePanelSize();
    void resizeEvent(QResizeEvent * );*/
private slots:
 /*   void on_addToListButton_clicked();
    void on_deleteFromListButton_clicked();
    void on_teachNetButton_clicked();
    void on_stopTeachNetButton_clicked();
    void on_startCheckButton_clicked();
    void on_clearCheckCanvasButton_clicked();
    void on_listWithShapes_itemClicked(QListWidgetItem *item);
*/

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
    QLabel *teachingERMSLabel;
    QLabel *teachingTimeLabel;
    QLabel *teachingEraLabel;
    QLabel *busyMovieLabel;

    //starttab
    QWidget *startTab;
    QLabel *startLabel;
    QVBoxLayout *startTabLayout;

    //pingtab
    QWidget *pingTab;
    QLabel *pingImageLabel;
    QVBoxLayout *pingTabLayout;

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
    QLabel *parityLabel;
    QComboBox *parityBox;
    QLabel *timeoutEnableLabel;
    QComboBox *timeoutEnableBox;
    QLabel *timeoutLabel;
    QSpinBox *timeoutValue;

    QLabel *autoBaudingLabel;
    QPushButton *autoBaudingButton;
    QLabel *openClosePortLabel;
    QPushButton *openClosePortButton;
    QFormLayout *optionsLayout;

    //starttab
    QWidget *binaryTab;

    //Port
    QTimer *timer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;

};

#endif // MAINWINDOW_H
