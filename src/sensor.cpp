//set(GL_DEP gl glu glew OpenNI2);
#include "sensor.h"
#include <iostream>

 CSensor::~CSensor(){
     m_colorStream.stop();
     m_depthStream.stop();
     m_colorStream.destroy();
     m_depthStream.destroy();
     openni::OpenNI::shutdown();
 }

 //obligatoire
 CSensor::CSensor(){

 }

bool CSensor::OnInit(bool show_color_stream){
  if (openni::OpenNI::initialize() != openni::STATUS_OK) {
      std::cout<<openni::OpenNI::getExtendedError()<<std::endl;
      return false;
  }
  //quand on a que une device, on fait ANY_DEVICE
  if (m_device.open(openni::ANY_DEVICE) != openni::STATUS_OK) {
      std::cout<<openni::OpenNI::getExtendedError()<<std::endl;
      return false;
  }

  //décider si le capteur supporter recalage., sinon, deux images à la même temps
  //isRegModeSupp().
  //espace.


  if (! (m_colorStream.create(m_device, openni::SENSOR_COLOR) == openni::STATUS_OK)){
      std::cout<<openni::OpenNI::getExtendedError()<<std::endl;
      return false;

  }
  if (! ( m_colorStream.start() == openni::STATUS_OK)){
      std::cout<<openni::OpenNI::getExtendedError()<<std::endl;
      return false;

  }
  if (!m_colorStream.isValid()) return false;

  if (! (m_depthStream.create(m_device, openni::SENSOR_DEPTH) == openni::STATUS_OK)){
      std::cout<<openni::OpenNI::getExtendedError()<<std::endl;
      return false;

  }
  if (! (m_depthStream.start() == openni::STATUS_OK)){
      std::cout<<openni::OpenNI::getExtendedError()<<std::endl;
      return false;

  }
  if (!m_depthStream.isValid()) return false;


  if (show_color_stream){ active_stream = color_stream;}
  else{ active_stream = depth_stream;}
  //examiner si registration est supporté
  if(m_device.isImageRegistrationModeSupported(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR)){
  if (m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR) != openni::STATUS_OK)
      return false;
  }
  //temps. Synchroniser les deux flux, ainsi le capteur va essayer de faire les deux à la même temps.
 // ne synchroniser que en affichage de profondeur. faut faire un méthode qui swtich entre unsyn & syn
  if (m_device.setDepthColorSyncEnabled(! show_color_stream) != openni::STATUS_OK)
      return false;
  return  true;
}
