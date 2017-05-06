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

    setTimeout(()=> process.exit(0), 50)
  })

  sendto(addr, Buffer(msg))
}
