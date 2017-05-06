
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

const recvfrom = require('..')

const sendto = require('sendto')
const isBuffer = Buffer.isBuffer

const addr = '/tmp/asdf'

module.exports = function handle (t) {
  t.test('recvfrom', buffers);
}

function buffers (t) {
  t.plan(2)

  const msg = 'Hello, world!'

  try{
    require('fs').unlinkSync(addr)
  } catch($){}

  recvfrom(addr, buf => {
    t.ok( isBuffer(buf),   'recv a buf' )
    t.is( String(buf),  msg, `buffer recv'd from sendto is msg: ${msg}`  )

    setTimeout(process.exit, 50, 0)
  })

  sendto(addr, Buffer(msg))
}
