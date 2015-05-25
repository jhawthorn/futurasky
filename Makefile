
run: src/js/pebble-js-app.js
	pebble build && pebble install --logs

src/js/pebble-js-app.js: src/js/pebble-js-app.coffee
	coffee -c src/js/pebble-js-app.coffee
