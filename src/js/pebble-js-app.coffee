
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

forecastToPebbleIcon = (icon_name) ->
  pbr = switch icon_name
    when "clear-day" then "ICON_CLEAR_DAY"
    when "clear-night" then "ICON_CLEAR_NIGHT"
    when "rain" then "ICON_RAIN"
    when "snow" then "ICON_SNOW"
    when "sleet" then "ICON_HAIL"
    when "wind" then "ICON_WIND"
    when "fog" then "ICON_FOG"
    when "cloudy" then "ICON_CLOUD"
    when "partly-cloudy-day" then "ICON_CLOUDY_DAY"
    when "partly-cloudy-night" then "ICON_CLOUDY_NIGHT"
    else "ICON_CLOUD"
  console.log("Using #{pbr} for '#{icon_name}'")
  PebbleResources[pbr]

Pebble.addEventListener "appmessage", (e) ->
  console.log("Refreshing weather")
  withLocalConditions (conditions) ->
    sendMessage
      temp: Math.round(conditions.apparentTemperature)
      icon: forecastToPebbleIcon(conditions.icon)

