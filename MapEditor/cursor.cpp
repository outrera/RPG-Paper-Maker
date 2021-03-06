/*
    RPG Paper Maker Copyright (C) 2017-2018 Marie Laporte

    This file is part of RPG Paper Maker.

    RPG Paper Maker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RPG Paper Maker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cursor.h"
#include <QtMath>
#include <QDateTime>
#include "rpm.h"
#include "floors.h"
#include "keyboardenginekind.h"

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

Cursor::Cursor(QVector3D* position) :
    m_positionSquare(position),
    m_positionReal(*position),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_frameDuration(250),
    m_frameNumber(4)
{

}

Cursor::~Cursor()
{
    delete m_program;
    delete m_texture;
}

void Cursor::initializeSquareSize(int s){
    m_squareSize = s;
}

int Cursor::getSquareX() const{
    return (int)((m_positionSquare->x() + 1) / m_squareSize);
}

int Cursor::getSquareY() const{
    return (int)((m_positionSquare->y() + 1) / m_squareSize);
}

int Cursor::getSquareZ() const{
    return (int)((m_positionSquare->z() + 1) / m_squareSize);
}

void Cursor::setX(int x, bool withReal){
    m_positionSquare->setX(x * m_squareSize);
    if (withReal)
        m_positionReal.setX(x * m_squareSize);
}

void Cursor::setY(int){

}

void Cursor::setYplus(int){

}

void Cursor::setZ(int z, bool withReal){
    m_positionSquare->setZ(z * m_squareSize);
    if (withReal)
        m_positionReal.setZ(z * m_squareSize);
}

void Cursor::setPositions(Position3D& position){
    setX(position.x());
    setY(position.y());
    setYplus(position.yPlus());
    setZ(position.z());
}

float Cursor::getX() const { return m_positionSquare->x(); }

float Cursor::getY() const { return m_positionSquare->y(); }

float Cursor::getZ() const { return m_positionSquare->z(); }

void Cursor::setFrameDuration(int i) { m_frameDuration = i; }

void Cursor::setFrameNumber(int i) { m_frameNumber = i; }

QVector3D* Cursor::position() const { return m_positionSquare; }

// -------------------------------------------------------
//
//  INTERMEDIARY FUNCTIONS
//
// -------------------------------------------------------

Portion Cursor::getPortion() const{
    return Portion(getSquareX() / RPM::portionSize,
                   getSquareY()/ RPM::portionSize,
                   getSquareZ() / RPM::portionSize);
}

// -------------------------------------------------------

void Cursor::loadTexture(QString path){
    m_texture = new QOpenGLTexture(QImage(path));
    m_texture->setMinificationFilter(QOpenGLTexture::Filter::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Filter::Nearest);
}

// -------------------------------------------------------

void Cursor::updatePositionSquare(){
    setX((int)(m_positionReal.x()) / m_squareSize, false);
    setY((int)(m_positionReal.y()) / m_squareSize);
    setZ((int)(m_positionReal.z()) / m_squareSize, false);
}

// ------------------------------------------------------

void Cursor::centerInSquare(int offset) {
    m_positionReal.setX(((int)((m_positionReal.x()) / m_squareSize) *
                        m_squareSize) + (m_squareSize / 2) - offset);
    m_positionReal.setZ(((int)((m_positionReal.z()) / m_squareSize) *
                        m_squareSize) + (m_squareSize / 2) - 1);
}

// ------------------------------------------------------

void Cursor::onKeyPressed(int key, double angle, int w, int h, double speed){
    int xPlus, zPlus, res;
    KeyBoardDatas* keyBoardDatas = RPM::get()->engineSettings()
            ->keyBoardDatas();
    w *= m_squareSize;
    h *= m_squareSize;

    if (speed == -1)
        speed = m_squareSize;

    if (keyBoardDatas->isEqual(key, KeyBoardEngineKind::MoveCursorUp)){
        xPlus = (int)(speed * (qRound(qCos(angle * M_PI / 180.0))));
        zPlus = (int)(speed * (qRound(qSin(angle * M_PI / 180.0))));
        res = m_positionReal.z() + zPlus;
        if (res >= 0 && res < h)
            m_positionReal.setZ(res);
        res = m_positionReal.x() + xPlus;
        if (res >= 0 && res < w)
            m_positionReal.setX(res);
    }
    else if (keyBoardDatas->isEqual(key, KeyBoardEngineKind::MoveCursorDown)){
        xPlus = (int)(speed * (qRound(qCos(angle * M_PI / 180.0))));
        zPlus = (int)(speed * (qRound(qSin(angle * M_PI / 180.0))));
        res = m_positionReal.z() - zPlus;
        if (res >= 0 && res < h)
            m_positionReal.setZ(res);
        res = m_positionReal.x() - xPlus;
        if (res >= 0 && res < w)
            m_positionReal.setX(res);
    }
    else if (keyBoardDatas->isEqual(key, KeyBoardEngineKind::MoveCursorLeft)){
        xPlus = (int)(speed *
                      (qRound(qCos((angle - 90) * M_PI / 180.0))));
        zPlus = (int)(speed *
                      (qRound(qSin((angle - 90) * M_PI / 180.0))));
        res = m_positionReal.x() + xPlus;
        if (res >= 0 && res < w)
            m_positionReal.setX(res);
        res = m_positionReal.z() + zPlus;
        if (res >= 0 && res < h)
            m_positionReal.setZ(res);
    }
    else if (keyBoardDatas->isEqual(key, KeyBoardEngineKind::MoveCursorRight)){
        xPlus = (int)(speed *
                      (qRound(qCos((angle - 90) * M_PI / 180.0))));
        zPlus = (int)(speed *
                      (qRound(qSin((angle - 90) * M_PI / 180.0))));
        res = m_positionReal.x() - xPlus;
        if (res >= 0 && res < w)
            m_positionReal.setX(res);
        res = m_positionReal.z() - zPlus;
        if (res >= 0 && res < h)
            m_positionReal.setZ(res);
    }
}

// -------------------------------------------------------

void Cursor::initialize(){
    initializeVertices();
    initializeGL();
}

// -------------------------------------------------------

void Cursor::initializeVertices(){

    QVector3D pos(0.0f, 0.15f, 0.0f);
    float w = 1.0f / m_frameNumber;
    float h = 1.0f;

    // Vertices
    m_vertices.clear();
    m_vertices.append(Vertex(Lands::verticesQuad[0] * m_squareSize + pos,
                      QVector2D(0.0f, 0.0f)));
    m_vertices.append(Vertex(Lands::verticesQuad[1] * m_squareSize + pos,
                      QVector2D(w, 0.0f)));
    m_vertices.append(Vertex(Lands::verticesQuad[2] * m_squareSize + pos,
                      QVector2D(w, h)));
    m_vertices.append(Vertex(Lands::verticesQuad[3] * m_squareSize + pos,
                      QVector2D(0.0f, h)));

    // indexes
    m_indexes.clear();
    for (int i = 0; i < Lands::nbIndexesQuad; i++)
        m_indexes.append(Lands::indexesQuad[i]);
}

// -------------------------------------------------------

void Cursor::initializeGL(){
    initializeOpenGLFunctions();

    // Create Shader
    m_program = Map::createProgram("cursor");

    // Uniform location of camera
    u_modelviewProjection = m_program->uniformLocation("modelviewProjection");
    u_cursorPosition = m_program->uniformLocation("cursorPosition");
    u_frameTex = m_program->uniformLocation("frameTex");

    // Create Buffer (Do not release until VAO is created)
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.allocate(m_vertices.constData(), Lands::nbVerticesQuad *
                            sizeof(Vertex));

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.allocate(m_indexes.constData(), Lands::nbIndexesQuad *
                           sizeof(GLuint));

    // Create Vertex Array Object
    m_vao.create();
    m_vao.bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(),
                                        Vertex::positionTupleSize,
                                        Vertex::stride());
    m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::texOffset(),
                                        Vertex::texCoupleSize,
                                        Vertex::stride());

    // Release
    m_vao.release();
    m_indexBuffer.release();
    m_vertexBuffer.release();
    m_program->release();
}

// -------------------------------------------------------

void Cursor::paintGL(QMatrix4x4 &modelviewProjection){

    // Calculating frame
    int totalTime = QTime::currentTime().msecsSinceStartOfDay() %
            (m_frameDuration * m_frameNumber);
    int frame = totalTime / m_frameDuration;

    m_program->bind();

    // Uniforms
    m_program->setUniformValue(u_modelviewProjection, modelviewProjection);
    m_program->setUniformValue(u_cursorPosition, *m_positionSquare);
    m_program->setUniformValue(u_frameTex, frame / (float)(m_frameNumber));

    // Draw
    {
      m_vao.bind();
      m_texture->bind();
      m_indexBuffer.bind();
      glDrawElements(GL_TRIANGLES, Lands::nbIndexesQuad, GL_UNSIGNED_INT, 0);
      m_indexBuffer.release();
      m_vao.release();
    }
    m_program->release();
}
