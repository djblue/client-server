var spawn = require('child_process').spawn
  , fs = require('fs')

var client = function (id, input) {
  return spawn('./client', [
    'test',
    id,
    '127.0.0.1',
    3000,
    input
  ])
}

var make_request = function (id, input) {

  var c = client(id, input)
    , log = fs.createWriteStream('c' + id + '.txt');

  c.stdout.pipe(log)
  c.stderr.pipe(log)
};

make_request(1, './input/script1')
make_request(2, './input/script2')
make_request(3, './input/script3')

/*
var c1 = client(1, './input/script1')
  , log1 = fs.createWriteStream('c1.txt');

c1.stdout.pipe(log1)
c1.stderr.pipe(log1)

var c2 = client(2, './input/script1')
  , log2 = fs.createWriteStream('c2.txt');

c2.stdout.pipe(log2)
c2.stderr.pipe(log2)

var c3 = client(3, './input/script1')
  , log3 = fs.createWriteStream('c3.txt');

c3.stdout.pipe(log3)
c3.stderr.pipe(log3)
*/
