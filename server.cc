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

#include <btBulletDynamicsCommon.h>
#include <gflags/gflags.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glog/logging.h>
#include <uv.h>

#include <functional>
#include <map>
#include <set>

#include "base/loop.h"
#include "base/idle.h"
#include "net/udp.h"
#include "net/addr.h"
#include "net/udp_server.h"
#include "base/timer.h"
#include "state.h"
#include "cube.h"
#include "world.h"
#include "game_server.h"

DEFINE_string(server_addr, "127.0.0.1", "Server ip address");
DEFINE_int32(server_port, 3389, "Server port");

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  GameServer server;
  return server.ListenAndServe(FLAGS_server_addr.c_str(), FLAGS_server_port);
}
