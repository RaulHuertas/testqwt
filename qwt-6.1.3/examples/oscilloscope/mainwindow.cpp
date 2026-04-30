#include "mainwindow.h"
#include "plot.h"
#include "knob.h"
#include "wheelbox.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qlistwidget.h>


MainWindow::MainWindow( QWidget *parent ):
    QWidget( parent )
{
    const double intervalLength = 10.0; // seconds

    d_plot = new Plot( this );
    d_plot->setIntervalLength( intervalLength );

    d_amplitudeKnob = new Knob( "Amplitude", 0.0, 200.0, this );
    d_amplitudeKnob->setValue( 160.0 );

    d_frequencyKnob = new Knob( "Frequency [Hz]", 0.1, 20.0, this );
    d_frequencyKnob->setValue( 17.8 );

    d_intervalWheel = new WheelBox( "Displayed [s]", 1.0, 100.0, 1.0, this );
    d_intervalWheel->setValue( intervalLength );

    d_timerWheel = new WheelBox( "Sample Interval [ms]", 0.0, 20.0, 0.1, this );
    d_timerWheel->setValue( 10.0 );

    d_stop_play_button = new QPushButton(this);
    d_stop_play_button->setText("Stop/Play");
     
    //Captures
    d_foto_actual = new QPushButton(this);
    d_foto_actual->setText("Foto Actual");
    d_capturesList = new QListWidget(this); 
    d_params_label = new QLabel(this);
    d_params_label->setText("<-Select a capture from the list");
    d_captures_widget = new QWidget(this);



    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget( d_intervalWheel );
    vLayout1->addWidget( d_timerWheel );
    vLayout1->addStretch( 10 );
    vLayout1->addWidget( d_amplitudeKnob );
    vLayout1->addWidget( d_frequencyKnob );

    QVBoxLayout* vLayout2 = new QVBoxLayout();
    vLayout2->addWidget( d_stop_play_button);
    vLayout2->addWidget(d_foto_actual);

    QVBoxLayout* vLayout3 = new QVBoxLayout();
    vLayout3->addWidget(d_capturesList);

    QVBoxLayout* vLayout4 = new QVBoxLayout();
    vLayout4->addWidget(d_params_label);
    vLayout4->addWidget(d_captures_widget);


    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 10 );
    layout->addLayout( vLayout1 );
    layout->addLayout( vLayout2 );
    layout->addLayout( vLayout3 );
    layout->addLayout( vLayout4 );

    //Widgets de captura


    connect( d_amplitudeKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( amplitudeChanged( double ) ) );
    connect( d_frequencyKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( frequencyChanged( double ) ) );
    connect( d_timerWheel, SIGNAL( valueChanged( double ) ),
        SIGNAL( signalIntervalChanged( double ) ) );

    connect( d_intervalWheel, SIGNAL( valueChanged( double ) ),
        d_plot, SLOT( setIntervalLength( double ) ) );

    connect(d_stop_play_button, SIGNAL(clicked()),
        d_plot, SLOT(stop_play()));
    connect(d_foto_actual, SIGNAL(clicked()),
        this, SLOT(createCapture()));

}

void MainWindow::start()
{
    d_plot->start();
}

double MainWindow::frequency() const
{
    return d_frequencyKnob->value();
}

double MainWindow::amplitude() const
{
    return d_amplitudeKnob->value();
}

double MainWindow::signalInterval() const
{
    return d_timerWheel->value();
}

void MainWindow::createCapture() {
    CaptureState newItem;
    newItem.amplitude = d_amplitudeKnob->value();
    newItem.frequency = d_frequencyKnob->value();

   

    newItem.pixmap = new QPixmap(
        d_plot->grab(QRect(QPoint(0, 0), QSize(-1, -1)))
    );


    captures.push_back(newItem);

    d_capturesList->addItem("Item "+QString::number(d_capturesList->count()+1));
}