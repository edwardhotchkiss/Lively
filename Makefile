
PEBBLE_DEV_HOME := ~/pebble-dev/PebbleKit-v1.1.1/Pebble/
PEBBLE_TOOLS := $(PEBBLE_DEV_HOME)tools/
PEBBLE_SDK := $(PEBBLE_DEV_HOME)sdk/
PEBBLE_WAF := $(PEBBLE_SDK)waf

setup:
	~/pebble-dev/PebbleKit-v1.1.1/Pebble/tools/create_pebble_project.py ~/pebble-dev/PebbleKit-v1.1.1/Pebble/sdk Lively

build:
	./waf configure
	./waf build

server:
	python -m SimpleHTTPServer 8000

full: build server

.PHONY: setup build server full