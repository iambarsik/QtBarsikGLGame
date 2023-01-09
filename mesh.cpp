#include "mesh.h"
#include <QFile>

mesh::mesh(QString meshName, QString textureName)
    : texture(nullptr)
    , m_index_count(0)
    , m_vertex_count(0)
    , OGL_vertex(QOpenGLBuffer::VertexBuffer)
    , OGL_index(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    OGL_vertex.create();
    OGL_index.create();
    LoadModelFromFile(meshName);
    LoadTextureFromFile(textureName);
}

void mesh::Draw(QOpenGLShaderProgram *program, QMatrix4x4 matrix)   {

    texture->bind();
    program->setUniformValue("mvp_matrix", matrix);
    program->setUniformValue("texture", 0);

    OGL_vertex.bind();
    OGL_index.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(Vertex));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    float r = 200.0;
    float g = 200.0;
    float b = 200.0;

    program->setUniformValue("red", r);
    program->setUniformValue("green", g);
    program->setUniformValue("blue", b);
    program->setUniformValue("isTextured", true);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(Vertex));

    // Try put normals
    offset += sizeof(QVector2D);
    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 2, sizeof(Vertex));

    glDrawElements(GL_TRIANGLE_STRIP, m_index_count, GL_UNSIGNED_SHORT, 0);
}

mesh::~mesh()   {
    OGL_vertex.destroy();
    OGL_index.destroy();
}

void mesh::LoadModelFromFile(QString fileName)  {
    QVector3D buff3D;
    QVector2D buff2D;

    QVector<VPolygon>  Pols;
    QVector<Vertex>    Vertexes;
    QVector<GLushort>  indices;
    QVector<QVector3D> v_position;
    QVector<QVector3D> v_normal;
    QVector<QVector2D> v_texture;

    bool bTexture = false;
    bool bNormale = false;

    if(!fileName.isEmpty())     {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))   {
            QTextStream fileText(&file);
            while (!fileText.atEnd())               {
                QString fileLine = fileText.readLine();
                if(fileLine.startsWith("vn "))  {
                    QStringList lineList = fileLine.split(" ");
                    buff3D.setX( lineList[1].toFloat() );
                    buff3D.setY( lineList[2].toFloat() );
                    buff3D.setZ( lineList[3].toFloat() );
                    v_normal.push_back(buff3D);
                    bNormale = true;
                }
                else if(fileLine.startsWith("vt ")) {
                    QStringList lineList = fileLine.split(" ");
                    buff2D.setX( lineList[1].toFloat() );
                    buff2D.setY( lineList[2].toFloat() );
                    v_texture.push_back(buff2D);
                    bTexture = true;
                }
                else if(fileLine.startsWith("v "))
                {
                    QStringList lineList = fileLine.split(" ");
                    buff3D.setX( lineList[1].toFloat() );
                    buff3D.setY( lineList[2].toFloat() );
                    buff3D.setZ( lineList[3].toFloat() );
                    v_position.push_back(buff3D);
                }
                else if(fileLine.startsWith("f "))  {
                    if(bTexture)  {
                        QStringList lineList = fileLine.split(" ");
                        int f[10] = { 0 };
                        int t[10] = { 0 };
                        int n[10] = { 0 };
                        int size = 0;
                        for(int i = 1; i < lineList.size(); i++) {
                            QStringList arg = lineList[i].split("/");
                            size++;
                            f[i - 1] = arg[0].toInt() - 1;
                            t[i - 1] = arg[1].toInt() - 1;
                            if(bNormale)    {
                                n[i - 1] = arg[2].toInt() - 1;
                            }
                        }
                        QVector<VPoint> p;
                        p.push_back({f[0],t[0],n[0]});
                        p.push_back({f[1],t[1],n[1]});
                        p.push_back({f[2],t[2],n[2]});
                        if(size > 3)    {
                            p.push_back({f[3],t[3],n[3]});
                        }
                        Pols.push_back({p});
                    }
                    if(bNormale == false && bTexture == false) {
                        QStringList lineList = fileLine.split(" ");
                        int f[3];
                        f[0] = lineList[1].toInt();
                        f[1] = lineList[2].toInt();
                        f[2] = lineList[3].toInt();

                        indices.push_back(f[0] - 1);
                        indices.push_back(f[0] - 1);
                        indices.push_back(f[1] - 1);
                        indices.push_back(f[2] - 1);
                        indices.push_back(f[2] - 1);
                    }
                }
            }
        }
        file.close();
    }
    if(bTexture == false) {
        for(int i = 0; i < v_position.size(); i++)  {
            Vertex v(v_position[i]);
            Vertexes.push_back(v);
        }
    } else {
        int all_vert = 0;
        for(int i = 0; i < Pols.size(); i++)  {
            int f[10] = { 0 };
            int t[10] = { 0 };
            int n[10] = { 0 };
            int ind[10] = { 0 };
            int size = 0;
            for(int j = 0; j < Pols[i].points.size(); j++)  {
                size++;
                f[j] = Pols[i].points[j].vertex;
                t[j] = Pols[i].points[j].texture;
                ind[j] = all_vert;
                if(bNormale)    {
                    n[j] = Pols[i].points[j].normal;
                    Vertex v(v_position[Pols[i].points[j].vertex],
                            v_texture[Pols[i].points[j].texture],
                            v_normal[Pols[i].points[j].normal]);
                    Vertexes.push_back(v);
                } else {
                    Vertex v(v_position[Pols[i].points[j].vertex],v_texture[Pols[i].points[j].texture]);
                    Vertexes.push_back(v);
                }
                all_vert++;
            }
            indices.push_back(ind[0]);
            indices.push_back(ind[0]);
            indices.push_back(ind[1]);
            indices.push_back(ind[2]);
            indices.push_back(ind[2]);
            if(size > 3)    {
                indices.push_back(ind[0]);
                indices.push_back(ind[0]);
                indices.push_back(ind[2]);
                indices.push_back(ind[3]);
                indices.push_back(ind[3]);
            }
        }
    }

    /*
    for(int i = 0; i < Vertexes.size(); i++)    {
        qDebug() << Vertexes[i].position << Vertexes[i].texCoord << "\n";
    }
    */

    // Transfer vertex data to VBO 0
    OGL_vertex.bind();
    OGL_vertex.allocate(Vertexes.data(), Vertexes.size()  * sizeof(Vertex));
    m_vertex_count = Vertexes.size();

    // Transfer index data to VBO 1
    OGL_index.bind();
    OGL_index.allocate(indices.data(), indices.size() * sizeof(GLushort));
    m_index_count = indices.size();

    //SaveToFile(QString("data.txt"), Vertexes);
}

void mesh::LoadTextureFromFile(QString fileName)    {
    texture = new QOpenGLTexture(QImage(fileName).mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void mesh::SaveToFile(QString fileName, QVector<Vertex> Vertexes)
{
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    QTextStream out(&file);
    for(int i = 0; i < Vertexes.size(); i++)    {
        Vertex *v = &Vertexes[i];
        float x = v->position.x();
        float y = v->position.y();
        float z = v->position.z();
        float u = v->texCoord.x();
        float w = v->texCoord.y();
        out << x << " " << y << " "  << z << " "  << u << " "  << w << "\n";
    }
    file.close();
}
