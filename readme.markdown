# recvfrom

unix datagrams on osx have system recv buffer limits of 4096 bytes.

this module performs a posix `recvfrom()` w/ configurable socket option `SO_RCVBUF`, to up the recv buffer limit size.

```bash
npm i recvfrom
node -e "require('recvfrom')recvfrom('/tmp/a', 9999, buf => console.log('recv buf:', buf))"
```

### recvfrom(*string*, *number*, *function*)

pass a unix datagram socket path, buffer recv size and a callback for handling the buffer data, something like:

```js
const recvfrom = require('recvfrom')
recvfrom('/tmp/mysocketpath', 9999, buf => console.log('recv buf:', buf))
```
