import PebbleResources from 'PebbleResources'

const withLocation = function(callback) {
  const locationSuccess = pos => callback(pos.coords)

  const locationError = err => console.warn(`Location error (${err.code}): ${err.message}`)

  const locationOptions = {
    timeout: 15000,
    maximumAge: 60000
  }
  navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
};

const withApiKey = function(callback) {
  const config = readConfig()
  callback(config.forecast_api_key)
};

function withLocalConditions(callback) {
  withLocation(coords => {
    withApiKey(api_key => {
      const url = `https://api.forecast.io/forecast/${api_key}/${coords.latitude},${coords.longitude}?exclude=daily,flags&units=ca`;
      fetch(url)
        .then(response => response.json())
        .then(data => callback(data))
    })
  })
}

Pebble.addEventListener('ready', e => {
  console.log('PebbleKit JS is ready.')
})

Pebble.addEventListener('showConfiguration', e => {
  Pebble.openURL(`https://jhawthorn.github.io/futurasky/#${encodeURIComponent(localStorage.getItem('config'))}`)
})

var readConfig = () => JSON.parse(localStorage.getItem('config'))

Pebble.addEventListener('webviewclosed', function(e) {
  const configJSON = e.response
  if(configJSON) {
    console.log(`Configuration window returned: ${configJSON}`)
    localStorage.setItem('config', configJSON)
  }
})

const sendMessage = function(data) {
  const success = e => console.log("Send successful")
  const error = e => console.log("Send failed")
  Pebble.sendAppMessage(data, success, error)
};


class Forecast {
  constructor(data) {
    ({time: this.time, precipProbability: this.precipProbability, cloudCover: this.cloudCover} = data);
    this.icon = this.guessIcon();
  }

  is_raining() {
    return this.precipProbability > 0.3;
  }

  is_clear() {
    return this.cloudCover < 0.2;
  }

  is_partly_cloudy() {
    return this.cloudCover >= 0.2;
  }

  is_mostly_cloudy() {
    return this.cloudCover >= 0.75;
  }

  guessIcon() {
    if (this.is_raining()) {
      return "ICON_RAIN";
    } else if (this.is_mostly_cloudy()) {
      return "ICON_CLOUD";
    } else if (this.is_partly_cloudy()) {
      return "ICON_CLOUDY_DAY";
    } else {
      return "ICON_CLEAR_DAY";
    }
  }
}

const forecastToPebbleIcon = icon_name => PebbleResources[icon_name]

const detectChange = function(forecasts) {
  const first = forecasts[0]
  for (let i = 0; i < forecasts.length; i++) {
    const forecast = forecasts[i]
    if (first.icon !== forecast.icon) {
      return forecast
    }
  }
  return null
};

const nextChange = function(data) {
  var change
  let hours = data.hourly.data
  hours = (hours.map((x) => new Forecast(x)))
  let minutes = data.minutely.data
  minutes = (minutes.map((x) => new Forecast(x)))
  let current = new Forecast(data.currently)
  const first = hours[0]
  if (minutes[0].is_raining()) {
    const change = detectChange(minutes);
    const time = change ? (change.time - current.time) : 0;
    return {
      icon: 'ICON_RAIN',
      reltime: time,
      is_current: true
    };
  } else if (change = detectChange(minutes)) {
    return {
      icon: change.icon,
      reltime: (change.time - current.time),
      is_current: false
    };
  } else if (change = detectChange(hours)) {
    return {
      icon: change.icon,
      reltime: (change.time - current.time),
      is_current: false
    };
  } else {
    return {
      icon: current.icon,
      reltime: 0,
      is_current: true
    };
  }
};

Pebble.addEventListener("appmessage", function(e) {
  console.log("Refreshing weather")
  withLocalConditions(function(conditions) {
    const change = nextChange(conditions)
    console.log(JSON.stringify(change))
    sendMessage({
      temp: Math.round(conditions.currently.temperature),
      icon: forecastToPebbleIcon(change.icon),
      duration: change.reltime
    })
  })
})
