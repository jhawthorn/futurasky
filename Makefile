JS_FILES=$(shell find js/)

build: src/js/pebble-js-app.js src/keys.h
	pebble build

src/js/pebble-js-app.js: appinfo.json $(JS_FILES)
	npm run compile

src/keys.h: appinfo.json
	coffee tools/generate_keys.coffee > src/keys.h

run: build
	pebble install --logs

clean:
	rm -f src/js/pebble-js-app.js src/keys.h
	rm -Rf build

.PHONY: build run clean
