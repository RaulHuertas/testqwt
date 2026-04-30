#include <qwidget.h>

class Plot;
class Knob;
class WheelBox;
class QPushButton;

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
    void signalIntervalChanged( double );

private:
    Knob *d_frequencyKnob;
    Knob *d_amplitudeKnob;
    WheelBox *d_timerWheel;
    WheelBox *d_intervalWheel;
    QPushButton* d_stop_play_button;
    Plot *d_plot;
};
