//
//  PhysicalOBJModel.cpp
//  ofxOBJModel_example
//
//  Created by Will Gallia on 07/10/2015.
//
//

#include "ofxPhysicalOBJModel.h"

ofxPhysicalOBJFace::ofxPhysicalOBJFace(string name): ofxOBJFace(name) {
    
}

void ofxPhysicalOBJFace::applyForce(ofVec3f force) {
    acc+= force;
}

void ofxPhysicalOBJFace::update() {
    
    for (ofVec3f &v : vertices) {
        if (v.y <= floorPos) {
            vel*= 0.0;
            return;
        }
    }
    
    
    vel+= acc;
    acc*= 0.0;
    
    for (ofVec3f &v : vertices) {
        v+= vel;
    }
    
    vel*= 0.99;
}

void ofxPhysicalOBJFace::setFloor(float floorYPos) {
    floorPos = floorYPos;
}

ofVec3f ofxPhysicalOBJFace::moveOutFrom(ofVec3f p, float amount) {
    
    calcCenter();
    
    ofVec3f dir = center - p;
    dir.normalize();
    dir*= amount;
    
    return dir;
    
}

void ofxPhysicalOBJFace::rotate(ofVec3f axis) {
    
    //        static float rotation = 0;
    calcCenter();
    //        rotation+= 50;
    rotation = 15;
    for (ofVec3f &v : vertices) {
        v.rotate(rotation, center, axis);
    }
    
    
}

void ofxPhysicalOBJFace::calcCenter() {
    center = ofVec3f();
    for (ofVec3f &v : vertices) {
        center+= v;
    }
    center/= vertices.size();
}

bool ofxPhysicalOBJFace::isMoving() {
    return vel.lengthSquared() > 0.01;
}



////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark ofxPhysicalOBJModel

const ofVec3f ofxPhysicalOBJModel::MODEL_CENTER(0.333, 0, 0); // surely no one would use this vector?

void ofxPhysicalOBJModel::explodeMesh(float amount, ofVec3f fromPoint) {
    

    
    if (fromPoint == MODEL_CENTER) {
        //        ofVec3f minPoint, maxPoint;
        //        group.getBounds(minPoint, maxPoint);
        //        fromPoint = (maxPoint - minPoint) * 0.5;
        fromPoint = modelCenter;
    }
    
    for (auto &group : groups) {
        for (auto &face : group.faces) {
            face.applyForce(face.moveOutFrom(fromPoint, amount*ofRandom(0.5, 1.5)));
            //            faces.update();
            face.setFloor(-50);
        }
    }
    
    //    group.moveOut(fromPoint, amount);
    meshDirty = true;
    //    auto vertices = mesh.getVertices();
}

void ofxPhysicalOBJModel::reset() {

}


void ofxPhysicalOBJModel::update() {
    
    
    
    ofVec3f gravity(0, -0.1, 0);
    
    for (auto &group : groups) {
        for (auto &face : group.faces) {
            face.applyForce(gravity);
            
            face.update();
            
            
        }
    }
    
    meshDirty = true;
}


bool ofxPhysicalOBJModel::inMotion() {
    
    for (auto &group : groups) {
        for (auto &face : group.faces) {
            if (face.isMoving()) {
                return true;
            }
        }
    }
    return false;
}




