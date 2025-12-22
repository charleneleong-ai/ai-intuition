## AI Intuition

This is my personal blog using [Quarto](https://quarto.org/), welcome! :)


### Setup

1. To get started, install the pre-requisites:

 - [Python 3.10](https://www.python.org/downloads/release/python-31014/)
 - [`pipx`](https://pipx.pypa.io/stable/installation/)
 - [AWS CLI](https://docs.aws.amazon.com/cli/latest/userguide/getting-started-install.html)
 - [Quarto](https://quarto.org/docs/download/tarball.html)


```
brew install pipx quarto texlive-latex-extra dvisvgm
```

2. Setup initial virtual env
```zsh
mise uv-init
```

3. Install packages
```zsh
mise uv-install
```


**NOTE**: If packages in `pyproject.toml` are updated, run `mise uv-sync` to sync the venv deps.
