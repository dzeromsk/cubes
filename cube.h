// The MIT License (MIT)
//
// Copyright (c) 2016 Dominik Zeromski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cube_model.h"

template <typename type, size_t size = 10> class CircularBuffer {
public:
  CircularBuffer() : n_(0) {}

  void append(const type &value) { array_[(n_++ % size)] = value; }

  type &operator[](int offset) { return array_[(n_ + offset) % size]; }

private:
  type array_[size];
  size_t n_;
};

class Cube {
public:
  Cube(glm::vec3 position, CubeModel *model, float scale = 1, float mass = 10)
      : cubeModel_(model), scale_(scale) {
    shape_ = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f) * scale_);
    motionState_ = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1),
                    btVector3(position.x, position.y, position.z)));

    initial_position_ = position;

    // btScalar mass = 10 * scale_;
    btVector3 inertia(0, 0, 0);
    shape_->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, motionState_, shape_,
                                                    inertia);
    bodyCI.m_friction = 100;
    bodyCI.m_restitution = .0f;
    body_ = new btRigidBody(bodyCI);

    // TODO(dzeromsk): find better api to start object inactive
    if (scale_ == 1) {
      body_->updateDeactivation(10.0f);
    }

    cubeModel_->Color(glm::vec3(1.0f, 1.0f, 1.0f));
  }

  ~Cube() {
    delete body_;
    delete motionState_;
    delete shape_;
  }

  void Reset() {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(initial_position_.x, initial_position_.y,
                                  initial_position_.z));
    transform.setRotation(btQuaternion(0, 0, 0, 1));
    body_->setCenterOfMassTransform(transform);
    body_->setLinearVelocity(btVector3(0, 0, 0));
    body_->setAngularVelocity(btVector3(0, 0, 0));
    body_->clearForces();
    body_->activate(true);
    if (scale_ == 1) {
      body_->updateDeactivation(10.0f);
    }
  }

  btRigidBody *GetBody() { return body_; }

  void Draw(const glm::mat4 &view, const glm::mat4 &projection) {
    // move
    btTransform trans;
    body_->getMotionState()->getWorldTransform(trans);
    glm::mat4 model;
    trans.getOpenGLMatrix(glm::value_ptr(model));

    models_.append(model);

    // color
    if (body_->wantsSleeping()) {
      cubeModel_->Color(glm::vec3(1.0f, 1.0f, 1.0f));
    } else {
      cubeModel_->Color(glm::vec3(0.5f, 0.0f, 0.0f));
    }

    Draw(model, view, projection);
  }

  void Draw(const int model, const glm::mat4 &view,
            const glm::mat4 &projection) {
    cubeModel_->Color(glm::vec3(0.3f, 0.2f, 0.2f));
    Draw(models_[model], view, projection);
  }

  void Draw(const glm::mat4 &model, const glm::mat4 &view,
            const glm::mat4 &projection) {
    // scale
    glm::mat4 scaled = glm::scale(model, glm::vec3(scale_));

    // draw
    cubeModel_->Draw(scaled, view, projection);
  }

  void Force(const glm::vec3 &i) {
    body_->activate(true);
    // body_->applyCentralImpulse(btVector3(i.x, i.y, i.z));
    auto a = body_->getLinearVelocity();
    if (a.length() < 18) {
      body_->setLinearVelocity(a + btVector3(i.x, i.y, i.z));
    }
  }

  glm::vec3 Position() {
    auto transform = body_->getCenterOfMassTransform();
    auto pos = transform.getOrigin();
    return glm::vec3(pos.x(), pos.y(), pos.z());
  }

private:
  btCollisionShape *shape_;
  btDefaultMotionState *motionState_;
  btRigidBody *body_;
  CubeModel *cubeModel_;
  glm::vec3 initial_position_;
  float scale_;
  CircularBuffer<glm::mat4, 60> models_;
};