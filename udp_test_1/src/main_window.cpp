/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/udp_test_1/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace udp_test_1 {
extern bool isRecved;
using namespace Qt;

string STR_USB_CAM = "USB CAM";

//QHostAddress ROBOT_IP = QHostAddress("192.168.0.60"); //ROBOT
//QHostAddress OPERATOR_IP = QHostAddress("192.168.0.130"); //OPERATOR
QHostAddress ROBOT_IP = QHostAddress("192.168.188.100"); //ROBOT
QHostAddress OPERATOR_IP = QHostAddress("192.168.188.253"); //OPERATOR
uint16_t ROBOT_PORT = 9999; //RX, TX
uint16_t other_PORT = 8888;
/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
    : QMainWindow(parent)
    , qnode(argc,argv)
{
    ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    setWindowIcon(QIcon(":/images/icon.png"));
    qnode.init();

    // text socket
    socket = new QUdpSocket(this);

    m_pUdpSocket = new QUdpSocket(this);

    _5ms_Timer = new QTimer(this);
    connect(_5ms_Timer, SIGNAL(timeout()),this, SLOT(showVideo()));//1
    _5ms_Timer->start(5);

    if(socket->bind(OPERATOR_IP, ROBOT_PORT, QUdpSocket::ShareAddress)){
      qDebug()<<"text message socket bind success"<<endl;
      ui.connection->setText("OK!");
      connection = true;
      connect(socket, SIGNAL(readyRead()), this, SLOT(udp_read()));
      ui.ip->setText(OPERATOR_IP.toString());
    }

    if(m_pUdpSocket->bind(OPERATOR_IP, other_PORT, QUdpSocket::ShareAddress))
    {
      qDebug()<<"cam image socket bind success"<<endl;
      connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(udp_cam_read()));
    }

    udp_image = cv::Mat::ones(240,320,CV_8UC3);

    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
}

MainWindow::~MainWindow() {
    delete _5ms_Timer;
    delete _1s_Timer;
}

/*****************************************************************************
** Functions
*****************************************************************************/
void MainWindow::udp_write(QString text){
  QByteArray packet;
  packet.append(text);
  qDebug() << "Message from: udp_write";
  socket->writeDatagram(packet, ROBOT_IP, ROBOT_PORT);
}

void MainWindow::udp_read(){
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
    socket->readDatagram(buffer.data(), buffer.size(), &ROBOT_IP, &ROBOT_PORT);
    ui.log->append("############");
    ui.log->append("ip : " + ROBOT_IP.toString());
    ui.log->append("port9999");
    ui.log->append(buffer);
    ui.log->append("############");
}

void MainWindow::udp_cam_read(){
//  QByteArray cam_buffer;
//  ROS_INFO("Image Trasfer");
//  cam_buffer.resize(m_pUdpSocket->pendingDatagramSize());
//  m_pUdpSocket->readDatagram(cam_buffer.data(), cam_buffer.size(), &ROBOT_IP, &other_PORT);
//  combineDatagram(cam_buffer);
//  cam_buffer.clear();

  QByteArray cam_buffer;
  ROS_INFO("Image Trasfer");
  cam_buffer.resize(m_pUdpSocket->pendingDatagramSize());
  m_pUdpSocket->readDatagram(cam_buffer.data(), cam_buffer.size(), &ROBOT_IP, &other_PORT);
  Decoding_Datagram(cam_buffer);
  cam_buffer.clear();
}

void MainWindow::Decoding_Datagram(QByteArray inputDatagram)
{
  std::vector<uchar> usb1_decoding(inputDatagram.begin(), inputDatagram.end());
  udp_image = imdecode(Mat(usb1_decoding),CV_LOAD_IMAGE_COLOR);
}

void MainWindow::showVideo()
{
  cv::resize(udp_image,udp_image,cv::Size(320,240));
  QImage cam_image((const unsigned char*)(udp_image.data), udp_image.cols, udp_image.rows, QImage::Format_RGB888);
  ui.robot_cam->setPixmap(QPixmap::fromImage(cam_image.rgbSwapped()));
}

void udp_test_1::MainWindow::on_pushButton_clicked(){
    switch(connection) {
        case true :
            text_data.clear();
            text_data.append("//" + ui.send_text->text());
            ui.send_text->clear();
            ui.send_log->append("Send" + text_data);
            udp_write(text_data);
            return;
        case false :
            ui.send_log->append("Connection Lost!" + text_data);
            return;
    }
}

void udp_test_1::MainWindow::on_pushButton_2_clicked(){
    ui.send_log->clear();
}

}  // namespace udp_test_1

