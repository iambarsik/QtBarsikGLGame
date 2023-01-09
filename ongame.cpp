#include "widget.h"

void Widget::OnGame()   {

    OnTouch();
        // main app logic in timer period 15ms (edit in defines.h)

    float step = 0.5f;
    float angle = 0.025f;

    if(myKey[BUTTON_UP])    { vCamera += (step * vFront); }
    if(myKey[BUTTON_DOWN])  { vCamera -= (step * vFront); }
    if(myKey[BUTTON_RIGHT]) { vCamera += QVector3D::crossProduct(vFront, vUp) * step; }
    if(myKey[BUTTON_LEFT])  { vCamera -= QVector3D::crossProduct(vFront, vUp) * step; }

    if(myKey[BUTTON_ACT5])  { move_y -= step; }
    if(myKey[BUTTON_ACT6])  { move_y += step; }
    if(myKey[BUTTON_ACT7])  { rotate_y += angle; }
    if(myKey[BUTTON_ACT2])  { rotate_y -= angle; }
    if(myKey[BUTTON_ACT1])  { rotate_x += angle; }
    if(myKey[BUTTON_ACT3])  { rotate_x -= angle; }

    rotationAxis.setX(rotate_x);
    rotationAxis.setY(rotate_y);
    rotationAxis.setZ(rotate_z);
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angle) * rotation;

    translationAxis.setX(vCamera.x());
    translationAxis.setY(vCamera.y());
    translationAxis.setZ(vCamera.z());

        // moving camera with mouse
    QPoint position;
    position = QCursor::pos();
    rotate_y -= (position.x() - this->width()/2)/1000.0f;
    rotate_x -= (position.y() - this->height()/2)/1000.0f;
    QCursor::setPos(this->width()/2,this->height()/2);

    update();
}
