# rtc

#### Test

```bash
python -m unittest discover
```

#### Demos

```bash
python -m demo
```

#### Autoflake

```bash
autoflake --in-place --remove-unused-variables --remove-all-unused-imports $(find -name "*.py")
```

#### Format

```bash
black .
```
