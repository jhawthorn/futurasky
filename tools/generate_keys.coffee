appinfo = require('../appinfo.json')

for key, value of appinfo.appKeys
  process.stdout.write "#define KEY_#{key.toUpperCase()} #{value}\n"

