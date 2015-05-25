
# Gotta love git rebase
api_key = 'lol u wish'

httpGet = (url, callback) ->
  req = new XMLHttpRequest()
  req.open('GET', url, true)
  req.onload = (e) ->
    if (req.readyState == 4 && req.status == 200)
      response = JSON.parse(req.responseText)
      callback(response)
     else
       console.log('Error making HTTP request')
  req.send(null)

withLocation = (callback) ->
  locationSuccess = (pos) ->
    callback(pos.coords)

  locationError = (err) ->
    console.warn("Location error (#{err.code}): #{err.message}")

  locationOptions =
    timeout: 15000
    maximumAge: 60000
  navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions)

withLocalConditions = (callback) ->
  withLocation (coords) ->
    url = "https://api.forecast.io/forecast/#{api_key}/#{coords.latitude},#{coords.longitude}?exclude=daily,flags&units=ca"
    httpGet url, (data) ->
      callback(data.currently)

Pebble.addEventListener 'ready', (e) ->
  console.log('PebbleKit JS is ready.')
  #fetchLocation()

sendMessage = (data) ->
  success = (e) ->
    console.log "Send successful"
  error = (e) ->
    console.log "Send failed"
  Pebble.sendAppMessage data, success, error

Pebble.addEventListener "appmessage", (e) ->
  console.log("Refreshing weather")
  withLocalConditions (conditions) ->
    console.log(JSON.stringify(conditions))
    sendMessage
      temp: Math.round(conditions.apparentTemperature)

