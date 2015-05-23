#include <iostream>
#include <fstream>
#include <stdexcept>
#ifndef __APPLE__
    #include <GL/glew.h>
    #include <GL/glut.h>
#else
    #include <GLUT/glut.h>
#endif

#include "mesh.h"

using namespace std;
using qglviewer::Vec;


///////////////////////////////////////////////////////////////////////////////
Mesh::Mesh() {
    this->mesh = new BMesh();

    // We ask OpenMesh to compute vertex and face normals
    mesh->request_vertex_normals();
    mesh->request_face_normals();
}


///////////////////////////////////////////////////////////////////////////////
Mesh::~Mesh() {
    delete mesh;
}


///////////////////////////////////////////////////////////////////////////////
void Mesh::draw() {
    BMesh::ConstFaceIter fit;

    glPushMatrix();

    // We draw each face
    for (fit = mesh->faces_begin(); fit != mesh->faces_end(); ++fit) {
        BMesh::FaceHandle f = *fit;
        BMesh::ConstFaceVertexIter cvit = mesh->cfv_iter(f);

        OpenMesh::Vec3f n = mesh->normal(f);
        glBegin(GL_QUADS);
            glNormal3f(n[0], n[1], n[2]);
            for (unsigned int i = 0; i < 4; i++) {
                OpenMesh::Vec3f v = mesh->point(*cvit);
                glVertex3f(v[0], v[1], v[2]);
                ++cvit;
            }
        glEnd();
    }
    glPopMatrix();
}


///////////////////////////////////////////////////////////////////////////////
bool Mesh::subdivide() {
    OpenMesh::Subdivider::Uniform::CatmullClarkT<BMesh> cc;
    cc.attach(*mesh);
    cc(1);
    cc.detach();

    // OpenMesh automatically computes normals
    mesh->update_normals();

    return true;
}



bool Mesh::saveMesh(const string &fileName) {
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if (!file.is_open())
        return false;

    file << "# Automatically generated" << endl << endl;
    file << "o mesh" << endl;

    // Vertices
    file << "# Vertices" << endl;
    BMesh::ConstVertexIter vit;
    for (vit = mesh->vertices_begin(); vit != mesh->vertices_end(); ++vit) {
        OpenMesh::Vec3f v = mesh->point(*vit);
        file << "v " << v[0] << " " << v[1] << " " << v[2] << endl;
    }

    // Normals
    file << "# Normals" << endl;
    for (vit = mesh->vertices_begin(); vit != mesh->vertices_end(); ++vit) {
        OpenMesh::Vec3f v = mesh->normal(*vit);
        file << "vn " << v[0] << " " << v[1] << " " << v[2] << endl;
    }

    file.close();
    return true;
}

