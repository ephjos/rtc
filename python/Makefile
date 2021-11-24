
.PHONY: test
test:
	pypy3 -m unittest discover

.PHONY: lint
lint:
	mypy rtc demo test

.PHONY: format
format:
	autoflake --in-place --remove-unused-variables --remove-all-unused-imports $(shell find -name "*.py")
	black .
