#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include "Grid.h"
#include "Cube.h"
#include "Orientation.h"
#include "Cylinder.h"
#include "RenderableData.h"
#include "Shader.h"
#include "Renderer.h"

#include <QOpenGLWidget>
#include <QGLFormat>
#include <QGLShaderProgram>

class GLWidget : public QOpenGLWidget
{
  Q_OBJECT
private:
  float m_q1;
  float m_q2;
  float m_q3;
  float m_q4;

  Grid* m_grid;

  Cylinder* m_xAxis;
  Cylinder* m_yAxis;
  Cylinder* m_zAxis;

  Cylinder* m_orientationX;
  Cylinder* m_orientationY;
  Cylinder* m_orientationZ;

  RenderableData* m_gridData;
  RenderableData* m_xAxisData;
  RenderableData* m_yAxisData;
  RenderableData* m_zAxisData;
  RenderableData* m_xOrientationData;
  RenderableData* m_yOrientationData;
  RenderableData* m_zOrientationData;

  Shader* m_shader;
  QGLShaderProgram m_shaderProgram;
  Renderer* m_renderer;

public:
  explicit GLWidget(QWidget *parent = nullptr);
  ~GLWidget();
protected:
  virtual void initializeGL() override;
  virtual void paintGL() override;
  virtual void resizeGL(int width, int height) override;
signals:

public slots:
  void dataReceived(QByteArray data);
  void resizeGL(QResizeEvent *event);
};

#endif
