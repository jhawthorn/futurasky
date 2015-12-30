
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

withApiKey = (callback) ->
  config = readConfig()
  callback(config.forecast_api_key)

withLocalConditions = (callback) ->
  withLocation (coords) ->
    withApiKey (api_key) ->
      url = "https://api.forecast.io/forecast/#{api_key}/#{coords.latitude},#{coords.longitude}?exclude=daily,flags&units=ca"
      httpGet url, (data) ->
        callback(data)

Pebble.addEventListener 'ready', (e) ->
  console.log('PebbleKit JS is ready.')

Pebble.addEventListener 'showConfiguration', (e) ->
  Pebble.openURL('https://jhawthorn.github.io/futurasky/#' + encodeURIComponent(localStorage.getItem('config')))

readConfig = ->
  JSON.parse(localStorage.getItem('config'))

Pebble.addEventListener 'webviewclosed', (e) ->
  configJSON = e.response
  console.log "Configuration window returned: #{configJSON}"
  localStorage.setItem 'config', configJSON

sendMessage = (data) ->
  success = (e) ->
    console.log "Send successful"
  error = (e) ->
    console.log "Send failed"
  Pebble.sendAppMessage data, success, error


class Forecast
  constructor: (data) ->
    {@time, @precipProbability, @cloudCover} = data
    @icon = this.guessIcon()

  is_raining: ->
    @precipProbability > 0.05

  is_clear: ->
    @cloudCover < 0.2

  is_partly_cloudy: ->
    @cloudCover >= 0.2

  is_mostly_cloudy: ->
    @cloudCover >= 0.75

  guessIcon: ->
    if this.is_raining()
      "ICON_RAIN"
    else if this.is_mostly_cloudy()
      "ICON_CLOUD"
    else if this.is_partly_cloudy()
      "ICON_CLOUDY_DAY"
    else
      "ICON_CLEAR_DAY"

forecastToPebbleIcon = (icon_name) ->
  #pbr = switch icon_name
  #  when "clear-day" then "ICON_CLEAR_DAY"
  #  when "clear-night" then "ICON_CLEAR_NIGHT"
  #  when "rain" then "ICON_RAIN"
  #  when "snow" then "ICON_SNOW"
  #  when "sleet" then "ICON_HAIL"
  #  when "wind" then "ICON_WIND"
  #  when "fog" then "ICON_FOG"
  #  when "cloudy" then "ICON_CLOUD"
  #  when "partly-cloudy-day" then "ICON_CLOUDY_DAY"
  #  when "partly-cloudy-night" then "ICON_CLOUDY_NIGHT"
  #  else "ICON_CLOUD"
  #console.log("Using #{pbr} for '#{icon_name}'")
  PebbleResources[icon_name]

detectChange = (forecasts) ->
  first = forecasts[0]
  for forecast in forecasts
    if first.icon != forecast.icon
      return forecast
  return null

nextChange = (data) ->
  hours = data.hourly.data
  hours = (new Forecast(x) for x in hours)
  minutes = data.minutely.data
  minutes = (new Forecast(x) for x in minutes)
  current = new Forecast(data.currently)
  first = hours[0]
  if minutes[0].is_raining()
    change = detectChange(minutes)
    time = if change then (change.time - current.time) else 0
    return {
      icon: 'ICON_RAIN'
      reltime: time
      is_current: true
    }
  else if change = detectChange(minutes)
    return {
      icon: change.icon
      reltime: (change.time - current.time)
      is_current: false
    }
  else if change = detectChange(hours)
    return {
      icon: change.icon
      reltime: (change.time - current.time)
      is_current: false
    }
  else
    return {
      icon: current.icon
      reltime: 0
      is_current: true
    }

Pebble.addEventListener "appmessage", (e) ->
  console.log("Refreshing weather")
  withLocalConditions (conditions) ->
    change = nextChange(conditions)
    console.log(JSON.stringify(change))
    sendMessage
      temp: Math.round(conditions.currently.apparentTemperature)
      icon: forecastToPebbleIcon(change.icon)
      duration: change.reltime
