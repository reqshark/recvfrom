# recvfrom

unix datagrams on osx have system recv buffer limits of 4096 bytes.

this module performs a posix `recvfrom()` w/ optional `SO_RCVBUF`, to increase the recv buffer limit size.

```bash
npm i recvfrom
node -e "require('recvfrom')('/tmp/a', 9999, buf => console.log(buf+''))"
```

### recvfrom(*string*, *number*, *function*)

parameter signature, `recvfrom(str, n, fn)`:

* `string`, a unix datagram socket path

* `number`, buffer recv size

* `function`, a callback for handling the buffer data

for example:

```js
const recvfrom = require('recvfrom')

const sockpath = `/tmp/mysocketpath`
const buffersz = 99999

recvfrom(sockpath, buffersz, buf => console.log('recv buf:', buf) )
```

the buffer limit configuration is an optional parameter, so you can pass a callback instead, after the unix socket path:

```js
require('recvfrom')('/tmp/mysocketpath', buf =>
  console.log('recv buf:', buf) )
```

### MIT
