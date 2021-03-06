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

struct Model {
  Program program;

  GLint view;
  GLint projection;
  GLint light_color;
  GLint light_position;

  // state
  GLint pos;
  GLint orie;
  GLint interacting;
  GLint scale;

  GLint position;
  GLint normal;

  GLuint VBO[2];
  GLuint VAO;

  size_t vertices_size;
  size_t frame_size;

  Model();

  ~Model();

  void Locations();

  void Attrib();

  void WriteVertices(const GLfloat *vertices, size_t size);

  void WriteState(const Frame &frame);

  void Draw(const Frame &frame, const glm::mat4 &v, const glm::mat4 &p);
};
