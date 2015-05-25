appinfo = require('../appinfo.json')

# Assumed resources are numbered sequentially as they appear, which is the case
# at least for SDK 2.0
process.stdout.write("this.PebbleResources = {\n")
for resource, index in appinfo.resources.media
  process.stdout.write("\"#{resource.name}\": #{index+1},\n")
process.stdout.write("};\n")

