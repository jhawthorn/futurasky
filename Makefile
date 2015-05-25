
build: src/js/pebble-js-app.js src/keys.h
	pebble build

src/js/pebble-js-app.js: src/js/pebble-js-app.coffee
	coffee -c src/js/pebble-js-app.coffee

src/keys.h: appinfo.json
	coffee tools/generate_keys.coffee > src/keys.h

run: build
	pebble install --logs

.PHONY: build run
