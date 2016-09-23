var appinfo = require('../appinfo.json')

for (var key in appinfo.appKeys) {
  var value = appinfo.appKeys[key]
  process.stdout.write("#define KEY_" + key.toUpperCase() + " " + value + "\n")
}

