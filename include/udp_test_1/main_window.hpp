/**
 * @file /include/udp_test_1/main_window.hpp
 *
 * @brief Qt based gui for udp_test_1.
 *
 * @date November 2010
 **/
#ifndef udp_test_1_MAIN_WINDOW_H
#define udp_test_1_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork>
#include <QUdpSocket>
#include <QHostAddress>
#include <iostream>
#include <QDialog>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include "sensor_msgs/Image.h"
#include "image_transport/image_transport.h"
/*****************************************************************************
** Namespace
*****************************************************************************/

namespace udp_test_1 {
using namespace cv;
using namespace std;
using namespace Qt;


/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();
    void udp_write(QString text);
    void sendVideo(QByteArray img, uint16_t port);

public Q_SLOTS:
    void udp_read();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void udp_cam_read();
    void Decoding_Datagram(QByteArray inputDatagram);
    void showVideo();

private:
    bool connection = false;
	Ui::MainWindowDesign ui;
	QNode qnode;

    QUdpSocket *socket;
    QUdpSocket *m_pUdpSocket;

    QString text_data = 0;
    Mat udp_image;

    QByteArray buffer;
    QByteArray image_buffer;

    quint8 image_cnt_past=0;
    quint8 image_cnt_now=0;

    QTimer *_5ms_Timer, *_1s_Timer;
};

}  // namespace udp_test_1

#endif // udp_test_1_MAIN_WINDOW_H
