
/*
  Copyright (c) 2017 Bent Cardan
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.
*/

#include <sys/socket.h>
#include <sys/un.h>

#include "nan.h"
#include "ref.h"

using v8::FunctionTemplate;
using v8::Function;
using v8::Number;
using v8::Boolean;
using v8::String;
using v8::Object;
using v8::Value;
using v8::Local;

using Nan::HandleScope;
using Nan::MaybeLocal;
using Nan::NewBuffer;
using Nan::Callback;
using Nan::Maybe;
using Nan::Set;
using Nan::New;
using Nan::To;

int fd;
struct sockaddr_un addr;

typedef struct udp_s {
  uv_poll_t poll_handle;
  uv_os_sock_t fd;
  Callback *cb;
  int len;
} udp_t;

void unixrecv(uv_poll_t *req, int status, int events) {
  HandleScope scope;

  if (events & UV_READABLE) {
    udp_t *ctx;
    ctx = reinterpret_cast<udp_t *>(req);

    char buf[ctx->len];
    ssize_t ss = recvfrom(ctx->fd, buf, sizeof(buf), 0, NULL, NULL);

    if(ss >= 0) {
      Local<Object> h = NewBuffer(ss).ToLocalChecked();
      memcpy(node::Buffer::Data(h), buf, ss);
      Local<Value> argv[] = { h };
      ctx->cb->Call(1, argv);
    }
  }
}

NAN_METHOD(recvfrom){
  String::Utf8Value path(info[0]);
  char *socket_path = *path;

  // open unix datagram socket for node.js
  if ( (fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    perror("socket error");

  int opt;
  Local<Function> fn;

  if (info[1]->IsNumber()) {
    // set socket's recv buffer size
    opt = To<int>(info[1]).FromJust();
    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &opt, sizeof (opt)) < 0)
      perror("SO_RCVBUF setsockopt() fail");

    fn = info[2].As<Function>();
  }

  if (info[1]->IsFunction()) {
    fn = info[1].As<Function>();
    opt = 4096;
  }

  Callback *cb = new Callback(fn);

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, socket_path);

  int rc = bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
  if(rc != 0)
    perror("bind() fail");

  udp_t *ctx;
  ctx = reinterpret_cast<udp_t *>(calloc(1, sizeof(udp_t)));
  ctx->poll_handle.data = ctx;
  ctx->cb = cb;
  ctx->fd = fd;
  ctx->len = opt;

  // libuv calls `unixrecv()`, polling fd during eventloop w/ `uv_poll_start()`
  // descriptor indicates as buffer data from kernel is available to read by user space
  if (ctx->fd != 0) {
    uv_poll_init_socket(uv_default_loop(), &ctx->poll_handle, ctx->fd);
    uv_poll_start(&ctx->poll_handle, UV_READABLE, unixrecv);

    // return socket ctx as a buffer up into javasript land
    info.GetReturnValue().Set(WrapPointer(ctx, 8));
  }
}

#define EXPORT_METHOD(C, S)                                                    \
  Set(C, New(#S).ToLocalChecked(),                                             \
    Nan::GetFunction(New<FunctionTemplate>(S)).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  HandleScope scope;
  EXPORT_METHOD(target, recvfrom);
}

NODE_MODULE(sendto, Init)