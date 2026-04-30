#include <qwidget.h>
#include <vector>
#include "capture_state.hpp"

class Plot;
class Knob;
class WheelBox;
class QPushButton;
class QListWidget;
class QLabel;
class QListWidgetItem;
class GrabDisplay;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow( QWidget * = NULL );

    void start();

    double amplitude() const;
    double frequency() const;
    double signalInterval() const;

Q_SIGNALS:
    void amplitudeChanged( double );
    void frequencyChanged( double );
    void verticalOffsetChanged(double);
    void signalIntervalChanged( double );
    

public Q_SLOTS:
    void createCapture();
    void captureSelected(QListWidgetItem* current, QListWidgetItem* prev);
    void saveCaptureToFile();

private:
    Knob *d_frequencyKnob;
    Knob *d_amplitudeKnob;
    Knob* d_verticalOffsetKnob;
    WheelBox *d_timerWheel;
    WheelBox *d_intervalWheel;
    QPushButton* d_stop_play_button;
    QPushButton* d_foto_actual;

    QListWidget* d_capturesList;
    QLabel* d_params_label;
    GrabDisplay* d_captures_widget;
    QPushButton* d_save_capture;

    Plot *d_plot;


    std::vector<CaptureState> captures;

};
