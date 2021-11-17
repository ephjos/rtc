
.PHONY: test
test:
	python -m unittest discover


.PHONY: demo
demo:
	python -m demo

.PHONY: format
format:
	autoflake --in-place --remove-unused-variables --remove-all-unused-imports $(shell find -name "*.py")
	black .
