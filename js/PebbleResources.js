import AppInfo from 'AppInfo'

// Assumed resources are numbered sequentially as they appear, which is the case
// at least for SDK 2.0

const PebbleResources = {}
for(let i = 0; i < AppInfo.resources.media.length; i++) {
  const resource = AppInfo.resources.media[i];
  PebbleResources[resource.name] = i+1
}

export default PebbleResources
