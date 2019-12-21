// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "glwidget.h"

#include <iostream>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "RenderableData.h"
#include "Renderable.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QApplication>
#include <QOpenGLContext>

namespace {
const float lengthAxis = 10000.0f;
const float radiusAxis = 1.0f;
const float lengthOrientation = 100.0f;
const float radiusOrientation = 1.0f;
}

GLWidget::GLWidget(QWidget *parent)
  : QOpenGLWidget(parent),
    m_q1(0),
    m_q2(0),
    m_q3(0),
    m_q4(0),
    m_grid(nullptr),
    m_xAxis(nullptr),
    m_yAxis(nullptr),
    m_zAxis(nullptr),
    m_orientationX(nullptr),
    m_orientationY(nullptr),
    m_orientationZ(nullptr),
    m_gridData(nullptr),
    m_xAxisData(nullptr),
    m_yAxisData(nullptr),
    m_zAxisData(nullptr),
    m_xOrientationData(nullptr),
    m_yOrientationData(nullptr),
    m_zOrientationData(nullptr),
    m_shader(nullptr),
    m_renderer(nullptr) {


  QSurfaceFormat glFormat;
  glFormat.setVersion( 3, 3 );
  glFormat.setProfile( QSurfaceFormat::OpenGLContextProfile::CoreProfile );
  glFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  setFormat(glFormat);

}

GLWidget::~GLWidget() {
  delete m_grid;

  delete m_xAxis;
  delete m_yAxis;
  delete m_zAxis;

  delete m_orientationX;
  delete m_orientationY;
  delete m_orientationZ;

  delete m_gridData;
  delete m_xAxisData;
  delete m_yAxisData;
  delete m_zAxisData;
  delete m_xOrientationData;
  delete m_yOrientationData;
  delete m_zOrientationData;
  delete m_shader;

  delete m_renderer;
}

void GLWidget::initializeGL() {
  // Initialise GLFW
  if( !glfwInit() ) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
  }

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if(glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
  }

  GLCall( glClearColor(1.0f, 1.0f, 1.0f, 1.0f) );
  GLCall( glEnable(GL_DEPTH_TEST) );
  GLCall( glDepthFunc(GL_LESS) );
  GLCall( glEnable(GL_BLEND) );
  GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

  static const GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  m_grid = new Grid();
  m_xAxis = new Cylinder(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), lengthAxis, radiusAxis);
  m_yAxis = new Cylinder(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), lengthAxis, radiusAxis);
  m_zAxis = new Cylinder(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), lengthAxis, radiusAxis);
  m_orientationX = new Cylinder(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), lengthOrientation, radiusOrientation);
  m_orientationY = new Cylinder(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), lengthOrientation, radiusOrientation);
  m_orientationZ = new Cylinder(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), lengthOrientation, radiusOrientation);

  m_gridData = new RenderableData(*m_grid);
  m_xAxisData = new RenderableData(*m_xAxis);
  m_yAxisData = new RenderableData(*m_yAxis);;
  m_zAxisData = new RenderableData(*m_zAxis);
  m_xOrientationData = new RenderableData(*m_orientationX);
  m_yOrientationData = new RenderableData(*m_orientationY);
  m_zOrientationData = new RenderableData(*m_orientationZ);

  m_shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "open-gl/gl-renderer/res/shaders/basicshader.vert");
  m_shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "open-gl/gl-renderer/res/shaders/basicshader.frag");
  m_shaderProgram.link();

  if (!m_shaderProgram.log().isEmpty()) {
    qDebug() << m_shaderProgram.log();
  }

  m_renderer = new Renderer();
}

void GLWidget::paintGL() {

  float screenWidth = 1000.0f;
  float screenHeight = 1000.0f;

  float halfScreenWidth = screenWidth/2;
  float halfScreenHeight = screenHeight/2;

  QMatrix4x4 Proj;
  Proj.perspective(glm::radians(2000.0f), (float) halfScreenWidth / (float)halfScreenHeight, 0.1f, 50000.0f);

  QMatrix4x4 View = QMatrix4x4();

  QMatrix4x4 orientationModel = QMatrix4x4();
  QMatrix4x4 sceneModel = QMatrix4x4();

  QMatrix4x4 MVPOrientation = QMatrix4x4();
  QMatrix4x4 MVPScene = QMatrix4x4();

  QVector3D position(0.0f, 0.0f, 0.0f);

  QQuaternion currentOrientation = QQuaternion(1,0,0,0);

  currentOrientation = QQuaternion(m_q1,m_q2,m_q3,m_q4);
  orientationModel = QMatrix4x4(currentOrientation.toRotationMatrix());;

  QVector3D whereCameraIsLookingFrom = position + QVector3D(130.f,100.f,350.f);
  QVector3D whatCameraIsLookingAt = position;
  QVector3D theDirectionTheCameraShouldConsiderAsUp = QVector3D(0.f,1.f,0.f);
  View.lookAt(
        whereCameraIsLookingFrom,
        whatCameraIsLookingAt,
        theDirectionTheCameraShouldConsiderAsUp);

  QMatrix4x4 projectionView = Proj * View;

  MVPOrientation = projectionView * orientationModel;

  m_renderer->Clear();

  m_shaderProgram.setUniformValueArray("u_MVP", &MVPOrientation, 1);
  m_shaderProgram.bind();
  m_renderer->Draw(*m_xOrientationData, GL_TRIANGLE_STRIP);
  m_renderer->Draw(*m_yOrientationData, GL_TRIANGLE_STRIP);
  m_renderer->Draw(*m_zOrientationData, GL_TRIANGLE_STRIP);

  MVPScene = projectionView * sceneModel;
  m_shaderProgram.setUniformValueArray("u_MVP", &MVPScene, 1);
  m_shaderProgram.bind();
  m_renderer->Draw(*m_xAxisData, GL_TRIANGLE_STRIP);
  m_renderer->Draw(*m_yAxisData, GL_TRIANGLE_STRIP);
  m_renderer->Draw(*m_zAxisData, GL_TRIANGLE_STRIP);
  m_renderer->Draw(*m_gridData , GL_LINES);

}

void GLWidget::resizeGL(int width, int height) {
  int side = qMin(width, height);
  glViewport((width - side) / 2, (height - side) / 2, side, side);
  this->resize(side,side);
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  glOrtho(-2, +2, -2, +2, 1.0, 15.0);
//  glMatrixMode(GL_MODELVIEW);
}

#include <QTextCodec>
void GLWidget::dataReceived(QByteArray data) {

  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QString string = codec->toUnicode(data);

  QStringList query;
  query << string.split(" ");

  if (query.size() == 4) {
    m_q1 = query[0].toFloat();
    m_q2 = query[1].toFloat();
    m_q3 = query[2].toFloat();
    m_q4 = query[3].toFloat();
  }

}

//#include <QResizeEvent>
//#include <QLayout>
//void GLWidget::resizeGL(QResizeEvent *parentEvent) {
//  this->resize(parentEvent->size().height(), parentEvent->size().height());
//}
