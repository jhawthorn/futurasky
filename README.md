# FuturaSky

A watchface for pebble with forecast.io driven weather

![](http://i.hawth.ca/u/pebble-screenshot_2015-05-25_23-35-35.png)

* Weather is fetched from forecast.io every 10 minutes
* Weather is cached on the watch, so it is available immediately when launched.
  No unnecessary refreshes when navigating menus or switching faces.
* Vibrate when bluetooth disconnected
* Icon shows bluetooth disconnected
* Battery status displayed when charging

## Acknowledgements

* Entire look and a lot of copy pasta from [Futura Weather 2](https://github.com/Niknam/futura-weather) by Niknam
* Weather icons are [Climacons](http://adamwhitcroft.com/climacons/) by Adam Whitcroft
* Powered by [forecast.io](http://forecast.io/) (api key required)

## Limitations

* Celsius only
* Bluetooth vibration can't be disabled
* No seconds display
* No low battery display
* Displaying 3-digit temperatures is probably broken

