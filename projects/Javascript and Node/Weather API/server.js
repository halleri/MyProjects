// Connect to a redis server provisioned over at
// Redis Labs. See the README for more info.
var client = redis.createClient(
  nconf.get('redisPort') || '6379',
  nconf.get('redisHost') || '127.0.0.1',
  {
    'auth_pass': nconf.get('redisKey'),
    'return_buffers': true
  }
).on('error', function (err) {
  console.error('ERR:REDIS: ' + err);
});