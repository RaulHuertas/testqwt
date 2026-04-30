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
#include "GrabDisplay.h"
#include <qfiledialog.h>
#include <qfile.h>
#include <qcolordialog.h>

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

    d_verticalOffsetKnob = new Knob("Vertical Offset", -100, 100, this);
    d_verticalOffsetKnob->setValue(0);

    d_intervalWheel = new WheelBox( "Displayed [s]", 1.0, 100.0, 1.0, this );
    d_intervalWheel->setValue( intervalLength );

    d_timerWheel = new WheelBox( "Sample Interval [ms]", 0.0, 20.0, 0.1, this );
    d_timerWheel->setValue( 10.0 );

    d_stop_play_button = new QPushButton(this);
    d_stop_play_button->setText("Stop/Play");
    d_change_color = new QPushButton(this);
    d_change_color->setText("Cambiar color");
     
    //Captures
    d_foto_actual = new QPushButton(this);
    d_foto_actual->setText("Foto Actual");
    d_capturesList = new QListWidget(this); 
    d_capturesList->setSelectionMode(QAbstractItemView::SingleSelection);
    d_params_label = new QLabel(this);
    d_params_label->setText("<-Select a capture from the list");
    d_captures_widget = new GrabDisplay(this);
    d_captures_widget->setMinimumSize(QSize(100,100));
    d_save_capture = new QPushButton(this);
    d_save_capture->setText("Salvar en...");

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget( d_intervalWheel );
    vLayout1->addWidget( d_timerWheel );
    vLayout1->addStretch( 10 );
    vLayout1->addWidget( d_amplitudeKnob );
    vLayout1->addWidget( d_frequencyKnob );
    vLayout1->addWidget( d_verticalOffsetKnob );
    vLayout1->addWidget( d_change_color );

    QVBoxLayout* vLayout2 = new QVBoxLayout();
    vLayout2->addWidget( d_stop_play_button);
    vLayout2->addWidget(d_foto_actual);

    QVBoxLayout* vLayout3 = new QVBoxLayout();
    vLayout3->addWidget(d_capturesList);

    QVBoxLayout* vLayout4 = new QVBoxLayout();
    vLayout4->addWidget(d_params_label);
    vLayout4->addWidget(d_captures_widget);
    vLayout4->addWidget(d_save_capture);


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
    connect(d_verticalOffsetKnob, SIGNAL(valueChanged(double)),
        SIGNAL(verticalOffsetChanged(double)));

    connect( d_timerWheel, SIGNAL( valueChanged( double ) ),
        SIGNAL( signalIntervalChanged( double ) ) );

    connect( d_intervalWheel, SIGNAL( valueChanged( double ) ),
        d_plot, SLOT( setIntervalLength( double ) ) );

    connect(d_stop_play_button, SIGNAL(clicked()),
        d_plot, SLOT(stop_play()));
    connect(d_foto_actual, SIGNAL(clicked()),
        this, SLOT(createCapture()));

    connect( 
        d_capturesList,
        SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
        this, 
        SLOT(captureSelected(QListWidgetItem*, QListWidgetItem*))
    );
    connect(d_save_capture, SIGNAL(clicked()),
        this, SLOT(saveCaptureToFile()));
    connect(d_change_color, SIGNAL(clicked()),
        this, SLOT(changeColor()));

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

void MainWindow::captureSelected(QListWidgetItem* current, QListWidgetItem* prev) {
    qDebug() << "Item selected";
    int index = -1;
    for (int j = 0; j < captures.size();j++) {
        if (d_capturesList->item(j) == current) {
            index = j;
            break;
        }
    }
    if (index < 0) {
        return;
    }
    
    const CaptureState& item = captures[index];
    d_params_label->setText(
        "Item "+QString::number(index+1)+", "+
        "Amplitude: "+ QString::number(item.amplitude) + ", "
        "Frequency: " + QString::number(item.frequency) 
    );
    d_captures_widget->setPixmap(item.pixmap);
    d_captures_widget->update();
}

void MainWindow::saveCaptureToFile() {
    int index = -1;
    for (int j = 0; j < captures.size(); j++) {
        if (d_capturesList->item(j) == d_capturesList->currentItem()) {
            index = j;
            break;
        }
    }
    if (index < 0) {
        return;
    }

    QString dst = QFileDialog::getSaveFileName(this, tr("Save File"),
        "im"+QString::number(index)+".png",
        tr("Images (*.png)"));
    if (dst.size() == 0) {
        return;
    }
    if (dst.isNull()) {
        return;
    } if (dst.isEmpty()) {
        return;
    }
    const CaptureState& item = captures[index];
    QFile file(dst);
    file.open(QIODevice::WriteOnly);
    item.pixmap->save(&file, "PNG");


}

void MainWindow::changeColor() {
    qDebug() << "a";
    QColor new_color = QColorDialog::getColor();
    qDebug() << "b";
    if (!new_color.isValid()) {
        return;
    }
    qDebug() << "c";
    d_plot->set_plot_color(new_color);
}