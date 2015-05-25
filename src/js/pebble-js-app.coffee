
locationSuccess = (pos) ->
  coordinates = pos.coords
  console.log("Got coordinates: " + JSON.stringify(coordinates))

locationError = (err) ->
  console.warn("Location error (#{err.code}): #{err.message}")

fetchLocation = ->
  locationOptions =
    timeout: 15000
    maximumAge: 60000
  navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions)

Pebble.addEventListener 'ready', (e) ->
  console.log('PebbleKit JS is ready.')
  fetchLocation()

Pebble.addEventListener "appmessage", (e) ->
  console.log("Got a message")

