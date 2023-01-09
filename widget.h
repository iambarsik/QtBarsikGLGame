#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QtOpenGL>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "defines.h"
#include "classes.h"
#include "mesh.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() override;

        // engine variables
        // ================================================================================
    QRect GameField;
    QRect Resolution;
    bool bRotateScreen;
    bool bStretchHorizontal;
    double fImageX, fImageY;
    double fImageZoom;

    mySettings Settings;

    bool myKey[BUTTON_COUNT];
    double dX,dY;

    QImage DEV_TILE_IMAGE;

    TouchButton *TouchKeys[BUTTON_COUNT];
    QList <QTouchEvent::TouchPoint> listKeys;

    QString sSymbols;
    QImage imgSymbol_medium[136];
    QImage imgSymbol_small[136];
    QImage imgSymbol_micro[136];
    QImage imgSymbol_big[136];
    QImage FONT_IMAGE;

    QTimer *GameLoop;
    long long iForGame, iForAnimation;
    int dev_player;

        // engine methods
    // ================================================================================
    void SetStartValues();
    void OnKeys(int Key, bool press);
    void OnTouch();
    void OnDrawText(int x, int y, int size, QString text_eng, QString text_rus, QPainter *pnt);
    void setColor(QColor brush, QColor pen, Qt::BrushStyle style, QPainter *qScr);
    int randomBetween(int low, int high) { return (qrand() % ((high + 1) - low) + low); }
        // ================================================================================

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();

    QOpenGLShaderProgram program;
    QMatrix4x4 matrix_projection;
    QVector3D rotationAxis;
    QQuaternion rotation;

    mesh *m_sword;
    mesh *m_level;

    QVector3D translationAxis;
    QVector3D moveAxis;
    float move_x, move_y, move_z;
    float rotate_x, rotate_y, rotate_z;

    QVector3D vCamera;
    QVector3D vFront;
    QVector3D vUp;
    QVector3D vLookDir;


        // ============================= QTs methods() ===================================

    //void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;

private slots:
    void OnGame();
    void OnAnimation();
    void GamePlayFunction();

protected:
    bool event(QEvent *event) override;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
