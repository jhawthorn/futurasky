
build: src/js/pebble-js-app.js src/keys.h
	pebble build

src/js/pebble-js-app.js: src/js/pebble-js-app.coffee appinfo.json
	coffee tools/generate_resources.coffee > $@
	coffee -c -p src/js/pebble-js-app.coffee >> $@

src/keys.h: appinfo.json
	coffee tools/generate_keys.coffee > src/keys.h

run: build
	pebble install --logs

clean:
	rm -f src/js/pebble-js-app.js src/keys.h
	rm -Rf build

.PHONY: build run clean
