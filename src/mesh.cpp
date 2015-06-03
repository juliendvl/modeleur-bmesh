#include <iostream>
#include <vector>
#include <fstream>
#ifndef __APPLE__
    #include <GL/glew.h>
    #include <GL/glut.h>
#else
    #include <GLUT/glut.h>
#endif

#include "mesh.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
Mesh::Mesh() {
    this->mesh = new BMesh();

    // We ask OpenMesh to compute vertex and face normals
    mesh->request_vertex_normals();
    mesh->request_face_normals();
}


///////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(const Mesh &m) {
    this->mesh = m.mesh;

    mesh->request_vertex_normals();
    mesh->request_face_normals();
}

///////////////////////////////////////////////////////////////////////////////
Mesh::~Mesh() {
    delete mesh;
}


///////////////////////////////////////////////////////////////////////////////
BMesh& Mesh::getMesh() {
    return *mesh;
}


///////////////////////////////////////////////////////////////////////////////
BMesh* Mesh::getPMesh() {
    return mesh;
}


///////////////////////////////////////////////////////////////////////////////
void Mesh::draw() {
    BMesh::ConstFaceIter fit;

    glPushMatrix();

    // We draw each face
    for (fit = mesh->faces_begin(); fit != mesh->faces_end(); ++fit) {
        BMesh::FaceHandle f = *fit;
        BMesh::ConstFaceVertexIter cvit = mesh->cfv_iter(f);

        glBegin(GL_QUADS);
            glColor3f(0.0,1.0,1.0);
            for (unsigned int i = 0; i < 4; i++) {
                OpenMesh::Vec3f v = mesh->point(*cvit);
                OpenMesh::Vec3f n = mesh->normal(*cvit);
                glNormal3f(n[0], n[1], n[2]);
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
    vector<OpenMesh::Vec3f> n;

    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if (!file.is_open())
        return false;

    file << "# Automatically generated" << endl << endl;
    file << "o mesh" << endl;

    // Vertices
    file << endl << "# Vertices" << endl;
    BMesh::ConstVertexIter vit;
    for (vit = mesh->vertices_begin(); vit != mesh->vertices_end(); ++vit) {
        OpenMesh::Vec3f v = mesh->point(*vit);
        file << "v " << v[0] << " " << v[1] << " " << v[2] << endl;
    }

    // Normals
    file << endl << "# Normals" << endl;
    for (vit = mesh->vertices_begin(); vit != mesh->vertices_end(); ++vit) {
        OpenMesh::Vec3f v = mesh->normal(*vit);
        n.push_back(v);
        file << "vn " << v[0] << " " << v[1] << " " << v[2] << endl;
    }


    // Faces
    file << endl << "# Faces" << endl;
    BMesh::ConstFaceIter fit;
    for (fit = mesh->faces_begin(); fit != mesh->faces_end(); ++fit) {
        BMesh::FaceHandle f = *fit;
        BMesh::ConstFaceVertexIter cvit = mesh->cfv_iter(f);

        file << "f ";
        for (unsigned int i = 0; i < 4; i++) {
            file << *cvit << "//";
            file << getNormalIndex(n, mesh->normal(*cvit)) << " ";
            ++cvit;
        }
        file << endl;
    }

    file.close();
    return true;
}


///////////////////////////////////////////////////////////////////////////////
int Mesh::getNormalIndex(const std::vector<OpenMesh::Vec3f> &v,
                         const OpenMesh::Vec3f &n)
{
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i] == n)
            return i;

    return -1;
}
