#ifndef MESH_H
#define MESH_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QString>
#include <QMatrix4x4>

struct Vertex   {
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
    Vertex(QVector3D pos)   {
        position = pos;
        texCoord = QVector2D(0.0f,1.0f);
    }
    Vertex(QVector3D pos, QVector2D tex)   {
        position = pos;
        texCoord = tex;
    }
    Vertex(QVector3D pos, QVector2D tex, QVector3D nor)   {
        position = pos;
        texCoord = tex;
        normal = nor;
    }
};

struct VPoint   {
    int vertex;
    int texture;
    int normal;
};

struct VPolygon{
    QVector<VPoint> points;
};

class mesh : protected QOpenGLFunctions
{
public:
    mesh(QString meshName, QString textureName);
    void Draw(QOpenGLShaderProgram *program,
              QMatrix4x4 matrix);
    ~mesh();

    QOpenGLTexture *texture;

private:
    void LoadModelFromFile(QString fileName);
    void LoadTextureFromFile(QString fileName);
    void SaveToFile(QString fileName, QVector<Vertex> Vertexes);

    int m_index_count;
    int m_vertex_count;

    QOpenGLBuffer OGL_vertex;
    QOpenGLBuffer OGL_index;
};

#endif // MESH_H
