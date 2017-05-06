var tape = require ('tape')
tape('==================== sendto testsuite summary ====================', tests);
function tests (t){
  t.test('===== recvfrom buffers =====', require('./recvfrom'));
}
